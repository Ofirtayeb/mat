#pragma once
// Communication_Thread.c

// Includes --------------------------------------------------------------------

#include <limits.h>
#include <stdio.h>
#include <windows.h>
#include "Communication_Thread.h"

// Function Declarations -------------------------------------------------------

static void SendDiagnosticsToMicrosoft(void);

// Function Definitions --------------------------------------------------------

DWORD WINAPI CommunicationThread(LPVOID lpParam)
{
	while (1)
	{
		SendDiagnosticsToMicrosoft();
	}
}

static void SendDiagnosticsToMicrosoft(void)
{
	volatile int i;

	for (i = 0; i < (INT_MAX / 2); i++)
	{
		// pass
	}

	// Make a sound
	// If this does not make any sound on your computer, replace
	// with printf("a").
	printf("\a");
}