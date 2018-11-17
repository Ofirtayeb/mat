/*Description: This is the parsing module - Here are the functions that deal with
parsing of the input file, output files of the programs tested and the excpected
results files.
*/
#include "pars.h"

/*Description: This functions retrieves the program name and its args
in order to send it to the create process function. Also, retrieves the path
to the excpected results file.
Parameters: char **Line - An adress of a pointer which holds the entire
line inside the input file. 
char **command_line - An adress of a pointer which will hold the needed command
line. This parameter serves also as an output.
char **output_path - An adress of a pointer which will hold thepath
to the excpected results file. This parameter serves also as an output.
Return: As described above - command_line, output_path.
*/
void create_command_line(char **Line, char **command_line, char **output_path) {
	char *P_last_space = NULL;
	P_last_space = strrchr(*Line, ' ');
	*P_last_space = '\0';
	P_last_space += 1;
	*output_path = strtok_s(P_last_space, "\n", &P_last_space);//puts \0 insted of \n
	*command_line = *Line;
}


/*Description: Counts the number of lines in the input file in order
to know how much tests and threads will be open.
Parameters: A file handler of the input file with all the tests to run.
Return: Number of lines in the input file.
*/
int line_counter(char *file_path) {
	FILE *input_file_stream;
	int lines = 1;
	char ch;
	errno_t retval;
	retval = fopen_s(&input_file_stream, file_path, "r");

	if (0 != retval)
	{
		printf("Failed to open file.\n");
		return STATUS_CODE_FAILURE;
	}
	while (!feof(input_file_stream))
	{
		ch = fgetc(input_file_stream);
		if (ch == '\n')
		{
			lines++;
		}
	}
	retval = fclose(input_file_stream);
	if (0 != retval)
		return STATUS_CODE_FAILURE;
	return lines;
}

/*Description: This function compares between two files. This is needed
in order to know whether the tested program gave the correct output or not.
Parameters: char *actual_results - a pointer to the path of the file containing
the output of the tested program.
char *expected_results - a pointer to the path of the file containing
the excpected output of the tested program.
Return: An int which defines whether the files are identical.
*/
int file_compare(char *actual_results, char *expected_results) {
	FILE *fp_actual_results = NULL;
	FILE *fp_expected_results = NULL;
	errno_t retval;
	char ch1, ch2;
	retval = fopen_s(&fp_actual_results, actual_results, "r");
	if (0 != retval)
	{
		printf("Failed to open file.\n");
		return STATUS_CODE_FAILURE;
	}
	retval = fopen_s(&fp_expected_results, expected_results, "r");
	if (0 != retval)
	{
		printf("Failed to open file.\n");
		return STATUS_CODE_FAILURE;
	}
	ch1 = getc(fp_actual_results);
	ch2 = getc(fp_expected_results);
	while (ch1 != EOF && ch2 != EOF) {
		if (ch1 != ch2){
			return STATUS_FILES_DIFFER;
		}
		ch1 = getc(fp_actual_results);
		ch2 = getc(fp_expected_results);
	}
	retval = fclose(fp_actual_results);
	if (0 != retval)
		return STATUS_CODE_FAILURE;
	retval = fclose(fp_expected_results);
	if (0 != retval)
		return STATUS_CODE_FAILURE;
	return 0;
}



