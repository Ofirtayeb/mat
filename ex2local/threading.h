#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>

#define TIMEOUT_IN_MILLISECONDS 10000
#define MAX_MESSAGE_LEN 11
#define NUM_THREADS 2
#define BRUTAL_TERMINATION_CODE 0x55
#define ERROR_CODE ((int)(-1))


static const int STATUS_CODE_SUCCESS = 0;
static const int STATUS_CODE_FAILURE = -1;


typedef struct Threads_to_run_s{
	HANDLE thread_handle;
	DWORD thread_id;
	DWORD status; //maybe int
	char message[MAX_MESSAGE_LEN];
	char *program_output;
	char *command_line;
	char *output_path;
	char *args_for_thread;
	struct Threads_to_run_s *next;
}Threads_to_run_s;


DWORD WINAPI test_and_compare(LPVOID *lpParam);
HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPVOID *recievd_Threads_to_run_s);
static BOOL CreateProcessSimple(LPTSTR p_command_line, PROCESS_INFORMATION *p_process_info);
void close_all_handels(HANDLE *all_handles, int num_of_lines);