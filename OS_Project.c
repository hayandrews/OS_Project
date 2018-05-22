//
//  OS_Project.c
//  
//
//  Created by Hayley Andrews on 5/15/18.
//

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

int start_time = -1;
int main_memory = -1;
int avail_mem = -1;
int serial_devices = -1;
int time_quantum = -1;
int cur_time = -1;
int avail_dev = -1;

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
	printf("PRINTING QUEUES\n");
	if (submit_queue != NULL) {
		printf("Submit_queue\n");
		printList(submit_queue);
	}
	if (ready_queue != NULL) {
		printf("ready_queue\n");
		printList(ready_queue);
	}
	if (hold_queue_1 != NULL) {
		printf("hold_queue_1\n");
		printList(hold_queue_1);
	}
	if (hold_queue_2 != NULL) {
		printf("hold_queue_2\n");
		printList(hold_queue_2);
	}
	if (wait_queue != NULL) {
		printf("wait_queue\n");
		printList(wait_queue);
	}
	if (complete_queue != NULL) {
		printf("complete_queue\n");
		printList(complete_queue);
	}
	fflush(stdout);
}

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
		printf("\n\n\nNEW ITERATION\t\tcur_line[0] = %c\n", cur_line[0]);
		/*if ready queue != NULL do CPU stuff*/
		/*if (ready_queue != NULL) {
			CPU = pop(&ready_queue);
			printf("CPU: \n");
			printList(CPU);
			printf("Ready Queue: \n");
			printList(ready_queue);
			CPU->time_left -= time_quantum;

		}*/

		/* if system ready for next line, read in next line*/
		if (cur_line[0] == 'R') {
			cur_length = next_line(cur_line);
		}
		printf("After cur_line[0]\tcur_line[0] = %c\n", cur_line[0]);
		/* if system just read in a new line, parse that line else break*/
		if (cur_line[0] != 'R') {
			parse_line(cur_line);
			printf("current line parsed\n");
			print_sys_vars();
			print_queues();
			if (submit_queue != NULL && cur_time >= submit_queue->arrive_time) {
				pop_sub();
				printf("popped submit queue\n");
				print_queues();
			}
		}
		else {
			break;
		}
		





		cur_time += time_quantum;
	}

	/*printList(submit_queue);
	printf("first arrival: %d\n", submit_queue->arrive_time);
	close_file();
	while (submit_queue != NULL) {
		pop_sub();
	}
	insert_ready(pop(&hold_queue_1));
	print_queues();
	while (ready_queue != NULL) {
		insert_fin(pop(&ready_queue));
		print_queues();
	}*/
    //outputJSON();
}
