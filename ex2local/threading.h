/*Description: This is the parallelization head module - Here are all the declarations 
of the functions that deal with creating and closing threads and processes, as well as
testing the needed tests. Also, here declared the struct which contains all
the tested program info.*/
#pragma once
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include "pars.h"



#define TIMEOUT_IN_MILLISECONDS 100000
#define MAX_MESSAGE_LEN 12
#define ERROR_CODE ((int)(-1))
#define THREAD_STATUS_FILE_COPMARE_FAILURE -69

static const int STATUS_CODE_SUCCESS = 0;
static const int STATUS_CODE_FAILURE = -1;
static const int THREAD_STATUS_CRASHED = -7;

typedef struct Threads_to_run_s{
	HANDLE thread_handle;
	DWORD return_value;
	DWORD status;
	char *message;
	char *program_output;
	char *command_line;
	char *output_path;
	struct Threads_to_run_s *next;
}Threads_to_run_s;


DWORD WINAPI test_and_compare(LPVOID lpParam);
HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, Threads_to_run_s **recievd_Threads_to_run_s);
static BOOL CreateProcessSimple(LPTSTR p_command_line, PROCESS_INFORMATION *p_process_info);
void close_all_handels(HANDLE *all_handles, int num_of_lines);