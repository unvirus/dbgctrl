// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

#include "dbgctrl.h"

//#define LOG_ENABLE       //���O�o�͂��鎞�͗L���ɂ���
#define LOG_FILE			        "/atmosphere/logs/sysmodule.log"
#define TARGET_TITILE_ID            (0x0100111001110000ULL) //�f�o�b�O����^�C�g��ID

// Size of the inner heap (adjust as necessary).
#define INNER_HEAP_SIZE 0x80000

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
    default:
        DcLogPrint("invalid evt=%d\n", evt);
        cbr = DEBUG_CB_ERROR;
        break;
    }

    return cbr;
}

// Main program entrypoint
int main(int argc, char* argv[])
{
    bool b;

#ifdef LOG_ENABLE
    b = DcLogStart(LOG_FILE);
    if (b == false)
    {
        //error
        return 0;
    }
#endif

    /*
    sysmodule��CFW�N�����Ɏ����Ŏ��s����� 
    ���̂Ƃ��A�v���͋N�����Ă��Ȃ��̂ŁADcTargetConnect()�̓G���[�ɂȂ� 
    �T���v�������Ȃ̂ł��̂悤�ȍ��ɂȂ��Ă��邾��
    �A�v���N�����ovl-sysmodules��sysmodule���N��������΂悢 
    ovl-sysmodules��sysmodule���~������ꍇ�́A�����I���ɂȂ�̂� 
    sysmodule�����쒆�ł������I�ɊJ������� 
    */
    b = DcTargetConnect(TARGET_TITILE_ID, 48, DebugCallback, NULL);
    if (b == false)
    {
        DcLogPrint("DcTargetConnect error\n");

#ifdef LOG_ENABLE
        DcLogStop();
#endif
        return 0;
    }

    //if 0 set to argument, API will return immediately.
    //timeout is nanoseconds unit.
    while (DcIsProcessing(1000000000ULL))
    {
        //�A�v�������s���ł���΃��[�v����
    };

    DcTargetDisconnect();

#ifdef LOG_ENABLE
    DcLogStop();
#endif

    return 0;
}
