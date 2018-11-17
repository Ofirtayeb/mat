/*Description: This is the parsing module header - Here are the functions that
deal with parsing of the input file, output files of the programs tested and the
excpected results files.
*/
#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "threading.h"

#define MAX_LINE_LEN 100
#define STATUS_FILES_DIFFER -3

void create_command_line(char **Line, char **command_line, char **output_path);

int line_counter(char *file_path);

int file_compare(char *actual_results, char *expected_results);