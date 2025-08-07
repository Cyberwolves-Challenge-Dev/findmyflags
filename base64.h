#ifndef BASE64_H
#define BASE64_H

#include <stddef.h>

// Encodes data to base64
// @param data Data to be encoded
// @param data_len Length of data to be encoded
// @param encoded_len Pointer to output length variable, or NULL if not used
// @return Allocated buffer of encoded_len bytes containing base64 string, or NULL on failure
//
// Caller is responsible for freeing the returned buffer. Returned buffer is null terminated to make it easier to use as a C string. The null terminator is not included in encoded_len.
char* base64_encode(const unsigned char* data, size_t data_len, size_t* encoded_len);

// Decodes base64 to data
// @param encoded Base64 to be decoded
// @param encoded_len Length of base64 to be decoded
// @param data_len Pointer to output length variable, or NULL if not used
// @return Allocated buffer of data_len bytes containing decoded data, or NULL on failure
//
// Caller is responsible for freeing the returned buffer. Returned buffer is null terminated to make it easier to use as a C string. The null terminator is not included in data_len.
unsigned char* base64_decode(const char* encoded, size_t encoded_len, size_t* data_len);

#endif