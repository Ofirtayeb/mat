#pragma once
// IO_Thread.h

#ifndef __IO_THREAD_H__
#define __IO_THREAD_H__

// Includes --------------------------------------------------------------------

#include <windows.h>

// Function Declarations -------------------------------------------------------

/*
* IO thread. This thread is in charge of the user interface.
* Notes:
*   The thread calls the Echo function in a loop.
*   If the user enters "exit", the thread exits.
*   Not all threads are an eternal loop. This is just for
*   for the sake of this example.
*/
DWORD WINAPI IOThread(LPVOID lpParam);

#endif // __IO_THREAD_H__