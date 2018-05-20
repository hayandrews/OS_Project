//
//  OS_Project.c
//  
//
//  Created by Hayley Andrews on 5/15/18.
//

#include <stdio.h>
#include <stdlib.h>

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

int sys_configs[4]; /*index 0 = start time, 1 = main memory, 
					2 = serial devices, 3 = time quantum*/
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

}

void parse_Q(char * command) {
	printf("%s", command);
}

void parse_L(char * command) {
	printf("%s", command);
}

void parse_D(char * command) {
	printf("%s", command);
	//outputJSON();
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
	
	for (;;) {
		cur_length = next_line(cur_line);
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
