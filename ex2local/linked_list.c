/*Description: This is the parsing module - Here are the functions that deal with
parsing of the input file, output files of the programs tested and the excpected
results files.
*/

#include "linked_list.h"

/*Description: Creates a linked list of all the programs need to be tested.
Parameters: Threads_to_run_s *head - A pointer which will hold the head of
the linked list. This is also an output of the function. 
char *file_path - The path of the file holding all the programs to be tested.
Return: int for a success or a failure of this function.
*/
int thread_list_creator(Threads_to_run_s *head,char *file_path) {
	char p_line[MAX_LINE_LEN] = { 0 };
	char *p_ret_str = NULL;
	char *command_line = NULL;
	char *output_path = NULL;//strcpy? allocate?
	FILE *input_file_stream = NULL;
	Threads_to_run_s *curr;

	errno_t retval;
	retval = fopen_s(&input_file_stream, file_path, "r");

	if (0 != retval)
	{
		printf("Failed to open file.\n");
		return STATUS_CODE_FAILURE;
	}

	p_ret_str = fgets(p_line, MAX_LINE_LEN, input_file_stream);
	if (NULL == p_ret_str)
		return STATUS_CODE_FAILURE;
	create_command_line(&p_ret_str, &command_line, &output_path);
	thread_list_creator_helper_command(head, command_line, output_path);
	head->next = NULL;
	curr = head;


	p_ret_str = fgets(p_line, MAX_LINE_LEN, input_file_stream);
	while (NULL != p_ret_str)
	{
		Threads_to_run_s *new_Threads_to_run_s;
		new_Threads_to_run_s = (Threads_to_run_s *)malloc(sizeof(Threads_to_run_s));
		create_command_line(&p_ret_str, &command_line, &output_path);
		thread_list_creator_helper_command(new_Threads_to_run_s, command_line, output_path);



		new_Threads_to_run_s->next = NULL;
		curr->next = new_Threads_to_run_s;
		curr = new_Threads_to_run_s;
		p_ret_str = fgets(p_line, MAX_LINE_LEN, input_file_stream);
	}
	retval = fclose(input_file_stream);
	if (0 != retval)
		return STATUS_CODE_FAILURE;
	return STATUS_CODE_SUCCESS;
}


/*Description: This is a help function for thread_list_creator function. Here we
allocate needed memory and create the nodes of the linked list.
Parameters: Threads_to_run_s *node - a pointer to a struct which contains 
all the info regrading the program to be tested. This acts as an output parameter.
char *command_line - a pointer to the program name and args.
char *output_path -  a pointer to the path of the excpect results file. 
Return: void.
*/
void thread_list_creator_helper_command(Threads_to_run_s *node, char *command_line, char *output_path) {
	char *program_path = NULL;
	char *backslash_search = NULL;
	node->command_line = (char*)malloc((strlen(command_line) + 1) * sizeof(char));
	if (node->command_line == NULL) return STATUS_CODE_FAILURE;
	strcpy_s(node->command_line, strlen(command_line) + 1, command_line);



	node->program_output = (char*)malloc((strlen(command_line) + 1) * sizeof(char));
	if (node->program_output == NULL) return STATUS_CODE_FAILURE;

	program_path = strchr(command_line, ' ');
	if (NULL != program_path) {
		*program_path = '\0';
	}
	program_path = strchr(command_line, '.');
	*(program_path + 1) = 't';
	*(program_path + 3) = 't';


	backslash_search = strrchr(command_line, '\\');
	if (NULL == backslash_search) {
		strcpy_s(node->program_output, strlen(command_line) + 1, command_line);
	}
	else {
		*backslash_search = '\0';
		backslash_search++;//now points to the start of the program.txt
		strcpy_s(node->program_output, strlen(command_line) + 1, backslash_search);
	}

	node->output_path = (char*)malloc((strlen(output_path) +1 )* sizeof(char));
	if (node->output_path == NULL) return STATUS_CODE_FAILURE;
	strcpy_s(node->output_path, strlen(output_path) + 1, output_path);


	node->thread_handle = (HANDLE *)malloc(sizeof(HANDLE));
	if (node->thread_handle == NULL) return STATUS_CODE_FAILURE;
	node->return_value = (DWORD *)malloc(sizeof(DWORD));
	if (node->return_value == NULL) return STATUS_CODE_FAILURE;
	node->status = (DWORD *)malloc(sizeof(DWORD));
	if (node->status == NULL) return STATUS_CODE_FAILURE;
	node->thread_handle = NULL;
	node->return_value = NULL;
	node->status = 69696969;
}


/*Description: Prints the results of the testing of the programs.
Parameters: Threads_to_run_s *node - a pointer to the struct which holds
all relevant info regarding the tested program.
char *file_path - a pointer to the path of the file in which the results 
are printed.
Return: void.
*/
void print_output(Threads_to_run_s *node, char *file_path) {
	errno_t retval;
	FILE *input_file_stream = NULL;
	Threads_to_run_s *temp = node;
	int test_number = 1;

	retval = fopen_s(&input_file_stream, file_path, "a+");
	if (0 != retval)
	{
		printf("Failed to open file.\n");
		return STATUS_CODE_FAILURE;
	}
	
	while (NULL != temp) {
		switch (temp->status)
		{
		case (0):	fprintf(input_file_stream, "test #%d : %s\n", test_number, temp->message);//succeded
			break;
		case (WAIT_TIMEOUT): fprintf(input_file_stream, "test #%d : %s\n", test_number, temp->message);//timedout
			break;
		case (THREAD_STATUS_FILE_COPMARE_FAILURE): fprintf(input_file_stream, "test #%d : %s\n", test_number, temp->message);//faild
			break;
		default: fprintf(input_file_stream, "test #%d : %s %d\n", test_number, temp->message,temp->return_value);//crashed
			break;
		}
		test_number++;
		temp = temp->next;
	}

	retval = fclose(input_file_stream);
	if (0 != retval)
	{
		printf("Failed to close file.\n");
		return STATUS_CODE_FAILURE;
	}
}

/*Description - gets adress to pointer of nodes linked list head and 
deletes all list and freeing the memory.
Parameters - Threads_to_run_s **head - points to a pointer of lists head
return nothing
*/
void free_linked_list(Threads_to_run_s **head) {

	Threads_to_run_s *current = *head;
	Threads_to_run_s *next = NULL;

	while (current != NULL)
	{
		next = current->next;
		if (NULL != current)
		{
			free(current->command_line);
			free(current->output_path);
			free(current->program_output);
			free(current->return_value);
			free(current->status);
			free(current->thread_handle);
			free(current);
		}
		current = next;
	}
	*head = NULL;
}