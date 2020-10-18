#include <stdio.h>
#include <ctype.h>
int parse_number(char* string, int base) {
	int limits[4] = { 48, -1, 0, -1 };
	if (base <= 10)
		limits[1] = 47 + base;
	else {
		limits[1] = 57;
		limits[2] = 65;
		limits[3] = 54 + base;
	}
	int is_ok = 1;
	int result = 0;
	int i = 0;
	while (isspace(string[i]))
		i++;
	int sign = 1;
	if (string[i] == '-')
		sign = -1;
	else if (string[i] != '+')
		i--;
	i++;
	while (string[i] != '\0') {
		if ((limits[0] <= (int)string[i]) &&  ((int)string[i] <= limits[1]))
			result = result * base + (int)string[i] - 48;
		else if ((limits[2] <= (int)string[i]) && ((int)string[i] <= limits[3]))
			result = result * base + (int)string[i] - 54;
		else {
			is_ok = 0;
			break;
		}
		i++;
	}
	if (is_ok == 0) {
		printf("Error\n");
		return 0;
	}
	result *= sign;
	return result;
}

int main() {
	printf("Enter string\n");
	char string[100];
	scanf_s("%s", string, 100);
	int base;
	printf("Enter the base: ");
	scanf_s("%d", &base);
	if (base <= 1)
		printf("Error\n");
	else {
		int res = parse_number(string, base);
		printf("%d", res);
	}
	return 0;
}