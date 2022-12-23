/*
 * Copyright (C) 2021 "IoT.bzh"
 * Author: Jose Bollo <jose.bollo@iot.bzh>
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

#ifndef AFB_HELPERS_UTILS_H
#define AFB_HELPERS_UTILS_H

#include <limits.h>


/* with gcc >= 7.2.0, this macro is useful when printing an int with snprintf:
 *
 * char[INT_STR_MAX]; // smaller value leads to a warning
 * snprintf(targetS, sizeof (targetS), "%d", target);
 * */

#define __str__(s) #s
#define __xstr__(s) __str__(s)
#define INT_STR_MAX sizeof(__xstr__(INT_MIN))
#define UINT_STR_MAX sizeof(__xstr__(UINT_MAX))

#endif /* AFB_HELPERS_UTILS_H */
