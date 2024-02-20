// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

#include "dbgctrl.h"

#define LOG_ENABLE       // debug log enable
#define LOG_FILE			    "/atmosphere/logs/sysmodule.log"
//#define TARGET_TITLE_ID         (0x1122334455667788ULL) // target application id
#define TARGET_TITLE_ID         (0x0100800015926000ULL) //suika game
#define ONE_SEC                 (1000000000ULL)

// Size of the inner heap (adjust as necessary).
#define INNER_HEAP_SIZE 0x20000

u64 ThreadId;

#ifdef __cplusplus
extern "C" {
#endif

// Sysmodules should not use applet*.
u32 __nx_applet_type = AppletType_None;

// Sysmodules will normally only want to use one FS session.
u32 __nx_fs_num_sessions = 1;

// Newlib heap configuration function (makes malloc/free work).
void __libnx_initheap(void)
{
    static u8 inner_heap[INNER_HEAP_SIZE];
    extern void* fake_heap_start;
    extern void* fake_heap_end;

    // Configure the newlib heap.
    fake_heap_start = inner_heap;
    fake_heap_end   = inner_heap + sizeof(inner_heap);
}

void userAppInit(void)
{
    DcInit();
}

void userAppExit(void)
{
    DcDeinit();
}

#ifdef __cplusplus
}
#endif

static DEBUG_CB_STATUS DebugCallback(DEBUG_EVENT evt, u64 thread_id, u64 addr, void* arg)
{
    DEBUG_CB_STATUS cbr;

    cbr = DEBUG_CB_CONTINUE;
    switch (evt)
    {
    case EVT_TARGET_CONNECT:
        DcLogPrint("EVT_TARGET_CONNECT\n");
        break;
    case EVT_TARGET_DISCONNECT:
        DcLogPrint("EVT_TARGET_DISCONNECT\n");
        break;
    case EVT_BREAK_POINT:
        DcLogPrint("EVT_BREAK_POINT\n");
        break;
    case EVT_WATCH_POINT:
        DcLogPrint("EVT_WATCH_POINT\n");
        break;
    case EVT_SW_BREAK_POINT:
        DcLogPrint("EVT_SW_BREAK_POINT\n");
        break;
    case EVT_TRAP:
        DcLogPrint("EVT_TRAP\n");
        break;
    case EVT_PREFETCH_ABORT:
        DcLogPrint("EVT_PREFETCH_ABORT\n");
        break;
    case EVT_DATA_ABORT:
        DcLogPrint("EVT_DATA_ABORT\n");
        break;
    case EVT_ALIGNMENT_FAULT:
        DcLogPrint("EVT_DATA_ABORT\n");
        break;
    case EVT_PAUSE:
        DcLogPrint("EVT_PAUSE\n");
        ThreadId = thread_id;
        cbr = DEBUG_CB_STOP;
        break;
    default:
        DcLogPrint("invalid evt=%d\n", evt);
        cbr = DEBUG_CB_ERROR;
        break;
    }

    return cbr;
}

// program entrypoint
int main(int argc, char* argv[])
{
    bool b;
    u32 cnt = 0;

#ifdef LOG_ENABLE
    b = DcLogStart(LOG_FILE);
    if (b == false)
    {
        //error
        return 0;
    }
#endif

    do
    {
        b = DcWaitForTarget(TARGET_TITLE_ID, (5 * ONE_SEC));
        // return true if target is found
    } while (b == false);
    
    b = DcTargetConnect(TARGET_TITLE_ID, 48, DebugCallback, NULL);
    if (b == false)
    {
        DcLogPrint("DcTargetConnect error\n");

#ifdef LOG_ENABLE
        DcLogStop();
#endif
        return 0;
    }

    // if 0 set to argument, API will return immediately.
    // timeout is nanoseconds unit.
    while (DcIsProcessing(ONE_SEC))
    {
        // Loop if application is running

        cnt ++;
        if (cnt == 10)
        {
            b = DcDebugPause();
            DcLogPrint("DcDebugPause=%d\n", b);
        }

        if (cnt == 20)
        {
            b = DcDebugContinue(ThreadId);
            DcLogPrint("DcDebugContinue=%d\n", b);
            break;
        }
    }

    DcTargetDisconnect();

#ifdef LOG_ENABLE
    DcLogStop();
#endif

    return 0;
}
