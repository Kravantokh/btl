#include "terminal_manager.h"
#include "terminal_manager_low_level.h"
#include "benutils/unicode.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>


#define TML_PROMPT_COLOR 190, 190, 190, 0

#define TML_SELECTION_COLOR 255, 255, 255, 0 

#define TML_TEXT_COLOR 75, 75, 75, 0

#define TML_MAX_NAME_LENGTH 64

// A char array to store the read characters.
static char search_term[TML_MAX_NAME_LENGTH + 1] = {0};
// Index of last read character
static uint8_t name_index = 0;
// The first executeable's name in the current list
static char exec_name[TML_MAX_NAME_LENGTH + 1] = {0};

static int width, height;

// Forward declarations
static void double_fork(char*);

// This function is called on every entered character. This handles echoing the entered characters and filling the buffer.
// This also handles sanitizng the input
void input_handler(char c){
	if(name_index >= TML_MAX_NAME_LENGTH){
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
		; // For some reason clang won't shut up about an expected expression unless I put this semicolon here.
		char* p = exec_name;
		while(*p != '\n')
			++p;
		*p = 0;
		double_fork(exec_name);
		tm_run = 0;
		return;
	// Backspace
	case (char)127:
	case (char)8:
		if(name_index >= 1){
			--name_index;
			search_term[name_index] = '\0';
		}
		break;
		
	// Normal characters
	default:
		search_term[name_index] = c;
		++name_index;
		search_term[name_index] = '\0';
		break;
	}

	tm_clear();
	tm_move_cursor(0,0);
	tm_set_fg( (tm_color)(struct tm_rgb_color) {TML_PROMPT_COLOR} );
	printf("%s\n", search_term);


	if(name_index > 0){
		char s1[256] = "ls -1 -A /usr/bin/ | grep ";
		char s2[] = " | tr -d '*' | tr -d '@'";
		
		strcat(s1, search_term);
		strcat(s1, s2);

		FILE* cmd_res = popen(s1, "r");

		if(cmd_res == NULL){
			fprintf(stderr, "popen failed!\n");
			return;
		}

		int row_num = 4;

		tm_set_fg( (tm_color)(struct tm_rgb_color) {TML_SELECTION_COLOR} );
		fgets(exec_name, 64, cmd_res);
		
		
		printf(" %s", exec_name);
		
		tm_set_fg( (tm_color)(struct tm_rgb_color) {TML_TEXT_COLOR} );
		
		printf(" ");
		while(row_num <= height){
			char c = fgetc(cmd_res);
			if(c == EOF)
				break;
			putc(c, stdout);
			if(c == '\n'){
				row_num++;
				printf(" ");
			}
		}
		pclose(cmd_res);
	}

}

// A function to run a given executable as a separate process by using the double fork technique.
static void double_fork(char* name){
	char path[TML_MAX_NAME_LENGTH + 24] = "/usr/bin/";
	
	strcat(path, name);
	
	printf("Forking with \"%s\"", path);
	
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


void quit(){
	tm_run = 0;
}

void resize(int r, int c){
	tm_clear();
	height = r;
	width = c;
}


void tm_initCall(){
	tm_clear();
	tm_getTerminalSize(&height, &width);	
	tm_setResizeCallback(&resize);
	tm_bindToAnyKeypress(&input_handler);
	tm_bindKey( (char)27, &quit);
}
