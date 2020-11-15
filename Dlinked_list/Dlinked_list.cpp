#include <stdio.h>
#include <stdlib.h>

typedef struct Node { float cont; Node* prev; Node* next; } Node;
typedef struct DList { Node* head; Node* tail; int length; } DList;

void dlist_create(DList* pdl) {
	*pdl = { NULL, NULL, 0 };
	return;
}

/*int dlist_length(DList dl) {
	int len = 0;
	Node* p = dl.head;
	while (p != NULL) {
		len++;
		p = (*p).next;
	}
	return len;
}*/

void dlist_print(DList dl) {
	Node* p = dl.head;
	printf("DLinked_List [");
	if (p != NULL) {
		printf(" %f", (*p).cont);
		p = (*p).next;
	}
	while (p != NULL) {
		printf(", %f", (*p).cont);
		p = (*p).next;
	}
	printf(" ]\n");
	return;
}

int dlist_prepend(DList* pdl, float n_node) {
	Node* n = (Node*)malloc(sizeof(Node));
	if (n == NULL) {
		printf("Memory error\n");
		return 1;
	}
	*n = { n_node, NULL, (*pdl).head };
	if ((*pdl).head == NULL) {
		(*pdl).head = n;
		(*pdl).tail = n;
	}
	else {
		(*pdl).head->prev = n;
		(*pdl).head = n;
	}
	(*pdl).length++;
	return 0;
}

int dlist_append(DList* pdl, float n_node) {
	Node* n = (Node*)malloc(sizeof(Node));
	if (n == NULL) {
		printf("Memory error\n");
		return 1;
	}
	*n = { n_node, (*pdl).tail, NULL };
	if ((*pdl).tail == NULL) {
		(*pdl).head = n;
		(*pdl).tail = n;
	}
	else {
		(*pdl).tail->next = n;
		(*pdl).tail = n;
	}
	(*pdl).length++;
	return 0;
}

int dlist_insert(DList* pdl, float n_node, int ind) {
	if (ind > (*pdl).length) {
		printf("Input error\n");
		return 1;
	}
	if (ind == 0)
		return dlist_prepend(pdl, n_node);
	if (ind == (*pdl).length)
		return dlist_append(pdl, n_node);
	Node* n = (Node*)malloc(sizeof(Node));
	if (n == NULL) {
		printf("Memory error\n");
		return 1;
	}
	(*n).cont = n_node;
	Node* p;
	if (ind <= (*pdl).length / 2) {
		p = (*pdl).head;
		for (int i = 0; i < ind - 1; i++)
			p = (*p).next;
	}
	else {
		p = (*pdl).tail;
		for (int i = (*pdl).length; i > ind; i--)
			p = (*p).prev;
	}
	(*n).prev = p;
	(*n).next = (*p).next;
	(*p).next = n;
	(*n).next->prev = n;
	(*pdl).length++;
	return 0;
}

int dlist_get(DList dl, int ind, float* res) {
	if (ind >= dl.length) {
		printf("Input error\n");
		return 1;
	}
	Node* p;
	if (ind < dl.length / 2) {
		p = dl.head;
		for (int i = 0; i < ind; i++)
			p = (*p).next;
	}
	else {
		p = dl.tail;
		for (int i = dl.length - 1; i > ind; i--)
			p = (*p).prev;
	}
	*res = (*p).cont;
	return 0;
}

int dlist_remove(DList* pdl, int ind, float* res) {
	if (ind >= pdl->length) {
		printf("Input error\n");
		return 1;
	}
	Node* p;
	if (ind < pdl->length / 2) {
		p = pdl->head;
		for (int i = 0; i < ind; i++)
			p = (*p).next;
	}
	else {
		p = pdl->tail;
		for (int i = pdl->length - 1; i > ind; i--)
			p = (*p).prev;
	}
	if ((*p).prev != NULL)
		p->prev->next = p->next;
	else
		pdl->head = p->next;
	if ((*p).next != NULL)
		(*p).next->prev = (*p).prev;
	else
		(*pdl).tail = (*p).prev;
	*res = (*p).cont;
	free(p);
	(*pdl).length--;
	return 0;
}

int dlist_find(DList dl, float data) {
	Node* p = dl.head;
	int i = 0;
	while ((p != NULL) && (p->cont != data)) {
		i++;
		p = p->next;
	}
	if (p == NULL)
		i = -1;
	return i;
}

int dlist_remove_first(DList* pdl, float data) {
	Node* p = pdl->head;
	while ((p != NULL) && (p->cont != data))
		p = p->next;
	if (p == NULL)
		return 1;
	if (p->prev != NULL)
		p->prev->next = p->next;
	else
		pdl->head = p->next;
	if (p->next != NULL)
		p->next->prev = p->prev;
	else
		pdl->tail = p->prev;
	free(p);
	pdl->length--;
	return 0;
}

int dlist_remove_all(DList* pdl, float data) {
	Node* p = pdl->head;
	Node* aux;
	while (p != NULL) {
		while ((p != NULL) && (p->cont != data))
			p = p->next;
		if (p == NULL)
			return 1;
		if (p->prev != NULL)
			p->prev->next = p->next;
		else
			pdl->head = p->next;
		if (p->next != NULL)
			p->next->prev = p->prev;
		else
			pdl->tail = p->prev;
		aux = p->next;
		free(p);
		p = aux;
		pdl->length--;
	}
	return 0;
}

void dlist_clear(DList* pdl) {
	Node* p = pdl->head;
	Node* aux;
	while (p != NULL) {
		aux = p;
		p = p->next;
		free(aux);
	}
	pdl->head = pdl->tail = NULL;
	pdl->length = 0;
	return;
}

int dlist_copy(DList old_dl, DList* new_dl) {
	new_dl->length = old_dl.length;
	if (old_dl.length == 0) {
		new_dl->head = new_dl->tail = NULL;
		return 0;
	}
	Node* p = old_dl.head;
	Node* n;
	Node* aux;
	aux = (Node*)malloc(sizeof(Node));
	if (aux == NULL) {
		printf("Memory error\n");
		*new_dl = { NULL, NULL, 0 };
		return 1;
	}
	aux->cont = p->cont;
	aux->prev = NULL;
	new_dl->head = aux;
	n = aux;
	p = p->next;
	while (p != NULL) {
		aux = (Node*)malloc(sizeof(Node));
		if (aux == NULL) {
			printf("Memory error\n");
			dlist_clear(new_dl);
			return 1;
		}
		aux->cont = p->cont;
		aux->prev = n;
		n->next = aux;
		n = aux;
		p = p->next;
	}
	n->next = NULL;
	new_dl->tail = n;
	return 0;
}

void dlist_concat(DList* dl1, DList* dl2) {
	if (dl2->head == NULL)
		return;
	if (dl1->tail == NULL) 
		*dl1 = *dl2;
	else {
		dl1->tail->next = dl2->head;
		dl2->head->prev = dl1->tail;
		dl1->tail = dl2->tail;
		dl1->length += dl2->length;
	}
	*dl2 = { NULL, NULL, 0 };
	return;
}

int main() {
	DList dl;
	dlist_create(&dl);
	for (int i = 5; i > 0; i--)
		dlist_prepend(&dl, (float)i);
	dlist_print(dl);
	printf("%d\n", dl.length);
	for (int i = 6; i <= 10; i++)
		dlist_append(&dl, (float)i);
	dlist_print(dl);
	printf("%d\n", dl.length);
	dlist_insert(&dl, 3.5, 3);
	dlist_print(dl);
	printf("%d\n", dl.length);
	dlist_insert(&dl, 9.5, 10);
	dlist_print(dl);
	printf("%d\n", dl.length);
	float res;
	dlist_get(dl, 10, &res);
	printf("%f\n", res);
	dlist_remove(&dl, 5, &res);
	dlist_print(dl);
	printf("%d, %f\n", dl.length, res);
	printf("%d, %d\n", dlist_find(dl, 3.5), dlist_find(dl, 11.5));
	dlist_remove_first(&dl, 3.5);
	dlist_print(dl);
	printf("%d\n", dl.length);
	dlist_remove_first(&dl, 3.5);
	dlist_print(dl);
	printf("%d\n", dl.length);
	dlist_prepend(&dl, 10);
	dlist_remove_all(&dl, 10);
	dlist_print(dl);
	printf("%d\n", dl.length);
	DList dl1;
	dlist_copy(dl, &dl1);
	dlist_print(dl1);
	printf("%d\n", dl1.length);
	dlist_remove_first(&dl, 2);
	dlist_concat(&dl, &dl1);
	dlist_print(dl);
	dlist_print(dl1);
	return 0;
}