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


void outputJSON(){
    output = fopen("JSONtest.json", "w");
    fputs("{",output);
    fputs("\"readyq\": ",output);
    fputs("[1, 5], ",output);
    fputs("\"current_time\": ",output);
    fputs("10, ",output);
    fputs("\"total_memory\": ",output);
    fputs("200, ",output);
    fputs("\"available_memory\": ",output);
    fputs("0, ",output);
    fputs("\"total_devices\": ",output);
    fputs("12, ",output);
    fputs("\"running\": ",output);
    fputs("2, ",output);
    fputs("\"submitq\": ",output);
    fputs("[], ",output);
    fputs("\"holdq2\": ",output);
    fputs("[], ",output);
    fputs("\"job\": ",output);
    fputs("[{\"arrival_time\": 4, \"devices_allocated\": 0, \"id\": 2, \"remaining_time\": 9}, {\"arrival_time\": 3, \"devices_allocated\": 0, \"id\": 1, \"remaining_time\": 6}, {\"arrival_time\": 8, \"devices_allocated\": 0, \"id\": 5, \"remaining_time\": 4}, {\"arrival_time\": 5, \"id\": 3, \"remaining_time\": 4}], ",output);
    fputs("\"holdq1\": ",output);
    fputs("[3], ",output);
    fputs("\"available_devices\": ",output);
    fputs("12, ",output);
    fputs("\"quantum\": ",output);
    fputs("4, ",output);
    fputs("\"completeq\": ",output);
    fputs("[], ",output);
    fputs("\"waitq\": ",output);
    fputs("[]",output);
    fputs("}",output);
    fclose(output);
}

