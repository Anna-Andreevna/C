#include <stdio.h>
#include <stdlib.h>
const char err[13] = "Memory Error";
int merge_sort(int *init_arr, int left, int right) {
	if (left >= right)
		return 0;
	int f = 1;
	static int* t_arr = (int*)malloc((right - left + 1) * sizeof(int));
	if (t_arr == NULL) {
		printf("%s\n\n", err);
		return 1;
	}
	int mid = (left + right) / 2;
	merge_sort(init_arr, left, mid);
	merge_sort(init_arr, mid + 1, right);
	int i = left, j = mid +1;
	for (int k = left; k <= right; k++) {
		if ((j > right) || ((i <= mid) && (init_arr[i] <= init_arr[j]))) {
			t_arr[k] = init_arr[i];
			i++;
		}
		else {
			t_arr[k] = init_arr[j];
			j++;
		}
	}
	for (int k = left; k <= right; k++)
		init_arr[k] = t_arr[k];
	if (f == 0)
		free(t_arr);
	return 0;
}

int main() {
	int size = 0;
	int crutch = 0;
	int* arr = &crutch;
	FILE* f;
	fopen_s(&f, "data.txt", "r");
	if (f == NULL) {
		scanf_s("%d", &size);
		arr = (int*)malloc(size * sizeof(int));
		if (arr == NULL) {
			printf("Memory Error");
			return 0;
		}
		for (int i = 0; i < size; i++)
			scanf_s("%d", &arr[i]);
		for (int i = 0; i < size; i++)
			printf("%d ", arr[i]);
		printf("\n");
		int stat;
		stat = merge_sort(arr, 0, size - 1);
		if (stat == 0)
			for (int i = 0; i < size; i++)
				printf("%d ", arr[i]);
		free(arr);
	}
	else {
		fscanf_s(f, "%d", &size);
		char c = fgetc(f);
		if (c == EOF) {
			if (size != 0)
				printf("Error: lack of input arguments\n");
			return 0;
		}
		while (c != EOF) {
			if (size != 0) {
				arr = (int*)malloc(size * sizeof(int));
				if (arr == NULL) {
					printf("Memory error");
					fclose(f);
					return 0;
				}
			}
			for (int i = 0; i < size; i++) {
				if (c == EOF) {
					printf("Error: lack of input arguments\n");
					free(arr);
					fclose(f);
					return 0;
				}
				fscanf_s(f, "%d", &arr[i]);
				c = fgetc(f);
			}
			for (int i = 0; i < size; i++)
				printf("%d ", arr[i]);
			printf("\n");
			int stat = merge_sort(arr, 0, size - 1);
			if (stat == 0)
				for (int i = 0; i < size; i++)
					printf("%d ", arr[i]);
			printf("\n\n");
			if (size != 0) 
				free(arr);
			if (c != EOF) {
				fscanf_s(f, "%d", &size);
				c = fgetc(f);
			}
		}
		fclose(f);
	}

	return 0;
}
