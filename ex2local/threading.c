#include "threading.h"


// Constants -------------------------------------------------------------------



DWORD WINAPI test_and_compare(LPVOID *lpParam) 
{
	PROCESS_INFORMATION proc_info;
	BOOL                ret_val;
	DWORD dwMilliseconds = TIMEOUT_IN_MILLISECONDS;//maybe need to be in h file
	TCHAR               *p_command = NULL; // takescommand likne from struct
	Threads_to_run_s *params_node;
	params_node = (Threads_to_run_s*)malloc(sizeof(Threads_to_run_s));
	/* Check if lpParam is NULL */
	if (NULL == lpParam)
	{
		return STATUS_CODE_FAILURE;
	}
	/*Convert (void *) to parameters type.*/

	params_node = (Threads_to_run_s *)*lpParam; /*casting*/

	p_command = (TCHAR *)malloc((strlen(params_node->command_line)+1)*sizeof(TCHAR));
	strcpy_s(p_command, strlen(params_node->command_line) + 1, params_node->command_line);
	ret_val = CreateProcessSimple(p_command, &proc_info);
	printf("node STATUS in test and compare 0x%x, line: %d   func: %s\n", params_node->status, __LINE__, __func__);
	params_node->status = WaitForSingleObject(params_node->thread_handle, dwMilliseconds);//probelm here
	//printf("after wait single : 0x%x, line: %d   func: %s\n", params_node->status, __LINE__,__func__);

	if (WAIT_FAILED == params_node->status) {
		params_node->status = GetLastError();
		strcpy_s(params_node->message, 11, "Crashed");
	}
	if (WAIT_TIMEOUT == params_node->status) {
		strcpy_s(params_node->message, 11, "Timed Out");
	}

	if (0 == params_node->status) {//check output

		params_node->output_path;
	}

	//CloseHandle(proc_info.hThread);   //ilya galoboy need to ask about it
	CloseHandle(proc_info.hProcess);
	return STATUS_CODE_SUCCESS;
}

HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, LPVOID *recievd_Threads_to_run_s)
{
	Threads_to_run_s *params_node;
	params_node = (Threads_to_run_s *)*recievd_Threads_to_run_s; /*casting*/
	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}

	if (NULL == recievd_Threads_to_run_s)
	{
		printf("Error when creating a thread");
		printf("Received null pointer");
		exit(ERROR_CODE);
	}
	//params_node->thread_handle = NULL;
	//params_node->thread_id = NULL;
	params_node->thread_handle = CreateThread(
		NULL,                /*  default security attributes */
		0,                   /*  use default stack size */
		p_start_routine,     /*  thread function */
		recievd_Threads_to_run_s,         /*argument to thread function */
		0,                   /*  use default creation flags */
		NULL);        /*  returns the thread identifier */


	return params_node->thread_handle;
}

static BOOL CreateProcessSimple(LPTSTR p_command_line, PROCESS_INFORMATION *p_process_info)
{
	STARTUPINFO start_info = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
															   /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
															   /* CreateProcess() means we have no special interest in this parameter. */
															   /* This is equivalent to what we are doing by supplying NULL to most other */
															   /* parameters of CreateProcess(). */

	/* Should check for NULL pointers. Skipped for the sake of simplicity. */

	return CreateProcess(NULL, /*  No module name (use Command line). */
		p_command_line,        /*  Command line. */
		NULL,                  /*  Process handle not inheritable. */
		NULL,                  /*  Thread handle not inheritable. */
		FALSE,                 /*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS, /*  creation/priority flags. */
		NULL,                  /*  Use parent's environment block. */
		NULL,                  /*  Use parent's starting directory. */
		&start_info,           /*  Pointer to STARTUPINFO structure. */
		p_process_info         /*  Pointer to PROCESS_INFORMATION structure. */
	);
}


void close_all_handels(HANDLE *all_handles,int num_of_lines) {
	for (int i = 0; i < num_of_lines; i++) {
		CloseHandle(all_handles[i]);
	}
}