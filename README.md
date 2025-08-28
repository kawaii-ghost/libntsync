# libntsync
`libntsync` is a Linux helper library for Linux NTSYNC driver, providing Windows events and semaphores.

`libntsync` is almost 100% compatible with Windows API especially Native API except it didn't support [named objects](https://learn.microsoft.com/en-us/windows/win32/sync/using-named-objects), [security attributes](https://learn.microsoft.com/en-us/windows/win32/api/wtypesbase/ns-wtypesbase-security_attributes), and [asynchronous procedure call](https://learn.microsoft.com/en-us/windows/win32/sync/asynchronous-procedure-calls). Otherwise, event (both for manual reset and auto reset) and semaphores are totally compatible.
Beside, `libntsync` also support `WaitForMultipleObjects` for both **WaitAny** and **WaitAll** mode for waiting them. `GetLastError()` is available if you don't want to deal with errno :>.

## Usage
Make sure the `ntsync` kernel module is loaded.

```sh
# modprobe -v ntsync
```
or you can put config file to `/etc/modules-load.d/` to make it automatically loaded on boot.

Put `#include <nt.h>` for the NT API version or `#include <win32.h>` if you prefer working with Win32 API.

Initialize the `ntsync` variable before use the library with the fd returned from `openat(AT_FDCWD, "/dev/ntsync", O_RDWR | O_CLOEXEC | O_NONBLOCK)` or use `ntsync_init()` and `ntsync_destroy()` helper function.

Then, you're good to go :).

## About libntsync
I was a Linux fans until I learned Windows Internals especially the Native API part.
On Linux, I miss some Windows API stuff like synchronization primitives.
`WaitForMultipleObjects` WaitAll and WaitAny is one of the idea I missed on Linux.
You can wait for one or wait for all with just single system call.
(_Well, technically you can emulate it with io_uring_).

With NTSYNC driver, my dreams come true.
I can get `WaitForMultipleObjects` for both mode, Windows Events, and Windows Semaphores.


> But, why you didn't implement NT Mutant / Windows Mutex

Windows Mutex idea is also cool since it has auto recovery support (no deadlock if the thread that owns it dead).
At first glance, I thought I could get the NT Mutant also.
Turns out that NT Mutant provided by NTSYNC is just for emulating it _(You can emulate the non recursive mutex with binary semaphore, 1 for unlock and 0 for lock)_.

1. You need to specify the owner.
Any call to NTSYNC Mutex ioctl requires to specify the owner.
2. The mutex isn't auto recovery
If the threads that own the mutex is dead, you'll get deadlock.
The mutex should be killed from outside using `NTSYNC_IOC_MUTEX_KILL`.
Therefore, I see no benefit about provide it.
Consider using pthread robust mutex if you want auto recovery mutex.

## libntsync API
```c
// Unofficial helper functions
bool ntsync_init(void);
void ntsync_exit(void);

// NT API variant
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

// Windows API variant
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

BOOL SetEvent(
        HANDLE Event
);

BOOL ResetEvent(
        HANDLE Event
);

BOOL PulseEvent(
        HANDLE Event
) __attribute__((deprecated));

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
```

## Contributing
This library is open source. Feel free to contribute and send some feedbacks (I guess) on issues :3.


