#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

void zfunction(int size, char* str, int* zf) {
	zf[0] = 0;
	int left = 0;
	int right = 0;
	for (int i = 1; i < size; i++){
		zf[i] = 0;
		zf[i] = (int)fmaxf(0, fminf(zf[i - left], right - i + 1));
		/*в статье нет + 1, но если я не ошиблась, то её отсутствие создаёт одну бесполезную операцию сравнения*/
		while ((i + zf[i] < size) && (str[i + zf[i]] == str[zf[i]]))
			zf[i]++;
		if (i + zf[i] > right) {
			left = i;
			right = i + zf[i] - 1;
		}
	}
	return;
}

int search(char* text, int text_s, char* pattern, int patt_s, int** p_ind) {
	int* res = (int*)malloc(sizeof(int));
	if (res == NULL) {
		printf("Memory error\n");
		return 1;
	}
	res[0] = 0;
	if ((text_s == 0) || (patt_s == 0) || (text_s < patt_s)) {
		p_ind[0] = res;
		return 0;
	}
	char* unit_str = (char*)malloc((text_s + patt_s + 1)*sizeof(char));
	if (unit_str == NULL) {
		printf("Memory error\n");
		free(res);
		return 1;
	}
	int* zf = (int*)malloc((text_s + patt_s + 1) * sizeof(int));
	if (zf == NULL) {
		printf("Memory error\n");
		free(res);
		free(unit_str);
		return 1;
	}
	for (int i = 0; i < patt_s; i++)
		unit_str[i] = pattern[i];
	unit_str[patt_s] = '#';
	for (int i = 0; i < text_s; i++)
		unit_str[i + patt_s + 1] = text[i];
	zfunction(text_s + patt_s + 1, unit_str, zf);
	for (int i = patt_s + 1; i <= text_s + patt_s; i++) {
		if (zf[i] == patt_s) {
			res[0]++;
			int* temp = (int*)realloc(res, (res[0] + 1)*sizeof(int));
			if (temp == NULL) {
				printf("Memory error\n");
				free(res);
				free(unit_str);
				free(zf);
				return 1;
			}
			res = temp;
			int k = res[0];
			res[k] = i - patt_s - 1;
		}
	}
	free(unit_str);
	free(zf);
	p_ind[0] = res;
	return 0;
}

int main() {
	int* ind = NULL;
	char text[5][15] = { "asdasdasda", "aaaaaaa", "", "ab", "ab" };
	int t_s[5] = { 10, 7, 0, 2, 2 };
	char patt[5][5] = { "sd", "aa", "", "c", "abc" };
	int p_s[5] = { 2, 2, 0, 1, 3 };
	int check[5][7] = { {3, 1, 4, 7}, {6, 0, 1, 2, 3, 4, 5}, {0}, {0}, {0} };
	int stat;
	for (int j = 0; j < 5; j++) {
		stat = search(text[j], t_s[j], patt[j], p_s[j], &ind);
		if (stat == 0) {
			assert(ind[0] == check[j][0]);
			for (int i = 1; i <= ind[0]; i++) {
				printf("%d ", ind[i]);
				assert(ind[i] == check[j][i]);
			}
			printf("\n");
		}
		free(ind);
	}
}