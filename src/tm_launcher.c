#include "terminal_manager.h"
#include "terminal_manager_low_level.h"
#include <stdio.h>
#include <stdlib.h>
#include "benutils/unicode.h"
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#define MAX_NAME_LENGTH 64

// A char array to store the read characters.
static char exec_name[MAX_NAME_LENGTH + 1] = {0};
// Index of last read character
static uint8_t name_index = 0;

static int width, height;

// This function is called on every entered character. This handles echoing the entered characters and filling the buffer.
// This also handles sanitizng the input
void input_handler(char c){
	if(name_index >= MAX_NAME_LENGTH){
		fprintf(stderr, "You surpassed the maximum buffer size!\n");
		return;
	}
	
	switch(c){
	// Sanitization
	case '_':
	case '|':
	case '&':
	case ';':
	case '(':
	case ')':
		return;
	// Enter
	case (char)10:

	// Backspace
	case (char)127:
	case (char)8:
		if(name_index >= 1){
			--name_index;
			exec_name[name_index] = '\0';
		}
		break;
		
	// Normal characters
	default:
		exec_name[name_index] = c;
		++name_index;
		exec_name[name_index] = '\0';
		break;
	}

	tm_clear();
	tm_move_cursor(0,0);
	printf("%s\n", exec_name);



	char s1[256] = "ls -1 -A /usr/bin/ | grep ";
	char s2[] = " | tr -d '*' | tr -d '@'";
	
	strcat(s1, exec_name);
	strcat(s1, s2);

	FILE* cmd_res = popen(s1, "r");

	if(cmd_res == NULL){
		fprintf(stderr, "popen failed!\n");
		return;
	}
	
	int row_num = 3;

	while(row_num <= height){
		char c = fgetc(cmd_res);
		if(c == EOF)
			break;
		if(c == '\n')
			row_num++;
		putc(c, stdout);
	}
	pclose(cmd_res);

}

// A function to run a given executable as a separate process by using the double fork technique.
static void run_forked(char* name){
	char path[MAX_NAME_LENGTH + 24] = "/usr/bin";
	
	strcat(path, name);

	// Child
if(fork() == 0){
		if (fork() == 0)
		// Grandchild
			execl(path, ".",(char*)NULL );
		else
			// Child
			exit(0);
	}
	
}

static void search(char* name){

	// first and second halves of the commnad.
	char ch1[256] = "ls -1 -A /usr/bin/ | grep ";
	char ch2[] = " | tr -d '*' | tr -d '@'";
	
	strcat(ch1, name);
	strcat(ch1, ch2);
	system(ch1);
}


void quit(){
	tm_run = 0;
}

void resize(int r, int c){
	tm_clear();
	printf("resized to %dx%d\n", r, c);
}


void tm_initCall(){
	tm_getTerminalSize(&height, &width);	
	tm_setResizeCallback(&resize);
	tm_bindToAnyKeypress(&input_handler);
	tm_bindKey( (char)27, &quit);
}
