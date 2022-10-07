

#ifndef KERNEL_H
#define KERNEL_H

#include "processes/processes.h"
#include "memory/memory.h"

typedef struct s_kernel {
	t_memory_infos *memory;
    t_processes *processes;
}               t_kernel;

t_kernel *get_kernel_struct();

#endif
