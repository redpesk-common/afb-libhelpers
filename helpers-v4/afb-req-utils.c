/*
 * Copyright (C) 2021 "IoT.bzh"
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
afb_req_reply_objstr_v(
		afb_req_t req,
		struct json_object *obj,
		const char *error,
		const char *info,
		va_list ap
) {
	char *str;
	struct json_object *content = json_object_new_object();
	afb_data_t data;

	if (obj != NULL)
		json_object_object_add(content, "data", obj);

	if (error != NULL)
		json_object_object_add(content, "error", json_object_new_string(error));

	if (info != NULL && vasprintf(&str, info, ap) >= 0) {
		json_object_object_add(content, "info", json_object_new_string(str));
		free(str);
	}

	data = afb_data_json_c_hold(content);
	afb_req_reply(req, error == NULL ? 0 : AFB_ERRNO_GENERIC_FAILURE, 1, &data);
}

void
afb_req_reply_objstr_f(
		afb_req_t req,
		struct json_object *obj,
		const char *error,
		const char *info,
		...
) {
	va_list ap;
	va_start(ap, info);
	afb_req_reply_objstr_v(req, obj, error, info, ap);
	va_end(ap);
}

void
afb_req_reply_objstr(
		afb_req_t req,
		struct json_object *obj,
		const char *error,
		const char *info
) {
	if (info == NULL)
		afb_req_reply_objstr_f(req, obj, error, NULL);
	else
		afb_req_reply_objstr_f(req, obj, error, "%s", info);
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

	if (fmt != NULL && (len = asprintf(&str, fmt, args)) >= 0)
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

