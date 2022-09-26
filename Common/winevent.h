/*
 * WIN32 Events for Linux
 * Linux实现版本
*/

#if defined(__linux) || defined(__linux__) || defined(linux)
#include <pthread.h>
#include <stdint.h>
 #include <errno.h>
 #include <deque>
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include <windows.h>
#define WAIT_FOREVER            INFINITE
#endif

#if defined(__linux) || defined(__linux__) || defined(linux)

#define WAIT_FOREVER            ((int) -1)
#define WAIT_OBJECT_0           ((int)0)
#define WAIT_TIMEOUT            ETIMEDOUT
typedef struct my_mevent_t_
{
    pthread_mutex_t Mutex;
    pthread_cond_t CVariable;
    pthread_condattr_t CVariable_attr;
    
    int RefCount;
    union
    {
        int FiredEvent; 
        int EventsLeft; 
    } Status;
    bool WaitAll;
    bool StillWaiting;

    void Destroy()
    {
        pthread_mutex_destroy(&Mutex);
        pthread_cond_destroy(&CVariable);
        pthread_condattr_destroy(&CVariable_attr);
    }
}MEVENT, *HMEVENT;

struct my_mevent_info_t_
{
    HMEVENT Waiter;
    int WaitIndex;
};
typedef my_mevent_info_t_ *HMEVENT_INFO;

typedef struct my_event_t_
{
    pthread_cond_t CVariable;
    pthread_condattr_t CVariable_attr;
    pthread_mutex_t Mutex;
    bool AutoReset;
    bool State; 
    std::deque<my_mevent_info_t_> RegisteredWaits;
}EVENT, *HEVENT;
HEVENT CreateEvent(const void* unused, bool manualReset, bool initialState, const char* strName);
int CloseHandle(HEVENT event);
int ResetEvent(HEVENT event);
int SetEvent(HEVENT event);
int WaitForSingleObject(HEVENT event, uint64_t milliseconds);
int WaitForMultipleObjects(int count, HEVENT *events,  bool waitAll, uint64_t milliseconds);

#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
typedef HANDLE HEVENT;
#endif

