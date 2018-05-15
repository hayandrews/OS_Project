#include <stdio.h>
#include <stdlib.h>

#define STRING_SIZE 1024

char s_input[STRING_SIZE];	/* input file name */
char cur_line[STRING_SIZE]; /* current line in file*/
int cur_length;				/* length for above string*/

int main() {
	//enter name of input file
	printf("Enter name of configuration text file: ");
	scanf("%s", s_input);
	open_file(s_input);
	cur_length = next_line(cur_line);
	for (int i = 0; i < cur_length; i++) {
		printf("%c", cur_line[i]);
	}
	printf("~~done printing, closing now. \n");
	fflush(stdout);
	close_file();
}