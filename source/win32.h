/* 
 * libwinsync - NTSYNC wrapper for Linux
 * Author: Kawaii Ghost <frweird@outlook.co.id>
 * Copyright (c) 2025 Kawaii Ghost. All Rights Reserved.
 * SPDX-License-Identifier: MIT
 */

#define WIN32
#include "nt.h"

typedef const char* LPCSTR;
typedef void* SECURITY_ATTRIBUTES;
typedef SECURITY_ATTRIBUTES* PSECURITY_ATTRIBUTES;
typedef SECURITY_ATTRIBUTES* LPSECURITY_ATTRIBUTES;

#define WAIT_OBJECT_0 0
#define WAIT_OBJECT_1 1
#define WAIT_OBJECT_2 2
#define WAIT_OBJECT_3 3
#define WAIT_OBJECT_4 4
#define WAIT_OBJECT_5 5
#define WAIT_OBJECT_6 6
#define WAIT_OBJECT_7 7
#define WAIT_OBJECT_8 8
#define WAIT_OBJECT_9 9
#define WAIT_OBJECT_10 10
#define WAIT_OBJECT_11 11
#define WAIT_OBJECT_12 12
#define WAIT_OBJECT_13 13
#define WAIT_OBJECT_14 14
#define WAIT_OBJECT_15 15
#define WAIT_OBJECT_16 16
#define WAIT_OBJECT_17 17
#define WAIT_OBJECT_18 18
#define WAIT_OBJECT_19 19
#define WAIT_OBJECT_20 20
#define WAIT_OBJECT_21 21
#define WAIT_OBJECT_22 22
#define WAIT_OBJECT_23 23
#define WAIT_OBJECT_24 24
#define WAIT_OBJECT_25 25
#define WAIT_OBJECT_26 26
#define WAIT_OBJECT_27 27
#define WAIT_OBJECT_28 28
#define WAIT_OBJECT_29 29
#define WAIT_OBJECT_30 30
#define WAIT_OBJECT_31 31
#define WAIT_OBJECT_32 32
#define WAIT_OBJECT_33 33
#define WAIT_OBJECT_34 34
#define WAIT_OBJECT_35 35
#define WAIT_OBJECT_36 36
#define WAIT_OBJECT_37 37
#define WAIT_OBJECT_38 38
#define WAIT_OBJECT_39 39
#define WAIT_OBJECT_40 40
#define WAIT_OBJECT_41 41
#define WAIT_OBJECT_42 42
#define WAIT_OBJECT_43 43
#define WAIT_OBJECT_44 44
#define WAIT_OBJECT_45 45
#define WAIT_OBJECT_46 46
#define WAIT_OBJECT_47 47
#define WAIT_OBJECT_48 48
#define WAIT_OBJECT_49 49
#define WAIT_OBJECT_50 50
#define WAIT_OBJECT_51 51
#define WAIT_OBJECT_52 52
#define WAIT_OBJECT_53 53
#define WAIT_OBJECT_54 54
#define WAIT_OBJECT_55 55
#define WAIT_OBJECT_56 56
#define WAIT_OBJECT_57 57
#define WAIT_OBJECT_58 58
#define WAIT_OBJECT_59 59
#define WAIT_OBJECT_60 60
#define WAIT_OBJECT_61 61
#define WAIT_OBJECT_62 62
#define WAIT_OBJECT_63 63
#define WAIT_TIMEOUT 102
#define WAIT_FAILED UINT32_MAX

#define ERROR_SUCCESS 0
#define ERROR_INVALID_FUNCTION 1
#define ERROR_ACCESS_DENIED 5
#define ERROR_INVALID_HANDLE 6
#define ERROR_INVALID_PARAMETER 87
#define ERROR_ARITHMETIC_OVERFLOW 534
#define ERROR_NOACCESS 998
#define ERROR_TIMEOUT 1460
#define ERROR_GEN_FAILURE 31

#define INFINITE UINT32_MAX

#define CREATE_EVENT_MANUAL_RESET 1
#define CREATE_EVENT_INITIAL_SET 2

bool ntsync_init(void);
void ntsync_exit(void);

DWORD GetLastError(void);

HANDLE CreateSemaphoreA(
        LPSECURITY_ATTRIBUTES SemaphoreAttributes,
        LONG InitialCount,
        LONG MaximumCount,
        LPCSTR Name
);

HANDLE CreateSemaphoreExA(
        LPSECURITY_ATTRIBUTES SemaphoreAttributes,
        LONG InitialCount,
        LONG MaximumCount,
        LPCSTR Name,
        DWORD Flags,
        DWORD DesiredAccess
);

BOOL ReleaseSemaphore(
        HANDLE Semaphore,
        LONG ReleaseCount,
        LONG *PreviousCount
);

HANDLE CreateEventA(
        LPSECURITY_ATTRIBUTES EventAttributes,
        BOOL ManualReset,
        BOOL InitialState,
        LPCSTR Name
);

HANDLE CreateEventExA(
        LPSECURITY_ATTRIBUTES EventAttributes,
        LPCSTR Name,
        DWORD Flags,
        DWORD DesiredAccess
);

BOOL SetEvent(HANDLE Event);
BOOL ResetEvent(HANDLE Event);
BOOL PulseEvent(HANDLE Event) __attribute__((deprecated));

DWORD WaitForSingleObject(
        HANDLE Handle,
        DWORD Milliseconds
);

DWORD WaitForSingleObjectEx(
        HANDLE Handle,
        DWORD Milliseconds,
        BOOL Alertable
);

DWORD WaitForMultipleObjects(
        DWORD Count,
        const HANDLE *Handles,
        BOOL WaitAll,
        DWORD Milliseconds
);

DWORD WaitForMultipleObjectsEx(
        DWORD Count,
        const HANDLE *Handles,
        BOOL WaitAll,
        DWORD Milliseconds,
        BOOL Alertable
);

BOOL CloseHandle(
        HANDLE Object
);
