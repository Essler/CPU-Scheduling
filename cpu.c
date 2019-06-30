#include <time.h>
#include "oslabs.h"

// Shortest-Remaining-Time-Next Preemptive Scheduler
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt,
                                       struct PCB current_process, struct PCB new_process, int time_stamp) {
    // If there is no currently-running process (i.e., the third argument is the NULLPCB), then the method returns the
    // PCB of the newly-arriving process, indicating that it is the process to execute next.
    if (pcb_is_null(current_process)) {
        // In this case, the PCB of the new process is modified so that:
        //   the execution start time set to the current timestamp,
        //   the execution end time is set to the sum of the current timestamp and the total burst time
        //   and the remaining burst time is set to the total burst time.
        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }

    // If there is a currently-running process, the method compares the remaining burst time of the
    // currently-running process and the total burst time of the newly-arriving process. If the new
    // process does not have a shorter burst time, then its PCB is simply added to the ready queue
    // and the return value is the PCB of the currently running process. As the newly-arriving process
    // is added to the ready queue, its execution start time and execution end time are set to 0, and
    // the remaining burst time is set to the total burst time.
    if (current_process.remaining_bursttime <= new_process.total_bursttime) {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt)++;
        return current_process;
    }

        // If the new process has a shorter burst time, then the PCB of the currently-running process is
        // added to the ready queue and the return value is the PCB of the new process.
        //
        // In this case, the PCB of the new process is modified so that:
        //   the execution start time is set to the current timestamp,
        //   the execution end time is set to the sum of the current timestamp and the total burst time
        //   and the remaining burst time is set to the total burst time.
        //
        // Also, the PCB of the currently-running process is added to the ready queue,
        // after marking its execution start time and execution end time as 0, and adjusting its remaining burst time.
    else {
        current_process.execution_starttime = 0;
        current_process.execution_endtime = 0;
        current_process.remaining_bursttime = current_process.total_bursttime;
        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt)++;

        new_process.execution_starttime = time_stamp;
        new_process.execution_endtime = time_stamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
}

// Shortest-Remaining-Time Preemptive Scheduler
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    struct PCB next_process = init_pcb(0, 0, 0, 0, 0, 0, 0);

    // If the ready queue is empty, the method returns the NULLPCB, indicating that there is no process to execute next.
    if (*queue_cnt == 0) {
        return next_process;
    }

    // Otherwise, the method finds the PCB of the process in the ready queue with the smallest remaining burst time,
    // removes this PCB from the ready queue and returns it.
    int smallest_rbt = 999;
    int smallest_rbt_index = 0;

    for (int i = 0; i < *queue_cnt; i++) {
        if (ready_queue[i].remaining_bursttime < smallest_rbt) {
            smallest_rbt = ready_queue[i].remaining_bursttime;
            smallest_rbt_index = i;
        }
    }

    next_process = ready_queue[smallest_rbt_index];

    for (int i = smallest_rbt_index; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }

    (*queue_cnt)--;

    // Before returning the PCB of the next process to execute, it is modified to set the execution start time as the
    // current timestamp and the execution end time as the sum of the current timestamp and the remaining burst time.
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;

    return next_process;
}

// Round-Robin Scheduler
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt,
                                     struct PCB current_process, struct PCB new_process, int timestamp,
                                     int time_quantum) {
    if (pcb_is_null(current_process)) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + MIN(time_quantum, new_process.total_bursttime);
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }

    if (current_process.remaining_bursttime <= new_process.total_bursttime) {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt)++;
        return current_process;
    }
}

// Round-Robin Scheduler
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int
*queue_cnt, int timestamp, int time_quantum) {
    struct PCB next_process = init_pcb(0, 0, 0, 0, 0, 0, 0);

    if (*queue_cnt == 0) {
        return next_process;
    }

    int earliest_ats = 999;
    int earliest_ats_index = 0;

    for (int i = 0; i < *queue_cnt; i++) {
        if (ready_queue[i].arrival_timestamp < earliest_ats) {
            earliest_ats = ready_queue[i].arrival_timestamp;
            earliest_ats_index = i;
        }
    }

    next_process = ready_queue[earliest_ats_index];

    for (int i = earliest_ats_index; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }

    (*queue_cnt)--;

    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + MIN(time_quantum, next_process.remaining_bursttime);

    return next_process;
}