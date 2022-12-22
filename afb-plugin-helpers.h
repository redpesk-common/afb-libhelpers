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

#ifdef __cplusplus
extern "C" {
#endif

#include <afb/afb-binding.h>

/* Make plugins CB signatures dynamic and chosen by developper.
*  Then we just need a function to correctly browse the plugin symbols and find
*  the plugin owning it
*/

typedef struct plugin_store_s plugin_store_t;
typedef plugin_store_t plugin_t;

/*
* Initial value for plugin stores
*/
#define PLUGIN_STORE_INITIAL NULL

/**
 * @brief Load a binding plugin and store it
 *
 * @param store the store address
 * @param path path of the plugin to load
 * @param path name for storing the plugin
 * @param api The binding api handle
 *
 * @return int 0 for a success, -1 for a failure
 */
int plugin_store_load(plugin_store_t **store, const char *path, const char *name, afb_api_t api);

/**
 * @brief Return the length of the plugin's linked list
 *
 * @param store the store
 * @return size_t Size of the linked list
 */
size_t plugin_store_length(plugin_store_t *store);

/**
 * @brief Unload a plugin from the plugins store. Free the memory and close the
 * dynamic library handle.
 *
 * @param store the store address
 * @param name the name of the plugin to unload
 */
void plugin_store_unload(plugin_store_t **store, const char *name);

/**
 * @brief Get the object by its name and plugin name
 *
 * @param store the store
 * @param plugname Name of the plugin
 * @param objname Name of the object
 * @return Returns the plugin instance found or NULL when not found
 */
void *plugin_store_get_object(plugin_store_t *store, const char *plugname, const char *objname);

/**
 * @brief Return an array of all objects of the given name.
 *
 * @param store The head of the linked list of plugins
 * @param name name of the objects
 * @return NULL terminated array of function pointer when found or NULL it do not found anything
 * The result must be freed using 'free'
 */
void** plugin_store_all_objects(plugin_store_t *store, const char *function);

/**
 * @brief Get a plugin by its name
 *
 * @param store the store
 * @param name Name of the plugin to retrieve
 * @return Returns the plugin instance found or NULL when not found
 */
plugin_t *plugin_store_get_plugin(plugin_store_t *store, const char *name);

/**
 * @brief Get a pointer to an object from a loaded plugins.
 *
 * @param plugin the plugin where to search
 * @param name name of the object to get
 * @return NULL if the object is not found or else, its address
 */
void* plugin_get_object(const plugin_t *plugin, const char *name);

#if 0 /* compatibility with ROMAIN's version */
typedef plugin_store_t plugins_store;
#define plugin_load(store, path, name, api) plugin_store_load(api, name, path, store)
#define plugin_load(store, name)            plugin_store_unload(store, name)
#define plugin_get_cb(api, store, name)     plugin_get_object(store, name)
#define get_plugin_by_name(store, name)     plugin_store_get_plugin(store, name)
#define plugin_seek_cb_by_plugin_name(api, store, name, function) plugin_store_get_object(store, name, function)
#define plugin_seek_cb_in_all(api, store, name)  plugin_store_all_objects(store, name, function)
#endif

#ifdef __cplusplus
}
#endif

#endif /* AFB_PLUGINS_HELPERS_H */
