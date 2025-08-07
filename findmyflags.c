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
	char* decoded = (char*)base64_decode("Q1NVLVNMRU4tMzQ4Mw==", 20, NULL);
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
	char flag[14] = {67,83,85,45,83,76,69,78,45,57,54,55,55,0};
	if (strcmp(input, flag) != 0) fail();
}

void flag_6(char* input) {
	char to_xor[14] = {21,5,3,123,5,26,19,24,123,98,99,96,101,86};
	for (int i = 0; i < sizeof(to_xor); i++) {
		to_xor[i] ^= 86;
	}
	if (strcmp(input, to_xor) != 0) fail();
}

void flag_7(char* input) {
	char to_subtract_and_xor[14] = {153,169,171,67,169,164,155,166,67,76,76,77,75,88};
	for (int i = 0; i < sizeof(to_subtract_and_xor); i++) {
		to_subtract_and_xor[i] -= 55;
		to_subtract_and_xor[i] ^= 33;
	}
	if (strcmp(input, to_subtract_and_xor) != 0) fail();
}

void secret_flag(char* input) {
	uint8_t input_hash[16];
	md5String(input, input_hash);
	uint8_t flag_hash[16] = {0x48,0xec,0x3a,0x12,0xf8,0x1e,0xb5,0x84,0x4b,0x53,0x6b,0xd2,0xe9,0xb8,0x7a,0x21};
	if (memcmp(input_hash, flag_hash, 16) != 0) fail();
}

void get_user_input(char* output_buffer, size_t output_buffer_length) {
	FILE* terminal_input = stdin;
	fgets(output_buffer, output_buffer_length, terminal_input);
	size_t newline_location_to_remove = strcspn(output_buffer, "\n");
	output_buffer[newline_location_to_remove] = 0;
	size_t carriage_return_location_to_remove = strcspn(output_buffer, "\n");
	output_buffer[carriage_return_location_to_remove] = 0;
}

int main() {
	char user_input[100];
	const size_t max_user_input_length = sizeof(user_input);

	printf("Input Flag 1: ");
	get_user_input(user_input, max_user_input_length);
	flag_1(user_input);

	printf("Input Flag 2: ");
	get_user_input(user_input, max_user_input_length);
	flag_2(user_input);

	char user_input_3[100];
	const size_t max_user_input_3_length = sizeof(user_input_3);
	printf("Input Flag 3: ");
	get_user_input(user_input_3, max_user_input_3_length);
	flag_3(user_input_3);

	printf("Input Flag 4: ");
	get_user_input(user_input, max_user_input_length);
	flag_4(user_input);

	printf("Input Flag 5: ");
	get_user_input(user_input, max_user_input_length);
	flag_5(user_input);

	printf("Input Flag 6: ");
	get_user_input(user_input, max_user_input_length);
	flag_6(user_input);

	printf("Input Flag 7: ");
	get_user_input(user_input, max_user_input_length);
	flag_7(user_input);

	if (strlen(user_input_3) == 14 && user_input_3[13] == 'F') {
		printf("\nYou've unlocked a secret :)\nInput Flag 8: ");
		get_user_input(user_input, max_user_input_length);
		secret_flag(user_input);
	}

	printf("\nCongrats! You made it to the end!");

	return 0;
}