/*
 * Copyright (C) 2021-2024 IoT.bzh Company
 * Author: Jose Bollo <jose.bollo@iot.bzh>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if AFB_BINDING_VERSION > 3

#define _GNU_SOURCE
#include <stdio.h>

#include "afb-data-utils.h"
#include "afb-req-utils.h"

void
afb_req_reply_data(
		afb_req_t req,
		int code,
		afb_data_t data
) {
	afb_req_reply(req, code, 1, &data);
}

void
afb_req_reply_json_c_hold(
		afb_req_t req,
		int code,
		json_object *jobject
) {
	afb_data_t data = afb_data_json_c_hold(jobject);
	afb_req_reply(req, code, 1, &data);
}

void
afb_req_reply_string_f(
		afb_req_t req,
		int code,
		const char *fmt,
		...
) {
	va_list ap;
	va_start(ap, fmt);
	afb_req_reply_string_v(req, code, fmt, ap);
	va_end(ap);
}

void
afb_req_reply_string_v(
		afb_req_t req,
		int code,
		const char *fmt,
		va_list args
) {
	char *str;
	int len;

	if (fmt != NULL && (len = vasprintf(&str, fmt, args)) >= 0)
		afb_req_reply_string_hold(req, code, str, (size_t)len);
	else
		afb_req_reply(req, code, 0, NULL);
}

void
afb_req_reply_string_copy(
		afb_req_t req,
		int code,
		const char *string,
		size_t length
) {
	afb_data_t data = afb_data_string_copy(string, length);
	afb_req_reply(req, code, 1, &data);
}

void
afb_req_reply_string_hold(
		afb_req_t req,
		int code,
		char *string,
		size_t length
) {
	afb_data_t data = afb_data_string_hold(string, length);
	afb_req_reply(req, code, 1, &data);
}

void
afb_req_reply_string_wrap(
		afb_req_t req,
		int code,
		const char *string,
		size_t length
) {
	afb_data_t data = afb_data_string_wrap(string, length);
	afb_req_reply(req, code, 1, &data);
}

void
afb_req_reply_string(
		afb_req_t req,
		int code,
		const char *string
) {
	afb_data_t data = afb_data_string(string);
	afb_req_reply(req, code, 1, &data);
}

#endif

