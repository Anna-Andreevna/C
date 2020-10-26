#include <stdio.h>
#include <ctype.h>
int parse_number(char* string, int base, int* res) {
	if (base <= 1)
		return 1;
	int limits[4] = { '0', -1, 0, -1 };
	if (base <= 10)
		limits[1] = '0' + base;
	else {
		limits[1] = '9';
		limits[2] = 'A';
		limits[3] = 'A' - 11 + base;
	}
	int i = 0;
	while (isspace(string[i]))
		i++;
	int sign = 1;
	if (string[i] == '-')
		sign = -1;
	else if (string[i] != '+')
		i--;
	i++;
	int is_ok = 1;
	int result = 0;
	while (string[i] != '\0') {
		if ((limits[0] <= (int)string[i]) &&  ((int)string[i] <= limits[1]))
			result = result * base + (int)string[i] - (int)'0';
		else if ((limits[2] <= (int)string[i]) && ((int)string[i] <= limits[3]))
			result = result * base + (int)string[i] - (int)'A' + 10;
		else {
			is_ok = 0;
			break;
		}
		i++;
	}
	if (is_ok == 0)
		return 1;
	*res = result * sign;
	return 0;
}

int main() {
	printf("Enter string\n");
	char string[100];
	scanf_s("%s", string, 100);
	int base;
	printf("Enter the base: ");
	scanf_s("%d", &base);
	int res = 0;
	int stat = parse_number(string, base, &res);
	if (stat != 0)
		printf("Error\n");
	else 
		printf("%d", res);
	return 0;
}