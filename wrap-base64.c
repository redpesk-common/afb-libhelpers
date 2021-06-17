/*
 Copyright (C) 2015-2021 IoT.bzh Company

 Author: José Bollo <jose.bollo@iot.bzh>

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

#include "wrap-base64.h"

int wrap_base64_encode(
		const uint8_t *data,
		size_t datalen,
		char **encoded,
		size_t *encodedlen,
		int width,
		int pad,
		int url)
{
	uint16_t u16 = 0;
	uint8_t u8 = 0;
	size_t in, out, rlen, n3, r3, iout, nout;
	int iw;
	char *result, c;

	/* compute unformatted output length */
	n3 = datalen / 3;
	r3 = datalen % 3;
	nout = 4 * n3 + r3 + !!r3;

	/* deduce formatted output length */
	rlen = nout;
	if (pad)
		rlen += ((~rlen) + 1) & 3;
	if (width)
		rlen += rlen / (unsigned)width;

	/* allocate the output */
	result = malloc(rlen + 1);
	if (result == NULL)
		return wrap_base64_nomem;

	/* compute the formatted output */
	iw = width;
	for (in = out = iout = 0 ; iout < nout ; iout++) {
		/* get in 'u8' the 6 bits value to add */
		switch (iout & 3) {
		case 0:
			u16 = (uint16_t)data[in++];
			u8 = (uint8_t)(u16 >> 2);
			break;
		case 1:
			u16 = (uint16_t)(u16 << 8);
			if (in < datalen)
				u16 = (uint16_t)(u16 | data[in++]);
			u8 = (uint8_t)(u16 >> 4);
			break;
		case 2:
			u16 = (uint16_t)(u16 << 8);
			if (in < datalen)
				u16 = (uint16_t)(u16 | data[in++]);
			u8 = (uint8_t)(u16 >> 6);
			break;
		case 3:
			u8 = (uint8_t)u16;
			break;
		}
		u8 &= 63;

		/* encode 'u8' to the char 'c' */
		if (u8 < 52) {
			if (u8 < 26)
				c = (char)('A' + u8);
			else
				c = (char)('a' + u8 - 26);
		} else {
			if (u8 < 62)
				c = (char)('0' + u8 - 52);
			else if (u8 == 62)
				c = url ? '-' : '+';
			else
				c = url ? '_' : '/';
		}

		/* put to output with format */
		result[out++] = c;
		if (iw && !--iw) {
			result[out++] = '\n';
			iw = width;
		}
	}

	/* pad the output */
	while (out < rlen) {
		result[out++] = '=';
		if (iw && !--iw) {
			result[out++] = '\n';
			iw = width;
		}
	}

	/* terminate */
	result[out] = 0;
	*encoded = result;
	*encodedlen = rlen;
	return wrap_base64_ok;
}

int wrap_base64_decode(
		const char *data,
		size_t datalen,
		uint8_t **decoded,
		size_t *decodedlen,
		int url)
{
	uint16_t u16;
	uint8_t u8, *result;
	size_t in, out, iin;
	char c;

	/* allocate enougth output */
	result = malloc(datalen + 1);
	if (result == NULL)
		return wrap_base64_nomem;

	/* decode the input */
	u16 = 0;
	for (iin = in = out = 0 ; in < datalen ; in++) {
		c = data[in];
		if (c == '=')
			break;
		if (c != '\n' && c != '\r') {
			if ('A' <= c && c <= 'Z')
				u8 = (uint8_t)(c - 'A');
			else if ('a' <= c && c <= 'z')
				u8 = (uint8_t)(c - 'a' + 26);
			else if ('0' <= c && c <= '9')
				u8 = (uint8_t)(c - '0' + 52);
			else if (c == '-' && url >= 0)
				u8 = (uint8_t)62;
			else if (c == '_' && url >= 0)
				u8 = (uint8_t)63;
			else if (c == '+' && url <= 0)
				u8 = (uint8_t)62;
			else if (c == '/' && url <= 0)
				u8 = (uint8_t)63;
			else {
				free(result);
				return wrap_base64_invalid;
			}
			if (!iin) {
				u16 = (uint16_t)u8;
				iin = 6;
			} else {
				u16 = (uint16_t)((u16 << 6) | u8);
				iin -= 2;
				u8 = (uint8_t)(u16 >> iin);
				result[out++] = u8;
			}
		}
	}
	while (in < datalen) {
		c = data[in++];
		if (c != '=' && c != '\n' && c != '\r') {
			free(result);
			return wrap_base64_invalid;
		}
	}
	/* terminate */
	*decoded = realloc(result, out + 1);
	if (out && *decoded == NULL) {
		free(result);
		return wrap_base64_nomem;
	}
	decoded[out] = 0; /* add zero at end to make life sweeter */
	*decodedlen = out;
	return wrap_base64_ok;
}
