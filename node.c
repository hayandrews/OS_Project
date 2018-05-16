#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int data;
    struct Node *next;
};

struct Node *holdQueue1 = NULL;
struct Node *holdQueue2 = NULL;

//Inserts Shortest Job at front of the list
void insertSJF(int data) {
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = data;
    new_node->next = NULL;
    //Empty queue make new_node the head
    if (holdQueue1 == NULL) {
        holdQueue1 = new_node;
    } else if (new_node->data < holdQueue1->data) { //Else if less than head, make new head
        new_node->next = holdQueue1;
        holdQueue1 = new_node;
    } else { //Else iterate through the list
        struct Node *pointer = holdQueue1;
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
    struct Node *new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = data;
    new_node->next = NULL;
    //If empty insert at head
    if (holdQueue2 == NULL) {
        holdQueue2 = new_node;
    } else {
        //Other wise add to the end of the list
        struct Node *pointer = holdQueue2;
        while(pointer->next != NULL){
            pointer = pointer->next;
        }
        pointer->next = new_node;
    }
}

int pop(struct Node *queue){
    if (queue != NULL) {
		int ret = queue->data;
		if (queue->next == NULL) {
			free(queue);
			queue = NULL;
		}
		else {
			struct Node *tmp = queue->next;
			queue->data = queue->next->data;
			queue->next = tmp->next;
			free(tmp);
		}
        return ret;
    } 
	printf("Queue is empty");
    return -1;
}

void printList(struct Node *queue){
    struct Node *pointer = queue;
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