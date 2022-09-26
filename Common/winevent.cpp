#if defined(linux)
#include <assert.h>
#include <algorithm>
#include <sys/time.h>
#endif
#include "winevent.h"


#if defined(__linux) || defined(__linux__) || defined(linux)
bool RemoveExpiredWaitHelper(my_mevent_info_t_ wait)
{
    int result = pthread_mutex_trylock(&wait.Waiter->Mutex);

    if (result == EBUSY)
    {
        return false;
    }

    assert(result == 0);

    if (wait.Waiter->StillWaiting == false)
    {
        --wait.Waiter->RefCount;
        assert(wait.Waiter->RefCount >= 0);
        if (wait.Waiter->RefCount == 0)
        {
            wait.Waiter->Destroy();
            delete wait.Waiter;
        }
        else
        {
            result = pthread_mutex_unlock(&wait.Waiter->Mutex);
            assert(result == 0);
        }

        return true;
    }

    result = pthread_mutex_unlock(&wait.Waiter->Mutex);
    assert(result == 0);

    return false;
}

int DestroyEvent(HEVENT event)
{
    int result = 0;

    result = pthread_mutex_lock(&event->Mutex);
    assert(result == 0);
    event->RegisteredWaits.erase(std::remove_if (event->RegisteredWaits.begin(), event->RegisteredWaits.end(), RemoveExpiredWaitHelper), event->RegisteredWaits.end());
    result = pthread_mutex_unlock(&event->Mutex);
    assert(result == 0);

    result = pthread_cond_destroy(&event->CVariable);
    pthread_condattr_destroy(&event->CVariable_attr);
    assert(result == 0);

    result = pthread_mutex_destroy(&event->Mutex);
    assert(result == 0);

    delete event;

    return 0;
}

int SetEvent(HEVENT event)
{
    int result = pthread_mutex_lock(&event->Mutex);
    assert(result == 0);

    event->State = true;

    //Depending on the event type, we either trigger everyone or only one
    if (event->AutoReset)
    {
        while (!event->RegisteredWaits.empty())
        {
            HMEVENT_INFO i = &event->RegisteredWaits.front();

            result = pthread_mutex_lock(&i->Waiter->Mutex);
            assert(result == 0);

            --i->Waiter->RefCount;
            assert(i->Waiter->RefCount >= 0);
            if (!i->Waiter->StillWaiting)
            {
                if (i->Waiter->RefCount == 0)
                {
                    i->Waiter->Destroy();
                    delete i->Waiter;
                }
                else
                {
                    result = pthread_mutex_unlock(&i->Waiter->Mutex);
                    assert(result == 0);
                }
                event->RegisteredWaits.pop_front();
                continue;
            }

            event->State = false;

            if (i->Waiter->WaitAll)
            {
                --i->Waiter->Status.EventsLeft;
                assert(i->Waiter->Status.EventsLeft >= 0);
                //We technically should do i->Waiter->StillWaiting = Waiter->Status.EventsLeft != 0
                //but the only time it'll be equal to zero is if we're the last event, so no one
                //else will be checking the StillWaiting flag. We're good to go without it.
            }
            else
            {
                i->Waiter->Status.FiredEvent = i->WaitIndex;
                i->Waiter->StillWaiting = false;
            }

            result = pthread_mutex_unlock(&i->Waiter->Mutex);
            assert(result == 0);

            result = pthread_cond_signal(&i->Waiter->CVariable);
            assert(result == 0);

            event->RegisteredWaits.pop_front();

            result = pthread_mutex_unlock(&event->Mutex);
            assert(result == 0);

            return 0;
        }

        //event->State can be false if compiled with WFMO support
        if (event->State)
        {
            result = pthread_mutex_unlock(&event->Mutex);
            assert(result == 0);

            result = pthread_cond_signal(&event->CVariable);
            assert(result == 0);

            return 0;
        }
    }
    else
    {
        for (size_t i = 0; i < event->RegisteredWaits.size(); ++i)
        {
            HMEVENT_INFO info = &event->RegisteredWaits[i];

            result = pthread_mutex_lock(&info->Waiter->Mutex);
            assert(result == 0);

            --info->Waiter->RefCount;
            assert(info->Waiter->RefCount >= 0);

            if (!info->Waiter->StillWaiting)
            {
                if (info->Waiter->RefCount == 0)
                {
                    info->Waiter->Destroy();
                    delete info->Waiter;
                }
                else
                {
                    result = pthread_mutex_unlock(&info->Waiter->Mutex);
                    assert(result == 0);
                }
                continue;
            }

            if (info->Waiter->WaitAll)
            {
                --info->Waiter->Status.EventsLeft;
                assert(info->Waiter->Status.EventsLeft >= 0);
                //We technically should do i->Waiter->StillWaiting = Waiter->Status.EventsLeft != 0
                //but the only time it'll be equal to zero is if we're the last event, so no one
                //else will be checking the StillWaiting flag. We're good to go without it.
            }
            else
            {
                info->Waiter->Status.FiredEvent = info->WaitIndex;
                info->Waiter->StillWaiting = false;
            }

            result = pthread_mutex_unlock(&info->Waiter->Mutex);
            assert(result == 0);

            result = pthread_cond_signal(&info->Waiter->CVariable);
            assert(result == 0);
        }
        event->RegisteredWaits.clear();

        result = pthread_mutex_unlock(&event->Mutex);
        assert(result == 0);

        result = pthread_cond_broadcast(&event->CVariable);
        assert(result == 0);
    }

    return 0;
}

int ResetEvent(HEVENT event)
{
    int result = pthread_mutex_lock(&event->Mutex);
    assert(result == 0);

    event->State = false;

    result = pthread_mutex_unlock(&event->Mutex);
    assert(result == 0);

    return 0;
}

#ifdef PULSE
int PulseEvent(HEVENT event)
{
    //This may look like it's a horribly inefficient kludge with the sole intention of reducing code duplication,
    //but in reality this is what any PulseEvent() implementation must look like. The only overhead (function 
    //calls aside, which your compiler will likely optimize away, anyway), is if only WFMO auto-reset waits are active
    //there will be overhead to unnecessarily obtain the event mutex for ResetEvent() after. In all other cases (being 
    //no pending waits, WFMO manual-reset waits, or any WFSO waits), the event mutex must first be released for the
    //waiting thread to resume action prior to locking the mutex again in order to set the event state to unsignaled, 
    //or else the waiting threads will loop back into a wait (due to checks for spurious CVariable wakeups).

    int result = SetEvent(event);
    assert(result == 0);
    result = ResetEvent(event);
    assert(result == 0);

    return 0;
}
#endif
HEVENT CreateEvent(const void* unused, bool manualReset, bool initialState, const char* strName)//const std::string& strEventName)
{
    //unused event name
    
    HEVENT event = new EVENT();
    
    pthread_condattr_init(&event->CVariable_attr);
#if _POSIX_MONOTONIC_CLOCK > 0
    pthread_condattr_setclock(&event->CVariable_attr, CLOCK_MONOTONIC);
#endif
    int result = pthread_cond_init(&event->CVariable, &event->CVariable_attr);
    assert(result == 0);

    result = pthread_mutex_init(&event->Mutex, 0);
    assert(result == 0);

    event->State = false;
    event->AutoReset = !manualReset;

    if (initialState)
    {
        result = SetEvent(event);
        assert(result == 0);
    }

    return event;
}

int CloseHandle(HEVENT event)
{
    DestroyEvent(event);
    return 0;
}

int UnlockedWaitForEvent(HEVENT event, uint64_t milliseconds)
{
    int result = 0;
    if (!event->State)
    {
        //Zero-timeout event state check optimization
        if (milliseconds == 0)
        {
            return WAIT_TIMEOUT;
        }

        timespec ts;
        if (milliseconds != (uint64_t) -1)
        {
            timeval tv;
            gettimeofday(&tv, NULL);

            uint64_t nanoseconds = ((uint64_t) tv.tv_sec) * 1000 * 1000 * 1000 + milliseconds * 1000 * 1000 + ((uint64_t) tv.tv_usec) * 1000;

            ts.tv_sec = nanoseconds / 1000 / 1000 / 1000;
            ts.tv_nsec = (nanoseconds - ((uint64_t) ts.tv_sec) * 1000 * 1000 * 1000);
        }

        do
        {
            //Regardless of whether it's an auto-reset or manual-reset event:
            //wait to obtain the event, then lock anyone else out
            if (milliseconds != (uint64_t) -1)
            {
                result = pthread_cond_timedwait(&event->CVariable, &event->Mutex, &ts);
            }
            else
            {
                result = pthread_cond_wait(&event->CVariable, &event->Mutex);
            }
        } while (result == 0 && !event->State);

        if (result == 0 && event->AutoReset)
        {
            //We've only accquired the event if the wait succeeded
            event->State = false;
        }
    }
    else if (event->AutoReset)
    {
        //It's an auto-reset event that's currently available;
        //we need to stop anyone else from using it
        result = 0;
        event->State = false;
    }
    //Else we're trying to obtain a manual reset event with a signaled state;
    //don't do anything

    return result;
}

int WaitForSingleObject(HEVENT event, uint64_t milliseconds)
{
    int tempResult;
    if (milliseconds == 0)
    {
        tempResult = pthread_mutex_trylock(&event->Mutex);
        if (tempResult == EBUSY)
        {
            return WAIT_TIMEOUT;
        }
    }
    else
    {
        tempResult = pthread_mutex_lock(&event->Mutex);
    }

    assert(tempResult == 0);

    int result = UnlockedWaitForEvent(event, milliseconds);

    tempResult = pthread_mutex_unlock(&event->Mutex);
    assert(tempResult == 0);

    return result;
}

int WaitForMultipleObjects(int count, HEVENT *events, bool waitAll, uint64_t milliseconds, int &waitIndex)
{
    HMEVENT wfmo = new MEVENT();
    pthread_condattr_init(&wfmo->CVariable_attr);
#if _POSIX_MONOTONIC_CLOCK > 0
    pthread_condattr_setclock(&wfmo->CVariable_attr, CLOCK_MONOTONIC);
#endif
    int result = 0;
    int tempResult = pthread_mutex_init(&wfmo->Mutex, 0);
    assert(tempResult == 0);

    tempResult = pthread_cond_init(&wfmo->CVariable, &wfmo->CVariable_attr);
    assert(tempResult == 0);

    my_mevent_info_t_ waitInfo;
    waitInfo.Waiter = wfmo;
    waitInfo.WaitIndex = -1;

    wfmo->WaitAll = waitAll;
    wfmo->StillWaiting = true;
    wfmo->RefCount = 1;

    if (waitAll)
    {
        wfmo->Status.EventsLeft = count;
    }
    else
    {
        wfmo->Status.FiredEvent = -1;
    }

    tempResult = pthread_mutex_lock(&wfmo->Mutex);
    assert(tempResult == 0);

    bool done = false;
    waitIndex = -1;

    for (int i = 0; i < count; ++i)
    {
        waitInfo.WaitIndex = i;

        //Must not release lock until RegisteredWait is potentially added
        tempResult = pthread_mutex_lock(&events[i]->Mutex);
        assert(tempResult == 0);

        //Before adding this wait to the list of registered waits, let's clean up old, expired waits while we have the event lock anyway
        events[i]->RegisteredWaits.erase(std::remove_if (events[i]->RegisteredWaits.begin(), events[i]->RegisteredWaits.end(), RemoveExpiredWaitHelper), events[i]->RegisteredWaits.end());

        if (UnlockedWaitForEvent(events[i], 0) == 0)
        {
            tempResult = pthread_mutex_unlock(&events[i]->Mutex);
            assert(tempResult == 0);

            if (waitAll)
            {
                --wfmo->Status.EventsLeft;
                assert(wfmo->Status.EventsLeft >= 0);
            }
            else
            {
                wfmo->Status.FiredEvent = i;
                waitIndex = i;
                done = true;
                break;
            }
        }
        else
        {
            events[i]->RegisteredWaits.push_back(waitInfo);
            ++wfmo->RefCount;

            tempResult = pthread_mutex_unlock(&events[i]->Mutex);
            assert(tempResult == 0);
        }
    }

    timespec ts;
    if (!done)
    {
        if (milliseconds == 0)
        {
            result = WAIT_TIMEOUT;
            done = true;
        }
        else if (milliseconds != (uint64_t) -1)
        {
            timeval tv;
            gettimeofday(&tv, NULL);

            uint64_t nanoseconds = ((uint64_t) tv.tv_sec) * 1000 * 1000 * 1000 + milliseconds * 1000 * 1000 + ((uint64_t) tv.tv_usec) * 1000;

            ts.tv_sec = nanoseconds / 1000 / 1000 / 1000;
            ts.tv_nsec = (nanoseconds - ((uint64_t) ts.tv_sec) * 1000 * 1000 * 1000);
        }
    }

    while (!done)
    {
        //One (or more) of the events we're monitoring has been triggered?

        //If we're waiting for all events, assume we're done and check if there's an event that hasn't fired
        //But if we're waiting for just one event, assume we're not done until we find a fired event
        done = (waitAll && wfmo->Status.EventsLeft == 0) || (!waitAll && wfmo->Status.FiredEvent != -1);

        if (!done)
        {
            if (milliseconds != (uint64_t) -1)
            {
                result = pthread_cond_timedwait(&wfmo->CVariable, &wfmo->Mutex, &ts);
            }
            else
            {
                result = pthread_cond_wait(&wfmo->CVariable, &wfmo->Mutex);
            }

            if (result != 0)
            {
                break;
            }
        }
    }

    waitIndex = wfmo->Status.FiredEvent;
    wfmo->StillWaiting = false;

    --wfmo->RefCount;
    assert(wfmo->RefCount >= 0);
    if (wfmo->RefCount == 0)
    {
        wfmo->Destroy();
        delete wfmo;
    }
    else
    {
        tempResult = pthread_mutex_unlock(&wfmo->Mutex);
        assert(tempResult == 0);
    }
    if (0 == result && !waitAll)
    {
        result = waitIndex + WAIT_OBJECT_0;
    }
    return result;
}

int WaitForMultipleObjects(int count, HEVENT *events,  bool waitAll, uint64_t milliseconds)
{
    int unused;
    return WaitForMultipleObjects(count, events, waitAll, milliseconds, unused);
}

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)


#endif