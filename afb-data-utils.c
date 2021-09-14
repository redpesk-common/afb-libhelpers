/*
 * Copyright (C) 2021 "IoT.bzh"
 * Author: Romain Forlot <romain.forlot@iot.bzh>
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

#include <string.h>
#include "afb-data-utils.h"

static afb_data_t _data_str_copy_(const char *str, size_t length, afb_type_t type)
{
	char *copy;
	size_t len;
	afb_data_t result = NULL;

	if (str) {
		if (!length)
			len = 1 + strlen(str);
		else if (!str[length - 1] || str[length])
			len = length;
		else
			len = length + 1;
		if (afb_create_data_alloc(&result, type, (void**)&copy, len) >= 0) {
			memcpy(copy, str, len - 1);
			copy[len - 1] = 0;
		}
	}
	return result;
}

static afb_data_t _data_str_(const char *str, size_t length, void (*dispose)(void*), void *closure, afb_type_t type)
{
	size_t len;
	afb_data_t result = NULL;

	if (!str)
		len = 0;
	else if (!length)
		len = 1 + strlen(str);
	else if (!str[length - 1])
		len = length;
	else if (!str[length])
		len = length + 1;
	else
		return result;
	afb_create_data_raw(&result, type, str, len, dispose, closure);
	return result;
}


afb_data_t afb_data_string_generic(const char *string, size_t length, void (*dispose)(void*), void *closure)
{
	return _data_str_(string, length, dispose, closure, AFB_PREDEFINED_TYPE_STRINGZ);
}

afb_data_t afb_data_string_wrap(const char *string, size_t length)
{
	return afb_data_string_generic(string, length, NULL, NULL);
}

afb_data_t afb_data_string_copy(const char *string, size_t length)
{
	return _data_str_copy_(string, length, AFB_PREDEFINED_TYPE_STRINGZ);
}

afb_data_t afb_data_string_hold(char *string, size_t length)
{
	return afb_data_string_generic(string, length, free, string);
}

afb_data_t afb_data_string(const char *string)
{
	return afb_data_string_generic(string, 0, NULL, NULL);
}

afb_data_t afb_data_json_generic(const char *jsonstr, size_t length, void (*dispose)(void*), void *closure)
{
	return jsonstr ? _data_str_(jsonstr, length, dispose, closure, AFB_PREDEFINED_TYPE_JSON) : NULL;
}

afb_data_t afb_data_json_wrap(const char *jsonstr, size_t length)
{
	return afb_data_json_generic(jsonstr, length, NULL, NULL);
}

afb_data_t afb_data_json_copy(const char *jsonstr, size_t length)
{
	return _data_str_copy_(jsonstr, length, AFB_PREDEFINED_TYPE_JSON);
}

afb_data_t afb_data_json_hold(char *jsonstr, size_t length)
{
	return afb_data_json_generic(jsonstr, length, free, jsonstr);
}

afb_data_t afb_data_json(const char *jsonstr)
{
	return afb_data_json_generic(jsonstr, 0, NULL, NULL);
}

static void dispose_json_c(void *closure)
{
	json_object *jobject = closure;
	json_object_put(jobject);
}

afb_data_t afb_data_json_c_hold(json_object *jobject)
{
	afb_data_t result;
	afb_create_data_raw(&result, AFB_PREDEFINED_TYPE_JSON_C, jobject, 0, dispose_json_c, jobject);
	return result;
}

#endif
