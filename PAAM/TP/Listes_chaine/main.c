//
//
// ************************* VERSION NON TERMINEE, NE PAS LIRE ************************
//
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct list{
	int val;
	struct list * next;
	pthread_mutex_t * lock;
};

void intialize_list(struct list** list) {
	*list = NULL;
}

void insert(struct list** list, int val) {
	struct list* new_elmt = (struct list *) malloc(sizeof(list));
	new_elmt->next=NULL;
	new_elmt->val=val;

	if (*list == NULL) {
		*list=new_elmt;
	}else{
		int current = (*list)->val;
		while (val<current) {
			*list=(*list)->next;
			current=(*list)->val;
		}
		struct list* new_next= (*list)->next;
		(*list)->next=new_next;
		new_elmt->next=new_next;
	}
}

int list_remove(struct list** list, int val) {
	if (*list == NULL) {
		fprintf(stderr, "Error : attempt to delete from an empty list \n");
	}
	struct list* course = *list;
	
	return 0;
}

void list_free(struct list** list) {
	if (*list == NULL)
		return;

	struct list* next;
	while (*list != NULL) {
		next=(*list)->next;
		free(*list);
		*list=next;
	}
}

int has(struct list * list) {
	return 0;
} 

int main (int argc, char ** argv, char ** env) {

	return 0;
}