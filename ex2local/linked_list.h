/*Description: This is the parsing module header - Here are all the declarations
of the functions that deal with parsing of the input file, output files of
the programs tested and the excpected results files.*/
#pragma once
#include "pars.h"
#include "threading.h" 



int thread_list_creator(Threads_to_run_s *head, char *file_path);
void thread_list_creator_helper_command(Threads_to_run_s *node, char *command_line, char *output_path);
void print_output(Threads_to_run_s *node, char *file_path);
void free_linked_list(Threads_to_run_s **head);