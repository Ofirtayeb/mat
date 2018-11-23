#pragma once
#include <math.h>
#include "linked_list.h"
#include <crtdbg.h>

#define _CRTDBG_MAP_ALLOC

void free_all(Threads_to_run_s **head, HANDLE **arr_of_thread_handle);
