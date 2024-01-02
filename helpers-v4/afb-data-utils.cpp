/*
 * Copyright (C) 2021-2024 IoT.bzh Company
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

#include <string>
#include "afb-data-utils"

namespace afb {

// conversion from afb::data to nlohmann::json
void to_json(nlohmann::json& jout, const afb::data &din)
{
	afb::data d(afb::type_json(), din);
	jout = nlohmann::json::parse(reinterpret_cast<const char*>(*d));
	d.unref();
}

// conversion from afb::received_data to nlohmann::json
void to_json(nlohmann::json& jout, const afb::received_data &params)
{
	to_json(jout, params[0]);
}

static void wrap_del(void *closure)
{
	nlohmann::json::string_t *str = reinterpret_cast<nlohmann::json::string_t*>(closure);
	delete str;
}

// conversion from nlohmann::json to afb::data
void from_json(const nlohmann::json& jin, afb::data &dout)
{
	nlohmann::json::string_t *ret = new nlohmann::json::string_t(jin.dump());
	dout = afb::data(afb::type_json(), ret->c_str(), 1 + ret->size(), wrap_del, ret);
}

static void dispose_string(void *closure)
{
	std::string *string = reinterpret_cast<std::string*>(closure);
	delete string;
}

afb::data data_string_copy(const char *string, size_t length)
{
	return afb::data(afb_data_string_copy(string, length));
}

afb::data data_string_copy(const std::string &&string)
{
	return data_string_copy(string.c_str(), string.length());
}

afb::data data_string_hold(char *string, size_t length)
{
	return afb::data(afb_data_string_hold(string, length));
}

afb::data data_string_hold(std::string *string)
{
	return afb::data(string == nullptr
		? afb_data_string_generic(nullptr, 0, nullptr, nullptr)
		: afb_data_string_generic(string->c_str(), string->length(),
				dispose_string, reinterpret_cast<void*>(string)));
}

afb::data data_string_wrap(const char *string, size_t length)
{
	return afb::data(afb_data_string_wrap(string, length));
}

afb::data data_string_wrap(const std::string &&string)
{
	return data_string_wrap(string.c_str(), string.length());
}

afb::data data_json_copy(const char *jsonstr, size_t length)
{
	return afb::data(afb_data_json_copy(jsonstr, length));
}

afb::data data_json_copy(const std::string &&jsonstr)
{
	return data_json_copy(jsonstr.c_str(), jsonstr.length());
}

afb::data data_json_hold(char *jsonstr, size_t length)
{
	return afb::data(afb_data_json_hold(jsonstr, length));
}

afb::data data_json_hold(std::string *jsonstr)
{
	return afb::data(afb_data_json_generic(jsonstr->c_str(), jsonstr->length(),
				dispose_string, reinterpret_cast<void*>(jsonstr)));
}

afb::data data_json_wrap(const char *jsonstr, size_t length)
{
	return afb::data(afb_data_json_wrap(jsonstr, length));
}

afb::data data_json_wrap(const std::string &&jsonstr)
{
	return data_json_wrap(jsonstr.c_str(), jsonstr.length());
}

afb::data data_json_c_hold(json_object *jobject)
{
	return afb::data(afb_data_json_c_hold(jobject));
}

}

#endif
