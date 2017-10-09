#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <limits.h>

#define N       128
#define LOOPS   2000
#define THREADS 20
#define ABORT   0xf0000000 /* f + 7x0 */
#define BACKOFF 500        /* initial backoff in microseconds */

#define STM    0
#define LOCK   1
#define ATOMIC 2

int version;

struct value {
  int value;
  int counter;
};

struct memory {
  pthread_mutex_t        lock;
  struct value* volatile values[N];
  volatile int           clock;
};

struct tx {
  int       readSet[N];      /* bitset */
  int       writeSet[N];     /* bitset */
  int       writenValues[N]; /* valeurs */
  int       clock;
  long long backoff; /* délai d'attente */
};

struct memory      memory;
__thread struct tx tx;

struct value* newValue(int value, int counter) {
  struct value* res = malloc(sizeof(struct value));
  res->value = value;
  res->counter = counter;
  return res;
}

void initMemory() {
  pthread_mutex_init(&memory.lock, NULL);
  for(int i=0; i<N; i++)
    memory.values[i] = newValue(0, 0);
  memory.clock = 1;
  srand(time(0));
}

void initThreads() {
  // première mise à zéro des variables lues et écrites,
  // les autres mises à zéro se font dans le commit
  memset(tx.readSet, 0, N);
  memset(tx.writeSet, 0, N);
}

void startTX(int nb_restart) {
  tx.clock = memory.clock;
  //Si il y a eu un restart, alors on attend un peu avant de recommencer
  if (nb_restart == 0) {
    tx.backoff = 0;
  } else {
    tx.backoff = 100;
    usleep(tx.backoff);
  }
}

/*
 *   ABORT: abort, other: ok
 */
int writeValue(int idx, int value) {
  tx.writeSet[idx]=1; //On a écrit dans l'id idx
  tx.writenValues[idx]=value; //On l'écrit vraiment
  return 0;
}

/*
 *   ABORT: abort, other: ok
 */
int readValue(int idx) {
  if (tx.writeSet[idx]) { //si il a déjà été écrit localement
    return tx.writenValues[idx]; //on renvoit sa valeur
  } else { //sinon
    struct value * val = memory.values[idx]; //on prend ses informations en mémoire 
    if (val->counter >= tx.clock) {
      return ABORT; // Si il est plus récent que nous, stop
    }

    tx.readSet[idx]=1;
    return val->value; //sinon, on l'a lu et on renvoit sa valeur
  }
}

/*
 *   ABORT: abort, other: ok
 */
int commitTX() {
  char commit = 1; // 1 si on commit, 0 sinon
  int toWrite[N]; // Tableau des valeurs a mettre à jour
  int nextWrite=0; //indice de toWrite sur lequel je suis en train d'écrire
  pthread_mutex_lock(&memory.lock);
  for(int i = 0; i<N; ++i) {
    if(tx.readSet[i]) {
      --tx.readSet[i];
      if (commit && memory.values[i]->counter >= tx.clock) {
       --commit; //on ne commit plus (commit = 0)
      }
    }
    if(tx.writeSet[i]){
      --tx.writeSet[i];
      if (commit){
        toWrite[nextWrite++]=i; //on doit mettre à jour l'emplacement mémoire i
        // donc toWrite contient i, et la prochaine valeur à mettre dans toWrite sera à la case nextWrite+1
        if (memory.values[i]->counter >= tx.clock) {
          --commit; //on ne commit plus (commit = 0)
        }
      }
    }
  }
  if (!commit) {
    pthread_mutex_unlock(&memory.lock);
    return ABORT;
  }

  for(int i = 0; i<nextWrite; ++i) { //pour toutes les valeurs à mettre à jour
    memory.values[toWrite[i]]->counter = memory.clock; //on commence par mettre l'horloge
    memory.values[toWrite[i]]->value = tx.writenValues[toWrite[i]]; //puis la valeur
  }
  ++memory.clock; // une fois que tout es fait, j'augemente la mémoire centrale

  pthread_mutex_unlock(&memory.lock);
  return 0;
}

void* f(void* arg) {
  initThreads();
  for(int i=0; i<LOOPS; i++) {
    if(version == STM) {
      int nb_restart = -1;
restart:
      ++nb_restart;
      startTX(nb_restart);

      int x = readValue(0);
      
      if(x == ABORT) {
        goto restart;
      }
    
      if(writeValue(0, x + 1) == ABORT) {
        goto restart;
      }

      if(commitTX() == ABORT) {
        goto restart;
      }
    } else if(version == LOCK) {
      pthread_mutex_lock(&memory.lock);
      memory.values[0]->value++;
      pthread_mutex_unlock(&memory.lock);
    } else {
      __sync_fetch_and_add(&memory.values[0]->value, 1);
    }
  }
  return 0;
}

int main(int argc, char **argv) {
  if(argc != 2) {
    fprintf(stderr, "usage: %s [0: STM, 1: LOCK, 2: ATOMIC_ADD]\n", argv[0]);
    return 1;
  }

  version = atoi(argv[1]);
  
  pthread_t threads[THREADS];
  initMemory();
  
  struct timeval start;
  gettimeofday(&start, 0);
  
  for(int i=0; i<THREADS; i++)
    pthread_create(&threads[i], 0, f, 0);
  
  for(int i=0; i<THREADS; i++)
    pthread_join(threads[i], 0);
  
  struct timeval end;
  gettimeofday(&end, 0);
  
  if(memory.values[0]->value != THREADS*LOOPS) {
    printf("value = %d while we expect %d\n", memory.values[0]->value, THREADS*LOOPS);
    printf("    => error!!!\n");
  } else {
    printf("Elapsed time: %0.3f ms\n",
           1e3*end.tv_sec + end.tv_usec*1e-3 - start.tv_sec*1e3 - start.tv_usec*1e-3);
  }
  
  return 0;
}