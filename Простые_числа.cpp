#include <stdio.h>
int main() {
	int N;
	printf("Enter the number\n");
	scanf_s("%d", &N);
	for (int i = 2; i <= N; i++) {
		int f = 1;
		for (int j = 2; j <= i / 2; j++) {
			if (i % j == 0) {
				f = 0;
				break;
			}
		}
		if (f)
			printf("%d ", i);
	}
	return 0;
}