#include <stdio.h>
#include "oslabs.h"

void run_process_arrival_srtp() {
    struct PCB ready_queue[QUEUEMAX];
    int queue_cnt = 0;
    struct PCB current_process = init_pcb(1, 1, 8, 1, 9, 8, 0);
    struct PCB new_process = init_pcb(2, 2, 6, 0, 0, 6, 0);
    int time_stamp = 2;

    printf("Doing handle_process_arrival_srtp...\n");
    struct PCB next_process_arrival = handle_process_arrival_srtp(ready_queue, &queue_cnt, current_process, new_process,
                                                                  time_stamp);
    print_pcb(next_process_arrival);
    print_queue(ready_queue, queue_cnt);
}

void run_process_completion_srtp() {
    struct PCB ready_queue[QUEUEMAX];
    int queue_cnt = 3;
    int time_stamp = 2;
    struct PCB p1 = init_pcb(1, 1, 23, 0, 0, 23, 0);
    struct PCB p2 = init_pcb(2, 1, 22, 0, 0, 22, 0);
    struct PCB p3 = init_pcb(3, 1, 24, 0, 0, 24, 0);
    ready_queue[0] = p1;
    ready_queue[1] = p2;
    ready_queue[2] = p3;

    printf("Doing handle_process_completion_srtp...\n");
    struct PCB next_process_completion = handle_process_completion_srtp(ready_queue, &queue_cnt, time_stamp);
    print_pcb(next_process_completion);
    print_queue(ready_queue, queue_cnt);
}


void run_process_arrival_rr() {
    struct PCB ready_queue[QUEUEMAX];
    int queue_cnt = 0;
    struct PCB current_process = init_pcb(1, 1, 8, 1, 9, 8, 0);
    struct PCB new_process = init_pcb(2, 2, 8, 0, 0, 8, 0);
    int time_stamp = 2;
    int time_quantum = 6;

    printf("Doing handle_process_arrival_rr...\n");
    struct PCB next_process_arrival = handle_process_arrival_rr(ready_queue, &queue_cnt, current_process, new_process,
                                                                  time_stamp, time_quantum);
    print_pcb(next_process_arrival);
    print_queue(ready_queue, queue_cnt);
}

void run_process_completion_rr() {
    struct PCB ready_queue[QUEUEMAX];
    int queue_cnt = 3;
    int time_stamp = 24;
    int time_quantum = 10;
    struct PCB p1 = init_pcb(1, 22, 8, 0, 0, 8, 0);
    struct PCB p2 = init_pcb(2, 21, 8, 0, 0, 8, 0);
    struct PCB p3 = init_pcb(3, 23, 8, 0, 0, 8, 0);
    ready_queue[0] = p1;
    ready_queue[1] = p2;
    ready_queue[2] = p3;

    printf("Doing handle_process_completion_rr...\n");
    struct PCB next_process_arrival = handle_process_completion_rr(ready_queue, &queue_cnt, time_stamp, time_quantum);
    print_pcb(next_process_arrival);
    print_queue(ready_queue, queue_cnt);
}

int main() {
    run_process_arrival_srtp();
    run_process_completion_srtp();

    run_process_arrival_rr();
    run_process_completion_rr();

    return 0;
}

void print_pcb(struct PCB pcb) {
    printf("\tPID: %i\n", pcb.process_id);
    printf("\tATS: %i\n", pcb.arrival_timestamp);
    printf("\tTBT: %i\n", pcb.total_bursttime);
    printf("\tEST: %i\n", pcb.execution_starttime);
    printf("\tEET: %i\n", pcb.execution_endtime);
    printf("\tRBT: %i\n", pcb.remaining_bursttime);
    printf("\tPPR: %i\n", pcb.process_priority);
}

struct PCB init_pcb(int pid, int ats, int tbt, int est, int eet, int rbt, int pp) {
    struct PCB new_pcb;
    new_pcb.process_id = pid;
    new_pcb.arrival_timestamp = ats;
    new_pcb.total_bursttime = tbt;
    new_pcb.execution_starttime = est;
    new_pcb.execution_endtime = eet;
    new_pcb.remaining_bursttime = rbt;
    new_pcb.process_priority = pp;

    return new_pcb;
}

int pcb_is_null(struct PCB pcb) {
    return (pcb.process_id == 0 &&
            pcb.arrival_timestamp == 0 &&
            pcb.total_bursttime == 0 &&
            pcb.execution_starttime == 0 &&
            pcb.execution_endtime == 0 &&
            pcb.remaining_bursttime == 0 &&
            pcb.process_priority == 0);
}

void print_queue(struct PCB ready_queue[QUEUEMAX], int queue_cnt) {
    for (int i = 0; i < queue_cnt; i++) {
        printf("ready_queue[%i]\n", i);
        printf("\tprocess_id:          %i\n", ready_queue[i].process_id);
        printf("\tarrival_timestamp:   %i\n", ready_queue[i].arrival_timestamp);
        printf("\ttotal_bursttime:     %i\n", ready_queue[i].total_bursttime);
        printf("\texecution_starttime: %i\n", ready_queue[i].execution_starttime);
        printf("\texecution_endtime:   %i\n", ready_queue[i].execution_endtime);
        printf("\tremaining_bursttime: %i\n", ready_queue[i].remaining_bursttime);
        printf("\tprocess_priority:    %i\n", ready_queue[i].process_priority);
    }
}