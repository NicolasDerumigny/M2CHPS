#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct node {
  struct node* next;
  int value;
};

struct node * file_lock = NULL;
struct node * volatile file_lock_free = NULL;
pthread_mutex_t file_mutex;

void push_lock(int value_to_push) {
	struct node * new_node;
	new_node= malloc(sizeof(struct node));
	new_node-> value=value_to_push;
	pthread_mutex_lock(&file_mutex);
	new_node-> next= file_lock;
	file_lock= new_node;
	pthread_mutex_unlock(&file_mutex);
}

int pop_lock() {
	pthread_mutex_lock(&file_mutex);
	int valeur_retiree= file_lock-> value;
	struct node* node_a_supprimer= file_lock;
	file_lock=file_lock -> next;
	pthread_mutex_unlock(&file_mutex);
	free(node_a_supprimer);
	return valeur_retiree;
}


void push_lock_free(int value_to_push) {
	struct node * new_node;
	new_node= malloc(sizeof(struct node));
	new_node-> value=value_to_push;
	do {
		new_node-> next= file_lock_free;
	} while (! __sync_bool_compare_and_swap(&file_lock_free, new_node -> next, new_node));
}

int pop_lock_free() {
	struct node* node_a_supprimer;
	do {
		node_a_supprimer= file_lock_free;
	} while(! __sync_bool_compare_and_swap(&file_lock_free, node_a_supprimer, node_a_supprimer -> next));
	int valeur_retiree = node_a_supprimer->value;
	free(node_a_supprimer);
	return valeur_retiree;
}


void * thread_start(void * arg) {
	printf("Hello World\n");

	for (int i=0; i<1000;++i) {
		push_lock_free(i);
	}

	for (int i = 0; i < 1000; ++i) {
		pop_lock_free();
	}


	return NULL;
}


int main() {
	pthread_mutex_init(&file_mutex, NULL);
	pthread_t new_thread[10];

	for (int i=0; i<10; ++i){
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		pthread_create(&(new_thread[i]), &attr, &thread_start, NULL);
	}


	for (int i=0; i<10; ++i){
		pthread_join(new_thread[i], (void**) NULL);
	}

	pthread_mutex_destroy(&file_mutex);
	return 0;
}