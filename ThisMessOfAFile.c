//
//  ThisMessOfAFile.c
//  
//
//  Created by Hayley Andrews on 5/19/18.
//

#include <stdio.h>
#include <stdlib.h>

#define STRING_SIZE 1024
FILE *input;
FILE *output;

struct Job
{
    /*
     A 10 J=1 M=5 S=4 R=3 P=1
     The example above states that job number 1 with priority 1 arrives at time 10, requires 5 units
     of main memory, holds no more than 4 devices at any point during execution(max demand), and
     runs for 3 units of time
     */
    int arrive_time;
    int job_num;
    int mem_req;
    int dev_req;
    int run_time;
    int queue_priority;
    struct Job *next;
    int completion_time;
    int dev_owned;
    int time_left;
};

struct Job *hold_queue_1 = NULL;
struct Job *hold_queue_2 = NULL;
struct Job *submit_queue = NULL;
struct Job *ready_queue;
struct Job *complete_queue;
struct Job *wait_queue;

int sys_configs[4]; /*index 0 = start time, 1 = main memory,
                     2 = serial devices, 3 = time quantum*/
int start_time = -1;
int main_memory = -1;
int avail_mem = -1;
int serial_devices = -1;
int time_quantum = -1;
int cur_time = -1;
int avail_dev = -1;

char s_input[STRING_SIZE];    /* input file name */
char cur_line[STRING_SIZE]; /* current line in file*/
int cur_length;                /* length for above string*/


/*
 --------------------------------------------------------------------------------
 NODE Functions
 --------------------------------------------------------------------------------
 */

/*used for inserting into hold_queue_2 only*/
void insert_hold_2(struct Job * node){
    //Empty queue make new_node the head
    if (hold_queue_1 == NULL) {
        hold_queue_1 = node;
        //Else if less than head, make new head
    } else if (node->run_time < hold_queue_1->run_time){
        node->next = hold_queue_1;
        hold_queue_1 = node;
        //Else iterate through the list
    } else{
        struct Job *pointer = hold_queue_1;
        while (pointer->next != NULL) {
            if (node->run_time < pointer->next->run_time) {
                break;
            }
            else {
                pointer = pointer->next;
            }
        }
        //If between two nodes, the node next is same as pointers next
        //Then pointers next is then node
        if (pointer->next != NULL) {
            node->next = pointer->next;
            pointer->next = node;
        }
        else { //pointer->next is null thus just add node as next
            pointer->next = node;
        }
    }
}

/*used for inserting into first-in-first-out queues:
 - hold_queue_2
 - ready_queue
 - wait_queue
 - complete_queue*/
void insertFIFO2(struct Job **queue, struct Job * node) {
    if (*queue == NULL) {
        *queue = node;
    }
    else {
        struct Job * pointer = *queue;
        while (pointer->next != NULL) {
            pointer = pointer->next;
        }
        pointer->next = node;
    }
}

/*Basically acts as constructor for jobs. Only place space should be malloced
 for a job, initializes all the struct fields. Jobs automatically placed in
 submit_queue.*/
void insert_sub(int arr_t, int job_n, int mem_r, int dev_r, int run_t, int queue_p) {
    struct Job *new_node = (struct Job*)malloc(sizeof(struct Job));
    new_node->arrive_time = arr_t;
    new_node->job_num = job_n;
    new_node->mem_req = mem_r;
    new_node->dev_req = dev_r;
    new_node->run_time = run_t;
    new_node->queue_priority = queue_p;
    new_node->next = NULL;
    new_node->completion_time = 0;
    new_node->dev_owned = 0;
    new_node->time_left = 0;
    //If empty insert at head
    if (submit_queue == NULL) {
        submit_queue = new_node;
    }
    else {
        //Other wise add to the end of the list
        struct Job *pointer = submit_queue;
        while (pointer->next != NULL) {
            pointer = pointer->next;
        }
        pointer->next = new_node;
    }
}

struct Job * pop(struct Job **queue) {
    struct Job *ret = *queue;
    if (ret) {
        *queue = ret->next;
    }
    ret->next = NULL;
    return ret;
}

void printList(struct Job *queue){
    struct Job *pointer = queue;
    if (pointer == NULL){
        printf("empty\n");
    } else{
        while(pointer != NULL){
            printf("{%d,%d,%d,%d,%d,%d} --> ",pointer->arrive_time, pointer->job_num,
                   pointer->mem_req, pointer->dev_req, pointer->run_time,
                   pointer->queue_priority);
            pointer = pointer->next;
        }
        printf("Null \n");
    }
    
}

void insert_ready(struct Job * node) {
    avail_mem -= node->mem_req;
    avail_dev -= node->dev_owned;
    insertFIFO2(&ready_queue, node);
}

void pop_sub() {
    struct Job * cur_job = pop(&submit_queue);
    /*in graph given there isn't an arrow from submit_queue to ready_queue but
     it also says:
     3. If there is enough main memory for the job, then a process is created
     for the job, the required main memory is allocated to the process, and the
     process is put in the Ready Queue.
     Thus we chose to move things directly to ready when possible.*/
    /*if (avail_mem >= cur_job->mem_req) {
     insert_ready(cur_job);
     }
     else*/ if (cur_job->queue_priority == 1) {
         insert_hold_2(cur_job);
     }
    else if (cur_job->queue_priority == 2) {
        insertFIFO2(&hold_queue_2, cur_job);
    }
    else {
        perror("Job's priority does not match a hold queue.");
        exit(1);
    }
    
}

/*used when filling ready queue and when process releases devices*/
void pop_wait() {
    struct Job * cur_job = wait_queue;
    while (cur_job != NULL && avail_dev >= cur_job->dev_owned) {
        cur_job = cur_job->next;
        insert_ready(pop(&wait_queue));
    }
}

void fill_ready() {
    pop_wait();
    struct Job * cur_job = hold_queue_1;
    while (cur_job != NULL && avail_mem >= cur_job->mem_req) {
        cur_job = cur_job->next;
        insert_ready(pop(&hold_queue_1));
    }
    cur_job = hold_queue_2;
    while (cur_job != NULL && avail_mem >= cur_job->mem_req) {
        cur_job = cur_job->next;
        insert_ready(pop(&hold_queue_2));
    }
}

void insert_fin(struct Job * node) {
    avail_mem += node->mem_req;
    avail_dev += node->dev_owned;
    insertFIFO2(&complete_queue, node);
    fill_ready();
}
/*
 --------------------------------------------------------------------------------
 File Functions
 --------------------------------------------------------------------------------
 */

int next_line(char * buffer) { /*uses buffer var to return value and returns
                                an additional int that is the length of the
                                string */
    if (input == NULL) {
        perror("file not opened");
        exit(1);
    }
    if (fgets(buffer, STRING_SIZE, input) != NULL) {
        return strlen(buffer);
    }
    else {
        return -1;
    }
}

void close_file() {
    fclose(input);
    printf("Close successful.\n");
}

void open_file(char * filename) {
    input = fopen(filename, "r");
    if (input == NULL) {
        perror("Invalid filename.\n");
        close_file();
        exit(1);
    }
    else {
        printf("Successfully opened %s\n", filename);
    }
}

/*
 --------------------------------------------------------------------------------
 JSON Output Functions
 --------------------------------------------------------------------------------
 */
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

/*
 --------------------------------------------------------------------------------
 Main Functions
 --------------------------------------------------------------------------------
 */


struct Job *safeJob(struct Job **queue){
    struct Job *cur_job = *queue;
    while(cur_job != NULL){
        if (cur_job->mem_req < avail_mem && cur_job->dev_req < avail_dev){
            break;
        }
        cur_job = cur_job->next;
    }
    return cur_job;
}

void inc_com() {
    perror("file has an incorrect command.");
    exit(1);
}

void parse_C(char * command) {
    /*string parsing found at
     https://stackoverflow.com/questions/4513316/split-string-in-c-every-white-space
     */
    if (start_time > -1) { /* If start time has already been initialized, there
                            shouldn't be another C*/
        inc_com();
    }
    
    char * pch;
    pch = strtok(command, " =CMSQ");
    start_time = atoi(pch);
    pch = strtok(NULL, " =CMSQ");
    main_memory = atoi(pch);
    pch = strtok(NULL, " =CMSQ");
    serial_devices = atoi(pch);
    pch = strtok(NULL, " =CMSQ");
    time_quantum = atoi(pch);
    
    avail_mem = main_memory;
    cur_time = start_time;
    avail_dev = serial_devices;
    
    cur_line[0] = 'R';
}

void parse_A(char * command) {
    int arr_t, job_n, mem_r, dev_r, run_t, queue_p;
    char * pch;
    pch = strtok(command, " =AJMSRP");
    arr_t = atoi(pch);
    pch = strtok(NULL, " =AJMSRP");
    job_n = atoi(pch);
    pch = strtok(NULL, " =AJMSRP");
    mem_r = atoi(pch);
    pch = strtok(NULL, " =AJMSRP");
    dev_r = atoi(pch);
    pch = strtok(NULL, " =AJMSRP");
    run_t = atoi(pch);
    pch = strtok(NULL, " =AJMSRP");
    queue_p = atoi(pch);
    
    if (mem_r > main_memory) {
        printf("job requires %d memory, but system only has %d memory.\n", mem_r, main_memory);
    }
    else if (dev_r > serial_devices) {
        printf("job requires %d devices, but system only has %d devices.\n", dev_r, serial_devices);
    }
    else {
        insert_sub(arr_t, job_n, mem_r, dev_r, run_t, queue_p);
    }
    
    cur_line[0] = 'R';
}

void parse_Q(char * command) {
    printf("%s", command);
    cur_line[0] = 'R';
}

void parse_L(char * command) {
    printf("%s", command);
    cur_line[0] = 'R';
}

void parse_D(char * command) {
    printf("%s", command);
    //outputJSON();
    cur_line[0] = 'R';
}

void parse_line(char * command) {
    /*we assume that the 'C' case happens exactly once and it happens at the
     very first line */
    switch (command[0]) {
        case 'C':
        printf("command is a System Configuration. \n");
        parse_C(command);
        break;
        case 'A':
        printf("command is a Job arrival. \n");
        parse_A(command);
        break;
        case 'Q':
        printf("command is a Request for devices. \n");
        parse_Q(command);
        break;
        case 'L':
        printf("command is a Release for devices. \n");
        parse_L(command);
        break;
        case 'D':
        parse_D(command);
        printf("command is a Display request. \n");
        break;
        default:
        inc_com();
    }
}

void print_queues() {
    printf("\nSubmit_queue\n");
    printList(submit_queue);
    printf("ready_queue\n");
    printList(ready_queue);
    printf("hold_queue_1\n");
    printList(hold_queue_1);
    printf("hold_queue_2\n");
    printList(hold_queue_2);
    printf("wait_queue\n");
    printList(wait_queue);
    printf("complete_queue\n");
    printList(complete_queue);
}

int main(void){
    printf("Please input filename:\n");
    scanf("%s", s_input);
    open_file(s_input);
    cur_length = next_line(cur_line);
    for (;;) {
        if (cur_line[0] == 'R') {
            cur_length = next_line(cur_line);
        }
        if (cur_length >= 0) {
            parse_line(cur_line);
        }
        else break;
    }
    printList(submit_queue);
    close_file();
    while (submit_queue != NULL) {
        pop_sub();
    }
    insert_ready(pop(&hold_queue_1));
    print_queues();
    while (ready_queue != NULL) {
        insert_fin(pop(&ready_queue));
        print_queues();
    }
    //outputJSON();
}
