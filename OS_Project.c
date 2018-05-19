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

void pop_sub(){

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

	printf("C:\tstart: %d\tmain: %d\tserial: %d\time: %d\n", start_time, 
		main_memory, serial_devices, time_quantum);
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

	printf("A:\tarr_t: %d\tjob_n: %d\tmem_r: %d\tdev_r: %d\trun_t: %d\tqueue_p: %d\n", 
		arr_t, job_n, mem_r, dev_r, run_t, queue_p);

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

int main(void){
    printList(hold_queue_1);
    insertSJF(1, 1, 1, 1, 5, 1);
    printList(hold_queue_1);
    insertSJF(1, 2, 1, 1, 6, 1);
    printList(hold_queue_1);
    insertSJF(1, 3, 1, 1, 4, 1);
    printList(hold_queue_1);
    insertSJF(1, 4, 1, 1, 8, 1);
    printList(hold_queue_1);
    insertSJF(1, 5, 1, 1, 7, 1);
    printList(hold_queue_1);
    insertSJF(1, 6, 1, 1, 8, 1);
    printList(hold_queue_1);
    printList(hold_queue_2);
    insertFIFO(1, 1, 1, 1, 5, 1);
    printList(hold_queue_2);
    insertFIFO(1, 2, 1, 1, 6, 1);
    printList(hold_queue_2);
    insertFIFO(1, 3, 1, 1, 4, 1);
    printList(hold_queue_2);
    insertFIFO(1, 4, 1, 1, 8, 1);
    printList(hold_queue_2);
    insertFIFO(1, 5, 1, 1, 7, 1);
    printList(hold_queue_2);
    insertFIFO(1, 6, 1, 1, 8, 1);
    printList(hold_queue_2);
	printList(pop(hold_queue_2));
    printList(hold_queue_2);
    printList(hold_queue_1);
	printList(pop(hold_queue_1));
    printList(hold_queue_1);

	/*printf("Please input filename:\n");
	scanf("%s", s_input);
	open_file(s_input);
	printList(submit_queue);
	for (;;) {
		cur_length = next_line(cur_line);
		if (cur_length < 0) {
			break;
		}
		parse_line(cur_line);
	}
	printList(submit_queue);
	close_file();*/
    outputJSON();
}
