/* 
 * libntsync - Linux NTSYNC helper libraries
 * Author: Kawaii Ghost <frweird@outlook.co.id>
 * Copyright (c) 2025 Kawaii Ghost. All Rights Reserved.
 * SPDX-License-Identifier: MIT
 */

#include "nt.h"
#include <errno.h>
#include <sys/ioctl.h>
#include <time.h>
#include <unistd.h>

int ntsync;

NTSTATUS RtlpGetNtStatusFromUnixErrno(void)
{
        switch (errno) {
                case EBADF:
                        return STATUS_INVALID_HANDLE;
                        break;
                case EFAULT:
                        return STATUS_ACCESS_VIOLATION;
                        break;
                case EINVAL:
                        return STATUS_INVALID_PARAMETER;
                        break;
                case ETIMEDOUT:
                        return STATUS_TIMEOUT;
                        break;
                case EOVERFLOW:
                        return STATUS_INTEGER_OVERFLOW;
                        break;
                case EPERM:
                        return STATUS_ACCESS_DENIED;
                        break;
                case ENOSYS:
                        return STATUS_NOT_IMPLEMENTED;
                        break;
                default:
                        return STATUS_UNSUCCESSFUL;
                        break;
        }
}

NTSTATUS NtCreateSemaphore(PHANDLE SemaphoreHandle, ULONG DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, LONG InitialCount, LONG MaximumCount)
{
        if (SemaphoreHandle == NULL) {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_1;
        }

        if (ObjectAttributes != NULL) {
                errno = ENOSYS;
                return STATUS_NOT_IMPLEMENTED;
        }

        if (InitialCount > MaximumCount) {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_4;
        }

        if (MaximumCount == 0) {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_5;
        }

        struct ntsync_sem_args args = {.count = InitialCount, .max = MaximumCount};
        int ret = ioctl(ntsync, NTSYNC_IOC_CREATE_SEM, &args);
        if (ret == -1) {
                return RtlpGetNtStatusFromUnixErrno();
        }

        SemaphoreHandle->DesiredAccess = DesiredAccess;
        SemaphoreHandle->Object = ret;

        return STATUS_SUCCESS;
}

NTSTATUS NtReleaseSemaphore(HANDLE SemaphoreHandle, LONG ReleaseCount, PLONG PreviousCount)
{
        if (ReleaseCount == 0) {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_2;
        }

        if (!(SemaphoreHandle.DesiredAccess & SEMAPHORE_MODIFY_STATE)) {
                errno = EPERM;
                return STATUS_ACCESS_DENIED;
        }

        int ret = ioctl(SemaphoreHandle.Object, NTSYNC_IOC_SEM_RELEASE, &ReleaseCount);
        if (ret == -1) {
                return RtlpGetNtStatusFromUnixErrno();
        }

        if (PreviousCount != NULL) {
                *PreviousCount = ReleaseCount;
        }

        return STATUS_SUCCESS;
}

NTSTATUS NtQuerySemaphore(HANDLE SemaphoreHandle, SEMAPHORE_INFORMATION_CLASS SemaphoreInformationClass, PVOID SemaphoreInformation, ULONG SemaphoreInformationLength, PULONG ReturnLength)
{
        if (SemaphoreInformationClass != SemaphoreBasicInformation) {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_2;
        }

        if (SemaphoreInformation == NULL) {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_3;
        }

        if (SemaphoreInformationLength != sizeof(SEMAPHORE_BASIC_INFORMATION)) {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_4;
        } 

        if (!(SemaphoreHandle.DesiredAccess & SEMAPHORE_QUERY_STATE)) {
                errno = EPERM;
                return STATUS_ACCESS_DENIED;
        }

        struct ntsync_sem_args args;
        int ret = ioctl(SemaphoreHandle.Object, NTSYNC_IOC_SEM_READ, &args);
        if (ret == -1) {
                return RtlpGetNtStatusFromUnixErrno();
        }

        ((SEMAPHORE_BASIC_INFORMATION *)SemaphoreInformation)->CurrentCount = args.count;
        ((SEMAPHORE_BASIC_INFORMATION *)SemaphoreInformation)->MaximumCount = args.max;
        
        if (ReturnLength != NULL) {
                *ReturnLength = sizeof(SEMAPHORE_BASIC_INFORMATION);
        }

        return STATUS_SUCCESS;
}

NTSTATUS NtCreateEvent(PHANDLE EventHandle, ULONG DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, EVENT_TYPE EventType, BOOLEAN InitialState)
{
        if (EventHandle == NULL) {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_1;
        }

        if (ObjectAttributes != NULL) {
                return STATUS_NOT_IMPLEMENTED;
        }

        struct ntsync_event_args args = {.manual = EventType, .signaled = InitialState};
        int ret = ioctl(ntsync, NTSYNC_IOC_CREATE_EVENT, &args);
        if (ret == -1) {
                return RtlpGetNtStatusFromUnixErrno();
        }

        EventHandle->DesiredAccess = DesiredAccess;
        EventHandle->Object = ret;

        return STATUS_SUCCESS;
}

NTSTATUS NtSetEvent(HANDLE EventHandle, PLONG PreviousState)
{
        if (!(EventHandle.DesiredAccess & EVENT_MODIFY_STATE)) {
                return STATUS_ACCESS_DENIED;
        }

        LONG State;
        int ret = ioctl(EventHandle.Object, NTSYNC_IOC_EVENT_SET, &State);
        if (ret == -1) {
                return RtlpGetNtStatusFromUnixErrno();
        }
        
        if (PreviousState != NULL) {
                *PreviousState = State;
        }

        return STATUS_SUCCESS;
}

NTSTATUS NtResetEvent(HANDLE EventHandle, PLONG PreviousState)
{
        if (!(EventHandle.DesiredAccess & EVENT_MODIFY_STATE)) {
                return STATUS_ACCESS_DENIED;
        }

        LONG State;
        int ret = ioctl(EventHandle.Object, NTSYNC_IOC_EVENT_RESET, &State);
        if (ret == -1) {
                return RtlpGetNtStatusFromUnixErrno();
        }
        
        if (PreviousState != NULL) {
                *PreviousState = State;
        }

        return STATUS_SUCCESS;
}

NTSTATUS NtPulseEvent(HANDLE EventHandle, PLONG PreviousState)
{
        if (!(EventHandle.DesiredAccess & EVENT_MODIFY_STATE)) {
                return STATUS_ACCESS_DENIED;
        }

        LONG State;
        int ret = ioctl(EventHandle.Object, NTSYNC_IOC_EVENT_PULSE, &State);
        if (ret == -1) {
                return RtlpGetNtStatusFromUnixErrno();
        }
        
        if (PreviousState != NULL) {
                *PreviousState = State;
        }

        return STATUS_SUCCESS;
}

NTSTATUS NtQueryEvent(HANDLE EventHandle, EVENT_INFORMATION_CLASS EventInformationClass, PVOID EventInformation, ULONG EventInformationLength, PULONG ReturnLength)
{

        if (EventInformationClass != EventBasicInformation) {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_2;
        }

        if (EventInformation == NULL) {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_3;
        }

        if (EventInformationLength != sizeof(EVENT_BASIC_INFORMATION)) {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_4;
        }

        if (!(EventHandle.DesiredAccess & EVENT_QUERY_STATE)) {
                errno = EPERM;
                return STATUS_ACCESS_DENIED;
        }

        struct ntsync_event_args args;
        int ret = ioctl(EventHandle.Object, NTSYNC_IOC_EVENT_READ, &args);
        if (ret == -1) {
                return RtlpGetNtStatusFromUnixErrno();
        }

        ((EVENT_BASIC_INFORMATION *)EventInformation)->EventState = args.signaled;
        ((EVENT_BASIC_INFORMATION *)EventInformation)->EventType = args.manual;
        
        if (ReturnLength != NULL) {
                *ReturnLength = sizeof(EVENT_BASIC_INFORMATION);
        }

        return STATUS_SUCCESS;
}

NTSTATUS NtWaitForSingleObject(HANDLE Handle, BOOLEAN Alertable, PLARGE_INTEGER TimeOut)
{
        if (Alertable) {
                return STATUS_NOT_IMPLEMENTED;
        }

        if (!(Handle.DesiredAccess & SYNCHRONIZE)) {
                errno = EPERM;
                return STATUS_ACCESS_DENIED;
        }

        struct ntsync_wait_args args = {.objs = (uintptr_t)&Handle.Object,
                                        .count = 1,
                                        .flags = NTSYNC_WAIT_REALTIME,
                                        .owner = 0,
                                        .alert = 0,
                                        .pad = 0};

        if (TimeOut == NULL) {
                args.timeout = UINT64_MAX;
        } else {
                struct timespec ts;
                if (timespec_get(&ts, TIME_UTC) != TIME_UTC) {
                        return RtlpGetNtStatusFromUnixErrno();
                }
                args.timeout = -(TimeOut->QuadPart) * 100 + ts.tv_nsec + ts.tv_sec * NSEC_PER_SEC;
        }

        int ret = ioctl(ntsync, NTSYNC_IOC_WAIT_ANY, &args);
        if (ret == -1) {
                return RtlpGetNtStatusFromUnixErrno();
        }

        return args.index;
}

NTSTATUS NtWaitForMultipleObjects(ULONG Count, const HANDLE *Handles, WAIT_TYPE WaitType, BOOLEAN Alertable, PLARGE_INTEGER TimeOut)
{
        if (Count > MAXIMUM_WAIT_OBJECTS) {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_1;
        }

        if (Handles == NULL) {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_2;
        }

        int opcode;
        if (WaitType == WaitAll) {
                opcode = NTSYNC_IOC_WAIT_ALL;
        } else if (WaitType == WaitAny) {
                opcode = NTSYNC_IOC_WAIT_ANY;
        } else {
                errno = EINVAL;
                return STATUS_INVALID_PARAMETER_3;
        }

        if (Alertable) {
                errno = ENOSYS;
                return STATUS_NOT_IMPLEMENTED;
        }

        int Objects[MAXIMUM_WAIT_OBJECTS];
        for (size_t i = 0; i < Count; i++) {
                if (!(Handles[i].DesiredAccess & SYNCHRONIZE)) {
                        errno = EPERM;
                        return STATUS_ACCESS_DENIED;
                }
                Objects[i] = Handles[i].Object;
        }

        struct ntsync_wait_args args = {.objs = (uintptr_t)Objects,
                                        .count = Count,
                                        .flags = NTSYNC_WAIT_REALTIME,
                                        .owner = 0,
                                        .alert = 0,
                                        .pad = 0};

        if (TimeOut == NULL) {
                args.timeout = UINT64_MAX;
        } else {
                struct timespec ts;
                if (timespec_get(&ts, TIME_UTC) != TIME_UTC) {
                        return RtlpGetNtStatusFromUnixErrno();
                }
                args.timeout = -(TimeOut->QuadPart) * 100 + ts.tv_nsec + ts.tv_sec * NSEC_PER_SEC;
        }

        int ret = ioctl(ntsync, opcode, &args);
        if (ret == -1) {
                return RtlpGetNtStatusFromUnixErrno();
        }

        return args.index;
}

NTSTATUS NtClose(HANDLE Handle)
{
        int ret = close(Handle.Object);
        if (ret == -1) {
                return RtlpGetNtStatusFromUnixErrno();
        } else {
                return STATUS_SUCCESS;
        }
}
