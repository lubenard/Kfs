#include "processes.h"
#include "../memory/vmm/malloc/malloc.h"
#include "../isr/irqs/irqs.h"
#include "../../lib/iolib.h"
#include "../kernel.h"

t_scheduler_queue *process_queue;

void put_process_end_list(t_scheduler_queue *process) {
    get_kernel_struct()->processes->last->next = process;
    process->next = 0;
}

void scheduler_loop() {
    t_processes *processes_infos = get_kernel_struct()->processes;
    while (processes_infos->size != 0) {
        get_kernel_struct()->processes->current_process = processes_infos->first->process;
        printd(KERN_INFO, "Current process is %s", processes_infos->first->process->name);
        get_kernel_struct()->processes->current_process->status = STATUS_RUN;
        // TODO: Implement context switch around here
        uint32_t current_tick = get_tick();
        uint32_t counter_tick = get_tick();
        while (current_tick + 100 != counter_tick) {
            printd(KERN_INFO, "Counting tick for %s %d/%d", processes_infos->first->process->name, current_tick + 100, counter_tick);
            counter_tick = get_tick();
        }
        printd(KERN_INFO, "Current process timeslice is over for %s", processes_infos->first->process->name);
        put_process_end_list(processes_infos->first);
    }
}

void add_process_to_queue(t_process *process) {
    t_processes *processes_infos = get_kernel_struct()->processes;
    if (process_queue == 0) {
        if (!(process_queue = malloc(sizeof(t_scheduler_queue))))
            return ;
        process_queue->process = process;
        processes_infos->size++;
        processes_infos->first = process_queue;
        processes_infos->last = process_queue;
    }
    scheduler_loop();
}