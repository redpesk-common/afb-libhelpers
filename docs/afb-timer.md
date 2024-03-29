# AFB Timer functions reference

## TimerHandleT

Members are:

* `count`: integer representing the number of times the timers should run.
* `delay`: millisecond integer representing the delay to wait before and between
 the callback run.
* `uid`: a string identifying the timer.
* `context`: an opaq pointer that could be used in the callback function.
* `evtSource`: a systemd event source struct. Should be NULL.
* `api`: the AFB api pointer.
* `callback`: a function pointer for the callback to call at timer expiration
* `freeCB`: a function pointer called after expiration of the timer. Mainly meant
 to release the context pointer by example.

## void TimerEvtStart(afb_api_t api, TimerHandleT *timerHandle, timerCallbackT callback, void *context)

Start a timer which invokes the callback when the delay expires for `count`
times.

* `api`: AFB api pointer.
* `timerHandle`: pointer to struct representing a timer.
* `callback`: a function pointer for the callback to call at timer expiration
* `context`: an opaq pointer that could be used in the callback function.

## void TimerEvtStop(TimerHandleT *timerHandle)

Manually stop the timer's run. If the `count` isn't finished then it will end
the timer and no other runs will occur.

* `timerHandle`: pointer to struct representing a timer.

## uint64_t LockWait(afb_api_t api, uint64_t utimeout)

It is function acting like a non-blocking sleep for an API. It lets the main API
event loop runs while you are waiting and will unlock at the first received
event and returns the remaining time to wait if an event occurs or 0 if no events
occured and timeout hits. Then you have to manually ensure that once an event
has been received that it was the one you are waiting for and if not launch again
the wait with the remaining time returned.

* `api`: AFB api pointer.
* `timeout`: timeout in microsecond.

Returns the remaining time in microsecond to wait if an event occurs or 0 if no
events occured and timeout hits
