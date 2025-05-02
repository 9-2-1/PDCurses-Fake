#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdbool.h>
#include "_richia.h"

static CRITICAL_SECTION request_section;
static CRITICAL_SECTION print_section;

#define MAX_REQUESTS 256
#define MAX_MESSAGE_LEN 1024

static struct request {
    HANDLE hevent;
    _Bool needReply;
    char msg[MAX_MESSAGE_LEN];
} requests [MAX_REQUESTS];

static int next_request_id = 0;

static HANDLE hReadStdinThread = INVALID_HANDLE_VALUE;
static HANDLE hReadStdinQuit = INVALID_HANDLE_VALUE;

static int _richia_read_stdin(char* buffer, int buffer_size) {
    int pos = 0;
    buffer[0] = '\0';

    while (WaitForSingleObject(hReadStdinQuit, 0) == WAIT_TIMEOUT) {
        char c = getchar();

        if (c == '\r' || c == '\n') {
            buffer[pos] = '\0';
            break;
        }

        buffer[pos] = c;
        pos++;
        Sleep(10);
    }
    return pos;
}

static DWORD WINAPI _richia_read_stdin_thread(LPVOID lpParam)
{
    char input_buffer[MAX_MESSAGE_LEN];
    unsigned int reqid;
    unsigned int offset;
    while (WaitForSingleObject(hReadStdinQuit, 0) != WAIT_OBJECT_0)
    {
        if (_richia_read_stdin(input_buffer, MAX_MESSAGE_LEN))
        {
            int result = sscanf(input_buffer, "%u %n", &reqid, &offset);
            if (result < 0)
                // EOF
                break;
            if (result != 1)
                continue;
            if (reqid >= MAX_REQUESTS)
                continue;
            EnterCriticalSection(&request_section);
            if (requests[reqid].hevent == INVALID_HANDLE_VALUE)
                goto leave;
            if (!requests[reqid].needReply)
                goto leave;
            // Reply received
            strncpy(requests[reqid].msg, input_buffer + offset, MAX_MESSAGE_LEN - 1);
            requests[reqid].needReply = FALSE;
            SetEvent(requests[reqid].hevent);
            leave:
            LeaveCriticalSection(&request_section);
            // requests[reqid].hevent = INVALID_HANDLE_VALUE;
        }
    }
    return 0;
}

static int _richia_newreqid()
{
    EnterCriticalSection(&request_section);
    int reqid;
    for (int i = 0; i < MAX_REQUESTS; i++)
    {
        reqid = (next_request_id + i) % MAX_REQUESTS;
        if (requests[reqid].hevent == INVALID_HANDLE_VALUE)
        {
            next_request_id = (reqid + 1) % MAX_REQUESTS;
            requests[reqid].hevent = CreateEvent(NULL, TRUE, FALSE, NULL);
            if (requests[reqid].hevent == NULL) {
                requests[reqid].hevent = INVALID_HANDLE_VALUE;
                reqid = -1;
                goto leave;
            }
            requests[reqid].needReply = TRUE;
            requests[reqid].msg[0] = '\0';
            goto leave;
        }
    }
    reqid = -1;
    leave:
    LeaveCriticalSection(&request_section);
    return reqid;
}

static char *_richia_waitreqid(unsigned int reqid)
{
    HANDLE handles[2] = { requests[reqid].hevent, hReadStdinQuit };
    DWORD result = WaitForMultipleObjects(2, handles, FALSE, INFINITE);
    
    char *ans_copy = NULL;
    EnterCriticalSection(&request_section);
    if (result == WAIT_OBJECT_0)
    {
        const char* ans = requests[reqid].msg;
        size_t size = strlen(ans);
        ans_copy = malloc(size + 1);
        if (ans_copy != NULL)
        {
            strncpy(ans_copy, ans, size + 1);
        }
    }
    CloseHandle(requests[reqid].hevent);
    requests[reqid].needReply = FALSE;
    requests[reqid].hevent = INVALID_HANDLE_VALUE;
    requests[reqid].msg[0] = '\0';
    LeaveCriticalSection(&request_section);
    return ans_copy;
}

void _richia_write(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    EnterCriticalSection(&print_section);
    vprintf(format, args);
    putchar('\n');
    LeaveCriticalSection(&print_section);
}

char *_richia_ask_and_wait(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    int reqid = _richia_newreqid();
    if (reqid == -1)
        return NULL;
    EnterCriticalSection(&print_section);
    printf("reqid %d ", reqid);
    vprintf(format, args);
    putchar('\n');
    LeaveCriticalSection(&print_section);
    _richia_flush();
    return _richia_waitreqid(reqid);
}

void _richia_flush()
{
    EnterCriticalSection(&print_section);
    fflush(stdout);
    LeaveCriticalSection(&print_section);
}

void _richia_freemsg(char *msg)
{
    free(msg);
}

void _richia_init()
{
    InitializeCriticalSection(&print_section);
    InitializeCriticalSection(&request_section);
    for (int i = 0; i < MAX_REQUESTS; i++)
    {
        requests[i].hevent = INVALID_HANDLE_VALUE;
        requests[i].needReply = FALSE;
        requests[i].msg[0] = '\0';
    }
    next_request_id = 0;
    hReadStdinQuit = CreateEvent(NULL, TRUE, FALSE, NULL);
    hReadStdinThread = CreateThread(NULL, 0, _richia_read_stdin_thread, NULL, 0, NULL);
}

void _richia_close()
{
    for (int i = 0; i < MAX_REQUESTS; i++)
    {
        if (requests[i].hevent != INVALID_HANDLE_VALUE)
        {
            CloseHandle(requests[i].hevent);
        }
    }
    SetEvent(hReadStdinQuit);
    WaitForSingleObject(hReadStdinThread, INFINITE);
    CloseHandle(hReadStdinThread);
    CloseHandle(hReadStdinQuit);
    DeleteCriticalSection(&print_section);
    DeleteCriticalSection(&request_section);
}