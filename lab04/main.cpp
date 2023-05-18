#include <iostream>
#include <cstdlib>
#include <ctime>

#include "toy_pcb.h"
#include "output.h"
#include "scheduler.h"

using std::printf, std::scanf, std::cin, std::cout, std::endl;
using std::srand, std::rand;

void init_process(PCB* pcb_list, int n){
    srand(std::time(nullptr));

    for (int i = 0; i < n; i++) {
        pcb_list[i].pid = i;
        pcb_list[i].states = TASK_INTERRUPTIBLE;
        pcb_list[i].arrive_time = rand() % 11; // 到达时间随机分布 [0, 10]
        pcb_list[i].start_time = -1;
        pcb_list[i].left_time = pcb_list[i].serve_time = rand() % 20 + 1; // 服务时间随机分布 [1, 20]
        pcb_list[i].end_time = -1;
        pcb_list[i].priority = rand() % 5 + 1; // 优先级随机分布 [1, 5]
        pcb_list[i].status = 0;
        pcb_list[i].turnaround_time = 0;
    }
}

int choose_algorithm() {
    printf("Please choose scheduling algorithm:\n");
    printf("1. FIFO\n");
    printf("2. Round-robin\n");
    printf("3. Priority\n");
    printf("4. SJF\n");
    printf("5. SJF-preemptive\n");
    printf("6. Priority-preemptive\n");
    printf("7. MFQ\n");

    int choice = 0;
    do {
        printf("Your choice (1-7): ");
        scanf("%d", &choice);
    } while (choice < 1 || choice > 7);

    return choice;
}

int main(){
    cout<<"Please input the number of random processes to simulate: "<<endl;
    int num;
    cin>>num;
    struct PCB pcb[20];
    init_process(pcb, num);
    print_pcb_table(pcb,num);

    //选择调度
    int algorithm = choose_algorithm();
    switch (algorithm) {
        case 1:
            printf("FIFO selected.\n");
            scheduler::first_in_first_out(pcb, num);
            break;
        case 2:
            printf("Round-robin selected.\n");
            scheduler::round_robin(pcb, num);
            break;
        case 3:
            printf("Priority selected.\n");
            scheduler::priority(pcb, num);
            break;
        case 4:
            printf("SJF selected.\n");
            scheduler::shortest_job_first(pcb, num);
            break;
        case 5:
            printf("SJF-preemptive selected.\n");
            scheduler::shortest_job_first_preemptive(pcb, num);
            break;
        case 6:
            printf("Priority-preemptive selected.\n");
            scheduler::priority_preemptive(pcb, num);
            break;
        case 7:
            printf("multilevel_feedback_queues selected.\n");
            scheduler::multilevel_feedback_queues(pcb, num);
        default:
            // 不可能到达的分支
            break;
    }
    return 0;
}

