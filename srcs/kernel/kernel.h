

#ifndef KFS_KERNEL_H
#define KFS_KERNEL_H

#include "processes/processes.h"

typedef struct s_kernel {
    t_process *processes_list;
}               t_kernel;

t_kernel *get_kernel_struct();

#endif
