
# Project 2 - Mutex and Condition Variables Server
The goal is to extend the Minix system with a server implementing 'mutex' and 'condition variables' functionalities.

Both mutexes and condition variables will be identified in the system by integers (`int` type). 
The implementations of the functions described below should be added to the system library (e.g., in `lib\libc\sys-minix\`).

### Mutexes
Functions:

* `cs_lock(int mutex_id)` - tries to reserve the mutex with the specified number.
If the mutex is not owned by any process, it should be assigned to the calling process.
In this case, the function returns `0` (success).
If another process owns the mutex, the current process should be suspended until the mutex can be assigned to it.
When the process acquires the mutex, the function returns `0` (success).
No process should request a mutex it already owns.
The behavior in such a case is undefined, except that the system or mutex server should not cease to function due to such an action.
* `cs_unlock(int mutex_id)` - releases the mutex with the specified number.
If the calling process owns the mutex, the function returns `0` (success), and the mutex server assigns the mutex to the next process in the queue of processes waiting for this mutex (if the queue is not empty).
If the calling process does not own this mutex, the function returns `-1` and sets `errno` to `EPERM`.
Processes waiting for one mutex should be placed in a queue (FIFO).

### Condition Variables
Functions:

* `cs_wait(int cond_var_id, int mutex_id)` - suspends the current process, waiting for the event identified by `cond_var_id`.
The calling process should own the mutex identified by `mutex_id`.
If the calling process does not have the appropriate mutex, the function should return `-1` and set `errno` to `EINVAL`.
If the calling process owns the mutex, the server should release the mutex and suspend the calling process until another process announces the cond_var_id event using the cs_broadcast function.
In this case, the server should place the process in the queue of processes waiting for mutex mutex_id and return `0` (success) upon receiving the mutex.
* `cs_broadcast(int cond_var_id)` - announces the event identified by `cond_var_id`. All processes that have suspended waiting for this event should be unblocked. Each of them, upon regaining its mutex, should be resumed.
It can be assumed that at any given time, a maximum of 1024 mutexes will be reserved by the server.

## Signals
Incoming signals should be immediately handled according to the procedures registered by the process.
However, the above blocking functions cannot be interrupted and should not return `EINTR` in `errno`.
If a process was waiting for a mutex, it should resume waiting after handling the signal.
If it was waiting for an event, it should regain the mutex and return success (spurious wakeup).

Mutexes of processes that are terminated should be immediately released.

## Repository Structure
The solution will be tested on MINIX 3.2.1.

- `src/` reflects part of the MINIX system source directory structure.
This part includes some directories in which parts of the implementation may be found.
To test, copy the contents of the repository's `src` directory to the `/usr/src directory` in MINIX.
As a result, some files of the original sources will be overwritten.
Then, install includes, recompile and install the standard library, and the new server.
To allow changes in basic system servers (e.g., PM) before testing, the system image will also be recompiled.
After restarting the system, the new functionality should work upon server startup.

- `tests/` contains several basic tests for the functionalities described above.
