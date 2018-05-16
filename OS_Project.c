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
extern struct Node *holdQueue1;
extern struct Node *holdQueue2;

int main(void){
    printList(holdQueue1);
    insertSJF(5);
    printList(holdQueue1);
    insertSJF(6);
    printList(holdQueue1);
    insertSJF(4);
    printList(holdQueue1);
    insertSJF(8);
    printList(holdQueue1);
    insertSJF(7);
    printList(holdQueue1);
    insertSJF(8);
    printList(holdQueue1);
    printList(holdQueue2);
    insertFIFO(5);
    printList(holdQueue2);
    insertFIFO(6);
    printList(holdQueue2);
    insertFIFO(4);
    printList(holdQueue2);
    insertFIFO(8);
    printList(holdQueue2);
    insertFIFO(7);
    printList(holdQueue2);
    insertFIFO(8);
    printList(holdQueue2);
    pop(holdQueue2);
    printList(holdQueue2);
    printList(holdQueue1);
    pop(holdQueue1);
    printList(holdQueue1);

}

void inc_com() {
	perror("file has an incorrect command.");
	exit(1);
}

void parse_line(char * command) {
	switch (command[0]) {
	case 'C':
		printf("command is a System Configuration. \n");
		break;
	case 'A':
		printf("command is a Job arrival. \n");
		break;
	case 'Q':
		printf("command is a Request for devices. \n");
		break;
	case 'L':
		printf("command is a Release for devices. \n");
		break;
	case 'D':
		printf("command is a Display request. \n");
		break;
	default:
		inc_com();
	}
}