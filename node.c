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
	int mem_req;
	int dev_req;
	int run_time;
    struct Job *next;
};

struct Job *holdQueue1 = NULL;
struct Job *holdQueue2 = NULL;
struct Job *submit_queue = NULL;

//Inserts Shortest Job at front of the list
void insertSJF(int arr_t, int mem_r, int dev_r, int run_t) {
	struct Job *new_node = (struct Job*)malloc(sizeof(struct Job));
	new_node->arrive_time = arr_t;
	new_node->mem_req = mem_r;
	new_node->dev_req = dev_r;
	new_node->run_time = run_t;
	new_node->next = NULL;
	//Empty queue make new_node the head
	if (holdQueue1 == NULL) {
		holdQueue1 = new_node;
	}
	else if (new_node->run_time < holdQueue1->run_time) { //Else if less than head, make new head
		new_node->next = holdQueue1;
		holdQueue1 = new_node;
	}
	else { //Else iterate through the list
		struct Job *pointer = holdQueue1;
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

void insertFIFO(int arr_t, int mem_r, int dev_r, int run_t) {
	struct Job *new_node = (struct Job*)malloc(sizeof(struct Job));
	new_node->arrive_time = arr_t;
	new_node->mem_req = mem_r;
	new_node->dev_req = dev_r;
	new_node->run_time = run_t;
	new_node->next = NULL;
    //If empty insert at head
    if (holdQueue2 == NULL) {
        holdQueue2 = new_node;
    } else {
        //Other wise add to the end of the list
        struct Job *pointer = holdQueue2;
        while(pointer->next != NULL){
            pointer = pointer->next;
        }
        pointer->next = new_node;
    }
}

struct Job * pop(struct Job *queue){
    if (queue != NULL) {
		struct Job *ret = (struct Job*)malloc(sizeof(struct Job));
		ret->arrive_time = queue->arrive_time;
		ret->mem_req = queue->mem_req;
		ret->dev_req = queue->dev_req;
		ret->run_time = queue->run_time;
		ret->next = NULL;
		if (queue->next == NULL) {
			free(queue);
			queue = NULL;
		}
		else {
			struct Job *tmp = queue->next;
			queue->arrive_time = tmp->arrive_time;
			queue->mem_req = tmp->mem_req;
			queue->dev_req = tmp->dev_req;
			queue->run_time = tmp->run_time;
			queue->next = tmp->next;
			free(tmp);
		}
        return ret;
    } 
	printf("Queue is empty");
    return NULL;
}

void printList(struct Job *queue){
    struct Job *pointer = queue;
    if (pointer == NULL){
        printf("empty\n");
    } else{
        while(pointer != NULL){
            printf("%d --> ",pointer->run_time);
            pointer = pointer->next;
        }
        printf("Null \n");
    }
    
}