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

#pragma once

#if AFB_BINDING_VERSION > 3

#include <afb/afb-binding.h>
#include <json-c/json.h>
#include <stdarg.h>

extern void
afb_req_reply_data(
		afb_req_t req,
		int code,
		afb_data_t data);

extern void
afb_req_reply_json_c_hold(
		afb_req_t req,
		int code,
		json_object *jobject);

extern void
afb_req_reply_string_copy(
		afb_req_t req,
		int code,
		const char *string,
		size_t length);

extern void
afb_req_reply_string_hold(
		afb_req_t req,
		int code,
		char *string,
		size_t length);

extern void
afb_req_reply_string_wrap(
		afb_req_t req,
		int code,
		const char *string,
		size_t length);

extern void
afb_req_reply_string(
		afb_req_t req,
		int code,
		const char *string);

extern void
afb_req_reply_string_v(
		afb_req_t req,
		int code,
		const char *fmt,
		va_list args);

__attribute__((format(printf, 3, 4)))
extern void
afb_req_reply_string_f(
		afb_req_t req,
		int code,
		const char *fmt,
		...);

#endif
