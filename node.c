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
    int data;
    struct Job *next;
};

struct Job *holdQueue1 = NULL;
struct Job *holdQueue2 = NULL;

//Inserts Shortest Job at front of the list
void insertSJF(int data) {
    struct Job *new_node = (struct Job*)malloc(sizeof(struct Job));
    new_node->data = data;
    new_node->next = NULL;
    //Empty queue make new_node the head
    if (holdQueue1 == NULL) {
        holdQueue1 = new_node;
    } else if (new_node->data < holdQueue1->data) { //Else if less than head, make new head
        new_node->next = holdQueue1;
        holdQueue1 = new_node;
    } else { //Else iterate through the list
        struct Job *pointer = holdQueue1;
        while(pointer->next != NULL){
            if (new_node->data < pointer->next->data){
                break;
            } else {
                pointer = pointer->next;
            }
        }
        //If between two nodes, the new_nodes next is same as pointers next
        //Then pointers next is then new_node
        if(pointer->next != NULL){
            new_node->next = pointer->next;
            pointer->next = new_node;
        } else { //pointer->next is null thus just add new_node as next
            pointer->next = new_node;
        }
    }
}

void insertFIFO(int data) {
    struct Job *new_node = (struct Job*)malloc(sizeof(struct Job));
    new_node->data = data;
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

int pop(struct Job *queue){
    if (queue != NULL) {
		int ret = queue->data;
		if (queue->next == NULL) {
			free(queue);
			queue = NULL;
		}
		else {
			struct Job *tmp = queue->next;
			queue->data = queue->next->data;
			queue->next = tmp->next;
			free(tmp);
		}
        return ret;
    } 
	printf("Queue is empty");
    return -1;
}

void printList(struct Job *queue){
    struct Job *pointer = queue;
    if (pointer == NULL){
        printf("empty\n");
    } else{
        while(pointer != NULL){
            printf("%d --> ",pointer->data);
            pointer = pointer->next;
        }
        printf("Null \n");
    }
    
}