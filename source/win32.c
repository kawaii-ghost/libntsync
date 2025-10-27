/* 
 * libwinsync - NTSYNC wrapper for Linux
 * Author: Kawaii Ghost <frweird@outlook.co.id>
 * Copyright (c) 2025 Kawaii Ghost. All Rights Reserved.
 * SPDX-License-Identifier: MIT
 */

#include "win32.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

bool ntsync_init(void)
{
        ntsync = openat(AT_FDCWD, "/dev/ntsync", O_RDWR | O_CLOEXEC | O_NONBLOCK);
        if (ntsync == -1) {
                return false;
        } else {
                return true;
        }
}

void ntsync_exit(void)
{
        if (ntsync != -1) {
                close(ntsync);
        }
}

PLARGE_INTEGER BaseFormatTimeOut(PLARGE_INTEGER TimeOut, DWORD Milliseconds)
{
        if (Milliseconds == INFINITE) {
                return NULL;
        }

        TimeOut->QuadPart = Milliseconds * -10000LL;
        return TimeOut;
}

DWORD GetLastError(void)
{
        switch (errno) {
                case EBADF:
                        return ERROR_INVALID_HANDLE;
                        break;
                case EFAULT:
                        return ERROR_NOACCESS;
                        break;
                case EINVAL:
                        return ERROR_INVALID_PARAMETER;
                        break;
                case ETIMEDOUT:
                        return ERROR_TIMEOUT;
                        break;
                case EOVERFLOW:
                        return ERROR_TOO_MANY_POSTS;
                        break;
                case EPERM:
                        return ERROR_ACCESS_DENIED;
                        break;
                case ENOSYS:
                        return ERROR_INVALID_FUNCTION;
                        break;
                default:
                        return ERROR_GEN_FAILURE;
                        break;
        }
}

HANDLE CreateSemaphoreA(LPSECURITY_ATTRIBUTES SemaphoreAttributes, LONG InitialCount, LONG MaximumCount, LPCSTR Name)
{
        if (SemaphoreAttributes != NULL || Name != NULL) {
                errno = ENOSYS;
                return NULL;
        }

        HANDLE Semaphore = NULL;
        NtCreateSemaphore(&Semaphore, SEMAPHORE_ALL_ACCESS, NULL, InitialCount, MaximumCount);
        return Semaphore;
}

HANDLE CreateSemaphoreExA(LPSECURITY_ATTRIBUTES SemaphoreAttributes, LONG InitialCount, LONG MaximumCount, LPCSTR Name, DWORD Flags, DWORD DesiredAccess)
{
        if (SemaphoreAttributes != NULL || Name != NULL) {
                errno = ENOSYS;
                return NULL;
        }

        if (Flags != 0) {
                errno = EINVAL;
                return NULL;
        }

        HANDLE Semaphore = NULL;
        NtCreateSemaphore(&Semaphore, DesiredAccess, NULL, InitialCount, MaximumCount);
        return Semaphore;
}

BOOL ReleaseSemaphore(HANDLE Semaphore, LONG ReleaseCount, LONG *PreviousCount)
{
        return !NtReleaseSemaphore(Semaphore, ReleaseCount, PreviousCount);
}

HANDLE CreateEventA(LPSECURITY_ATTRIBUTES EventAttributes, BOOL ManualReset, BOOL InitialState, LPCSTR Name)
{
        if (EventAttributes != NULL || Name != NULL) {
                errno = ENOSYS;
                return NULL;
        }

        HANDLE Event = NULL;
        NtCreateEvent(&Event, EVENT_ALL_ACCESS, NULL, ManualReset, InitialState);
        return Event;
}

HANDLE CreateEventExA(LPSECURITY_ATTRIBUTES EventAttributes, LPCSTR Name, DWORD Flags, DWORD DesiredAccess)
{
        if (EventAttributes != NULL || Name != NULL) {
                errno = ENOSYS;
                return NULL;
        }

        HANDLE Event = NULL;
        NtCreateEvent(&Event, DesiredAccess, NULL, Flags & CREATE_EVENT_MANUAL_RESET, Flags & CREATE_EVENT_INITIAL_SET);
        return Event;
}

BOOL SetEvent(HANDLE Event)
{
        return !NtSetEvent(Event, NULL);
}

BOOL ResetEvent(HANDLE Event)
{
        return !NtResetEvent(Event, NULL);
}

BOOL PulseEvent(HANDLE Event)
{
        return !NtPulseEvent(Event, NULL);
}

DWORD WaitForSingleObject(HANDLE Handle, DWORD Milliseconds)
{
        return WaitForSingleObjectEx(Handle, Milliseconds, FALSE);
}

DWORD WaitForSingleObjectEx(HANDLE Handle, DWORD Milliseconds, BOOL Alertable)
{
        LARGE_INTEGER TimeOut;
        NTSTATUS Status = NtWaitForSingleObject(Handle, Alertable, BaseFormatTimeOut(&TimeOut, Milliseconds));
        if (Status != STATUS_WAIT_0 || Status != STATUS_TIMEOUT) {
                return WAIT_FAILED;
        } else {
                return Status;
        }
}

DWORD WaitForMultipleObjects(DWORD Count, const HANDLE *Handles, BOOL WaitAll, DWORD Milliseconds)
{
        return WaitForMultipleObjectsEx(Count, Handles, WaitAll, Milliseconds, FALSE);
}

DWORD WaitForMultipleObjectsEx(DWORD Count, const HANDLE *Handles, BOOL WaitAll, DWORD Milliseconds, BOOL Alertable)
{
        LARGE_INTEGER TimeOut;
        NTSTATUS Status = NtWaitForMultipleObjects(Count, Handles, !WaitAll, Alertable, BaseFormatTimeOut(&TimeOut, Milliseconds));
        if (Status != STATUS_TIMEOUT || !(Status > STATUS_WAIT_0 && Status < Count - 1)) {
                return WAIT_FAILED;
        } else {
                return Status;
        }
}

BOOL CloseHandle(HANDLE Object)
{
        return !NtClose(Object);
}
