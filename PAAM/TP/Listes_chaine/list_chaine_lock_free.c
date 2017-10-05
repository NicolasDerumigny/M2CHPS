#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

struct node;

union coloredPointer {
	struct {
		uintptr_t ptr: (sizeof(struct node *) << 3) - 1;
		uintptr_t color: 1;
	};
	uintptr_t rawValue;
};


struct node {
  union coloredPointer next;
  uint64_t value;
};

struct node * newList() {
	struct node * newNode;
	newNode = malloc (sizeof(*newNode));
	newNode->next.rawValue = (uintptr_t) NULL;
	return newNode;
}


uint8_t del(union coloredPointer * plist, int value) {
	restart: {
		union coloredPointer * pred = plist;
		while (1) {
			struct node * curr = (struct node *) (pred->ptr);
			if (curr==NULL || value < curr->value) {
				//pas trouvé
				return -1;
			}

			if (curr->value == value) {
				// trouvé
				union coloredPointer n;
				do {
					n = curr->next;
				} while (__sync_val_compare_and_swap(
						&(curr->next.rawValue),
						n.rawValue,
						n.rawValue | 1LL)
						!= n.rawValue);
			}

			if (curr->next.color) {
				// Si on tombe sur quelqu'un à supprimer
				if (__sync_val_compare_and_swap((struct node **) pred, curr, (struct node *) curr->next.ptr) != curr) {
					// Et la suppression n'a pas fonctionnée : on est trop loin dans la liste
					goto restart;
				} else {
					// La suppression a fonctionné
					continue;
				}
			}

			pred = &(curr->next);
		}
	}
	return 0;
}

uint8_t add(union coloredPointer * plist, int value) {
	struct node * newNode;
	newNode = malloc (sizeof(*newNode));
	newNode->value = value;
	// On construit le nouvel élément
	restart: {
		union coloredPointer * pred = plist;
		while (1) {
			struct node * curr = (struct node *) (pred->ptr);

			if (curr == NULL || value < curr->value) {
				// Si on se retrouve à l'nedroit d'insertion ou à la fin de la liste
				newNode->next = *pred;
				if (__sync_val_compare_and_swap((struct node **) pred, curr, newNode) != curr) {
					// L'insertion n'a pas fonctionné, on recommence
					goto restart;
				} else {
					return 0;
				}
			}

			if (curr->next.color) {
				// On supprime le prochain s'il est marqué
				if (__sync_val_compare_and_swap((struct node **) pred, curr, (struct node *) curr->next.ptr) != curr) {
					// On est allé trop loin, la suppression n'a pas fonctionné
					goto restart;
				} else {
					continue;
				}
			}

			pred = &(curr->next);
		}
	}
	return 0;
}


void print(union coloredPointer * plist) {
	union coloredPointer * pred = plist;
	printf("List :\n");
	while (1) {
		struct node * curr = (struct node *) (pred->ptr);
		if (curr == NULL) {
			printf("done \n");
			return;
		}

		printf("%lu (%i) ", curr->value, pred->color);
		fflush(stdout);
		

		pred = &(curr->next);
	}
}


int main(int argc, char const *argv[], char const *env[]) {
	struct node * test = newList();


	print((union coloredPointer *) test);
	add((union coloredPointer *) test, 7);
	print((union coloredPointer *) test);
	add((union coloredPointer *) test, 8);
	print((union coloredPointer *) test);
	del((union coloredPointer *) test, 7);
	print((union coloredPointer *) test);
	del((union coloredPointer *) test, 8);
	print((union coloredPointer *) test);
	del((union coloredPointer *) test, 7);
	print((union coloredPointer *) test);


	return 0;
}