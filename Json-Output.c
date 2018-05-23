//
//  Json-Output.c
//  
//
//  Created by Hayley Andrews on 5/19/18.
//

#include <stdio.h>
#include <stdlib.h>
#include "Json-Output.h"
#include "node.h"


FILE *output;
extern struct Job *hold_queue_1;
extern struct Job *hold_queue_2;
extern struct Job *submit_queue;
extern struct Job *ready_queue;
extern struct Job *complete_queue;
extern struct Job *wait_queue;
extern int main_memory;
extern int avail_mem;
extern int serial_devices;
extern int time_quantum;
extern int cur_time;
extern int avail_dev;


void printQueueToFile(struct Job *queue){
    if (queue == NULL) {
        fputs("[], ",output);
    } else{
        struct Job *pointer = queue;
        fputs("[",output);
        while (pointer != NULL){
            if (pointer->next != NULL){
                fprintf(output, "%d, ", pointer->job_num);
            } else {
                fprintf(output, "%d], ", pointer->job_num);
            }
            pointer = pointer->next;
            
        }
    }
}

//TODO Need to figure out what I actually need to print for following queues
//Submit_queue, the hold queues, ready queue, and wait queue
void printQueueJobCheck(){
    int jobcount = 0;
    fputs("[",output);
    //Need to print currently running job here
    /*
     if (running != NULL){
        jobcount = jobcount + 1;
        fprintf(output, "{\"arrival_time\": %d, \"devices_allocated\": %d, \"id\": %d, \"remaining_time\": %d}", tmp->arrive_time, tmp->dev_owned, tmp->job_num, tmp->time_left);
     }
     */
    if (submit_queue != NULL) {
        struct Job *tmp = submit_queue;
        while (tmp != NULL){
            if (jobcount != 0){
                fputs(", ",output);
            }
            fprintf(output, "{\"arrival_time\": %d, \"devices_allocated\": %d, \"id\": %d, \"remaining_time\": %d}", tmp->arrive_time, tmp->dev_owned, tmp->job_num, tmp->time_left);
            jobcount = jobcount + 1;
        }
    }
    if (hold_queue_1 != NULL) {
        struct Job *tmp = hold_queue_1;
        while (tmp != NULL){
            if (jobcount != 0){
                fputs(", ",output);
            }
            fprintf(output, "{\"arrival_time\": %d, \"devices_allocated\": %d, \"id\": %d, \"remaining_time\": %d}", tmp->arrive_time, tmp->dev_owned, tmp->job_num, tmp->time_left);
            jobcount = jobcount + 1;
        }
    }
    if (hold_queue_2 != NULL) {
        struct Job *tmp = hold_queue_2;
        while (tmp != NULL){
            if (jobcount != 0){
                fputs(", ",output);
            }
            fprintf(output, "{\"arrival_time\": %d, \"devices_allocated\": %d, \"id\": %d, \"remaining_time\": %d}", tmp->arrive_time, tmp->dev_owned, tmp->job_num, tmp->time_left);
            jobcount = jobcount + 1;
        }
    }
    if (ready_queue != NULL) {
        struct Job *tmp = ready_queue;
        while (tmp != NULL){
            if (jobcount != 0){
                fputs(", ",output);
            }
            fprintf(output, "{\"arrival_time\": %d, \"devices_allocated\": %d, \"id\": %d, \"remaining_time\": %d}", tmp->arrive_time, tmp->dev_owned, tmp->job_num, tmp->time_left);
            jobcount = jobcount + 1;
        }
    }
    if (complete_queue != NULL) {
        struct Job *tmp = complete_queue;
        while (tmp != NULL){
            if (jobcount != 0){
                fputs(", ",output);
            }
            fprintf(output, "{\"arrival_time\": %d, \"id\": %d, \"remaining_time\": %d, \"completion_time\": %d}", tmp->arrive_time, tmp->job_num, tmp->time_left, tmp->completion_time);
            jobcount = jobcount + 1;
        }
    }
    if (wait_queue != NULL) {
        struct Job *tmp = wait_queue;
        while (tmp != NULL){
            if (jobcount != 0){
                fputs(", ",output);
            }
            fprintf(output, "{\"arrival_time\": %d, \"devices_allocated\": %d, \"id\": %d, \"remaining_time\": %d}", tmp->arrive_time, tmp->dev_owned, tmp->job_num, tmp->time_left);
            jobcount = jobcount + 1;
        }
    }
    fputs("],",output);
}

void outputJSON(){
    output = fopen("JSONtest.json", "w");
    fputs("{",output);
    
    fputs("\"readyq\": ",output);
    printQueueToFile(ready_queue);
    
    fputs("\"current_time\": ",output);
    fprintf(output, "%d, ", cur_time);
    
    fputs("\"total_memory\": ",output);
    fprintf(output, "%d, ", main_memory);
    
    fputs("\"available_memory\": ",output);
    fprintf(output, "%d, ", avail_mem);
    
    fputs("\"total_devices\": ",output);
    fprintf(output, "%d, ", serial_devices);
    
    //TODO - Currently Running
    fputs("\"running\": ",output);
    fputs("2, ",output);
    
    fputs("\"submitq\": ",output);
    printQueueToFile(submit_queue);
    //fputs("[], ",output);
    
    fputs("\"holdq2\": ",output);
    printQueueToFile(hold_queue_2);
    //fputs("[], ",output);
    
    //TODO - All Jobs
    fputs("\"job\": ",output);
    fputs("[{\"arrival_time\": 4, \"devices_allocated\": 0, \"id\": 2, \"remaining_time\": 9}, {\"arrival_time\": 3, \"devices_allocated\": 0, \"id\": 1, \"remaining_time\": 6}, {\"arrival_time\": 8, \"devices_allocated\": 0, \"id\": 5, \"remaining_time\": 4}, {\"arrival_time\": 5, \"id\": 3, \"remaining_time\": 4}], ",output);
    
    fputs("\"holdq1\": ",output);
    printQueueToFile(hold_queue_1);
    //fputs("[3], ",output);
    
    fputs("\"available_devices\": ",output);
    fprintf(output, "%d, ", avail_dev);
    
    fputs("\"quantum\": ",output);
    fprintf(output, "%d, ", time_quantum);
    
    //TODO - Completed Jobs
    fputs("\"completeq\": ",output);
    fputs("[], ",output);
    
    fputs("\"waitq\": ",output);
    fputs("[]",output);
    fputs("}",output);
    fclose(output);
}

