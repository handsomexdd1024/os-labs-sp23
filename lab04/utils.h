//
// Created by xdd on 23-5-18.
//

#ifndef LAB04_UTILS_H
#define LAB04_UTILS_H

#include "toy_pcb.h"

bool cmp_time(PCB a,PCB b){
    return a.arrive_time<b.arrive_time;
}

bool cmp_priority(PCB a,PCB b){
    return a.priority<b.priority;
}

bool cmp_job_length(PCB a, PCB b){
    return a.serve_time<b.serve_time;
}

bool all_finished(PCB* pcb_list, int n) {
    for (int i = 0; i < n; i++) {
        if (pcb_list[i].status != FINISHED) {
            return false;
        }
    }
    return true;
}

#endif //LAB04_UTILS_H
