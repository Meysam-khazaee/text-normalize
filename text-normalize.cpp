#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <cstdint>
#include "text-normalize.h"

TextNormalize::TextNormalize(unsigned char* out, size_t out_len)
{
	this->out = out;
	this->out_len = out_len;
	this->out_pos = 0;
	this->space_written = 0;
}

void TextNormalize::normalizeReset()
{
	this->out_pos = 0;
	this->space_written = 0;
}

enum normalizeAction {
	NORMALIZE_COPY,				// no operation, all characters 0x20 < c < 0x80, that are not A32 and WSP
	NORMALIZE_SKIP,				// ignore normalize, control character < 0x20 that are not whitespace, and all > 0x7f
	NORMALIZE_AS_WHITESPACE,	// ignore consecutive whitespace: \t, \n, \f, \v, \r, [ ]
	NORMALIZE_ADD_32			// all uppercase characters to lowercase
};

/* use shorter names in the table */
#define IGN NORMALIZE_SKIP 
#define WSP NORMALIZE_AS_WHITESPACE
#define A32 NORMALIZE_ADD_32
#define NOP NORMALIZE_COPY

static const enum normalizeAction charAction[256] = {
	IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, WSP, WSP, WSP, WSP, WSP, IGN, IGN,
	IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN,
	WSP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, /* 0x20 - 0x2f */
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, A32, A32, A32, A32, A32, A32, A32, A32, A32, A32, A32, A32, A32, A32, A32,
	A32, A32, A32, A32, A32, A32, A32, A32, A32, A32, A32, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP,
	NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, NOP, /* 0x70 - 0x7f */
	IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN,
	IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN,
	IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN,
	IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN,
	IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN,
	IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN,
	IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN,
	IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN, IGN };

/* Normalizes the text at @buf of length @buf_len, @buf can include \0 characters.
 * Stores the normalized text in @state's buffer.
 * Returns how many bytes it consumed of the input. */
size_t TextNormalize::normalizeBuffer(const unsigned char* buffer, const size_t bufLength)
{
	size_t i;
	const unsigned char* out_end = this->out + this->out_len;
	unsigned char* p = this->out + this->out_pos;

	if (NULL == buffer) {
		return 0;
	}

	for (i = 0; i < bufLength && p < out_end; i++) {
		unsigned char c = buffer[i];
		switch (charAction[c]) {
		case NORMALIZE_SKIP:
			continue;
		case NORMALIZE_AS_WHITESPACE:
			/* convert consecutive whitespaces to a single space */
			if (!this->space_written) {
				*p++ = ' ';
			}
			this->space_written = 1;
			continue;
		case NORMALIZE_ADD_32:
			/* aka uppercase to lowercase */
			c += 32;
			/* fall through */
		case NORMALIZE_COPY:
			this->space_written = 0;
			*p++ = c;
		}
	}
	this->out_pos = p - this->out;
	return i;
}

/* Normalizes the text in @fmap and stores the result in @state's buffer.
 * Returns number of characters written to buffer. */
 //size_t TextNormalize::normalizeMap(fmap_t* map, size_t offset)
 //{
 //	const unsigned char* map_loc;
 //	unsigned int map_pgsz;
 //	uint64_t map_len;
 //	size_t buff_len;
 //	size_t acc;
 //	size_t acc_total;
 //	size_t acc_len;
 //
 //	map_len = map->len;
 //	map_pgsz = map->pgsz;
 //	buff_len = this->out_len;
 //
 //	acc_total = 0;
 //	acc = 0;
 //
 //	while (1) {
 //		/* Break out if we've reached the end of the map or our buffer. */
 //		if (!(acc_len = MIN_3(map_pgsz, map_len - offset, buff_len - acc_total))) break;
 //
 //		/* If map_loc is NULL, then there's nothing left to do but recover. */
 //		if (!(map_loc = (const unsigned char*)fmap_need_off_once(map, offset, acc_len))) break;
 //		offset += acc_len;
 //
 //		/* If we didn't normalize anything, no need to update values, just break out. */
 //		if (!(acc = normalizeBuffer(map_loc, acc_len))) break;
 //		acc_total += acc;
 //	}
 //
 //	return acc_total;
 //}
