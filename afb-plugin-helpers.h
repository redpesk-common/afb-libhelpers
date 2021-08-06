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
 * limitations under
 *
 */
#ifndef AFB_PLUGINS_HELPERS_H
#define AFB_PLUGINS_HELPERS_H
#define _GNU_SOURCE
#define AFB_BINDING_VERSION 4

#ifdef __cplusplus
extern "C" {
#endif

#include <afb/afb-binding.h>

/* Make plugins CB signatures dynamic and chosen by developper.
*  Then we just need a function to correctly browse the plugin symbols and find
*  the plugin owning it
*/

typedef struct plugins_store plugins_store;

/**
 * @brief Return the length of the plugin's linked list
 *
 * @param store head of the linked list
 * @return size_t Size of the linked list
 */
size_t plugin_store_length(plugins_store *store);

/**
 * @brief Get the plugin by name object
 *
 * @param store the head of the plugin's linked list
 * @param name Name of the plugin to retrieve
 * @return plugins_store* Returns the plugin_store pointer instance of the found
 * plugin or NULL when not found.
 */
plugins_store* get_plugin_by_name(plugins_store *store, const char *name);

/**
 * @brief Load a binding plugin and store it
 *
 * @param api The Binding api handle
 * @param pluginpath path of the plugin to load
 * @param plugins_store NULL terminated structure where will be stored the plugin once loaded.
 *
 * @return int 0 for a success, -1 for a failure
 */
int plugin_load(afb_api_t api, const char *plugin_name, const char *pluginpath, plugins_store **plugins_store);

/**
 * @brief Get a pointer to a function from a loaded plugins. You have to know
 * the signature of it to be able to call it.
 *
 * @param api Api pointer
 * @param plugin
 * @param function
 * @return void*
 */
void* plugin_get_cb(afb_api_t api, plugins_store *plugin, const char *function);


void* plugin_seek_cb_by_plugin_name(afb_api_t api, plugins_store *store, const char *name, const char *function);

/**
 * @brief Return an array of pointer to the function sought. Signature of those
 * functions aren't known and the user must cast them manually.
 *
 * @param api the api pointer
 * @param store The head of the linked list of plugins
 * @param function name of the function sought
 * @return void** array of function pointer when found or NULL it do not found anything
 */
void** plugin_seek_cb_in_all(afb_api_t api, plugins_store *store, const char *function);

/**
 * @brief Unload a plugin from the plugins store. Free the memory and close the
 * dynamic library handle.
 *
 * @param store the head linked list of plugins
 * @param plugin_name the name of the plugin to unload
 */
void plugin_unload(plugins_store **store, const char *plugin_name);

#ifdef __cplusplus
}
#endif

#endif /* AFB_PLUGINS_HELPERS_H */
