#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "constants.h"

const int MAX_LABEL_SIZE = 100;
const int MAX_PRINT_SIZE = 150;
const int MAX_GOAL_SIZE = MAX_LABEL_SIZE + sizeof(int) + sizeof(bool) + sizeof(double) + sizeof(time_t);

const unsigned int MAX_INLINE_ARGS = 20;
const char* goals_src = "res/goals";


// returns malloc'd void*
// argc is count of space-separated tokens
void* extract_value(int argc, char* argv[], char* flag, char type) {
	void* val = NULL;
	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], flag)) {
			if (type == 'i') {  // int
				val = malloc(sizeof(int));
				*(int*)val = atoi(argv[i + 1]);
			} else if (type == 'c') {  // char
				val = malloc(sizeof(char));
				*(char*)val = argv[i + 1][0];
			} else if (type == 's') {  // string
				val = malloc(strlen(argv[i + 1] + 1));
				((char*)val)[strlen(argv[i + 1])] = '\0';
				strcpy(val, argv[i + 1]);
			} else if (type == 'b') {  // bool
				val = malloc(1);
				*(char*)val = -1;
			} else if (type == 'l') {  // long
				val = malloc(sizeof(long));
				*(long*)val = strtol(argv[i + 1], NULL, 10);
			} else if (type == 'd') {  // double
				val = malloc(sizeof(double));
				*(double*)val = strtod(argv[i + 1], NULL);
			} else if (type == 't') { // time_t
				val = malloc(sizeof(time_t));
				*(time_t*)val = (time_t) strtol(argv[i + 1], NULL, 10);
			}
		} 
	}
	return val;
}

// returns malloc'd char**, each char* must be independently freed
char** getargv(char* in, int argc) {
	char* dup = strdup(in);
	void* track = (void*) dup;
	char** argv = malloc(argc * sizeof(char*));
	char* tok;
	int i = 0;
	do {
		tok = strsep(&dup, "*");
		argv[i] = strdup(tok);
		i++;
	} while (i < argc && dup != NULL);
	free(track);
	return argv;
}
