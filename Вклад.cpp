#include <stdio.h>
void simple_int(float sum, int time, float interest) {
	float c_sum = sum;
	float profit = sum * interest / 100 / 12;
	printf("Month Current summ\n");
	for (int i = 0; i <= time; i++) {
		printf("%5d %12.2f\n", i, c_sum);
		c_sum += profit;
	}
	return;
}

void compound_int(float sum, int time, float interest) {
	float c_sum = sum;
	float profit_coeff = interest / 100 / 12 + 1;
	printf("Month Current summ\n");
	for (int i = 0; i <= time; i++) {
		printf("%5d %12.2f\n", i, c_sum);
		c_sum *= profit_coeff;
	}
	return;
}

int main() {
	float init_sum;
	int time;
	float interest;
	int type;
	int conf = 1;
	do {
		printf("Enter the initial sum: ");
		scanf_s("%f", &init_sum);
		printf("Enter the period in months: ");
		scanf_s("%d", &time);
		printf("Enter the annual interest: ");
		scanf_s("%f", &interest);
		printf("Coose deposit type: 0 - simple interest, 1 - compound interest\n");
		scanf_s("%d", &type);
		if (type == 0)
			simple_int(init_sum, time, interest);
		else if (type == 1)
			compound_int(init_sum, time, interest);
		else
			printf("Incorrect type\n");
		printf("If you want to redo, print 1, otherwise 0\n");
		scanf_s("%d", &conf);
	} while (conf == 1);
	return 0;
}