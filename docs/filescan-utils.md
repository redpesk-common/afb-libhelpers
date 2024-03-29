# Filescan utils functions reference

## const char *GetMiddleName(const char *name)

Get rid of the binder name prefix 'afbd-'

* name will be typically the full binder name

Returns the process middle name of the running binder.

## const char *GetBinderName()

Get the Binder Name without the prefix set by the Application Framework 'afbd-'

Returns the Binder name without the prefix.

## json_object* ScanForConfig (const char* searchPath, CtlScanDirModeT mode, const char *prefix, const char *extension)

Scan a directory searching all files matching pattern: 'prefix*extention'.

* `searchPath`: directory where to begin the searching.
* `mode`: either or not the search will be recursive.
* `prefix`: file prefix that will be looking for.
* `extention`: file extention that will be looking for.

Returns a json_object array of object with 2 parts a 'fullpath' describing the
fullpath to reach the file and 'filename' containing the matched files.

## char *GetAFBRootDirPathUsingFd(int fd)

Get the binder root directory path (the path specified with '--rootdir' option
at binder launch, if the option is not used, the path from where the binder
is launched) using binder root directory file descriptor.

* `fd` : Binder root directory file descriptor.

Returns a string representing the path to binder root directory.

## char *GetAFBRootDirPath(afb_api_t apiHandle)

For binding with a version >= 3, same as 'GetAFBRootDirPathUsingFd' function,
but use pointer to the AFB API as parameter instead of
binder root directory file descriptor.

* `apiHandle` : pointer to the AFB API.

Returns a string representing the path to binder root directory.

## char* GetBindingDirPath()

For binding with a version <= 2, same as 'GetAFBRootDirPath' function,
but the pointer to the AFB API is not needed.
Kept for compatibility issues.

## char* GetBindingDirPath(afb_api_t api)

For binding with a version >= 3, same as 'GetAFBRootDirPath' function.
Deprecated, please use 'GetAFBRootDirPath' function.
Kept for compatibility issues.

## char *GetRunningBindingDirPath(afb_api_t api)

For binding with a version >= 3, get the binding directory path
(the path to the directory that contains the binding).

* `api` : pointer to the AFB API.

Returns a string representing the path to the binding directory.

## const char *getEnvDirList(const char *prefix, const char *suffix)

Get the environment directory colon separated path list. This take the prefix
add the binder's name then the suffix as environment variable name and also
search for another variable without the binder's name (so only prefix+suffix).

* `prefix`: Environment variable prefix
* `suffix`: Environment variable suffix

Returns a string representing a colon separated path list or NULL is case of
error or none environment variables found.
