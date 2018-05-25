//
//  printingStates.c
//  
//
//  Created by Hayley Andrews on 5/23/18.
//

#include <stdio.h>

void print_state() {
    if (CPU){
        printf("Job %d is in the CPU, remaining service time is: %d\n", CPU->job_num, CPU->time_left);
    }
    if (hold_queue_1 != NULL){
        struct Job *tmp = hold_queue_1;
        while (tmp != NULL){
            printf("Job %d is in Hold Queue 1, remaining service time is: %d\n", tmp->job_num, tmp->time_left);
            tmp = tmp->next;
        }
    }
    if (hold_queue_2 != NULL){
        struct Job *tmp = hold_queue_2;
        while (tmp != NULL){
            printf("Job %d is in Hold Queue 2, remaining service time is: %d\n", tmp->job_num, tmp->time_left);
            tmp = tmp->next;
        }
    }
    if (submit_queue != NULL){
        struct Job *tmp = submit_queue;
        while (tmp != NULL){
            printf("Job %d is in submit queue, remaining service time is: %d\n", tmp->job_num, tmp->time_left);
            tmp = tmp->next;
        }
    }
    if (ready_queue != NULL){
        struct Job *tmp = ready_queue;
        while (tmp != NULL){
            printf("Job %d is READY, remaining service time is: %d\n", tmp->job_num, tmp->time_left);
            tmp = tmp->next;
        }
    }
    if (complete_queue != NULL){
        struct Job *tmp = complete_queue;
        int jobcount = 0;
        while (tmp != NULL){
            jobcount = jobcount + 1;
            printf("Job %d is complete, turnaround time is: %d, weighted turnaround time is: %d\n", (tmp->completion_time - tmp->arrive_time), tmp->completion_time);
            tmp = tmp->next;
        }
    }
    if (wait_queue != NULL){
        struct Job *tmp = wait_queue;
        while (tmp != NULL){
            printf("Job %d is in the wait queue, remaining service time is: %d\n", tmp->job_num, tmp->time_left);
            tmp = tmp->next;
        }
    }
    /*
     (a) A list of each job that has entered the system; for each job, print the
     state of the job (e.g. running on the CPU, in the Hold Queue, or finished
     at time 11), the remaining service time for unfinished jobs and the
     turnaround time and weighted turnaround time for finished jobs.
     
     (b) The contents of each queue.
     
     Note: As long as the display is readable and has the required information, that is fine.
     */
}
