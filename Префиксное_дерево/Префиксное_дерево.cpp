#include <stdio.h>
#include <stdlib.h>

#define M_ERR printf("Memory error at %s:%d\n", __FILE__, __LINE__)
#define NULL_PTR_ERR printf("Pointer is NULL at %s:%d\n", __FILE__, __LINE__)
//#define ASSERT_EQUAL_INT(actual, expected) do { int a = (actual); int b = (expected); \
		if (a != b) { printf("TEST FALLED %d != %d at %d line", a, b, __LINE__); break; } } while (0)
#define ASSERT_EQUAL(expected, actual) do { if ((actual) != (expected)) { printf("TEST FALLED at %d line\n", __LINE__); break; } } while (0)

typedef void (*Destructor)(void* data);
typedef void* (*Updater)(void* old, void* next);
typedef void (*Enumerator)(const char* key, void* data, void* user);

typedef struct Branch { void* ptr; char symbol; } Branch;

typedef struct Node { 
	Node* father;
	Branch* children;
	int child_num;
	int is_key;
	void* data;
} Node;

typedef struct Trie { Node* root; Destructor dstr; } Trie;

static Node* _create_node(Node* father, int is_key) {
	Node* ptr = (Node*)malloc(sizeof(Node));
	if (ptr == NULL) { return NULL;	}
	ptr->father = father;
	ptr->children = NULL;
	ptr->child_num = 0;
	ptr->is_key = is_key;
	ptr->data = NULL;
	return ptr;
}

static Node* _find_son(const Node* node, const char desc_symb) {
	if (node->child_num == 0) { return NULL; }
	Branch* br = node->children;
	for (int i = 0; i < node->child_num; i++) {
		if (br[i].symbol == desc_symb) { return (Node*)(br[i].ptr); }
		if (br[i].symbol > desc_symb) { return NULL; }
	}
	return NULL;
}

static int _find_nearest(const Trie* tree, const char* key, Node** res) { // возвращает длину совпадающей подстроки
	if ((tree == NULL) || (key == NULL)) { return 0; }
	int i = 0;
	Node* curr_n = tree->root;
	while (key[i] != '\0') {
		*res = curr_n;
		curr_n = _find_son(curr_n, key[i]);
		if (curr_n == NULL) { return i; }
		i++;
	}
	*res = curr_n;
	return i;
}

static int _add_son(Node* father, Node* son, char symb) {
	Branch* new_children = (Branch*)malloc((father->child_num + 1) * sizeof(Branch));
	if (new_children == NULL) { return 1; }
	int i = 0;
	while ((i < father->child_num) && ((father->children)[i].symbol < symb)) {
		new_children[i] = father->children[i];
		i++;
	}
	new_children[i] = { son, symb };
	while (i < father->child_num) {
		new_children[i + 1] = father->children[i];
		i++;
	}
	free(father->children);
	father->children = new_children;
	father->child_num++;
	return 0;
}

static int _del_node(Node* node) {
	Node* father = node->father;
	if (father == NULL) { return 1; }
	if (father->child_num == 1) { father->children = NULL; }
	else {
		Branch* new_children = (Branch*)malloc((father->child_num - 1) * sizeof(Branch));
		if ((new_children == NULL)) { return -1; }
		int i = 0;
		while ((Node*)(father->children)[i].ptr != node) {
			new_children[i] = (father->children)[i];
			i++;
		}
		i++;
		while (i < father->child_num) {
			new_children[i - 1] = (father->children)[i];
			i++;
		}
		free(father->children);
		father->children = new_children;
	}
	free(node);
	father->child_num--;
	return 0;
}

static void _rec_traverse(Node* node, char** key, int depth, Enumerator en, void* user, int* stat) {
	if (node->is_key == 1) { en(*key, node->data, user); }
	if (node->child_num != 0) {
		char* n_key = (char*)realloc(*key, depth + 2);
		if (n_key == NULL) {
			*stat = 1;
			return;
		}
		*key = n_key;
		(*key)[depth + 1] = '\0';
		for (int i = 0; i < node->child_num; i++) {
			(*key)[depth] = (node->children)[i].symbol;
			_rec_traverse((Node*)(node->children)[i].ptr, key, depth + 1, en, user, stat);
			if (*stat != 0) { return; }
		}
		n_key = (char*)realloc(*key, depth + 1);
		if (n_key == NULL) {
			*stat = 1;
			return;
		}
		*key = n_key;
		(*key)[depth] = '\0';
	}
	return;
}

Trie* trie_create(Destructor dstr = NULL) {
	Trie* tree = (Trie*)malloc(sizeof(Trie));
	if (tree == NULL) { return NULL; }
	tree->root = _create_node(NULL, 0);
	if (tree->root == NULL) {
		M_ERR;
		free(tree);
		return NULL;
	}
	tree->dstr = dstr;
	return tree;
}

void trie_free(Trie* tree) {
	Node* curr_n = tree->root;
	Node* father;
	while (curr_n != NULL) {
		if (curr_n->child_num != 0) { 
			curr_n = (Node*)(curr_n->children)[curr_n->child_num - 1].ptr; 
		}
		else {
			if (curr_n->children != NULL) { free(curr_n->children); }
			father = curr_n->father;
			if (curr_n->is_key == 1) { free(curr_n->data); }
			free(curr_n);
			curr_n = father;
			if (curr_n != NULL) { curr_n->child_num--; }
		}
	}
	free(tree);
	return;
}

int trie_has(const Trie* tree, const char* key) {
	if ((tree == NULL) || (key == NULL)) {
		NULL_PTR_ERR;
		return 0;
	}
	Node* curr_n = NULL;
	int len = _find_nearest(tree, key, &curr_n);
	if ((key[len] == '\0') && (curr_n->is_key == 1)) { return 1; }
	else { return 0; }
}

void* trie_get(const Trie* tree, const char* key) {
	if ((tree == NULL) || (key == NULL)) {
		NULL_PTR_ERR;
		return 0;
	}
	Node* curr_n = NULL;
	int len = _find_nearest(tree, key, &curr_n);
	if ((key[len] == '\0') && (curr_n->is_key == 1)) { return curr_n->data; }
	else { return NULL; }
}

int trie_set(Trie* tree, const char* key, void* data) {
	if ((tree == NULL) || (key == NULL)) {
		NULL_PTR_ERR;
		return -1;
	}
	Node* curr_n = NULL;
	Node* son;
	int len = _find_nearest(tree, key, &curr_n);
	while (key[len] != '\0') {
		son = _create_node(curr_n, 0);
		if ((son == NULL)) {
			M_ERR;
			trie_free(tree);
			free(tree);
			return -1;
		}
		if (_add_son(curr_n, son, key[len]) != 0) {
			M_ERR;
			trie_free(tree);
			free(tree);
			free(son);
			return -1;
		}
		curr_n = son;
		len++;
	}
	if (curr_n->is_key == 1) {
		if (tree->dstr != NULL) { tree->dstr(curr_n->data); }
		free(curr_n->data);
		curr_n->data = data;
		return 1;
	}
	else {
		curr_n->is_key = 1;
		curr_n->data = data;
		return 0;
	}
}

int trie_update(Trie* tree, const char* key, Updater up, void* next) {
	if ((tree == NULL) || (key == NULL)) {
		NULL_PTR_ERR;
		return -1;
	}
	Node* curr_n = NULL;
	Node* son;
	int len = _find_nearest(tree, key, &curr_n);
	while (key[len] != '\0') {
		son = _create_node(curr_n, 0);
		if ((son == NULL)) {
			M_ERR;
			trie_free(tree);
			free(tree);
			return -1;
		}
		if (_add_son(curr_n, son, key[len]) != 0) {
			M_ERR;
			trie_free(tree);
			free(tree);
			free(son);
			return -1;
		}
		curr_n = son;
		len++;
	}
	if (curr_n->is_key == 1) {
		curr_n->data = up(curr_n->data, next);
		return 1;
	}
	else {
		curr_n->is_key = 1;
		curr_n->data = up(NULL, next);
		return 0;
	}
}

int trie_delete(Trie* tree, const char* key) {
	if ((tree == NULL) || (key == NULL)) {
		NULL_PTR_ERR;
		return -1;
	}
	Node* curr_n = NULL;
	int len = _find_nearest(tree, key, &curr_n);
	if ((key[len] != '\0') || (curr_n->is_key == 0)) { return 1; }
	curr_n->is_key = 0;
	if (tree->dstr != NULL) { tree->dstr(curr_n->data); }
	free(curr_n->data);
	Node* father = curr_n->father;
	while ((curr_n->child_num == 0) && (curr_n->is_key == 0) && (father != NULL)) {
		_del_node(curr_n);
		curr_n = father;
		father = curr_n->father;
	}
	return 0;
}

int trie_traverse(const Trie* tree, Enumerator en, void* user) {
	char* first_key = (char*)malloc(sizeof(char));
	if (first_key == NULL) {
		M_ERR;
		return 1;
	}
	*first_key = '\0';
	int stat = 0;
	_rec_traverse(tree->root, &first_key, 0, en, user, &stat);
	free(first_key);
	return stat;
}

typedef struct XY { int x; int y; } XY;

void dest(void* data) { printf("(%d, %d) was deleted\n", ((XY*)data)->x, ((XY*)data)->y); }
void en(const char* key, void* data, void* user) { printf("%s: (%d, %d)\n", key, ((XY*)data)->x, ((XY*)data)->y); }
void* up(void* old, void* next) {
	if (old != NULL) {
		((XY*)old)->y += *(int*)next;
		return old;
	}
	else {
		void* new_dat = malloc(sizeof(XY));
		if (new_dat == NULL) {
			M_ERR;
			return NULL;
		}
		*(XY*)new_dat = { 0, *(int*)next };
		return new_dat;
	}
}

int main() {
	Trie* tr = trie_create(&dest);
	XY* v[5];
	for (int i = 0; i < 4; i++) {
		v[i] = (XY*)malloc(sizeof(XY));
		if (v[i] == NULL) { M_ERR; break; }
		*v[i] = { i * 2 + 1, i * 2 + 2 };
	}
	ASSERT_EQUAL(0, trie_set(tr, "abc", v[0]));
	ASSERT_EQUAL(0, trie_set(tr, "ad", v[1]));
	ASSERT_EQUAL(0, trie_set(tr, "bs", v[2]));
	trie_traverse(tr, en, NULL);
	ASSERT_EQUAL(1, trie_has(tr, "abc"));
	ASSERT_EQUAL(v[0], trie_get(tr, "abc"));
	ASSERT_EQUAL(1, trie_has(tr, "ad"));
	ASSERT_EQUAL(v[1], trie_get(tr, "ad"));
	ASSERT_EQUAL(1, trie_has(tr, "bs"));
	ASSERT_EQUAL(v[2], trie_get(tr, "bs"));
	ASSERT_EQUAL(0, trie_has(tr, "qwe"));
	ASSERT_EQUAL(NULL, trie_get(tr, "qwe"));
	ASSERT_EQUAL(1, trie_set(tr, "ad", v[3]));
	ASSERT_EQUAL(1, trie_has(tr, "ad"));
	ASSERT_EQUAL(v[3], trie_get(tr, "ad"));
	int n = 2;
	ASSERT_EQUAL(0, trie_update(tr, "adg", up, &n));
	ASSERT_EQUAL(1, trie_has(tr, "adg"));
	ASSERT_EQUAL(0, ((XY*)trie_get(tr, "adg"))->x);
	ASSERT_EQUAL(n, ((XY*)trie_get(tr, "adg"))->y);
	ASSERT_EQUAL(1, trie_update(tr, "abc", up, &n));
	ASSERT_EQUAL(1, trie_has(tr, "abc"));
	ASSERT_EQUAL(1, ((XY*)trie_get(tr, "abc"))->x);
	ASSERT_EQUAL(2 + n, ((XY*)trie_get(tr, "abc"))->y);
	trie_traverse(tr, en, NULL);
	ASSERT_EQUAL(0, trie_delete(tr, "abc"));
	ASSERT_EQUAL(0, trie_has(tr, "abc"));
	ASSERT_EQUAL(1, trie_delete(tr, "abd"));
	ASSERT_EQUAL(0, trie_has(tr, "abd"));
	trie_traverse(tr, en, NULL);
	trie_free(tr);
	return 0;
}