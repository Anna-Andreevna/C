#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct Node { void* cont_pointer; int cont_size; Node* prev; Node* next; } Node;
typedef struct DList { Node* head; Node* tail; int length; } DList;

void dlist_create(DList* pdl) {
	*pdl = { NULL, NULL, 0 };
	return;
}

/*void dlist_print(DList dl) {
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
}*/

int dlist_prepend(DList* pdl, void* cont_p, int cont_s) {
	Node* n = (Node*)malloc(sizeof(Node));
	if (n == NULL) {
		printf("Memory error\n");
		return 1;
	}
	*n = { cont_p, cont_s, NULL, (*pdl).head };
	if ((*pdl).head == NULL) {
		(*pdl).head = n;
		(*pdl).tail = n;
	}
	else {
		(*pdl).head->prev = n;
		(*pdl).head = n;/**/
	}
	(*pdl).length++;
	return 0;
}

int dlist_append(DList* pdl, void* cont_p, int cont_s) {
	Node* n = (Node*)malloc(sizeof(Node));
	if (n == NULL) {
		printf("Memory error\n");
		return 1;
	}
	*n = { cont_p, cont_s, (*pdl).tail, NULL };
	if ((*pdl).tail == NULL) {
		(*pdl).head = n;
		(*pdl).tail = n;
	}
	else {
		(*pdl).tail->next = n;
		(*pdl).tail = n;/**/
	}
	(*pdl).length++;
	return 0;
}

Node* _dl_find_ind(DList dl, int ind) {
	if (ind >= dl.length) {
		return NULL;
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
	return p;
}

int dlist_insert(DList* pdl, void* cont_p, int cont_s, int ind) {
	if (ind > (*pdl).length) {
		printf("Input error\n");
		return 1;
	}
	if (ind == 0)
		return dlist_prepend(pdl, cont_p, cont_s);
	if (ind == (*pdl).length)
		return dlist_append(pdl, cont_p, cont_s);
	Node* n = (Node*)malloc(sizeof(Node));
	if (n == NULL) {
		printf("Memory error\n");
		return 1;
	}
	(*n).cont_pointer = cont_p;
	n->cont_size = cont_s;
	Node* p = _dl_find_ind(*pdl, ind - 1);
	(*n).prev = p;
	(*n).next = (*p).next;
	(*p).next = n;
	(*n).next->prev = n;
	(*pdl).length++;
	return 0;
}

int dlist_get(DList dl, int ind, void** res) {
	if (ind >= dl.length) {
		printf("Input error\n");
		return 1;
	}
	Node* p = _dl_find_ind(dl, ind);
	*res = (*p).cont_pointer;
	return 0;
}

void _dl_remove(DList* pdl, Node* p_node) {
	if (p_node == NULL)
		return;
	if (p_node->prev != NULL)
		p_node->prev->next = p_node->next;
	else
		pdl->head = p_node->next;
	if (p_node->next != NULL)
		p_node->next->prev = p_node->prev;
	else
		pdl->tail = p_node->prev;
	free(p_node);
	return;
}

int dlist_remove(DList* pdl, int ind, void** res) {
	if (ind >= pdl->length) {
		printf("Input error\n");
		return 1;
	}
	Node* p = _dl_find_ind(*pdl, ind);
	*res = (*p).cont_pointer;
	_dl_remove(pdl, p);
	(*pdl).length--;
	return 0;
}

int dlist_find(DList dl, void* data_p, int data_s) {
	Node* p = dl.head;
	int i = 0;
	while ((p != NULL) && (p->cont_size != data_s) && (memcmp(p->cont_pointer, data_p, data_s) != 0)) {
		i++;
		p = p->next;
	}
	if (p == NULL)
		i = -1;
	return i;
}

Node* _dl_find_from(DList dl, void* data_p, int data_s, Node* first_n) {
	Node* p = first_n;
	while ((p != NULL) && ((p->cont_size == data_s) && (memcmp(p->cont_pointer, data_p, data_s) != 0)))
		p = p->next;
	return p;
}

int dlist_remove_first(DList* pdl, void* data_p, int data_s) {
	Node* p = _dl_find_from(*pdl, data_p, data_s, pdl->head);
	if (p == NULL)
		return 1;
	_dl_remove(pdl, p);
	pdl->length--;
	return 0;
}

void dlist_remove_all(DList* pdl, void* data_p, int data_s) {
	Node* p = pdl->head;
	Node* aux;
	while (p != NULL) {
		aux = _dl_find_from(*pdl, data_p, data_s, p);
		if (aux == NULL)
			return;
		p = aux->next;
		_dl_remove(pdl, aux);
		pdl->length--;
	}
	return;
}

void dlist_clear(DList* pdl) {
	Node* p = pdl->head;
	Node* aux;
	while (p != NULL) {
		free(p->cont_pointer);
		aux = p;
		p = p->next;
		free(aux);
	}
	*pdl = { NULL, NULL, 0 };
	return;
}

int dlist_copy(DList old_dl, DList* new_dl) {
	new_dl->length = old_dl.length;
	if (old_dl.length == 0) {
		new_dl->head = new_dl->tail = NULL;
		return 0;
	}
	Node* curr_old = old_dl.head;
	Node* curr_new = NULL;
	Node** previous;
	previous = &(new_dl->head);
	while (curr_old != NULL) {
		curr_new = (Node*)malloc(sizeof(Node));
		if (curr_new == NULL) {
			printf("Memory error\n");
			dlist_clear(new_dl);
			return 1;
		}
		curr_new->cont_pointer = malloc(curr_old->cont_size);
		if (curr_new->cont_pointer == NULL) {
			dlist_clear(new_dl);
			printf("Memory error\n");
			return 1;
		}
		curr_new->cont_size = curr_old->cont_size;
		memcpy(curr_new->cont_pointer, curr_old->cont_pointer, curr_new->cont_size);
		curr_new->prev = *previous;
		if (*previous != NULL)
			curr_new->prev->next = curr_new;
		curr_old = curr_old->next;
	}
	curr_new->next = NULL;
	new_dl->tail = curr_new;
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

void dlist_foreach(DList dl, void (*pfun)(Node*)) {
	Node* p = dl.head;
	while (p != NULL) {
		(*pfun)(p);
		p = p->next;
	}
	return;
}

void dl_sqr(Node* n) {
	int* m = (int*)(n->cont_pointer);
	*m = *m * *m;
	return;
}

int dlist_find_custom(DList dl, int (*predicate)(Node*), void** ret_data) {
	Node* p = dl.head;
	int i = 0;
	while ((p != NULL) && (predicate(p) != 1)) {
		p = p->next;
		i++;
	}
	if (p == NULL)
		return -1;
	*ret_data = p->cont_pointer;
	return i;
}

int dl_pred(Node* n) {
	if ((n->cont_size == 4) && (*(int*)(n->cont_pointer) == 9))
		return 1;
	return 0;
}

void test_append() {
	DList dl;
	int* cont;
	Node* node;
	dlist_create(&dl);
	for (int i = 0; i < 10; i++) {
		cont = (int*)malloc(sizeof(int));
		if (cont == NULL) {
			printf("Memory error\n");
			dlist_clear(&dl);
			return;
		}
		*cont = i;
		dlist_append(&dl, (void*)cont, sizeof(int));
		assert(dl.length == i + 1);
	}
	node = dl.head;
	for (int i = 0; i < 10; i++) {
		assert(*(int*)(node->cont_pointer) == i);
		node = node->next;
	}
	node = dl.tail;
	for (int i = 9; i >= 0; i--) {
		assert(*(int*)(node->cont_pointer) == i);
		node = node->prev;
	}
	dlist_clear(&dl);
	return;
}

void test_prepend() {
	DList dl;
	int* cont;
	Node* node;
	dlist_create(&dl);
	for (int i = 0; i < 10; i++) {
		cont = (int*)malloc(sizeof(int));
		if (cont == NULL) {
			printf("Memory error\n");
			dlist_clear(&dl);
			return;
		}
		*cont = i;
		dlist_prepend(&dl, (void*)cont, sizeof(int));
		assert(dl.length == i + 1);
	}
	node = dl.head;
	for (int i = 9; i >= 0; i--) {
		assert(*(int*)(node->cont_pointer) == i);
		node = node->next;
	}
	node = dl.tail;
	for (int i = 0; i <10; i++) {
		assert(*(int*)(node->cont_pointer) == i);
		node = node->prev;
	}
	dlist_clear(&dl);
	return;
}

void test_insert() {
	DList dl;
	dlist_create(&dl);
	int* cont = (int*)malloc(sizeof(int));
	if (cont == NULL) {
		printf("Memory error\n");
		dlist_clear(&dl);
		return;
	}
	*cont = 0;
	dlist_insert(&dl, (void*)cont, sizeof(int), 0);
	assert(dl.length == 1);
	assert((*(int*)(dl.head->cont_pointer) == 0) && (*(int*)(dl.tail->cont_pointer) == 0));
	cont = (int*)malloc(sizeof(int));
	if (cont == NULL) {
		printf("Memory error\n");
		dlist_clear(&dl);
		return;
	}
	*cont = 1;
	dlist_insert(&dl, (void*)cont, sizeof(int), 1);
	assert(dl.length == 2);
	Node* p = _dl_find_ind(dl, 1);
	assert(*(int*)(p->cont_pointer) == 1);
	assert(p->prev->next == p);
	cont = (int*)malloc(sizeof(int));
	if (cont == NULL) {
		printf("Memory error\n");
		dlist_clear(&dl);
		return;
	}
	*cont = 2;
	dlist_insert(&dl, (void*)cont, sizeof(int), 1);
	assert(dl.length == 3);
	p = _dl_find_ind(dl, 1);
	assert(*(int*)(p->cont_pointer) == 2);
	assert((p->prev->next == p) && (p->prev == dl.head) && (*(int*)(p->prev->cont_pointer) == 0));
	assert((p->next->prev == p) && (p->next == dl.tail) && (*(int*)(p->next->cont_pointer) == 1));
	assert(dlist_insert(&dl, (void*)cont, sizeof(int), 5) == 1);
	assert(dl.length == 3);
	dlist_clear(&dl);
	return;
}

void test_remove() {
	DList dl;
	dlist_create(&dl);
	int* cont;
	for (int i = 0; i < 5; i++) {
		cont = (int*)malloc(sizeof(int));
		if (cont == NULL) {
			printf("Memory error\n");
			dlist_clear(&dl);
			return;
		}
		*cont = i;
		dlist_append(&dl, (void*)cont, sizeof(int));
	}
	void* res;
	assert(dlist_remove(&dl, 10, &res) == 1);
	assert(dl.length == 5);
	Node* node = _dl_find_ind(dl, 2);
	Node* prev = node->prev;
	Node* next = node->next;
	assert(dlist_remove(&dl, 2, &res) == 0);
	assert(dl.length == 4);
	assert(*(int*)res == 2);
	assert((prev->next == next) && (next->prev == prev));
	next = dl.head->next;
	assert(dlist_remove(&dl, 0, &res) == 0);
	assert(dl.length == 3);
	assert(*(int*)res == 0);
	assert((dl.head == next) && (next->prev == NULL));
	prev = dl.tail->prev;
	assert(dlist_remove(&dl, 2, &res) == 0);
	assert(dl.length == 2);
	assert(*(int*)res == 4);
	assert((dl.tail == prev) && (prev->next == NULL));
	dlist_clear(&dl);
	return;
}

void test_remove_first_and_all() {
	int arr[8] = { 1, 2, 1, 2, 1, 2, 1, 2 };
	int* cont;
	DList dl;
	dlist_create(&dl);
	for (int i = 0; i < 8; i++) {
		cont = (int*)malloc(sizeof(int));
		if (cont == NULL) {
			printf("Memory error\n");
			dlist_clear(&dl);
			return;
		}
		*cont = arr[i];
		dlist_append(&dl, (void*)cont, sizeof(int));
	}
	cont = (int*)malloc(sizeof(int));
	if (cont == NULL) {
		printf("Memory error\n");
		dlist_clear(&dl);
		return;
	}
	*cont = 3;
	assert(dlist_remove_first(&dl, (void*)cont, sizeof(int)) == 1);
	assert(dl.length == 8);
	*cont = 1;
	assert(dlist_remove_first(&dl, (void*)cont, sizeof(int)) == 0);
	assert(dl.length == 7);
	Node* p = dl.head;
	for (int i = 1; i < 8; i++) {
		assert(*(int*)(p->cont_pointer) == arr[i]);
		p = p->next;
	}
	dlist_remove_all(&dl, (void*)cont, sizeof(int));
	assert(dl.length == 4);
	p = dl.tail;
	for (int i = 0; i < 4; i++) {
		assert(*(int*)(p->cont_pointer) == 2);
		p = p->prev;
	}
	dlist_clear(&dl);
	return;
}

int main() {
	test_append();
	test_insert();
	test_prepend();
	test_remove();
	test_remove_first_and_all();
	return 0;
}