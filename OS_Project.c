//Hayley Andrews, Jessica Morris

#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "file.h"
#include "Json-Output.h"

#define STRING_SIZE 1024

char s_input[STRING_SIZE];	/* input file name */
char cur_line[STRING_SIZE]; /* current line in file*/
int cur_length;				/* length for above string*/
extern struct Job *hold_queue_1;
extern struct Job *hold_queue_2;
extern struct Job *submit_queue;
extern struct Job *ready_queue;
extern struct Job *complete_queue;
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
void print_state() {
	/*
	(a) A list of each job that has entered the system; for each job, print the
	state of the job (e.g. running on the CPU, in the Hold Queue, or finished 
	at time 11), the remaining service time for unfinished jobs and the 
	turnaround time and weighted turnaround time for finished jobs.

	(b) The contents of each queue.

	Note: As long as the display is readable and has the required information, that is fine.
	*/
}

/* called by parse_line when a C command is read from file.
- if initialization has already happened, does nothing.
- initializes start_time, main_memory, serial_devices, time_quantum to input
- sets avail_mem, cur_time, avail_dev*/
void parse_C(char * command) {
/*string parsing found at 
https://stackoverflow.com/questions/4513316/split-string-in-c-every-white-space
*/
	if (start_time > -1) { /* Checks for a second C command*/
		return;
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

/* called by parse_line when an A command is read from file.
- checks that the job does not require more than system has
- puts job in submit queue*/
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

/* called by parse_line when a Q command is read from file.
- holds line until arrival time specified matches system time
- checks CPU to make sure job requesting devices is in CPU
   - dumps if it doesn't
   - adds to job in CPU if it is*/
void parse_Q(char * command) {
	printf("%s", command);
	int arr_t, job_n, dev_r;
	char * pch;
	pch = strtok(command, " =QJD");
	arr_t = atoi(pch);
	if (cur_time >= arr_t) {
		pch = strtok(NULL, " =QJD");
		job_n = atoi(pch);
		if (CPU->job_num == job_n) {
			pch = strtok(NULL, " =QJD");
			dev_r = atoi(pch);
			CPU->dev_owned += dev_r;
		}
		else {
			printf("Job requesting devices is not on the CPU.\n");
		}
		cur_line[0] = 'R';
	}
}

/* called by parse_line when an L command is read from file.
- holds line until arrival time specified matches system time
- checks CPU to make sure job releasing devices is in CPU
   - dumps if it doesn't
   - takes from job in CPU if it is*/
void parse_L(char * command) {
	printf("%s", command);
	int arr_t, job_n, dev_r;
	char * pch;
	pch = strtok(command, " =LJD");
	arr_t = atoi(pch);
	if (cur_time >= arr_t) {
		pch = strtok(NULL, " =LJD");
		job_n = atoi(pch);
		if (CPU->job_num == job_n) {
			pch = strtok(NULL, " =LJD");
			dev_r = atoi(pch);
			CPU->dev_owned -= dev_r;
		}
		cur_line[0] = 'R';
	}
}

/* called by parse_line when a D command is read from file.
- holds line until arrival time specified matches system time
- outputs a Json file and prints the state*/
void parse_D(char * command) {
	printf("%s", command);
	int arr_t;
	char * pch;
	pch = strtok(command, " =D");
	arr_t = atoi(pch);
	if (cur_time >= arr_t) {
		//outputJSON();
		print_state();
		cur_line[0] = 'R';
	}
}

/* checks if command passed in is NULL. If not, it calls the various parse 
functions. Incorrect letters are also handled.*/
void parse_line(char * command) {
	if (!command) {
		return;
	}
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
	}
}

/*DELETE BEFORE SUBMISSION*/
void print_queues() {
	printf("\tPRINTING QUEUES\n");
	if (submit_queue != NULL) {
		printf("\tSubmit_queue: ");
		printList(submit_queue);
	}
	if (ready_queue != NULL) {
		printf("\tready_queue: ");
		printList(ready_queue);
	}
	if (hold_queue_1 != NULL) {
		printf("\thold_queue_1: ");
		printList(hold_queue_1);
	}
	if (hold_queue_2 != NULL) {
		printf("\thold_queue_2: ");
		printList(hold_queue_2);
	}
	if (wait_queue != NULL) {
		printf("\twait_queue: ");
		printList(wait_queue);
	}
	if (complete_queue != NULL) {
		printf("\tcomplete_queue: ");
		printList(complete_queue);
	}
	fflush(stdout);
}

/*DELETE BEFORE SUBMISSION*/
void print_sys_vars() {
	printf("\tmain_mem: %d", main_memory);
	printf("\tavail_mem: %d", avail_mem);
	printf("\tserial_dev: %d", serial_devices);
	printf("\tquantum: %d", time_quantum);
	printf("\tcur_time: %d", cur_time);
	printf("\tavail_dev: %d\n", avail_dev);
}

int main(void){
	printf("Please input filename:\n");
	scanf("%s", s_input);
	open_file(s_input);
	cur_line[0] = 'R';
	for (;;) {
		printf("\n\n\nNEW ITERATION\n");
		print_sys_vars();
		print_queues();
		printf("STARTING INTERNAL\n");

		/*internal event*/
		if (ready_queue != NULL) {
			printf("Loading CPU....\n");
			CPU = pop(&ready_queue);
			printf("CPU: ");
			printList(CPU);
			if (CPU->time_left - time_quantum < 0) {
				CPU->time_left = 0;
				//cur_time += (time_quantum - CPU->time_left);
				//need to put completion time
				insert_fin(CPU);
				//go to external loop
			}
			else if (CPU->time_left - time_quantum == 0) {
				CPU->time_left = 0;
				//cur_time += time_quantum;
				//need to put completion time
				insert_fin(CPU);
				//go to external loop
			}
			else {
				
				CPU->time_left -= time_quantum;
				//cur_time += time_quantum;
				//would be outside of internal loop
				insertFIFO2(&ready_queue, CPU);
			}
			//CPU->time_left -= time_quantum;
			CPU = NULL;
		}

		printf("DONE INTERNAL\n");
		print_sys_vars();
		print_queues();
		printf("STARTING EXTERNAL\n");

		/*external event*/
		if (cur_line[0] == 'R') {
			cur_length = next_line(cur_line);
		}

		/* if system just read in a new line, parse that line else break*/
		if (cur_line[0] != 'R') {
			parse_line(cur_line);
			if (submit_queue != NULL && cur_time >= submit_queue->arrive_time) {
				pop_sub();
				printf("popped submit queue\n");
				print_queues();
			}
		}
		else {
			if (cur_line[0] != 'R') {
				printf("current line isn't done yet.\n");
			}
			else if (ready_queue != NULL) {
				printf("ready queue still has jobs.\n");
			}
			else if (wait_queue != NULL) {
				printf("wait queue still has processes\n");
			}
			else if (hold_queue_1 != NULL) {
				printf("hold queue 1 still has jobs.\n");
			}
			else if (hold_queue_2 != NULL) {
				printf("hold queue 2 still has jobs.\n");
			}
			else {
				break;
			}
		}

		cur_time += time_quantum;
		printf("DONE EXTERNAL\n");
		print_sys_vars();
		print_queues();
	}


	close_file();

	/*The system turnaround time and system weighted turnaround only at the 
	last display. Assume that the input file has a “D 9999” command at the end,
	so that you dump the final state of the system.
	*/

}