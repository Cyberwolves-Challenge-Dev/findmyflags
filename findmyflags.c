#include "base64.h"
#include "md5.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void fail() {
	printf("\nIncorrect flag :(");
	exit(1);
}

void flag_1(char* input) {
	if (strcmp(input, "CSU-SLEN-2401") != 0) fail();
}

void flag_2(char* input) {
	char to_decode[21] = "Q1NVLVNMRU4tMzQ4Mw==";
	char* decoded = (char*)base64_decode(to_decode, 20, NULL);
	if (strcmp(input, decoded) != 0) fail();
	free(decoded);
}

void flag_3(char* input) {
	if (strlen(input) < 13) fail();

	char XXX_[5];
	strncpy(XXX_, input + 0, 4);
	XXX_[4] = 0;

	char XXXX_[6];
	strncpy(XXXX_, input + 4, 5);
	XXXX_[5] = 0;

	char XXXX[5];
	strncpy(XXXX, input + 9, 4);
	XXXX[4] = 0;

	if (strcmp(XXX_, "CSU-") != 0 || strcmp(XXXX_, "SLEN-") != 0 || strcmp(XXXX, "6588") != 0) fail();
}

void flag_4(char* input) {
	if (
		strlen(input) != 13 ||
		input[0] != 'C' ||
		input[1] != 'S' ||
		input[2] != 'U' ||
		input[3] != '-' ||
		input[4] != 'S' ||
		input[5] != 'L' ||
		input[6] != 'E' ||
		input[7] != 'N' ||
		input[8] != '-' ||
		input[9] != '8' ||
		input[10] != '1' ||
		input[11] != '1' ||
		input[12] != '1'
	) fail();
}

void flag_5(char* input) {
	char str[14] = {67,83,85,45,83,76,69,78,45,57,54,55,55,0};
	if (strcmp(input, str) != 0) fail();
}

void flag_6(char* input) {
	char str[14] = {21,5,3,123,5,26,19,24,123,98,99,96,101,86};
	for (int i = 0; i < sizeof(str); i++) {
		str[i] ^= 86;
	}
	if (strcmp(input, str) != 0) fail();
}

void flag_7(char* input) {
	char str[14] = {153,169,171,67,169,164,155,166,67,76,76,77,75,88};
	for (int i = 0; i < sizeof(str); i++) {
		str[i] -= 55;
		str[i] ^= 33;
	}
	if (strcmp(input, str) != 0) fail();
}

void secret_flag(char* input) {
	uint8_t hashed[16];
	md5String(input, hashed);
	uint8_t correct[16] = {0x48,0xec,0x3a,0x12,0xf8,0x1e,0xb5,0x84,0x4b,0x53,0x6b,0xd2,0xe9,0xb8,0x7a,0x21};
	if (memcmp(hashed, correct, 16) != 0) fail();
}

int main() {
	char input[100];

	printf("Input Flag 1: ");
	fgets(input, sizeof(input), stdin);
	input[strcspn(input, "\n")] = 0;
	input[strcspn(input, "\r")] = 0;
	flag_1(input);

	printf("Input Flag 2: ");
	fgets(input, sizeof(input), stdin);
	input[strcspn(input, "\n")] = 0;
	input[strcspn(input, "\r")] = 0;
	flag_2(input);

	char input_3[100];
	printf("Input Flag 3: ");
	fgets(input_3, sizeof(input_3), stdin);
	input_3[strcspn(input_3, "\n")] = 0;
	input_3[strcspn(input_3, "\r")] = 0;
	flag_3(input_3);

	printf("Input Flag 4: ");
	fgets(input, sizeof(input), stdin);
	input[strcspn(input, "\n")] = 0;
	input[strcspn(input, "\r")] = 0;
	flag_4(input);

	printf("Input Flag 5: ");
	fgets(input, sizeof(input), stdin);
	input[strcspn(input, "\n")] = 0;
	input[strcspn(input, "\r")] = 0;
	flag_5(input);

	printf("Input Flag 6: ");
	fgets(input, sizeof(input), stdin);
	input[strcspn(input, "\n")] = 0;
	input[strcspn(input, "\r")] = 0;
	flag_6(input);

	printf("Input Flag 7: ");
	fgets(input, sizeof(input), stdin);
	input[strcspn(input, "\n")] = 0;
	input[strcspn(input, "\r")] = 0;
	flag_7(input);

	if (strlen(input_3) == 14 && input_3[13] == 'F') {
		printf("\nYou've unlocked a secret :)\nInput Flag 8: ");
		fgets(input, sizeof(input), stdin);
		input[strcspn(input, "\n")] = 0;
		input[strcspn(input, "\r")] = 0;
		secret_flag(input);
	}

	printf("\nCongrats! You made it to the end!");

	return 0;
}

// gcc ../utils/base64.c ../utils/md5.c findmyflags.c -o findmyflags