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

#include <dlfcn.h>
#include <string.h>

#include "afb-plugin-helpers.h"


struct plugin_store_s
{
	plugin_store_t *prev;
	void *dl_handle;
	char name[];
};

int plugin_store_load(plugin_store_t **store, const char *path, const char *name, afb_api_t api)
{
	void *dl_handle;
	size_t name_length;
	plugin_store_t *item;

	/* check args */
	if (!store || !name || !path) {
		AFB_API_ERROR(api, "Plugin load bad args");
		return -1;
	}

	/* allocate result */
	name_length = strlen(name);
	item = malloc(name_length + 1 + sizeof *item);
	if(! item) {
		AFB_API_ERROR(api, "alloc error");
		return -1;
	}

	/* open lib */
	dl_handle = dlopen(path, RTLD_NOW);
	if (!dl_handle) {
		AFB_API_ERROR(api, "Plugin load fails plugin_path=%s err= %s", path, dlerror());
		free(item);
		return -1;
	}
	afb_setup_shared_object(api, dl_handle);

	memcpy(item->name, name, 1 + name_length);
	item->dl_handle = dl_handle;
	item->prev = *store;
	*store = item;

	return 0;
}

size_t plugin_store_length(plugin_store_t *store) {
	int s = 0;
	while(store) {
		s++;
		store = store->prev;
	}
	return s;
}

static plugin_store_t** search(plugin_store_t **store, const char *name)
{
	for (;;) {
		plugin_store_t *item = *store;
		if (! item)
			return NULL;
		if(! strcasecmp(item->name, name))
			return store;
		store = &item->prev;
	}
}

void plugin_store_unload(plugin_store_t **store, const char *name)
{
	plugin_store_t **ref = store && name ? search(store, name) : NULL;
	if (ref) {
		plugin_store_t *item = *ref;
		*ref = item->prev;
		dlclose(item->dl_handle);
		free(item);
	}
}

plugin_t *plugin_store_get_plugin(plugin_store_t *store, const char *name)
{
	plugin_store_t **ref = store && name ? search(&store, name) : NULL;
	return ref ? *ref : NULL;
}

void *plugin_store_get_object(plugin_store_t *store, const char *plugname, const char *objname)
{
	plugin_store_t **ref = store && plugname && objname ? search(&store, plugname) : NULL;
	return ref ? dlsym((*ref)->dl_handle, objname) : NULL;
}

void* plugin_get_object(const plugin_t *plugin, const char *name)
{
	return plugin && name ? dlsym(plugin->dl_handle, name) : NULL;
}

static void **getall(plugin_store_t *store, const char *name, void **link)
{
	void *pair[2];
	for (;;) {
		if (store) {
			pair[1] = dlsym(store->dl_handle, name);
			store = store->prev;
			if (pair[1]) {
				pair[0] = link;
				return getall(store, name, pair);
			}
		}
		else {
			unsigned count = 1;
			void **result, **iter = link;
			while (iter) {
				count++;
				iter = iter[0];
			}
			result = malloc(count * sizeof *result);
			if (result) {
				count = 0;
				iter = link;
				while (iter) {
					result[count++] = iter[1];
					iter = iter[0];
				}
				result[count] = NULL;
			}
			return result;
		}
	}
}

void** plugin_store_all_objects(plugin_store_t *store, const char *name)
{
	return getall(store, name, NULL);
}
