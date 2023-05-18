//
// Created by xdd on 23-5-18.
//

#ifndef LAB04_OUTPUT_H
#define LAB04_OUTPUT_H

#include <cstdio>
#include "toy_pcb.h"

using std::printf;

void print_pcb_table(PCB* pcb_list, int n) {
    printf("+-----+--------------+------------+----------+\n");
    printf("| PID | Arrive Time  | Serve Time  | Priority |\n");
    printf("+-----+--------------+------------+----------+\n");

    for (int i = 0; i < n; i++) {
        printf("| %-3d | %-12d | %-10d | %-8d |\n",
               pcb_list[i].pid, pcb_list[i].arrive_time,
               pcb_list[i].serve_time, pcb_list[i].priority);
    }

    printf("+-----+--------------+------------+----------+\n");
}

void print_result(PCB pcb_list[], int n) {
    printf("+-----+------------+------------+------------+------------+------------+------------+\n");
    printf("| PID | Serve Time | Arrive Time| Start Time |  End Time  |  Priority  | Turnaround |\n");
    printf("+-----+------------+------------+------------+------------+------------+------------+\n");
    for (int i = 0; i < n; i++) {
        printf("|%5d|%12d|%12d|%12d|%12d|%12d|%12d|\n",
               pcb_list[i].pid, pcb_list[i].serve_time, pcb_list[i].arrive_time,
               pcb_list[i].start_time, pcb_list[i].end_time, pcb_list[i].priority,
               pcb_list[i].turnaround_time);
    }
    printf("+-----+------------+------------+------------+------------+------------+------------+\n");
}

#endif //LAB04_OUTPUT_H
