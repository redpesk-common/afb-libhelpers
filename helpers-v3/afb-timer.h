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


#ifndef AFB_TIMER_INCLUDE
#define AFB_TIMER_INCLUDE

#if AFB_BINDING_VERSION <= 3

#ifdef __cplusplus
extern "C" {
#endif

#include <systemd/sd-event.h>
#include <afb/afb-binding.h>

// ctl-timer.c
// ----------------------

typedef struct TimerHandleS {
    int count;
    int delay;
    const char*uid;
    void *context;
    sd_event_source *evtSource;
    afb_api_t api;
    int (*callback) (struct TimerHandleS *handle);
    int (*freeCB) (void *context) ;
} TimerHandleT;

typedef int (*timerCallbackT)(TimerHandleT *context);

extern void TimerEvtStart(afb_api_t api, TimerHandleT *timerHandle, timerCallbackT callback, void *context);
extern void TimerEvtStop(TimerHandleT *timerHandle);

extern uint64_t LockWait(afb_api_t api, uint64_t utimeout);

#ifdef __cplusplus
}
#endif
#endif /* AFB_BINDING_VERSION <= 3 */

#endif // CTL_TIMER_INCLUDE
