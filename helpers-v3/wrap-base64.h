/*
 Copyright (C) 2015-2025 IoT.bzh Company

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

#pragma once

#include <stdlib.h>
#include <stdint.h>

#define wrap_base64_ok        0
#define wrap_base64_nomem     -1
#define wrap_base64_invalid   -2

/**
 * encode a buffer of data as a fresh allocated base64 string
 *
 * @param data        pointer to the head of the data to encode
 * @param datalen     length in byt of the data to encode
 * @param encoded     pointer to a pointer receiving the encoded value
 * @param encodedlen  pointer to a size receiving the encoded size
 * @param width       width of the lines or zero for one long line
 * @param pad         if not zero pads with = according standard
 * @param url         if not zero emit url variant of base64
 *
 * @return wrap_base64_ok in case of success
 *    or wrap_base64_nomem if allocation memory failed
 */
extern
int wrap_base64_encode(
		const uint8_t *data,
		size_t datalen,
		char **encoded,
		size_t *encodedlen,
		int width,
		int pad,
		int url);

/**
 * decode a base64 string as a fresh buffer of data
 *
 * @param data        pointer to base64 string to be decode
 * @param datalen     length of the string to decode
 * @param encoded     pointer to a pointer receiving the decoded value
 * @param encodedlen  pointer to a size receiving the decoded size
 * @param url         indicates processing of variants
 *                     - url = 0: any variant (even mixed)
 *                     - url > 0: only url variant
 *                     - url < 0: only standard variant
 *
 * @return wrap_base64_ok in case of success,
 *  wrap_base64_nomem if allocation memory failed,
 *  or wrap_base64_invalid if the data isn't a valid base64 input
 */
extern
int wrap_base64_decode(
	const char *data,
	size_t datalen,
	uint8_t **decoded,
	size_t *decodedlen,
	int url);
