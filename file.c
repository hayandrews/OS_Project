//
//  file.c
//  Deals with opening, reading, writing, and closing files.
//
//  Created by Jessica Morris on 5/15/18.
//
#include <stdio.h>
#include <stdlib.h>

#define STRING_SIZE 1024

FILE *input;

int next_line(char * buffer) { /*uses buffer var to return value and returns 
							   an additional int that is the length of the 
							   string */
	if (input == NULL) {
		perror("file not opened");
		exit(1);
	}
	if (fgets(buffer, STRING_SIZE, input) != NULL) {
		return strlen(buffer);
	}
	else {
		return -1;
	}
}

void close_file() {
	fclose(input);
	printf("Close successful.\n");
}

void open_file(char * filename) {
	input = fopen(filename, "r");
	if (input == NULL) {
		perror("Invalid filename.\n");
		close_file();
		exit(1);
	}
	else {
		printf("Successfully opened %s\n", filename);
	}
}
