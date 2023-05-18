//
// Created by xdd on 23-5-18.
//

#ifndef LAB04_SCHEDULER_H
#define LAB04_SCHEDULER_H

#include <algorithm>
#include <queue>
#include "utils.h"
#include "output.h"

namespace scheduler {

    using std::sort;
    using std::queue;

    void first_in_first_out(PCB *pcb_lists, int num_process) {
        int cur_pid=0;
        int cur_time=0;
        PCB pcb_list[20];
        for (int i = 0; i < num_process; i++) {  //拷贝pcb
            pcb_list[i]=pcb_lists[i];
        }

        sort(pcb_list,pcb_list + num_process,cmp_time);  //排序

        while (true) {
            if (pcb_list[cur_pid].status==FINISHED) {
                int next_pid = cur_pid;
                for (int i = 0; i < num_process; i++) {
                    next_pid = (next_pid + 1) % num_process;
                    if (pcb_list[next_pid].status != FINISHED)
                        break;  //找到下一个未完成的进程pid
                }
                cur_pid = next_pid;
                if (pcb_list[cur_pid].status == FINISHED) {  //如果全部进程都完成了
                    break;
                }
            }

            if (pcb_list[cur_pid].left_time == 0) {  //执行完成
                pcb_list[cur_pid].states=TASK_ZOMBIE;
                pcb_list[cur_pid].status=FINISHED;
                pcb_list[cur_pid].end_time=cur_time;
                pcb_list[cur_pid].turnaround_time=cur_time-pcb_list[cur_pid].arrive_time;
                continue;
            }

            if(cur_time<pcb_list[cur_pid].arrive_time){
                cur_time=pcb_list[cur_pid].arrive_time;
            }

            if(pcb_list[cur_pid].start_time==-1){  //给start_time赋值
                pcb_list[cur_pid].start_time=cur_time;
            }

            //执行进程
            pcb_list[cur_pid].left_time--;
            cur_time++;
        }
        print_result(pcb_list,num_process);
    }

    void round_robin(struct PCB *pcb_lists, int num_process){
        int cur_pid=-1;  //-1表示没有进程在运行
        int cur_time=0;  //当前进程已经使用的时间片
        int time_used=0;  //当前进程已使用的时间片
        bool cpu_idle=true;  //cpu是否是空闲状态

        queue<int> ready_queue;
        PCB pcb_list[20];
        for(int i=0;i<num_process;i++){  //拷贝pcb
            pcb_list[i]=pcb_lists[i];
        }

        while(true) {
            //将到达时间等于当前时间的加入就绪队列
            for(int i=0;i<num_process;i++){
                if(
                        pcb_list[i].states!=TASK_STOPPED
                        && pcb_list[i].states!=TASK_ZOMBIE
                        && pcb_list[i].arrive_time==cur_time
                        ){
                    ready_queue.push(i);
                }
            }

            if(cur_pid==-1 && !ready_queue.empty()){
                //若没有在执行的，选取队列头部调度
                cur_pid=ready_queue.front();
                ready_queue.pop();

                //初始化每个进程的开始时间
                if(pcb_list[cur_pid].start_time==-1){
                    pcb_list[cur_pid].start_time=cur_time;
                }
                pcb_list[cur_pid].states=TASK_RUNNING;
                cpu_idle=false;
            }

            if(cur_pid==-1 && ready_queue.empty()){
                //没有进程执行，cpu空闲
                cpu_idle=true;
            }

            if(cur_pid!=-1 && pcb_list[cur_pid].left_time==0){
                //当前进程执行完毕
                pcb_list[cur_pid].states=TASK_ZOMBIE;
                pcb_list[cur_pid].status=FINISHED;
                pcb_list[cur_pid].end_time=cur_time;
                pcb_list[cur_pid].turnaround_time=cur_time-pcb_list[cur_pid].arrive_time;
                cur_pid=-1;
                time_used=0;
                continue;
            }

            if(cur_pid!=-1){
                pcb_list[cur_pid].left_time--;
                time_used++;

                if(time_used >= TIME_SLICE_LEN){
                    //时间片用完，将进程放入队列尾部
                    pcb_list[cur_pid].states=TASK_INTERRUPTIBLE;
                    ready_queue.push(cur_pid);
                    cur_pid=-1;
                    time_used=0;
                }
            }

            if (all_finished(pcb_list, num_process)) {
                // 所有进程都已经执行完毕，跳出循环
                break;
            }

            cur_time++;
        }
        print_result(pcb_list,num_process);
    }

    void priority(struct PCB *pcb_lists, int num_process){
        int cur_pid=0;
        int cur_time=0;
        struct PCB pcb_list[20];
        bool no_wait=true;  //表示此时没有线程在等待(有的话代表虽然优先级更低，但是可以执行，不用等待)
        for(int i=0;i<num_process;i++){  //拷贝pcb
            pcb_list[i]=pcb_lists[i];
        }

        sort(pcb_list,pcb_list+num_process,cmp_time);  //先根据先来后到排序
        sort(pcb_list,pcb_list+num_process,cmp_priority);  //再根据优先级排序

        while(true) {
            //当前完成进程进行切换
            if(pcb_list[cur_pid].status==FINISHED){
                int next_pid=cur_pid;
                for(int i=0;i<num_process;i++){
                    if(pcb_list[i].status!=FINISHED && i<next_pid){next_pid=i;  //如果有前面未完成的进程，优先级高
                        next_pid=i;
                        break;
                    }
                    else{
                        next_pid=(next_pid+1)%num_process;
                        if(pcb_list[next_pid].status!=FINISHED)break;  //找到下一个未完成的进程pid
                    }
                }
                cur_pid=next_pid;
                if(pcb_list[cur_pid].status==FINISHED){  //如果全部进程都完成了
                    break;
                }
            }

            if(pcb_list[cur_pid].left_time==0){  //执行完成
                pcb_list[cur_pid].states=TASK_ZOMBIE;
                pcb_list[cur_pid].status=FINISHED;
                pcb_list[cur_pid].end_time=cur_time;
                pcb_list[cur_pid].turnaround_time=cur_time-pcb_list[cur_pid].arrive_time;
                continue;
            }

            if(cur_time<pcb_list[cur_pid].arrive_time){
                if(pcb_list[cur_pid].status!=FINISHED){
                    for(int i=0;i<num_process;i++){
                        //如果高优先级的还没有来
                        if(
                                pcb_list[i].arrive_time<=cur_time
                                && pcb_list[i].arrive_time<pcb_list[cur_pid].arrive_time
                                && pcb_list[i].status!=FINISHED
                                ){
                            no_wait=true;
                            cur_pid=i;  //切换进程
                            break;
                        }
                        else
                            no_wait=false;
                    }
                    if(!no_wait){
                        cur_time++;
                        continue;
                    }
                }
            }

            if(pcb_list[cur_pid].start_time==-1){  //给start_time赋值
                pcb_list[cur_pid].start_time=cur_time;
            }

            //执行进程
            pcb_list[cur_pid].left_time--;
            cur_time++;
        }
        print_result(pcb_list,num_process);
    }

    void shortest_job_first(struct PCB *pcb_lists, int num_process){
        int cur_pid=0;
        int cur_time=0;
        struct PCB pcb_list[20];
        bool no_wait=true;  //表示此时没有线程在等待(有的话代表虽然任务更长，但是可以执行，不用等待)
        for(int i=0;i<num_process;i++){  //拷贝pcb
            pcb_list[i]=pcb_lists[i];
        }

        sort(pcb_list, pcb_list + num_process, cmp_job_length);  //根据优先级排序

        while(true){
            //当前完成进程进行切换
            if(pcb_list[cur_pid].status==FINISHED){
                int next_pid=cur_pid;
                for(int i=0;i<num_process;i++){
                    if(pcb_list[i].status!=FINISHED && i<next_pid){next_pid=i;  //如果有前面未完成的进程，优先级高
                        next_pid=i;
                        break;
                    }
                    else{
                        next_pid=(next_pid+1)%num_process;
                        if(pcb_list[next_pid].status!=FINISHED)break;  //找到下一个未完成的进程pid
                    }
                }
                cur_pid=next_pid;
                if(pcb_list[cur_pid].status==FINISHED){  //如果全部进程都完成了
                    break;
                }
            }

            if(pcb_list[cur_pid].left_time==0){  //执行完成
                pcb_list[cur_pid].states=TASK_ZOMBIE;
                pcb_list[cur_pid].status=FINISHED;
                pcb_list[cur_pid].end_time=cur_time;
                pcb_list[cur_pid].turnaround_time=cur_time-pcb_list[cur_pid].arrive_time;
                continue;
            }

            if(cur_time<pcb_list[cur_pid].arrive_time){
                if(pcb_list[cur_pid].status!=FINISHED){
                    for(int i=0;i<num_process;i++){
                        //如果高优先级的还没有来
                        if(
                                pcb_list[i].arrive_time<=cur_time
                                && pcb_list[i].arrive_time<pcb_list[cur_pid].arrive_time
                                && pcb_list[i].status!=FINISHED
                                ){
                            no_wait=true;
                            cur_pid=i;  //切换进程
                            break;
                        }
                        else
                            no_wait=false;
                    }
                    if(!no_wait){
                        cur_time++;
                        continue;
                    }
                }
            }

            if(pcb_list[cur_pid].start_time==-1){  //给start_time赋值
                pcb_list[cur_pid].start_time=cur_time;
            }

            //执行进程
            pcb_list[cur_pid].left_time--;
            cur_time++;
        }
        print_result(pcb_list,num_process);
    }

    void shortest_job_first_preemptive(struct PCB *pcb_lists, int num_process){
        int cur_pid=0;
        int cur_time=0;
        struct PCB pcb_list[20];
        bool no_wait=true;  //表示此时没有线程在等待(有的话代表虽然任务更长，但是可以执行，不用等待)
        for(int i=0;i<num_process;i++){  //拷贝pcb
            pcb_list[i]=pcb_lists[i];
        }

        sort(pcb_list, pcb_list + num_process, cmp_job_length);  //根据优先级排序

        while(true){
            //当前完成进程进行切换
            if(pcb_list[cur_pid].status==FINISHED){
                int next_pid=cur_pid;
                for(int i=0;i<num_process;i++){
                    if(pcb_list[i].status!=FINISHED && i<next_pid){next_pid=i;  //如果有前面未完成的进程，优先级高
                        next_pid=i;
                        break;
                    }
                    else{
                        next_pid=(next_pid+1)%num_process;
                        if(pcb_list[next_pid].status!=FINISHED)break;  //找到下一个未完成的进程pid
                    }
                }
                cur_pid=next_pid;
                if(pcb_list[cur_pid].status==FINISHED){  //如果全部进程都完成了
                    break;
                }
            }

            if(pcb_list[cur_pid].left_time==0){  //执行完成
                pcb_list[cur_pid].states=TASK_ZOMBIE;
                pcb_list[cur_pid].status=FINISHED;
                pcb_list[cur_pid].end_time=cur_time;
                pcb_list[cur_pid].turnaround_time=cur_time-pcb_list[cur_pid].arrive_time;
                continue;
            }

            bool preemptive=false;
            //处理抢占式
            for(int i=0;i<num_process;i++){
                if (
                        pcb_list[i].arrive_time==cur_time
                        && i!=cur_pid && pcb_list[i].left_time<pcb_list[cur_pid].left_time
                        ) {
                    cur_pid=i;  //使得新来的抢占
                    preemptive=true;
                    break;
                }
            }
            if(preemptive)continue;

            if(cur_time<pcb_list[cur_pid].arrive_time){
                if(pcb_list[cur_pid].status!=FINISHED){
                    for(int i=0;i<num_process;i++){
                        //如果高优先级的还没有来
                        if(
                                pcb_list[i].arrive_time<=cur_time
                                && pcb_list[i].arrive_time<pcb_list[cur_pid].arrive_time
                                && pcb_list[i].status!=FINISHED)
                        {
                            no_wait=true;
                            cur_pid=i;  //切换进程
                            break;
                        }
                        else
                            no_wait=false;
                    }
                    if(!no_wait){
                        cur_time++;
                        continue;
                    }
                }
            }

            if(pcb_list[cur_pid].start_time==-1){  //给start_time赋值
                pcb_list[cur_pid].start_time=cur_time;
            }

            //执行进程
            pcb_list[cur_pid].left_time--;
            cur_time++;
        }
        print_result(pcb_list,num_process);
    }

    void priority_preemptive(struct PCB *pcb_lists, int num_process){
        int cur_pid=0;
        int cur_time=0;
        struct PCB pcb_list[20];
        bool no_wait=true;  //表示此时没有线程在等待(有的话代表虽然优先级更低，但是可以执行，不用等待)
        for(int i=0;i<num_process;i++){  //拷贝pcb
            pcb_list[i]=pcb_lists[i];
        }

        sort(pcb_list,pcb_list+num_process,cmp_time);  //先根据先来后到排一下
        sort(pcb_list,pcb_list+num_process,cmp_priority);  //再根据优先级排序

        while(true){
            //当前完成进程进行切换
            if(pcb_list[cur_pid].status==FINISHED){
                int next_pid=cur_pid;
                for(int i=0;i<num_process;i++){
                    if(pcb_list[i].status!=FINISHED && i<next_pid){next_pid=i;  //如果有前面未完成的进程，优先级高
                        next_pid=i;
                        break;
                    }
                    else{
                        next_pid=(next_pid+1)%num_process;
                        if(pcb_list[next_pid].status!=FINISHED)break;  //找到下一个未完成的进程pid
                    }
                }
                cur_pid=next_pid;
                if(pcb_list[cur_pid].status==FINISHED){  //如果全部进程都完成了
                    break;
                }
            }

            if(pcb_list[cur_pid].left_time==0){  //执行完成
                pcb_list[cur_pid].states=TASK_ZOMBIE;
                pcb_list[cur_pid].status=FINISHED;
                pcb_list[cur_pid].end_time=cur_time;
                pcb_list[cur_pid].turnaround_time=cur_time-pcb_list[cur_pid].arrive_time;
                continue;
            }

            bool preemptive=false;
            //处理抢占式
            for(int i=0;i<num_process;i++){
                if(
                        pcb_list[i].arrive_time==cur_time
                        && i!=cur_pid && pcb_list[i].priority<pcb_list[cur_pid].priority
                        ){
                    cur_pid=i;  //使得新来的抢占
                    preemptive=true;
                    break;
                }
            }
            if(preemptive)continue;

            if(cur_time<pcb_list[cur_pid].arrive_time){
                if(pcb_list[cur_pid].status!=FINISHED){
                    for(int i=0;i<num_process;i++){
                        //如果高优先级的还没有来
                        if (
                                pcb_list[i].arrive_time<=cur_time
                                && pcb_list[i].arrive_time<pcb_list[cur_pid].arrive_time
                                && pcb_list[i].status!=FINISHED
                                ) {
                            no_wait=true;
                            cur_pid=i;  //切换进程
                            break;
                        }
                        else
                            no_wait=false;
                    }
                    if(!no_wait){
                        cur_time++;
                        continue;
                    }
                }
            }

            if(pcb_list[cur_pid].start_time==-1){  //给start_time赋值
                pcb_list[cur_pid].start_time=cur_time;
            }

            //执行进程
            pcb_list[cur_pid].left_time--;
            cur_time++;
        }
        print_result(pcb_list,num_process);
    }

    void multilevel_feedback_queues(struct PCB *pcb_lists, int num_process){
        int cur_pid=0;
        int cur_time=0;
        int time_used1=0,time_used2=0,time_used3=0;
        struct PCB pcb_list[20];
        bool no_wait=true;  //表示此时没有线程在等待(有的话代表虽然优先级更低，但是可以执行，不用等待)
        for(int i=0;i<num_process;i++){  //拷贝pcb
            pcb_list[i]=pcb_lists[i];
        }

        sort(pcb_list,pcb_list+num_process,cmp_time);  //先根据先来后到排一下
        queue<int> que1;
        queue<int> que2;
        queue<int> que3;
        int quantum1=2,quantum2=4,quantum3=8;
        while (true)
        {
            for(int i=0;i<num_process;i++){  //新进入的进程放入队列
                if(pcb_list[i].arrive_time==cur_time){
                    que1.push(i);
                }
            }

            if(!que1.empty()){  //处理一级队列
                cur_pid=que1.front();

                //初始化每个进程的开始时间
                if(pcb_list[cur_pid].start_time==-1){
                    pcb_list[cur_pid].start_time=cur_time;
                }

                if(pcb_list[cur_pid].left_time==0){  //如果执行完当前进程
                    pcb_list[cur_pid].status=FINISHED;
                    que1.pop();  //弹出第一个
                    pcb_list[cur_pid].end_time=cur_time;
                    pcb_list[cur_pid].turnaround_time=cur_time-pcb_list[cur_pid].arrive_time;
                }
                else{
                    cur_time++;
                    pcb_list[cur_pid].left_time--;
                    time_used1++;
                    if(time_used1==quantum1 && pcb_list[cur_pid].left_time!=0){  //没做完 放入下级队列
                        que1.pop();
                        que2.push(cur_pid);
                        time_used1=0;
                    }
                }
                continue;
            }

            else if(!que2.empty()){  //处理二级队列
                cur_pid=que2.front();
                if(pcb_list[cur_pid].left_time==0){  //如果执行完当前进程
                    pcb_list[cur_pid].status=FINISHED;
                    que2.pop();  //弹出第一个
                    pcb_list[cur_pid].end_time=cur_time;
                    pcb_list[cur_pid].turnaround_time=cur_time-pcb_list[cur_pid].arrive_time;
                }
                else{  //执行进程
                    cur_time++;
                    pcb_list[cur_pid].left_time--;
                    time_used2++;
                    if(time_used2==quantum2 && pcb_list[cur_pid].left_time!=0){  //没做完 放入下级队列
                        que2.pop();
                        que3.push(cur_pid);
                        time_used2=0;
                    }
                }
                continue;
            }

            else if(!que3.empty()){
                cur_pid=que3.front();
                if(pcb_list[cur_pid].left_time==0){  //如果执行完当前进程
                    pcb_list[cur_pid].status=FINISHED;
                    que3.pop();  //弹出第一个
                    pcb_list[cur_pid].end_time=cur_time;
                    pcb_list[cur_pid].turnaround_time=cur_time-pcb_list[cur_pid].arrive_time;
                }
                else{  //执行进程
                    cur_time++;
                    pcb_list[cur_pid].left_time--;
                    time_used3++;
                    if(time_used3==quantum3 && pcb_list[cur_pid].left_time!=0){  //没做完 放入下级队列
                        que3.pop();
                        que3.push(cur_pid);
                        time_used2=0;
                    }
                }
                continue;
            }

            if(all_finished(pcb_list,num_process)){
                break;  //全部完成 退出
            }

            //没有进程来 cpu空闲
            if(que1.empty() && que2.empty() && que3.empty()) {
                cur_time++;
                continue;
            }
        }
        print_result(pcb_list,num_process);
    }
}

#endif //LAB04_SCHEDULER_H
