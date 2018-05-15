#include <stdio.h>
#include <stdlib.h>

#define STRING_SIZE 1024

FILE *input;

int next_line(char * buffer) {
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

void open_file(char * filename) {
	input = fopen(filename, "r");
	if (input == NULL) {
		perror("Invalid filename.\n");
		close();
		exit(1);
	}
	else {
		printf("Successfully opened %s\n", filename);
	}
}

void close_file() {
	fclose(input);
	printf("Close successful.\n");
}