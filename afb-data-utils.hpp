/*
 * Copyright (C) 2021 "IoT.bzh"
 * Author: Romain Forlot <romain.forlot@iot.bzh>
 * Author: José Bollo <jose.bollo@iot.bzh>
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

#ifdef __cplusplus
#if AFB_BINDING_VERSION > 3

#include <afb/afb-binding>

#include "afb-data-utils.h"
#include "json.hpp"

// still in afb namespace (at least needed for nlohmann's convertions)
namespace afb {

	// conversion from afb::data to nlohmann::json
	void to_json(nlohmann::json& jout, const afb::data &din);

	// conversion from afb::received_data to nlohmann::json
	void to_json(nlohmann::json& jout, const afb::received_data &params);

	// conversion from nlohmann::json to afb::data
	void from_json(const nlohmann::json& jin, afb::data &dout);


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
	 * @return the created data, not valid if out of memory or string == NULL
	 */
	afb::data data_string_copy(const char *string, size_t length = 0);

	/**
	 * Creates a data of type AFB_PREDEFINED_TYPE_STRINGZ encapsulating
	 * a copy of the given UTF8 string and return it.
	 *
	 * @param string  an UTF8 string to copy
	 *
	 * @return the created data, not valid if out of memory
	 */
	afb::data data_string_copy(const std::string &&string);

	/**
	 * Creates a data of type AFB_PREDEFINED_TYPE_STRINGZ encapsulating
	 * the given UTF8 string and return it. When the data will be released
	 * the given string will be freed using 'free'.
	 *
	 * If the given string is NULL, this is allowed and represents the NULL string,
	 * not the string without character but the string whose pointer is NULL In that
	 * case, the length is not accurate.
	 *
	 * If the given length is zero, the length of the string is computed.
	 *
	 * @param string  an UTF8 string to hold
	 * @param length  the length of the string or zero to compute it
	 *
	 * @return the created data, not valid if out of memory
	 */
	afb::data data_string_hold(char *string, size_t length = 0);

	/**
	 * Creates a data of type AFB_PREDEFINED_TYPE_STRINGZ encapsulating
	 * the given UTF8 string and return it. When the data will be released
	 * the given string will be deleted using 'delete'.
	 *
	 * If the given string is NULL, this is allowed and represents the NULL string,
	 * not the string without character but the string whose pointer is NULL.
	 *
	 * @param string  an UTF8 string to hold
	 *
	 * @return the created data, not valid if out of memory
	 */
	afb::data data_string_hold(std::string *string);

	/**
	 * Creates a data of type AFB_PREDEFINED_TYPE_STRINGZ encapsulating
	 * the given UTF8 string and return it.
	 *
	 * If the given string is NULL, this is allowed and represents the NULL string,
	 * not the string without character but the string whose pointer is NULL In that
	 * case, the length is not accurate.
	 *
	 * If the given length is zero, the length of the string is computed.
	 *
	 * @param string  an UTF8 string to wrap
	 * @param length  the length of the string or zero to compute it
	 *
	 * @return the created data, not valid if out of memory
	 */
	afb::data data_string_wrap(const char *string, size_t length = 0);

	/**
	 * Creates a data of type AFB_PREDEFINED_TYPE_STRINGZ encapsulating
	 * the given UTF8 string and return it.
	 *
	 * @param string  an UTF8 string to wrap
	 *
	 * @return the created data, not valid if out of memory
	 */
	afb::data data_string_wrap(const std::string &&string);

	/**
	 * Creates a data of type AFB_PREDEFINED_TYPE_JSON encapsulating
	 * a copy of the given UTF8 JSON string and return it.
	 *
	 * The string MUST not be NULL
	 *
	 * If the given length is zero, the length of the string is computed.
	 *
	 * @param jsonstr an UTF8 JSON string to copy
	 * @param length  the length of the json string or zero to compute it
	 *
	 * @return the created data, not valid if out of memory or jsonstr == NULL
	 */
	afb::data data_json_copy(const char *jsonstr, size_t length = 0);

	/**
	 * Creates a data of type AFB_PREDEFINED_TYPE_JSON encapsulating
	 * a copy of the given UTF8 JSON string and return it.
	 *
	 * @param jsonstr an UTF8 JSON string to copy
	 *
	 * @return the created data, not valid if out of memory
	 */
	afb::data data_json_copy(const std::string &&jsonstr);

	/**
	 * Creates a data of type AFB_PREDEFINED_TYPE_JSON encapsulating
	 * the given UTF8 JSON string and return it. When the data will be released
	 * the given JSON string will be freed using 'free'.
	 *
	 * The JSON string MUST not be NULL
	 *
	 * If the given length is zero, the length of the string is computed.
	 *
	 * @param jsonstr an UTF8 JSON string to hold
	 * @param length  the length of the string or zero to compute it
	 *
	 * @return the created data, not valid if out of memory
	 */
	afb::data data_json_hold(char *jsonstr, size_t length = 0);

	/**
	 * Creates a data of type AFB_PREDEFINED_TYPE_JSON encapsulating
	 * the given UTF8 JSON string and return it. When the data will be released
	 * the given JSON string will deleted using 'delete'.
	 *
	 * The JSON string MUST not be NULL
	 *
	 * @param jsonstr an UTF8 JSON string to hold
	 *
	 * @return the created data, not valid if out of memory
	 */
	afb::data data_json_hold(std::string *jsonstr);

	/**
	 * Creates a data of type AFB_PREDEFINED_TYPE_JSON encapsulating
	 * the given UTF8 JSON string and return it.
	 *
	 * The JSON string MUST not be NULL
	 *
	 * If the given length is zero, the length of the string is computed.
	 *
	 * @param jsonstr an UTF8 JSON string to wrap
	 * @param length  the length of the string or zero to compute it
	 *
	 * @return the created data, not valid if out of memory
	 */
	afb::data data_json_wrap(const char *jsonstr, size_t length = 0);

	/**
	 * Creates a data of type AFB_PREDEFINED_TYPE_JSON encapsulating
	 * the given UTF8 JSON string and return it.
	 *
	 * @param jsonstr an UTF8 JSON string to wrap
	 *
	 * @return the created data, not valid if out of memory
	 */
	afb::data data_json_wrap(const std::string &&jsonstr);

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
	 * @return the created data, not valid if out of memory
	 */
	afb::data data_json_c_hold(json_object *jobject);
};

#endif // AFB_BINDING_VERSION > 3
#endif // __cplusplus
