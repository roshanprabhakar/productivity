#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <mach/error.h>
#include "goals.h"

const int MAX_GOAL_SIZE = MAX_LABEL_SIZE + sizeof(double) + sizeof(long) + sizeof(int);

void dump_goal(goal_t* goal) {
	printf("[label: '%s'  priority: %d  commit: %lf  due: %lu]\n", goal->label,
		goal->priority, goal->hrs_commit, goal->due);
}

void dump_glist(glist_t* list) {
	while (list != NULL) {
		dump_goal(list->cur);
		list = list->next;
	}
}

// requires a dynamically allocated char* goal
goal_t* create_goal(char* goal, double commitment, long due, int priority) {
	goal_t* g = (goal_t*) malloc(sizeof(goal_t));
	g->label = goal;
	g->hrs_commit = commitment;
	g->due = due;
	g->priority = priority;
	return g;
}

// elems present in order: int, long, double, char*, '\0'
// WARNING no checking can be done to ensure input validity
goal_t* goal_from_string(char* str) {
	goal_t* g = (goal_t*) malloc(sizeof(goal_t));
	g->priority = *(int*)str;
	str += sizeof(int);
	g->due = *(long*)str;
	str += sizeof(long); 
	g->hrs_commit = *(double*)str;
	str += sizeof(double);
	g->label = strdup(str);
	return g;
}

// returns dynamically allocated string
// WARNING this string is NOT null terminated
char* string_from_goal(goal_t* g) {
	int i1 = 0;
	int i2 = i1 + sizeof(int);
	int i3 = i2 + sizeof(long);
	int i4 = i3 + sizeof(double);
	int i5 = i4 + strlen(g->label);

	char* buf = malloc(i5 + 1);
	*(int*)(buf + i1) = g->priority;
	*(long*)(buf + i2) = g->due;
	*(double*)(buf + i3) = g->hrs_commit;
	strcpy((char*)(buf + i4), g->label);
	buf[i5] = '\0';

	return buf;
}

void write_goal(goal_t* g) {
	char* str = string_from_goal(g);
	FILE* f = fopen("res/goals", "a");
	int tok = 0;
	for (;tok < sizeof(int) + sizeof(long) + sizeof(double);tok++) {
		fputc(str[tok], f);
	}
	fputs(str + tok, f);
	fputc('\0', f);
	int err = fclose(f);
	free(str);
	if (err < 0) printf("unable to write goal to file\n");
}

// f assumed to point to start of goal object
goal_t* read_goal(FILE** f) {
	goal_t* g = malloc(sizeof(goal_t));
	
	int p = fread(&(g->priority), sizeof(int), 1, *f);
	int d = fread(&(g->due), sizeof(long), 1, *f);
	int c = fread(&(g->hrs_commit), sizeof(double), 1, *f);

	if (p == 0 || d == 0 || c == 0) {
		free(g);
		return NULL;
	}
	
	char tmp[MAX_LABEL_SIZE];
	int i = 0;
	char ch;
	do {
		ch = fgetc(*f);
		tmp[i] = ch;
	} while (ch != '\0' && i < MAX_LABEL_SIZE);
	tmp[MAX_LABEL_SIZE - 1] = '\0';
	g->label = strdup(tmp);
	return g;
}

/*
glist_t* read_goals() {
	char line[MAX_GOAL_SIZE];
	glist_t* list = NULL;
	FILE* f = fopen("res/goals", "r");
	while (fgets(line, MAX_GOAL_SIZE, f) != NULL) {
		goal_t* g = goal_from_string(line);
		glist_t* g_elem = (glist_t*) calloc(sizeof(glist_t), 1);
		g_elem->cur = g;
		if (list == NULL) {
			list = g_elem;
		} else {
			glist_t* cur = list->next;
			glist_t* prev = list;
			while (cur != NULL && cur->cur->priority < g->priority) {
				cur = cur->next;
				prev = cur;
			}

			prev->next = g_elem;
			g_elem->next = cur;
		}
	}
	return list;
}
*/
