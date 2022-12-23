/*
 * Copyright (C) 2021 "IoT.bzh"
 * Author: Jose Bollo <jose.bollo@iot.bzh>
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

#pragma once

#if AFB_BINDING_VERSION > 3

#include <afb/afb-binding.h>
#include <json-c/json.h>

/**
 * Creates a data of type AFB_PREDEFINED_TYPE_STRINGZ encapsulating
 * a copy of the given UTF8 string and return it.
 *
 * The string MUST not be NULL
 *
 * If the given length is zero, the length of the string is computed.
 *
 * @param string  an UTF8 string to copy
 * @param length  the length of the string or zero to compute it
 *
 * @return the created data or NULL when out of memory or string == NULL
 */
afb_data_t afb_data_string_copy(const char *string, size_t length);

/**
 * Creates a data of type AFB_PREDEFINED_TYPE_STRINGZ encapsulating
 * the given zero terminated UTF8 string and return it.
 *
 * If the given string is NULL, this is allowed and represents the NULL string,
 * not the string without character but the string whose pointer is NULL In that
 * case, the length is not accurate.
 *
 * If the given length is zero, the length of the string is computed.
 * Otherwise, the length can either include the ending zero or not.
 * It is an error (and NULL is returned) if the length is not zero and
 * doesn't refers to a zero at string[length-1] and string[length].
 *
 * If dispose is not NULL, this is a function that called when the
 * created data is released, meaning it is not more referenced (@see
 * afb_data_addref, afb_data_unref). The dispose function is called
 * with the given closure.
 *
 * @param string  a zero terminated the UTF8 string to encapsulate
 * @param length  the length of the string or zero to compute it
 * @param dispose function to call when data is released (can be NULL)
 * @param closure closure the the function dispose
 *
 * @return the created data or NULL when out of memory
 *
 * @relates afb_create_data_raw
 */
afb_data_t afb_data_string_generic(const char *string, size_t length, void (*dispose)(void*), void *closure);

/**
 * Shortcut for afb_data_string_generic(string, length, free, string);
 *
 * @see afb_data_string_generic
 */
afb_data_t afb_data_string_hold(char *string, size_t length);

/**
 * Shortcut for afb_data_string_generic(string, 0, NULL, NULL);
 *
 * @see afb_data_string_generic
 */
afb_data_t afb_data_string(const char *string);

/**
 * Shortcut for afb_data_string_generic(string, length, NULL, NULL);
 *
 * @see afb_data_string_generic
 */
afb_data_t afb_data_string_wrap(const char *string, size_t length);

/**
 * Creates a data of type AFB_PREDEFINED_TYPE_JSON encapsulating
 * a copy of the given UTF8 JSON string and return it.
 *
 * The JSON string MUST not be NULL
 *
 * If the given length is zero, the length of the string is computed.
 *
 * @param jsonstr an UTF8 JSON string to copy
 * @param length  the length of the string or zero to compute it
 *
 * @return the created data or NULL when out of memory or jsonstr == NULL
 */
afb_data_t afb_data_json_copy(const char *jsonstr, size_t length);

/**
 * Creates a data of type AFB_PREDEFINED_TYPE_JSON encapsulating
 * the given zero terminated UTF8 JSON string and return it.
 *
 * The JSON string MUST not be NULL
 *
 * If the given length is zero, the length of the JSON string is computed.
 * Otherwise, the length can either include the ending zero or not.
 * It is an error (and NULL is returned) if the length is not zero and
 * doesn't refers to a zero at string[length-1] and string[length].
 *
 * If dispose is not NULL, this is a function that called when the
 * created data is released, meaning it is not more referenced (@see
 * afb_data_addref, afb_data_unref). The dispose function is called
 * with the given closure.
 *
 * @param jsonstr an UTF8 JSON string to copy
 * @param length  the length of the string or zero to compute it
 * @param dispose function to call when data is released (can be NULL)
 * @param closure closure the the function dispose
 *
 * @return the created data or NULL when out of memory or jsonstr == NULL
 *
 * @relates afb_create_data_raw
 */
afb_data_t afb_data_json_generic(const char *jsonstr, size_t length, void (*dispose)(void*), void *closure);

/**
 * Shortcut for afb_data_json_generic(jsonstr, length, NULL, NULL);
 *
 * @see afb_data_json_generic
 */
afb_data_t afb_data_json_wrap(const char *jsonstr, size_t length);

/**
 * Shortcut for afb_data_json_generic(jsonstr, length, free, jsonstr);
 *
 * @see afb_data_json_generic
 */
afb_data_t afb_data_json_hold(char *jsonstr, size_t length);

/**
 * Shortcut for afb_data_json_generic(jsonstr, 0, NULL, NULL);
 *
 * @see afb_data_json_generic
 */
afb_data_t afb_data_json(const char *jsonstr);

/**
 * Creates a data of type AFB_PREDEFINED_TYPE_JSON_C encapsulating
 * the given json-c json_object and return it.
 *
 * The json_objet will be automatically released when the data will
 * be released. So if the object must be kept, the caller of the
 * function has to call json_object_get before.
 *
 * @param jobject an UTF8 JSON string to encapsulate
 *
 * @return the created data or NULL when out of memory
 */
afb_data_t afb_data_json_c_hold(json_object *jobject);

#endif /* AFB_BINDING_VERSION > 3 */

