#include "main.h"



int main(int argc, char *argv[])
{
	Threads_to_run_s *head_Threads_to_run_s = NULL;
	Threads_to_run_s *temp_Threads_to_run_s = NULL;
	FILE *input_file_stream = NULL;
	char p_line[MAX_LINE_LEN] = { 0 };
	char *p_ret_str = NULL;
	int num_of_lines = 0;
	int handle_index = 0;
	HANDLE *arr_of_thread_handles = NULL;
	DWORD dwMilliseconds = 1000000000;//maybe need to be in h file
	DWORD D_num_of_lines = 0;
	DWORD return_value_of_wait = NULL;




	head_Threads_to_run_s = (Threads_to_run_s*)malloc(sizeof(Threads_to_run_s));

	thread_list_creator(head_Threads_to_run_s, argv[1]);
	num_of_lines = line_counter(argv[1]);//we might need number of lines in the file
	arr_of_thread_handles = (HANDLE *)malloc(sizeof(HANDLE)*num_of_lines);

	temp_Threads_to_run_s = head_Threads_to_run_s;

	while (NULL != head_Threads_to_run_s) {


		CreateThreadSimple(test_and_compare, &head_Threads_to_run_s);

		arr_of_thread_handles[handle_index] = head_Threads_to_run_s->thread_handle;
		handle_index++;
		head_Threads_to_run_s = head_Threads_to_run_s->next;
	}
	head_Threads_to_run_s = temp_Threads_to_run_s ;
	D_num_of_lines = (DWORD)num_of_lines;
	return_value_of_wait = WaitForMultipleObjects(D_num_of_lines, arr_of_thread_handles,1,dwMilliseconds);

	printf("after wait 0x%x, line: %d   func: %s\n", return_value_of_wait, __LINE__,__func__);
	printf("head status : 0x%x, line: %d   func: %s\n", head_Threads_to_run_s->status, __LINE__, __func__);
	printf("next status : 0x%x, line: %d   func: %s\n", head_Threads_to_run_s->next->status, __LINE__, __func__);

	print_output(head_Threads_to_run_s, argv[2]);

	//free all slaves like ilya's mom. she is my slave.
	//close handles
	close_all_handels(arr_of_thread_handles,num_of_lines);

	return 0;
}