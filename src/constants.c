#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <time.h>
#include "constants.h"

const int MAX_LABEL_SIZE = 100;
const int MAX_PRINT_SIZE = 80;
const int MAX_GOAL_SIZE = MAX_LABEL_SIZE + sizeof(int) + sizeof(bool) + sizeof(double) + sizeof(time_t);

const unsigned int MAX_INLINE_ARGS = 20;
const time_t WARNING_TIME = 6 * 60 * 60;

const int START_ARG_LINE_SIZE = 100;

const char* goals_ext = "/.config/track/goals";

const char* home_dir() {
	return getpwuid(getuid())->pw_dir;
}

char* goals_path() {
	char path[strlen(home_dir()) + strlen(goals_ext) + 1];
	strcpy(path, home_dir());
	strcat(path, goals_ext);
	return strdup(path);
}

// returns malloc'd void*
// argc is count of space-separated tokens
// TODO catch out of bounds access
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
				val = malloc(strlen(argv[i + 1]) + 1);
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


char** getargv(char* in, int* argc, char* delim) {
	size_t size = 1 * sizeof(char*);
	int cargc = 0;
	// printf("init size: %lu, argc: %d\n", size, cargc);

	char* dup = strdup(in);
	void* track = (void*) dup;
	// printf("dup: %p (%s)\n", dup, dup);

	char** argv = malloc(sizeof(char*) * size);
	// printf("argv init contents: \n");
	for (int i = 0; i < cargc; i++) printf("%s", argv[i]);

	// printf("------- init loop ------\n");
	while (true) {
		// printf("---------- ITER %d START ----------\n", cargc);

		if (cargc * sizeof(char*) == size) {
			// printf("resizing!!\n");
			size += sizeof(char*);
			argv = realloc(argv, size);
			if (argv == NULL) {
				printf("could not read passed args\n");
				return NULL;
			}
			// printf("new argv base loc: %p\n", argv);
			// printf("argv after realloc: \n");
			// for (int i = 0; i < cargc; i++) {
				// printf("argv + %d = %p, argv[%d] = %p (%s)\n", i, argv + i, i, argv[i], argv[i]);
			// }
		}

		char* tok = strsep(&dup, delim);
		// printf("tok: '%s'\n", tok);

		if (*tok == 0) break;

		argv[cargc] = strdup(tok);
		// printf("strdup'd tok: %p (%s) @ %p\n", argv[cargc], argv[cargc], argv + cargc);

		cargc++;

		// printf("argv base loc: %p\n", argv);
		// printf("current argv contents: \n");
		// for (int i = 0; i < cargc; i++) {
		// 	printf("argv + %d = %p, argv[%d]: %p (%s)\n", i, argv + i, i, argv[i], argv[i]);
		// }

		if (dup == NULL) break;
		// printf("---------- ITER %d END ----------\n", cargc - 1);
	}

	free(track);
	*argc = cargc;
	return argv;
}