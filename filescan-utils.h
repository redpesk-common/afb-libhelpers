/*
 * Copyright (C) 2016 "IoT.bzh"
 * Author Fulup Ar Foll <fulup@iot.bzh>
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
 *
 * reference:
 *   amixer contents; amixer controls;
 *   http://www.tldp.org/HOWTO/Alsa-sound-6.html
 */

#ifndef FILESCAN_UTILS_H
#define FILESCAN_UTILS_H

#ifdef __cplusplus
    extern "C" {
#endif

#if (AFB_BINDING_VERSION != 3)
# error "Current version of helper only support bindings version 3"
#endif

#include <afb/afb-binding.h>

#include <json-c/json.h>

#ifndef CONTROL_MAXPATH_LEN
  #define CONTROL_MAXPATH_LEN 255
#endif

/**
 * @brief enum describing which mode to use in ScanForConfig function
 *
 */
typedef enum {
  CTL_SCAN_FLAT=0, /**< Simple flat search */
  CTL_SCAN_RECURSIVE=1, /**< Recursive search */
} CtlScanDirModeT;

/**
 * @brief Get rid of the binder name prefix 'afbd-'
 *
 * @param name will be typically the full binder name
 *
 * @return const char*
 */
extern const char *GetMiddleName(const char *name);

/**
 * @brief Get the Binder Name without the prefix set by the AGL appfw 'afbd-'
 *
 * @return const char* the Binder name without the prefix.
 */
extern const char *GetBinderName();

/**
 * @brief Scan a directory searching all files matching pattern:
 * 'prefix*extention'.
 *
 * @param searchPath directory where to begin the searching
 * @param mode either or not the search will be recursive.
 * @param prefix file prefix that will be looking for
 * @param extention file extention that will be looking for
 *
 * @return json_object* a json_object array of object with 2 parts a 'fullpath'
 * describing the fullpath to reach the file and 'filename' containing the
 * matched files.
 */
extern json_object* ScanForConfig (const char* searchPath, CtlScanDirModeT mode, const char *prefix, const char *extension);

/**
 * @brief Get the Binder root directory path using root directory fd.
 *
 * @param fd : Binder root directory file descriptor.
 *
 * @return char* string representing the path to the binder root directory.
 */
extern char *GetAFBRootDirPathUsingFd(int fd);

/**
 * @brief Get the Binder root directory path using AFB API.
 *
 * @param apiHandle : pointer to the AFB API.
 *
 * @return char* string representing the path to binder root directory.
 */
extern char *GetAFBRootDirPath(afb_api_t apiHandle);

/**
 * For compatibility issues :
 * 'GetBindingDirPath_' is linked to 'GetAFBRootDirPathUsingFd'
 * 'GetBindingDirPath' is linked to 'GetAFBRootDirPath'
 */
#define GetBindingDirPath_  GetAFBRootDirPathUsingFd
#define GetBindingDirPath   GetAFBRootDirPath

/*
 * @brief Get the Binding directory
 *
 * @param apiHandle : Api use in binding
 *
 * @return char* string representing the path to binding directory.
 * NULL if something wrong happened.
 */
extern char *GetRunningBindingDirPath(afb_api_t apiHandle);

/**
 * @brief Get the environment directory colon separated path list. This take the
 * prefix add the binder's name then the suffix as environment variable name and
 * also search for another variable without the binder's name (so only
 * prefix+suffix).
 *
 * @param prefix Environment variable prefix
 * @param suffix Environment variable suffix
 *
 * @return const char* a string representing a colon separated path list or NULL
 * is case of error or none environment variables found.
 */
extern char *getEnvDirList(const char *prefix, const char *suffix);

#ifdef __cplusplus
    }
#endif

#endif /* FILESCAN_UTILS_H */
