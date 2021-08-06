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

#include <afb-plugin-helpers.h>
#include <dlfcn.h>
#include <string.h>

struct plugins_store {
	void *dl_handle;
	plugins_store *prev;
	char name[];
};


static int add_plugin_to_store(plugins_store **head, const char *plugin_name, void *dl_handle) {
	if(! head || ! plugin_name || ! dl_handle)
		return -1;

	plugins_store *item = (plugins_store*) malloc(sizeof(plugins_store)+1+strlen(plugin_name));
	if(! item)
		return -1;

	strncpy(item->name, plugin_name, 1+strlen(plugin_name));
	item->dl_handle = dl_handle;
	item->prev = *head;
	*head = item;

	return 0;
}

size_t plugin_store_length(plugins_store *store) {
	int s = 0;
	while(store) {
		s++;
		store = store->prev;
	}

	return s;
}

int plugin_load(afb_api_t api, const char *plugin_name, const char *plugin_path, plugins_store **store) {
	void *dl_handle = dlopen(plugin_path, RTLD_NOW);
	if (!dl_handle) {
		AFB_API_ERROR(api, "Plugin load fails plugin_path=%s err= %s", plugin_path, dlerror());
		return -1;
	}

	afb_setup_shared_object(api, dl_handle);

	if(add_plugin_to_store(store, plugin_name, dl_handle)) {
		AFB_API_ERROR(api, "Fails to add plugin to the store plugin=%s", plugin_name);
		return -1;
	}

	return 0;
}

plugins_store* get_plugin_by_name(plugins_store *store, const char *name) {
	while(store->prev) {
		if(! strcasecmp(store->name, name))
			return store;
		store = store->prev;
	}
	return NULL;
}

void* plugin_get_cb(afb_api_t api, plugins_store *plugin, const char *function) {
	return plugin && function ? dlsym(plugin->dl_handle, function) : NULL;
}

void* plugin_seek_cb_by_plugin_name(afb_api_t api, plugins_store *store, const char *name, const char *function) {
	plugins_store *item = get_plugin_by_name(store, name);
	return item ? plugin_get_cb(api, item, function) : NULL;
}

void** plugin_seek_cb_in_all(afb_api_t api, plugins_store *store, const char *function) {
	int nb = 0;
	void *cb = NULL;
	void **cbs = NULL;

	while(store) {
		cb = plugin_get_cb(api, store, function);
		if(cb) {
			nb++;
			cbs = reallocarray(cbs, nb, sizeof(void*));
			cbs[nb-1] = cb;
		}
		store = store->prev;
	}

	cbs = reallocarray(cbs, nb+1, sizeof(void*));
	cbs[nb] = NULL;
	return cbs;
}

void plugin_unload(plugins_store **store, const char *plugin_name) {
	plugins_store *iter = *store;
	while(iter) {
		if(! strcasecmp(iter->name, plugin_name)) {
			*store = iter->prev;
			dlclose(iter->dl_handle);
			free(iter);
			return;
		}
		store = &iter->prev;
		iter = *store;
	}
}
