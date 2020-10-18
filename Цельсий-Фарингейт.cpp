#include <stdio.h>
float F_to_C(float F) {
	float C = (F - 32) * 5 / 9;
	return C;
}

float C_to_F(float C) {
	float F = C * 9 / 5 + 32;
	return F;
}

int main() {
	printf("Choose mode: 1 - from C to F, 2 - from F to C\n");
	int mode;
	scanf_s("%d", &mode);
	printf("Enter the number\n");
	float inp;
	float out;
	scanf_s("%f", &inp);
	if (mode == 1) {
		out = C_to_F(inp);
		printf("%f C = %f F", inp, out);
	}
	else
		if (mode == 2) {
			out = F_to_C(inp);
			printf("%f F = %f C", inp, out);
		}
		else
			printf("Incorrect mode\n");
	return 0;
}

