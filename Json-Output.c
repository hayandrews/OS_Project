//
//  Json-Output.c
//  
//
//  Created by Hayley Andrews on 5/19/18.
//

#include <stdio.h>
#include <stdlib.h>


FILE *output;
extern struct Job *hold_queue_1;
extern struct Job *hold_queue_2;
extern struct Job *submit_queue;
extern struct Job *ready_queue;
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

