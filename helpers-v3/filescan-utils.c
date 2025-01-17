/*
 * Copyright (C) 2016-2025 IoT.bzh Company
 * Author: Fulup Ar Foll <fulup@iot.bzh>
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

#define _GNU_SOURCE
#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/prctl.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "filescan-utils.h"

static int ScanDir(char* searchPath, CtlScanDirModeT mode, size_t extentionLen,
    const char* prefix, const char* extention, json_object* responseJ)
{
    int found = 0;
    DIR* dirHandle;
    struct dirent* dirEnt;
    dirHandle = opendir(searchPath);
    if (!dirHandle) {
        AFB_DEBUG("CONFIG-SCANNING dir=%s not readable", searchPath);
        return 0;
    }

    //AFB_NOTICE ("CONFIG-SCANNING:ctl_listconfig scanning: %s", searchPath);
    while ((dirEnt = readdir(dirHandle)) != NULL) {

        // recursively search embedded directories ignoring any directory starting by '.' or '_'
        if (dirEnt->d_type == DT_DIR && mode == CTL_SCAN_RECURSIVE) {
            char newpath[CONTROL_MAXPATH_LEN + 1];
            if (dirEnt->d_name[0] == '.' || dirEnt->d_name[0] == '_')
                continue;

            strncpy(newpath, searchPath, sizeof(newpath) - 1);
            newpath[sizeof(newpath) - 1] = 0;
            strncat(newpath, "/", sizeof(newpath) - strlen(newpath) - 1);
            strncat(newpath, dirEnt->d_name, sizeof(newpath) - strlen(newpath) - 1);
            found += ScanDir(newpath, mode, extentionLen, prefix, extention, responseJ);
            continue;
        }

        // Unknown type is accepted to support dump filesystems
        if (dirEnt->d_type == DT_REG ||
            dirEnt->d_type == DT_UNKNOWN ||
            dirEnt->d_type == DT_LNK) {

            // check prefix and extention
            ssize_t extentionIdx = strlen(dirEnt->d_name) - extentionLen;
            if (extentionIdx <= 0)
                continue;
            if (prefix && strncasecmp(dirEnt->d_name, prefix, strlen(prefix)))
                continue;
            if (extention && strcasecmp(extention, &dirEnt->d_name[extentionIdx]))
                continue;

            struct json_object* pathJ = json_object_new_object();
            json_object_object_add(pathJ, "fullpath", json_object_new_string(searchPath));
            json_object_object_add(pathJ, "filename", json_object_new_string(dirEnt->d_name));
            json_object_array_add(responseJ, pathJ);
            found++;
        }
    }
    closedir(dirHandle);
    return found;
}

// List Avaliable Configuration Files
json_object* ScanForConfig(const char* searchPath, CtlScanDirModeT mode, const char* prefix, const char* extention)
{
    json_object* responseJ = json_object_new_array();
    char* dirPath;
    char* dirList;
    size_t extentionLen = 0;
    int count = 0;

    if (!searchPath)
        return responseJ;

    dirList = strdup(searchPath);

    if (extention)
        extentionLen = strlen(extention);

    // loop recursively on dir
    for (dirPath = strtok(dirList, ":"); dirPath && *dirPath; dirPath = strtok(NULL, ":")) {
        count += ScanDir(dirPath, mode, extentionLen, prefix, extention, responseJ);
    }
    if (count == 0) {
        json_object_put(responseJ);
        free(dirList);
        return NULL;
    }
    free(dirList);
    return (responseJ);
}

char* GetMiddleName(const char* name)
{
    char* fullname = strdup(name);
    if (fullname != NULL) {
        char *start = strchr(fullname, '-');
        if (start == NULL)
            fullname[0] = 0;
        else {
            size_t len = strcspn(++start, "-@.");
            memmove(fullname, start, len);
            fullname[len] = 0;
        }
    }
    return fullname;
}

const char* GetBinderName()
{
    static char* binderName = NULL;

    if (binderName)
        return binderName;

    binderName = getenv("AFB_BINDER_NAME");
    if (!binderName) {
        static char psName[17];
        // retrieve binder name from process name afb-name-trailer
        prctl(PR_GET_NAME, psName, NULL, NULL, NULL);
        binderName = GetMiddleName(psName);
        if (binderName == NULL)
            binderName = "";
    }

    return binderName;
}

char *GetAFBRootDirPathUsingFd(int fd)
{
    // A file description should not be greater than 999.999.999
    char fd_link[CONTROL_MAXPATH_LEN];
    char retdir[CONTROL_MAXPATH_LEN];
    ssize_t len;

    sprintf(fd_link, "/proc/self/fd/%d", fd);

    if ((len = readlink(fd_link, retdir, sizeof(retdir) - 1)) == -1) {
        perror("lstat");
        strncpy(retdir, "/tmp", CONTROL_MAXPATH_LEN - 1);
    } else {
        retdir[len] = '\0';
    }

    return strndup(retdir, sizeof(retdir));
}

char *GetAFBRootDirPath(afb_api_t apiHandle)
{
    char *rd = getenv("AFB_ROOTDIR");
    if (rd)
        return strdup(rd);
#if (AFB_BINDING_VERSION > 3)
    return NULL;
#else

    int fd;

    fd = afb_api_rootdir_get_fd(apiHandle);

    return GetAFBRootDirPathUsingFd(fd);
#endif
}

char *GetRunningBindingDirPath(afb_api_t apiHandle)
{
    int ret;

    char *lastSlashInPath, *bindingDirectoryPath;
    const char *bindingPath;

    json_object *settingsJ, *bindingPathJ = NULL;

    settingsJ = afb_api_settings(apiHandle);
    if(!settingsJ)
        return NULL;

    ret = json_object_object_get_ex(settingsJ, "binding-path", &bindingPathJ);
    if(!ret || !bindingPathJ || !json_object_is_type(bindingPathJ, json_type_string))
        return NULL;

    bindingPath = json_object_get_string(bindingPathJ);

    lastSlashInPath = rindex(bindingPath, '/');
    if(!lastSlashInPath)
        return strdup(".");

    bindingDirectoryPath = strndup(bindingPath, lastSlashInPath - bindingPath);

    return bindingDirectoryPath;
}


/**
 * @brief Takes an input string and makes it upper case. The output buffer
 * should be able to contains the whole input string else it will be truncated
 * at the output_size.
 *
 * @param input the input string to transform in an upper case version
 * @param output the upper case version of the input string
 * @param output_size the output buffer size, if size < to strlen of input then
 * the output will be truncated.
 *
 * @return size_t the string length of the resulting output variable, which should
 * be equal to the size of input string length.
 */
static size_t toUpperString(const char *input, char *output, size_t output_size) {
    int i = 0;

    if(input && output) {
        while(input[i] && i <= output_size) {
            output[i] = (char)toupper(input[i]);
            i++;
        }
        output[i] = '\0';
        return strlen(output);
    }

   return 0;
}

char *getEnvDirList(const char *prefix, const char *suffix) {
    size_t lenSimple = 0, lenFull = 0, plen = 0, slen = 0, blen = 0;
    char *envConfigPathSimple = NULL, *envConfigPathFull = NULL;
    char *upperPrefix = NULL, *upperSuffix = NULL, *upperBinderName = NULL;
    char *envDirList = NULL;
    const char *binderName = GetBinderName(), *envDirListSimple = NULL;
    const char *envDirListFull = NULL;

    if(! prefix || ! suffix)
        return NULL;

    plen = strlen(prefix);
    upperPrefix = alloca(plen + 1);

    slen = strlen(suffix);
    upperSuffix = alloca(slen + 1);

    blen = strlen(binderName);
    upperBinderName = alloca(blen + 1);

    lenSimple = plen + slen + 1;
    envConfigPathSimple = alloca(lenSimple + 1);

    lenFull = slen + blen + slen + 2;
    envConfigPathFull = alloca(lenFull + 1);

    if(toUpperString(prefix, upperPrefix, plen) != plen ||
       toUpperString(suffix, upperSuffix, slen) != slen ||
       toUpperString(binderName, upperBinderName, blen) != blen ||
       (snprintf(envConfigPathSimple, lenSimple + 1, "%s_%s", upperPrefix, upperSuffix) >= lenSimple + 1) ||
       (snprintf(envConfigPathFull, lenFull + 1, "%s_%s_%s", upperPrefix, upperBinderName, upperSuffix) >= lenFull + 1)) {
        return NULL;
    }

    envDirListFull = getenv(envConfigPathFull);
    envDirListSimple = getenv(envConfigPathSimple);
    if(envDirListSimple && envDirListFull) {
        lenFull = strlen(envDirListSimple) + strlen(envDirListFull) + 1;
        envDirList = malloc(lenFull + 1);
        snprintf(envDirList, lenFull + 1, "%s:%s", envDirListFull, envDirListSimple);
    }
    else {
        envDirList = envDirListSimple ? strdup(envDirListSimple) : envDirListFull ? strdup(envDirListFull) : NULL;
    }

    return envDirList;
}
