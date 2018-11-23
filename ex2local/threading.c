/*Description: This is the parallelization module - Here are all the functions
that deal with creating and closing threads and processes, as well as testing
the needed tests.
*/

#include "threading.h"

/*Description: This is the function which is opened in each thread. This function
creates a new process which runs the program that is tested. Here we make a 
comprasion between the output file and the excpected outputfile. 
Parameters:	LPVOID lpParam - A pointer to a specific node in the linked list,
this node is of struct Threads_to_run_s which contains all info needed to run 
the test.
Return: DWORD which is a success or a failure number.
*/
DWORD WINAPI test_and_compare(LPVOID lpParam) 
{
	PROCESS_INFORMATION proc_info;
	BOOL                ret_val = NULL;
	TCHAR               *p_command = NULL; // takescommand likne from struct
	Threads_to_run_s *params_node = NULL;
	DWORD waitinmillsec = TIMEOUT_IN_MILLISECONDS;
	DWORD returned_status = 0;
	int file_comp_flag;

	/* Check if lpParam is NULL */
	if (NULL == lpParam)
	{
		return STATUS_CODE_FAILURE;
	}

	params_node = (Threads_to_run_s *)lpParam; /*casting*/

	p_command = (TCHAR *)malloc((strlen(params_node->command_line)+1)*sizeof(TCHAR));

	printf("before copy : p_command:%s\n", p_command);
	strcpy_s(p_command, strlen(params_node->command_line) + 1, params_node->command_line);
	p_command = _T(p_command);
	printf("after copy : p_command:%s\n", p_command);
	ret_val = CreateProcessSimple(p_command, &proc_info);
	params_node->thread_handle = proc_info.hThread;
	printf("before wait: LINE:%d, function %s second file\n", __LINE__, __func__); 
	params_node->status = WaitForSingleObject(proc_info.hProcess, waitinmillsec);
	printf("after wait: LINE:%d, function %s second file\n", __LINE__, __func__);
	printf("GetExitCodeProcess args: proc_info.hProcess: %p --- returned_status: %lu\n", proc_info.hProcess, returned_status);
	ret_val = GetExitCodeProcess(proc_info.hProcess, &returned_status);
	printf("GetExitCodeProcess args: proc_info.hProcess: %p  --- returned_status: %lu\n", proc_info.hProcess, returned_status);
	printf("after GetExitCodeProcess:\n, %d, %d, %d\n ", params_node->status,ret_val, returned_status);
	params_node->return_value = returned_status;


	printf("before fail : %s ---  %s\n", params_node->program_output, params_node->output_path);

	if (WAIT_FAILED == params_node->status) {
		printf("inside fail : %s ---  %s\n", params_node->program_output, params_node->output_path);
		return STATUS_CODE_FAILURE;
	}
	if (WAIT_TIMEOUT == params_node->status) {
		strcpy_s(params_node->message, 11, "Timed Out");
	}
	else if (0 == params_node->return_value){
		if (WAIT_OBJECT_0 == params_node->status) {//check output
			printf("LINE:%d, function:%s, params_node->program_output:%s, arams_node->output_path:%s \n", __LINE__, __func__, params_node->program_output, params_node->output_path);
			file_comp_flag = file_compare(params_node->program_output, params_node->output_path);
			switch (file_comp_flag) {
			case(0):
				strcpy_s(params_node->message, 11, "Succeeded");
				break;
			case (STATUS_FILES_DIFFER):
				params_node->status = THREAD_STATUS_FILE_COPMARE_FAILURE;
				strcpy_s(params_node->message, 11, "Failed");
				break;
			default:
				strcpy_s(params_node->message, 11, "O_fail");
				break;
			}
		}
	} else {
		params_node->status = THREAD_STATUS_CRASHED;//ofir need to define this
		strcpy_s(params_node->message, 11, "Crashed"); 
	}
	
	CloseHandle(proc_info.hThread);
	CloseHandle(proc_info.hProcess);
	free(p_command);
	p_command = NULL;
	return STATUS_CODE_SUCCESS;
}

/*Description: A wrapper to the function createthread().
Parameters:	LPTHREAD_START_ROUTINE p_start_routine - the functions the thread needs
to run.
Threads_to_run_s **recievd_Threads_to_run_s - An address of a pointer which contains
the info regarding the program to be tested.
Return: A handle of the thread that was created.
*/
HANDLE CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine, Threads_to_run_s **recievd_Threads_to_run_s)
{
	HANDLE thread_handle = NULL;

	if (NULL == p_start_routine)
	{
		printf("Error when creating a thread\n");
		printf("Received null pointer\n");
		exit(ERROR_CODE);
	}

	if (NULL == recievd_Threads_to_run_s)
	{
		printf("Error when creating a thread\n");
		printf("Received null pointer\n");
		exit(ERROR_CODE);
	}
	//params_node->thread_handle = NULL;
	//params_node->thread_id = NULL;
	thread_handle = CreateThread(
		NULL,                /*  default security attributes */
		0,                   /*  use default stack size */
		p_start_routine,     /*  thread function */
		*recievd_Threads_to_run_s,         /*argument to thread function */ ///problem in sending the params and casting
		0,                   /*  use default creation flags */
		NULL);				/*  returns the thread identifier */


	return thread_handle;
}

/*Description: A wrapper to the function createprocess().
Parameters: LPTSTR p_command_line - the program path with its arguments.
PROCESS_INFORMATION *p_process_info - a truct which contains data about the 
process created.
Return: A BOOL in case of failure or success.
*/
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

/*Description: Closes all handles of the created threads and processes.
Parameters: HANDLE *all_handles - A pointer to an array of all handles.
int num_of_lines - number of handles to be closed.
Return: void.
*/
void close_all_handels(HANDLE *all_handles,int num_of_lines) {
	for (int i = 0; i < num_of_lines; i++) {
		CloseHandle(all_handles[i]);
	}
}