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
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <systemd/sd-event.h>

#include "afb-timer.h"

static int TimerNext (sd_event_source* source, uint64_t timer, void* handle) {
    TimerHandleT *timerHandle = (TimerHandleT*) handle;
    int done;
    uint64_t usec;

    // Stop Timer when count is null
    if (timerHandle->count-- == 0) {
        if (timerHandle->freeCB) timerHandle->freeCB(timerHandle->context);
        timerHandle->freeCB=NULL;
        sd_event_source_set_enabled (timerHandle->evtSource, SD_EVENT_OFF);
        sd_event_source_unref (timerHandle->evtSource);
        free (timerHandle);
        return -1;
    }

    done= timerHandle->callback(timerHandle);
    if (!done) {
        AFB_API_WARNING(timerHandle->api, "TimerNext Callback Fail Tag=%s", timerHandle->uid);
        return -1;
    }

    // otherwise validate timer for a new run
    sd_event_now(afb_api_get_event_loop(timerHandle->api), CLOCK_MONOTONIC, &usec);
    sd_event_source_set_time(source, usec + timerHandle->delay*1000);
    sd_event_source_set_enabled(source, SD_EVENT_ONESHOT);

    return 0;
}

void TimerEvtStop(TimerHandleT *timerHandle) {
    timerHandle->count = 0; // Force timer to close
}

void TimerEvtStart(afb_api_t api, TimerHandleT *timerHandle, timerCallbackT callback, void *context) {
    uint64_t usec;

    // populate CB handle
    timerHandle->callback=callback;
    timerHandle->context=context;
    timerHandle->api=api;

    // set a timer with ~250us accuracy
    sd_event_now(afb_api_get_event_loop(api), CLOCK_MONOTONIC, &usec);
    sd_event_add_time(afb_api_get_event_loop(api), &timerHandle->evtSource, CLOCK_MONOTONIC, usec+timerHandle->delay*1000, 0, TimerNext, timerHandle);
}

uint64_t LockWait(afb_api_t api, uint64_t utimeout) {
    uint64_t current_usec, pre_usec;

    struct sd_event *event = afb_api_get_event_loop(api);

    sd_event_now(event, CLOCK_MONOTONIC, &pre_usec);
    sd_event_run(event, utimeout);
    sd_event_now(event, CLOCK_MONOTONIC, &current_usec);

    uint64_t diff = current_usec - pre_usec;
    utimeout = utimeout < diff ? 0 : utimeout - diff;
    return utimeout;
}
