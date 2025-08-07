#include "base64.h"
#include <stdlib.h>

static const char encode_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

char* base64_encode(const unsigned char* data, size_t data_len, size_t* encoded_len) {
	// sanity checks
	if (data == NULL) return NULL;
	if (data_len == 0) return NULL;

	// allocate 4 bytes of output for every 3 bytes of data, rounded up to a multiple of 4
	size_t to_alloc = ((data_len - 1) / 3) * 4 + 4;
	// allocate extra byte for null termination
	to_alloc++;
	// check for size_t overflow
	if (to_alloc < data_len) return NULL;
	// allocate encoded output
	char* encoded = malloc(to_alloc);
	// check if malloc failed
	if (encoded == NULL) return NULL;

	// prepare to iterate through data
	const unsigned char* data_end = data + data_len;
	const unsigned char* data_pos = data;
	char* encoded_pos = encoded;

	// while we are 3 bytes or more from the end of our data...
	while (data_end - data_pos >= 3) {
		// set 4 encoded bytes by mapping 3 data bytes to base64 table
		*encoded_pos++ = encode_table[data_pos[0] >> 2];
		*encoded_pos++ = encode_table[((data_pos[0] & 0x03) << 4) | (data_pos[1] >> 4)];
		*encoded_pos++ = encode_table[((data_pos[1] & 0x0f) << 2) | (data_pos[2] >> 6)];
		*encoded_pos++ = encode_table[data_pos[2] & 0x3f];
		// move to next 3 bytes of data
		data_pos += 3;
	}

	// if there are leftover bytes (data is not multiple of 3)...
	if (data_end - data_pos) {
		// encode one of the remaining data bytes
		*encoded_pos++ = encode_table[data_pos[0] >> 2];
		// if that was the last byte...
		if (data_end - data_pos == 1) {
			// finish encoding
			*encoded_pos++ = encode_table[(data_pos[0] & 0x03) << 4];
			// add padding
			*encoded_pos++ = '=';
		}
		// otherwise, if there are two remaining bytes...
		else {
			// finish encoding
			*encoded_pos++ = encode_table[((data_pos[0] & 0x03) << 4) | (data_pos[1] >> 4)];
			*encoded_pos++ = encode_table[(data_pos[1] & 0x0f) << 2];
		}
		// add padding
		*encoded_pos++ = '=';
	}

	// null terminate output
	*encoded_pos = 0;

	// set encoded output length if pointer was provided
	if (encoded_len != NULL) *encoded_len = encoded_pos - encoded;

	// return pointer to encoded output
	return encoded;
}

static const unsigned char decode_table[256] = {
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,
	 0,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,  0,  0,  0, 63,
	 0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
};

unsigned char* base64_decode(const char* encoded, size_t encoded_len, size_t* data_len) {
	// sanity checks
	if (encoded == NULL) return NULL;
	if (encoded_len == 0) return NULL;
	if (encoded_len % 4 != 0) return NULL;

	// allocate 3 bytes of data for every 4 bytes of encoded input
	size_t to_alloc = (encoded_len / 4) * 3;
	// allocate extra byte for null termination
	to_alloc++;
	// allocate data output
	unsigned char* data = malloc(to_alloc);
	// check if malloc failed
	if (data == NULL) return NULL;

	// prepare to iterate through encoded input
	const char* encoded_end = encoded + encoded_len;
	const char* encoded_pos = encoded;
	unsigned char* data_pos = data;

	// while we have not yet reached end...
	while (encoded_pos < encoded_end) {
		// set 3 data bytes by mapping 4 encoded bytes to decoding table
		int n = decode_table[encoded_pos[0]] << 18 | decode_table[encoded_pos[1]] << 12 | decode_table[encoded_pos[2]] << 6 | decode_table[encoded_pos[3]];
		*data_pos++ = n >> 16;
		// account for padding at end of string
		if (encoded_pos[2] != '=') *data_pos++ = n >> 8 & 0xFF;
		if (encoded_pos[3] != '=') *data_pos++ = n & 0xFF;
		// move to next 4 bytes of data
		encoded_pos += 4;
	}

	// null terminate output
	*data_pos = 0;

	// set data output length if pointer was provided
	if (data_len != NULL) *data_len = data_pos - data;

	// return pointer to data output
	return data;
}