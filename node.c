#include <stdio.h>
#include <stdlib.h>

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
};

struct Job *hold_queue_1 = NULL;
struct Job *hold_queue_2 = NULL;
struct Job *submit_queue = NULL;
struct Job *ready_queue = NULL;
extern int avail_mem;

//Inserts Shortest Job at front of the list
void insertSJF(int arr_t, int job_n, int mem_r, int dev_r, int run_t, int queue_p) {
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
	//Empty queue make new_node the head
	if (hold_queue_1 == NULL) {
		hold_queue_1 = new_node;
	}
	else if (new_node->run_time < hold_queue_1->run_time) { //Else if less than head, make new head
		new_node->next = hold_queue_1;
		hold_queue_1 = new_node;
	}
	else { //Else iterate through the list
		struct Job *pointer = hold_queue_1;
		while (pointer->next != NULL) {
			if (new_node->run_time < pointer->next->run_time) {
				break;
			}
			else {
				pointer = pointer->next;
			}
		}
		//If between two nodes, the new_nodes next is same as pointers next
		//Then pointers next is then new_node
		if (pointer->next != NULL) {
			new_node->next = pointer->next;
			pointer->next = new_node;
		}
		else { //pointer->next is null thus just add new_node as next
			pointer->next = new_node;
		}
	}
}

void insertSJF2(struct Job * node){
    if (hold_queue_1 == NULL) {
        hold_queue_1 = node;
    } else if (node->run_time < hold_queue_1->run_time){
        node->next = hold_queue_1;
        hold_queue_1 = node;
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

void insertFIFO(int arr_t, int job_n, int mem_r, int dev_r, int run_t, int queue_p) {
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
    //If empty insert at head
    if (hold_queue_2 == NULL) {
        hold_queue_2 = new_node;
    } else {
        //Other wise add to the end of the list
        struct Job *pointer = hold_queue_2;
        while(pointer->next != NULL){
            pointer = pointer->next;
        }
        pointer->next = new_node;
    }
}

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

void pop_sub() {
	struct Job * cur_job = pop(&submit_queue);
	insertFIFO2(&hold_queue_2, cur_job);
	//if (cur_job->mem_req < avail_mem) {
	//	//insert into ready queue
	//}
	//else if (cur_job->queue_priority == 1) {
	//	//insertSJF(cur_job);
	//}
	//else if (cur_job->queue_priority == 2) {
	//}
	//else {
	//	perror("Job's priority does not match a hold queue.");
	//	exit(1);
	//}
	
}
