/* 
 * libwinsync - NTSYNC wrapper for Linux
 * Author: Kawaii Ghost <frweird@outlook.co.id>
 * Copyright (c) 2025 Kawaii Ghost. All Rights Reserved.
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <linux/ntsync.h>

extern int ntsync;

typedef bool BOOL;
typedef bool BOOLEAN;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef LONG* PLONG;
typedef uint32_t ULONG;
typedef ULONG* PULONG;
typedef int64_t LONGLONG;
typedef uint64_t ULONGLONG;
typedef uint32_t NTSTATUS;
typedef void VOID;
typedef VOID* PVOID;
typedef PVOID _OBJECT_ATTRIBUTES;
typedef _OBJECT_ATTRIBUTES OBJECT_ATTRIBUTES;
typedef OBJECT_ATTRIBUTES* POBJECT_ATTRIBUTES;

#ifdef WIN32
typedef PVOID _HANDLE;
typedef _HANDLE HANDLE;
#else
typedef struct _HANDLE {
        ULONG DesiredAccess;
        int Object;
} HANDLE;
#endif

typedef HANDLE* PHANDLE;

typedef union _LARGE_INTEGER {
        struct {
                DWORD LowPart;
                LONG HighPart;
        } DUMMYSTRUCTNAME;
        struct {
                DWORD LowPart;
                LONG HighPart;
        } u;
        LONGLONG QuadPart;
} LARGE_INTEGER, *PLARGE_INTEGER;

typedef enum _WAIT_TYPE
{
        WaitAll,
        WaitAny,
} WAIT_TYPE;

typedef enum _EVENT_TYPE
{
        NotificationEvent,
        SynchronizationEvent,
} EVENT_TYPE;

typedef enum _EVENT_INFORMATION_CLASS
{
    EventBasicInformation
} EVENT_INFORMATION_CLASS;

typedef struct _EVENT_BASIC_INFORMATION
{
    EVENT_TYPE EventType;
    LONG EventState;
} EVENT_BASIC_INFORMATION, *PEVENT_BASIC_INFORMATION;

typedef enum _SEMAPHORE_INFORMATION_CLASS
{
    SemaphoreBasicInformation
} SEMAPHORE_INFORMATION_CLASS;

typedef struct _SEMAPHORE_BASIC_INFORMATION
{
    LONG CurrentCount;
    LONG MaximumCount;
} SEMAPHORE_BASIC_INFORMATION, *PSEMAPHORE_BASIC_INFORMATION;

#define TRUE true
#define FALSE false
#define NSEC_PER_SEC 1000000000LL

#define MAXIMUM_WAIT_OBJECTS NTSYNC_MAX_WAIT_COUNT
#define STATUS_SUCCESS 0
#define STATUS_WAIT_0 0
#define STATUS_WAIT_1 1
#define STATUS_WAIT_2 2
#define STATUS_WAIT_3 3
#define STATUS_WAIT_4 4
#define STATUS_WAIT_5 5
#define STATUS_WAIT_6 6
#define STATUS_WAIT_7 7
#define STATUS_WAIT_8 8
#define STATUS_WAIT_9 9
#define STATUS_WAIT_10 10
#define STATUS_WAIT_11 11
#define STATUS_WAIT_12 12
#define STATUS_WAIT_13 13
#define STATUS_WAIT_14 14
#define STATUS_WAIT_15 15
#define STATUS_WAIT_16 16
#define STATUS_WAIT_17 17
#define STATUS_WAIT_18 18
#define STATUS_WAIT_19 19
#define STATUS_WAIT_20 20
#define STATUS_WAIT_21 21
#define STATUS_WAIT_22 22
#define STATUS_WAIT_23 23
#define STATUS_WAIT_24 24
#define STATUS_WAIT_25 25
#define STATUS_WAIT_26 26
#define STATUS_WAIT_27 27
#define STATUS_WAIT_28 28
#define STATUS_WAIT_29 29
#define STATUS_WAIT_30 30
#define STATUS_WAIT_31 31
#define STATUS_WAIT_32 32
#define STATUS_WAIT_33 33
#define STATUS_WAIT_34 34
#define STATUS_WAIT_35 35
#define STATUS_WAIT_36 36
#define STATUS_WAIT_37 37
#define STATUS_WAIT_38 38
#define STATUS_WAIT_39 39
#define STATUS_WAIT_40 40
#define STATUS_WAIT_41 41
#define STATUS_WAIT_42 42
#define STATUS_WAIT_43 43
#define STATUS_WAIT_44 44
#define STATUS_WAIT_45 45
#define STATUS_WAIT_46 46
#define STATUS_WAIT_47 47
#define STATUS_WAIT_48 48
#define STATUS_WAIT_49 49
#define STATUS_WAIT_50 50
#define STATUS_WAIT_51 51
#define STATUS_WAIT_52 52
#define STATUS_WAIT_53 53
#define STATUS_WAIT_54 54
#define STATUS_WAIT_55 55
#define STATUS_WAIT_56 56
#define STATUS_WAIT_57 57
#define STATUS_WAIT_58 58
#define STATUS_WAIT_59 59
#define STATUS_WAIT_60 60
#define STATUS_WAIT_61 61
#define STATUS_WAIT_62 62
#define STATUS_WAIT_63 63
#define STATUS_INVALID_PARAMETER 0xC000000D
#define STATUS_INVALID_PARAMETER_1  0xC00000EF
#define STATUS_INVALID_PARAMETER_2  0xC00000F0
#define STATUS_INVALID_PARAMETER_3  0xC00000F1
#define STATUS_INVALID_PARAMETER_4  0xC00000F2
#define STATUS_INVALID_PARAMETER_5  0xC00000F3
#define STATUS_INVALID_PARAMETER_6  0xC00000F4
#define STATUS_INVALID_PARAMETER_7  0xC00000F5
#define STATUS_INVALID_PARAMETER_8  0xC00000F6
#define STATUS_INVALID_PARAMETER_9  0xC00000F7
#define STATUS_INVALID_PARAMETER_10 0xC00000F8
#define STATUS_INVALID_PARAMETER_11 0xC00000F9
#define STATUS_INVALID_PARAMETER_12 0xC00000FA
#define STATUS_INVALID_HANDLE 0xC0000008
#define STATUS_ACCESS_VIOLATION 0xC0000005
#define STATUS_NOT_SUPPORTED 0xC00000BB
#define STATUS_ACCESS_DENIED 0xC0000022
#define STATUS_TIMEOUT 0x00000102
#define STATUS_INTEGER_OVERFLOW 0xC0000095
#define STATUS_UNSUCCESSFUL 0xc0000001
#define STATUS_NOT_IMPLEMENTED 0xC0000002L

#define SYNCHRONIZE 0x00100000L
#define DELETE 0x00010000L
#define READ_CONTROL 0x00020000L
#define WRITE_DAC 0x00040000L
#define WRITE_OWNER 0x00080000L
#define STANDARD_RIGHT_REQUIRED (DELETE | READ_CONTROL | WRITE_DAC | WRITE_OWNER)
#define EVENT_QUERY_STATE 0x0001
#define EVENT_MODIFY_STATE 0x0002
#define EVENT_ALL_ACCESS (EVENT_QUERY_STATE | EVENT_MODIFY_STATE | STANDARD_RIGHT_REQUIRED | SYNCHRONIZE)

#define SEMAPHORE_QUERY_STATE 0x0001
#define SEMAPHORE_MODIFY_STATE 0x0002
#define SEMAPHORE_ALL_ACCESS (SEMAPHORE_QUERY_STATE | SEMAPHORE_MODIFY_STATE | STANDARD_RIGHT_REQUIRED | SYNCHRONIZE)

NTSTATUS
NtCreateEvent(
        PHANDLE EventHandle,
        ULONG DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes,
        EVENT_TYPE EventType,
        BOOLEAN InitialState
        );

NTSTATUS
NtSetEvent(
        HANDLE EventHandle,
        PLONG PreviousState
        );

NTSTATUS
NtResetEvent(
        HANDLE EventHandle,
        PLONG PreviousState
        );

NTSTATUS 
NtPulseEvent(
        HANDLE EventHandle,
        PLONG PreviousState
        ) __attribute__((deprecated));

NTSTATUS
NtQueryEvent(
        HANDLE EventHandle,
        EVENT_INFORMATION_CLASS EventInformationClass,
        PVOID EventInformation,
        ULONG EventInformationLength,
        PULONG ReturnLength
        );

NTSTATUS
NtCreateSemaphore(
        PHANDLE SemaphoreHandle,
        ULONG DesiredAccess,
        POBJECT_ATTRIBUTES ObjectAttributes,
        LONG InitialCount,
        LONG MaximumCount
        );

NTSTATUS
NtReleaseSemaphore(
        HANDLE SemaphoreHandle,
        LONG ReleaseCount,
        PLONG PreviousCount
        );

NTSTATUS
NtQuerySemaphore(
        HANDLE SemaphoreHandle,
        SEMAPHORE_INFORMATION_CLASS SemaphoreInformationClass,
        PVOID SemaphoreInformation,
        ULONG SemaphoreInformationLength,
        PULONG ReturnLength
        );

NTSTATUS
NtWaitForSingleObject(
        HANDLE Handle,
        BOOLEAN Alertable,
        PLARGE_INTEGER TimeOut
        );

NTSTATUS
NtWaitForMultipleObjects(
        ULONG Count,
        const HANDLE *Handles,
        WAIT_TYPE WaitType,
        BOOLEAN Alertable,
        PLARGE_INTEGER TimeOut
        );

NTSTATUS
NtClose(
        HANDLE Handle
        );
