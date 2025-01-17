/*
 * Copyright (C) 2021-2025 IoT.bzh Company
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
#ifndef PLUGINS_HELPERS_H
#define PLUGINS_HELPERS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <afb/afb-binding.h>

/* Make plugins CB signatures dynamic and chosen by developper.
*  Then we just need a function to correctly browse the plugin symbols and find
*  the plugin owning it
*/

typedef struct plugin_s plugin_t;
typedef plugin_t *plugin_store_t;

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
extern
int plugin_store_load(plugin_store_t *store, const char *path, const char *name, afb_api_t api);

/**
 * @brief Load a binding plugin and store it
 *
 * @param store the store address
 * @param path path of the plugin to load
 * @param path name for storing the plugin
 * @param api The binding api handle
 *
 * @return the loaded plugin or NULL on error
 */
extern
plugin_t *plugin_store_get_load(plugin_store_t *store, const char *path, const char *name, afb_api_t api);

/**
 * @brief Return the length of the plugin's linked list
 *
 * @param store the store
 * @return size_t Size of the linked list
 */
extern
size_t plugin_store_length(plugin_store_t store);

/**
 * @brief Unload a plugin from the plugins store. Free the memory and close the
 * dynamic library handle.
 *
 * @param store the store address
 * @param plugname the name of the plugin to unload
 */
extern
void plugin_store_unload(plugin_store_t *store, const char *plugname);

/**
 * @brief Unload a plugin from the plugins store. Free the memory and close the
 * dynamic library handle.
 *
 * @param store the store address
 * @param plugin the plugin to unload
 */
extern
void plugin_store_drop(plugin_store_t *store, plugin_t *plugin);

/**
 * @brief Drop all plugins from the plugins store. Free the memory and close the
 * dynamic library handles.
 *
 * @param store the store address
 */
extern
void plugin_store_drop_all(plugin_store_t *store);

/**
 * @iterate over plugins of the store until it returns a value not null
 *
 * @param store the store
 * @param callback the function to call for each plugin
 * @param closure any data that the callback needs
 *
 * @return the last returned value not null or zero
 */
extern
int plugin_store_iter(plugin_store_t store, int (*callback)(void*,const plugin_t*), void *closure);

/**
 * @brief Get the object by its name and plugin name
 *
 * @param store the store
 * @param plugname Name of the plugin
 * @param objname Name of the object
 * @return Returns the plugin instance found or NULL when not found
 */
extern
void *plugin_store_get_object(plugin_store_t store, const char *plugname, const char *objname);

/**
 * @brief Return an array of all objects of the given name.
 *
 * @param store The head of the linked list of plugins
 * @param objname name of the objects
 * @return NULL terminated array of function pointer when found or NULL it do not found anything
 * The result must be freed using 'free'
 */
extern
void** plugin_store_all_objects(plugin_store_t store, const char *objname);

/**
 * @brief Get a plugin by its name
 *
 * @param store the store
 * @param plugname Name of the plugin to retrieve
 * @return Returns the plugin instance found or NULL when not found
 */
extern
plugin_t *plugin_store_get_plugin(plugin_store_t store, const char *plugname);

/**
 * @brief Get a pointer to an object from a loaded plugins.
 *
 * @param plugin the plugin where to search
 * @param objname name of the object to get
 * @return NULL if the object is not found or else, its address
 */
extern
void* plugin_get_object(const plugin_t *plugin, const char *objname);


/**
 * @brief Get the name of a loaded plugins.
 *
 * @param plugin the plugin
 * @return the name or NULL if plugin==NULL
 */
extern
const char *plugin_name(const plugin_t *plugin);

#ifdef __cplusplus
}
#endif

#endif /* PLUGINS_HELPERS_H */
