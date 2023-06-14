#pragma once

#include <stdint.h>
#include <glob.h>
#include <stdbool.h>

typedef struct RLE RLE;

RLE* create_rle();
void delete_rle(RLE* rle);

void encode_rle(RLE* rle, const char* data, size_t size);
char* decode_rle(RLE* rle, size_t* size);

char* serialize_rle(RLE* rle, size_t* size);
void deserialize_rle(RLE* rle, const char* data, size_t size);

void print_rle(RLE* rle, uint8_t counts_per_line);
