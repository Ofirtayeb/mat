#pragma once
#include <math.h>
#include "linked_list.h"

void free_main_malloc(HANDLE *arr_of_thread_handle, int num_of_lines);
void free_all(Threads_to_run_s **head, HANDLE *arr_of_thread_handle, int num_of_lines);
