#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void zfunction(int size, char* str, int* res) {
	res[0] = 0;
	for (int i = 1; i < size; i++) {
		int j = 0;
		while ((i + j < size) && (str[i + j] == str[j]))
			j++;
		res[i] = j;
	}
}

int search(int text_s, char* text, int patt_s, char* pattern, int* indexes) {
	indexes = (int*)malloc(sizeof(int));
	if (indexes == NULL) {
		printf("Mamory error\n");
		return 1;
	}
	*indexes = 0;
	if ((text == 0) || (patt_s == 0)) {
		return 0;
	}
	char* unit_str = (char*)malloc(text_s + patt_s + 1);
	if (unit_str == NULL) {
		printf("Mamory error\n");
		free(indexes);
		return 1;
	}
	int* zf = (int*)malloc((text_s + patt_s + 1) * sizeof(int));
	if (zf == NULL) {
		printf("Mamory error\n");
		free(indexes);
		free(unit_str);
		return 1;
	}
	zfunction(text_s + patt_s + 1, unit_str, zf);
	for (int i = patt_s + 1; i <= text_s + patt_s; i++) {
		if (zf[i] == patt_s) {
			indexes[0]++;
			int* temp = (int*)realloc(indexes, indexes[0]);
			if (temp == NULL) {
				printf("Mamory error\n");
				free(indexes);
				free(unit_str);
				free(zf);
				return 1;
			}
			indexes = temp;
			indexes[indexes[0]] = i;
		}
	}
	free(unit_str);
	free(zf);
}

int main() {
	int p;
	int* ind = &p;
	char text[] = "asdasdasda";
	char patt[] = "sd";
	int stat = search(10, text, 2, patt, ind);
	if (stat == 0) {
		assert(ind[0] == 3);
		int temp[] = { 1, 4, 7 };
		for (int i = 0; i < 3; i++)
			assert(ind[i + 1] == temp[i]);
	}
	/*char text[] = "aaaaa";
	char patt[] = "aa";
	int stat = search(5, text, 2, patt, ind);
	if (stat == 0) {
		assert(ind[0] == 4);
		int temp[] = { 1, 2, 3, 4 };
		for (int i = 0; i < 4; i++)
			assert(ind[i + 1] == temp[i]);
	}*/
}