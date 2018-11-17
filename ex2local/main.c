/*Authors: Ofir Taib 305472508, Ilya Broun 310944426
Project: ex2
Description: This is the main module - calls all the relevant functions in order to 
run the programs, test them and return the results of the tests.
*/
#include "main.h"


/*Description: Frees all dynamic memories allocated in the main function.
Parameters: Threads_to_run_s **head - an andress of a pointer to the head
of the linked list of all programs tested.
int num_of_lines - number of programs to test.
Return: void.
*/
void free_main_malloc(HANDLE *arr_of_thread_handle, int num_of_lines) {
	for (int i = 0; i < num_of_lines; i++) {
		free(arr_of_thread_handle[i]);
	}
}


/*Description: Frees dynamic memories allocated for the linked list of
programs to be tested.
Parameters: Threads_to_run_s **head - an andress of a pointer to the head
of the linked list of all programs tested.
HANDLE *arr_of_thread_handle - a pointer to an array of handles.
int num_of_lines - number of programs tested.
Return: void.
*/
void free_all(Threads_to_run_s **head, HANDLE *arr_of_thread_handle, int num_of_lines) {

	free_main_malloc(arr_of_thread_handle, num_of_lines);
	free_linked_list(&head);
}




/*Main function
Parameters: <path to test file> <path to result file>
Return: 0 or fail.
*/
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
	DWORD dwMilliseconds = INFINITE;//maybe need to be in h file
	DWORD D_num_of_lines = 0;
	DWORD return_value_of_wait = NULL;




	head_Threads_to_run_s = (Threads_to_run_s*)malloc(sizeof(Threads_to_run_s));

	thread_list_creator(head_Threads_to_run_s, argv[1]);
	num_of_lines = line_counter(argv[1]);//we might need number of lines in the file
	arr_of_thread_handles = (HANDLE *)malloc(sizeof(HANDLE)*num_of_lines);

	temp_Threads_to_run_s = head_Threads_to_run_s;

	while (NULL != head_Threads_to_run_s) {


		arr_of_thread_handles[handle_index] = CreateThreadSimple(test_and_compare, &head_Threads_to_run_s);
		head_Threads_to_run_s->thread_handle = arr_of_thread_handles[handle_index];
		handle_index++;
		head_Threads_to_run_s = head_Threads_to_run_s->next;
	}
	head_Threads_to_run_s = temp_Threads_to_run_s;
	D_num_of_lines = (DWORD)num_of_lines;
	return_value_of_wait = WaitForMultipleObjects(D_num_of_lines, arr_of_thread_handles,1,dwMilliseconds);

	print_output(head_Threads_to_run_s, argv[2]);
	close_all_handels(arr_of_thread_handles,num_of_lines);
	free_all(&head_Threads_to_run_s, arr_of_thread_handles, num_of_lines);
	return 0;
}