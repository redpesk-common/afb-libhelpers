# Filescan utils functions reference

## const char *GetMiddleName(const char *name)

Get rid of the binder name prefix 'afbd-'

* name will be typically the full binder name

Returns the process middle name of the running binder.

## const char *GetBinderName()

Get the Binder Name without the prefix set by the AGL appfw 'afbd-'

Returns the Binder name without the prefix.

## json_object* ScanForConfig (const char* searchPath, CtlScanDirModeT mode, const char *prefix, const char *extension)

Scan a directory searching all files matching pattern: 'prefix*extention'.

* `searchPath`: directory where to begin the searching.
* `mode`: either or not the search will be recursive.
* `prefix`: file prefix that will be looking for.
* `extention`: file extention that will be looking for.

Returns a json_object array of object with 2 parts a 'fullpath' describing the
fullpath to reach the file and 'filename' containing the matched files.

## char *GetBindingDirPath_(afb_api_t api)

For binding with a version >= 3, get the root directory path.

* `api` : pointer to the AFB API.

Returns a string representing the path to binding root directory.

## const char *getEnvDirList(const char *prefix, const char *suffix)

Get the environment directory colon separated path list. This take the prefix
add the binder's name then the suffix as environment variable name and also
search for another variable without the binder's name (so only prefix+suffix).

* `prefix`: Environment variable prefix
* `suffix`: Environment variable suffix

Returns a string representing a colon separated path list or NULL is case of
error or none environment variables found.