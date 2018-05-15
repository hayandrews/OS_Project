//
//  OS_Project.c
//  
//
//  Created by Hayley Andrews on 5/15/18.
//

#include <stdio.h>
#include <stdlib.h>

struct Node
{
    int data;
    struct Node *next;
}

Node *holdQueue1 = NULL;
Node *holdQueue2 = NULL;

//Inserts Shortest Job at front of the list
void insertSJF(int data) {
    new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->data = data;
    new_node->next = NULL;
    //Empty queue make new_node the head
    if (holdQueue1 == NULL) {
        holdQueue1 = new_node;
    } else if (new_node->data < holdQueue1->data) { //Else if less than head, make new head
        new_node->next = holdQueue1;
        holdQueue1 = new_node;
    } else { //Else iterate through the list
        Node *pointer = holdQueue1;
        while(pointer->next != NULL){
            if (new_node->data < tmpnode->data){
                break;
            } else {
                tmpnode = tmpnode->next;
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
    
}

int main(void){
    return 0;
}
