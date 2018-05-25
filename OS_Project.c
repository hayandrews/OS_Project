//Hayley Andrews, Jessica Morris
//TODO: Ln 141, Ln 182->45, Ln 336
//maybe change parse_Q&L to not accept out of range numbers for commands
//handle device requests that are too big for system, wait queue
//popping from hold queues to ready queue is untested
//weighted turnharound is turnaround/run_time

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "node.h"
#include "file.h"
#include "Json-Output.h"

#define STRING_SIZE 1024

char s_input[STRING_SIZE];	/* input file name */
char cur_line[STRING_SIZE]; /* current line in file*/
int cur_length;				/* length for above string*/
int i;						/* for use in for loops*/
bool flag = true;			/* too tired to explain*/

/* hold queues are for jobs that are waiting for system memory */
extern struct Job *hold_queue_1;
extern struct Job *hold_queue_2;

/* queue for jobs that the system can run. Jobs do not stay here long.*/
extern struct Job *submit_queue; 

/* queue for jobs that hold system memory. This queue's jobs cycle through CPU*/
extern struct Job *ready_queue; 

/* queue for jobs that have been completed*/
extern struct Job *complete_queue;

/* Queue for jobs that hold system memory and are waiting to hold devices*/
extern struct Job *wait_queue;	

struct Job *CPU;

int start_time = -1;		/* assumes start time will start at 0 or later*/
int main_memory = -1;
int avail_mem = -1;
int serial_devices = -1;
int time_quantum = -1;
int cur_time = -1;
int avail_dev = -1;

/* called whenever a D command is processed. */
void print_state(int time) {
    printf("\n");
    printf("PRINTING CURRENT STATES AT TIME %d\n", time);
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
            printf("Job %d is complete, turnaround time is: %d, weighted turnaround time is: %d\n", tmp->job_num, (tmp->completion_time - tmp->arrive_time), ((tmp->completion_time - tmp->arrive_time)/cur_time));
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
    
    printf("PRINTING QUEUE CONTENTS AT TIME %d\n", time);
    if (hold_queue_1){
        struct Job *tmp = hold_queue_1;
        printf("Hold Queue 1 Holds: ");
        while (tmp != NULL){
            printf("Job %d, ", tmp->job_num);
            tmp = tmp->next;
        }
        printf("\n");
    } else {
        printf("Hold Queue 1 is Empty\n");
    }
    if (hold_queue_2 != NULL){
        struct Job *tmp = hold_queue_2;
        printf("Hold Queue 2 Holds: ");
        while (tmp != NULL){
            printf("Job %d, ", tmp->job_num);
            tmp = tmp->next;
        }
        printf("\n");
    } else {
        printf("Hold Queue 2 is Empty\n");
    }
    
    if (submit_queue != NULL){
        struct Job *tmp = submit_queue;
        printf("Submit Queue Holds: ");
        while (tmp != NULL){
            printf("Job %d, ", tmp->job_num);
            tmp = tmp->next;
        }
        printf("\n");
    } else {
        printf("Submit Queue is Empty\n");
    }
    
    if (ready_queue != NULL){
        struct Job *tmp = ready_queue;
        printf("Ready Queue Holds: ");
        while (tmp != NULL){
            printf("Job %d, ", tmp->job_num);
            tmp = tmp->next;
        }
        printf("\n");
    } else {
        printf("Ready Queue is Empty\n");
    }
    
    if (complete_queue != NULL){
        struct Job *tmp = complete_queue;
        printf("Completed Queue Holds: ");
        while (tmp != NULL){
            printf("Job %d, ", tmp->job_num);
            tmp = tmp->next;
        }
        printf("\n");
    } else {
        printf("Completed Queue is Empty\n");
    }
    
    if (wait_queue != NULL){
        struct Job *tmp = wait_queue;
        printf("Waut Queue Holds: ");
        while (tmp != NULL){
            printf("Job %d, ", tmp->job_num);
            tmp = tmp->next;
        }
        printf("\n");
    } else {
        printf("Wait Queue is Empty\n");
    }
    printf("\n");
    
	/*
	(a) A list of each job that has entered the system; for each job, print the
	state of the job (e.g. running on the CPU, in the Hold Queue, or finished 
	at time 11), the remaining service time for unfinished jobs and the 
	turnaround time and weighted turnaround time for finished jobs.

	(b) The contents of each queue.

	Note: As long as the display is readable and has the required information, that is fine.
	*/
}

/* called by main before CPU starts to parse initial C command
- initializes start_time, main_memory, serial_devices, time_quantum to input
- sets avail_mem, cur_time, avail_dev*/
void parse_C(char * command) {
/*string parsing found at 
https://stackoverflow.com/questions/4513316/split-string-in-c-every-white-space
*/
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

/* called by main in internal event when cur_line[0] = A
- checks that the job does not require more than system has
- puts job in submit queue*/
void parse_A(char * command, int time) {
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
		//printf("job requires %d memory, but system only has %d memory.\n", mem_r, main_memory);
	}
	else if (dev_r > serial_devices) {
		//printf("job requires %d devices, but system only has %d devices.\n", dev_r, serial_devices);
	}
	else {
		insert_sub(arr_t, job_n, mem_r, dev_r, run_t, queue_p, time);
	}

	cur_line[0] = 'R';
}

/* called by main in internal event when cur_line[0] = Q
- holds line until arrival time specified matches system time
- checks CPU to make sure job requesting devices is in CPU
   - dumps if it doesn't
   - adds to job in CPU if it is*/
void parse_Q(char * command, int time) {
	/*need to copy string each time, since strtok eats away at the string*/
	char com[STRING_SIZE];
	for (int j = 0; j < cur_length; j++) {
		com[j] = command[j];
	}
	int arr_t, job_n, dev_r;
	char * pch;
	pch = strtok(com, " =QJD");
	arr_t = atoi(pch);
	pch = strtok(NULL, " =QJD");
	job_n = atoi(pch);
	pch = strtok(NULL, " =QJD");
	dev_r = atoi(pch);
	if (time < arr_t) return; //too soon, don't process
	if (CPU && CPU->job_num == job_n && (CPU->dev_req - CPU->dev_owned) >= dev_r) {
		CPU->dev_owned += dev_r;	//device doesn't technically own yet
		if (dev_r > avail_dev) {
			insertFIFO2(&wait_queue, CPU);
			CPU = NULL;
		} else 
			avail_dev -= dev_r;
	}
	else {
		//printf("Job requesting devices is not on the CPU.\n");
	}
	cur_line[0] = 'R';
}

/* called by main in internal event when cur_line[0] = L
- holds line until arrival time specified matches system time
- checks CPU to make sure job releasing devices is in CPU
   - dumps if it doesn't
   - takes from job in CPU if it is*/
void parse_L(char * command, int time) {
	/*need to copy string each time, since strtok eats away at the string*/
	char com[STRING_SIZE];
	for (int j = 0; j < cur_length; j++) {
		com[j] = command[j];
	}
	int arr_t, job_n, dev_r;
	char * pch;
	pch = strtok(com, " =LJD");
	arr_t = atoi(pch);
	pch = strtok(NULL, " =LJD");
	job_n = atoi(pch);
	pch = strtok(NULL, " =LJD");
	dev_r = atoi(pch);
	if (time < arr_t) return; //too soon, don't process
	if (CPU && CPU->job_num == job_n) {
		if (CPU->dev_owned < dev_r) {
			avail_dev += CPU->dev_owned;
			CPU->dev_owned = 0;
		}
		else {
			CPU->dev_owned -= dev_r;
			avail_dev += dev_r;
		}
	}
	else {
		//printf("Job releasing devices is not on the CPU.\n");
	}
	cur_line[0] = 'R'; //release processed, ready for next line
}

/* called in main during the external event
- holds line until arrival time <= system time
   - unless arrival time is 9999
- outputs a Json file and prints the state*/
void parse_D(char * command, int time) {
	int arr_t;
	char * pch;
	pch = strtok(command, " =D");
	arr_t = atoi(pch);
	//printf("\tcomparing %d >= %d\n", time, arr_t);
	if (time >= arr_t) {
		outputJSON(s_input, time);
		print_state(time);
		cur_line[0] = 'R';
	}
	else if (arr_t == 9999) {
		cur_line[0] = 'R';
	}
}

int main(void){
	printf("Please input filename:\n");
	scanf("%s", s_input);
	open_file(s_input);
	cur_length = next_line(cur_line); //loading C command
	parse_C(cur_line); // parsing initial command
	for (;;) {
		//printf("\n\n\nNEW ITERATION\n");
		//printf("STARTING INTERNAL\n");

		/*internal event*/
		if (ready_queue) CPU = pop(&ready_queue);
		//else printf("ready_queue empty.\n");
		/*we make the assumption that for instance if cur_time = 1, and i = 3, 
		the work being done is for time unit 4*/
		for (i = 0; i < time_quantum && flag; i++) {
			if (cur_line[0] == 'R') cur_length = next_line(cur_line);
			//printf("\tin iteration %d, passing in %d cur_line reads in as %s", i, cur_time + i, cur_line);
			if (CPU) {
				//printf("\tCPU: ");
				//printList(CPU);
			}

			if (CPU) {
				CPU->time_left--;
				if (CPU->time_left <= 0) {
					insert_fin(CPU);
					CPU->completion_time = cur_time + i + 1;
					flag = false;
					CPU = NULL;
				}
			}

			//else printf("CPU empty.\n");
			if (cur_line[0] == 'Q') {
				parse_Q(cur_line, cur_time + i);
				if (cur_line[0] == 'R') {//if command Q ran, it interrupted
					flag = false;
				}
			}
			else if (cur_line[0] == 'L') {
				parse_L(cur_line, cur_time + i);
				if (cur_line[0] == 'R') {//if command L ran, it interrupted
					flag = false;
				}
			}
			else if (cur_line[0] == 'A'){
				parse_A(cur_line, cur_time+i);		//store in submit queue till it arrives
			}
			else if (cur_line[0] == 'D') {
				//printf("\tcur_line = D\n");
				parse_D(cur_line, cur_time + i);
			}
			
		}
		flag = true;
		if (CPU)
			insertFIFO2(&ready_queue, CPU);
		CPU = NULL;
		cur_time += i;


		//printf("DONE INTERNAL\n");
		//printf("STARTING EXTERNAL\n");

		/*external event*/
		while (submit_queue && cur_time >= submit_queue->arrive_time) {
			pop_sub();
		}
		while (wait_queue && wait_queue->dev_owned <= avail_dev) {
			avail_dev -= wait_queue->dev_owned;
			insertFIFO2(&ready_queue, pop(&wait_queue));
		}
		while (hold_queue_1 && hold_queue_1->mem_req <= avail_mem) {
			insertFIFO2(&ready_queue, pop(&hold_queue_1));
		}
		while (hold_queue_2 && hold_queue_2->mem_req <= avail_mem) {
			insertFIFO2(&ready_queue, pop(&hold_queue_2));
		}
		//printf("DONE EXTERNAL\n");
		if (cur_line[0] != 'R') {
			//printf("current line isn't done yet.\n");
		}
		else if (ready_queue != NULL) {
			//printf("ready queue still has jobs.\n");
		}
		else if (wait_queue != NULL) {
			//printf("wait queue still has processes\n");
		}
		else if (hold_queue_1 != NULL) {
			//printf("hold queue 1 still has jobs.\n");
		}
		else if (hold_queue_2 != NULL) {
			//printf("hold queue 2 still has jobs.\n");
		}
		else if (submit_queue) {
			//printf("submit queue still has jobs.\n");
		}
		else {
			break;
		}

	}
	close_file();

	/*The system turnaround time and system weighted turnaround only at the 
	last display. Assume that the input file has a “D 9999” command at the end,
	so that you dump the final state of the system.
	*/

}
