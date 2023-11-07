#include "processes.h"
#include "../memory/vmm/malloc/malloc.h"
#include "../isr/irqs/irqs.h"
#include "../../lib/iolib.h"
#include "../kernel.h"

void put_process_end_list(t_scheduler_queue *process) {
    get_kernel_struct()->processes->last->next = process;
    get_kernel_struct()->processes->processes_list = process->next;
    process->next = 0;
}

void context_switch(t_process *process) {
    printd(KERN_INFO, "before: process->regs is %p (or %p ?)", process->regs, &process->regs);
    if (process->regs.cr3 != 0) {
        printd(KERN_INFO, "regs.cr3 of process '%s' is not null. Loading page directory for this process.", process->name);
        printd(KERN_INFO, "Switching for page table at addr %p (or %p)", process->regs.cr3, process->page_directory->page_directory);
        enable_paging(process->page_directory->page_directory);
        printd(KERN_INFO, "Enabled paging for %s", process->name);
    }
    printd(KERN_INFO, "process->regs is %p (or %p ?)", process->regs, &process->regs);
    printd(KERN_INFO, "retry compute %p", (char *) process + 64);
    switch_regs((t_process_registers *)((char *) process + 64));
}

void scheduler_loop() {
    t_processes *processes_infos = get_kernel_struct()->processes;
    //while (processes_infos->size != 0) {
        processes_infos->current_process = processes_infos->first;
        printd(KERN_INFO, "Current process is %s", processes_infos->current_process->process->name);
        //processes_infos->current_process->process->status = STATUS_RUN;
        context_switch(processes_infos->current_process->process);
        uint32_t current_tick = get_tick();
        uint32_t counter_tick = get_tick();
        printd(KERN_INFO, "counter_tick is %d, current_tick is %d", counter_tick, current_tick);
        while (current_tick + 100 != counter_tick) {
            //printd(KERN_INFO, "Counting tick for %s %d/%d", processes_infos->first->process->name, current_tick + 100, counter_tick);
            counter_tick = get_tick();
        }
        printd(KERN_INFO, "Current process timeslice is over for %s", processes_infos->first->process->name);
        put_process_end_list(processes_infos->current_process);
    //}
}

void display_process_list() {
    t_scheduler_queue *head;
    printd(KERN_INFO, "process list contains: %d processes", get_kernel_struct()->processes->size);
    head = get_kernel_struct()->processes->processes_list;
    while (head) {
        printd(KERN_INFO, " - %s", head->process->name);
        head = head->next;
    }
}

void add_process_to_queue(t_process *process) {
    t_processes *processes_infos = get_kernel_struct()->processes;
    t_scheduler_queue *process_node;

    printd(KERN_INFO, "Adding process %d to queue", process->pid);
    if (!(process_node = malloc(sizeof(t_scheduler_queue))))
        return ;
    process_node->process = process;
    processes_infos->size++;
    if (processes_infos->processes_list == 0) {
        processes_infos->processes_list = process_node;
        processes_infos->first = process_node;
        processes_infos->last = process_node;
    } else {
        processes_infos->last->next = process_node;
        processes_infos->last = process_node;
    }
    display_process_list();
    scheduler_loop();
}