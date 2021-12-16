#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include "goals.h"

const int MAX_GOAL_SIZE = MAX_LABEL_SIZE + sizeof(double) + sizeof(unsigned long) + sizeof(int);

void dump_goal(goal_t* goal) {
	printf("[label: '%s'  priority: %d  commit: %lf  due: %lu]\n", goal->label,
		goal->priority, goal->hrs_commit, goal->due);
}

void dump_glist(glist_t* list) {
	printf("--- LL DUMP ---\n");
	while (list != NULL) {
		dump_goal(list->cur);
		list = list->next;
	}
	printf("----- END -----\n");
}

// requires a dynamically allocated char* goal
goal_t* create_goal(char* goal, double commitment, unsigned long due, int priority) {
	goal_t* g = (goal_t*) malloc(sizeof(goal_t));
	g->label = goal;
	g->hrs_commit = commitment;
	g->due = due;
	g->priority = priority;
	return g;
}

/* parse a commitment labeled in min(mi)/hr(h)/day(d)/(mon(mo) == 30d)
 * return -1 on error: no label
 * else return time in minutes
 */
double commitment(char* str) {
	int i = 0;
	for (; str[i] != '\0' && (!isalpha(str[i]) || str[i] == '.'); i++);
	double val = atof(str);
	if (str[i] != '\0' && str[i] != 'm') {
		if (str[i] == 'h') {
			return 60 * val;
		}
		if (str[i] == 'd') {
			return val * 24 * 60;
		}
	} else if (str[i + 1] != '\0') {
		if (str[i + 1] == 'i') {
			return val;
		}
		if (str[i + 1] == 'o') {
			return val * 30 * 24 * 60;
		}
	}
	return -1;
}

/* parse given timestamp into unix timestamp
 * argument is of form:
 * min(mi)/hr(h)/day(d)/(mon(mo) == 30d) (FLAGGED -d)
 * MO/DA/YEAR HR:MI (DEFAULT)
 * MO: 1-12
 * DA: 1-31
 * HR: 0-23
 * MI: 0-59
 */
unsigned long due(char* str) {
	char* dup1 = strdup(str);
	void* dup2 = (void*) dup1;
	char* tok = strsep(&dup1, " ");
	if (!strcmp("-d", tok)) {
		return (unsigned long)(time(0) + 60 * commitment(dup1));
	}
	free(dup2);
	char* dup3 = strdup(str);
	void* dup4 = (void*) dup3;
	struct tm time;
	time.tm_mon = atoi(strsep(&dup3, "/")) - 1;
	time.tm_mday = atoi(strsep(&dup3, "/"));
	time.tm_year = atoi(strsep(&dup3, " ")) - 1900;
	time.tm_hour = atoi(strsep(&dup3, ":"));
	time.tm_isdst = (time.tm_year % 4 == 0) ? 1 : -1;
	time.tm_min = atoi(dup3);
	time.tm_sec = 0;
	free(dup4);
	
	//printf("parsed time: %d/%d/%d %d:%d\n", time.tm_mon + 1, time.tm_mday, 1900 + time.tm_year,
	// 	time.tm_hour, time.tm_min);
	
	return mktime(&time);
}


goal_t* parse_goal(char* goal, char* commitment, char* due, char* priority) {
	return NULL;
}


// elems present in order: int, long, double, char*, '\0'
// WARNING no checking can be done to ensure input validity
goal_t* goal_from_string(char* str) {
	goal_t* g = (goal_t*) malloc(sizeof(goal_t));
	g->priority = *(int*)str;
	str += sizeof(int);
	g->due = *(unsigned long*)str;
	str += sizeof(unsigned long); 
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
	int i3 = i2 + sizeof(unsigned long);
	int i4 = i3 + sizeof(double);
	int i5 = i4 + strlen(g->label);

	char* buf = malloc(i5 + 1);
	*(int*)(buf + i1) = g->priority;
	*(unsigned long*)(buf + i2) = g->due;
	*(double*)(buf + i3) = g->hrs_commit;
	strcpy((char*)(buf + i4), g->label);
	buf[i5] = '\0';

	return buf;
}

void write_goal(goal_t* g) {
	if (g == NULL) {
		printf("attempted to write null goal\n");
	}
	char* str = string_from_goal(g);
	FILE* f = fopen("res/goals", "a");
	int tok = 0;
	for (;tok < sizeof(int) + sizeof(unsigned long) + sizeof(double);tok++) {
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
	
	// read fixed-size data
	if (fread(&(g->priority), 1, sizeof(int), *f) < sizeof(int) ||
			fread(&(g->due), 1, sizeof(unsigned long), *f) < sizeof(unsigned long) ||
			fread(&(g->hrs_commit), 1, sizeof(double), *f) < sizeof(double)) {
		free(g);
		return NULL;
	}

	// read variable-size label
	char tmp[MAX_LABEL_SIZE];
	int i = 0;
	do {
		tmp[i] = fgetc(*f);
		i++;
	} while (tmp[i - 1] != '\0' && i < MAX_LABEL_SIZE);
	tmp[MAX_LABEL_SIZE - 1] = '\0';
	g->label = strdup(tmp);
	return g;
}

glist_t* read_goals() {
	FILE* f = fopen("res/goals", "r");

	glist_t* list = NULL;
	goal_t* g;
	while ((g = read_goal(&f)) != NULL) {
		glist_t* g_elem = calloc(sizeof(glist_t), 1);
		g_elem->cur = g;
		if (list == NULL) {
			list = g_elem;
		} else {
			glist_t* cur = list;
			glist_t* prev = NULL;
			while (cur != NULL && cur->cur->priority < g->priority) {
				prev = cur;
				cur = cur->next;
			}
			if (prev == NULL) {
				list = g_elem;
			} else {
				prev->next = g_elem;
			}
			g_elem->next = cur;
		}
	}
	return list;
}

