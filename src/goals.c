#include <string.h>
#include <stdlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include "constants.h"
#include "goals.h"


void dump_goal(goal_t* goal) {
	printf("[ID: %d label: '%s'  priority: %d  commit: %lf  due: %lu banner: %d]\n", 
		goal->ID, goal->label, goal->priority, goal->hrs_commit, (long) goal->due, goal->is_banner);
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
// WARNING goal ID remains uninitialized
goal_t* create_goal(char* goal, double commitment, time_t due, int priority, bool is_banner) {
	goal_t* g = (goal_t*) malloc(sizeof(goal_t));
	g->label = goal;
	g->hrs_commit = commitment;
	g->due = due;
	g->priority = priority;
	g->is_banner = is_banner;
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

//str: MO/DA/YEAR HR:MI
time_t readable_to_epoch(char* str) {
	char* minute = strdup(str);
	char* block_start = minute;
	char* year = strsep(&minute, " ");
	char* month = strsep(&year, "/");
	char* day = strsep(&year, "/");
	char* hour = strsep(&minute, ":");
	
	time_t rawtime;
	struct tm timeinfo;
	timeinfo.tm_mon = atoi(month) - 1;
	timeinfo.tm_mday = atoi(day);
	timeinfo.tm_year = atoi(year) - 1900;
	timeinfo.tm_hour = atoi(hour);
	timeinfo.tm_min = atoi(minute);
	timeinfo.tm_sec = 0;
	rawtime = mktime(&timeinfo);

	free(block_start);

	return rawtime;
}

// WARNING returned string must be freed;
char* epoch_to_readable(time_t time) {
	return asctime(localtime(&time));
}

// elems present in order: bool, int, time_t, double, char*, '\0'
// WARNING no checking can be done to ensure input validity
goal_t* goal_from_string(char* str) {
	goal_t* g = (goal_t*) malloc(sizeof(goal_t));
	g->is_banner = *(bool*)str;
	str += sizeof(bool);
	g->priority = *(int*)str;
	str += sizeof(int);
	g->due = *(time_t*)str;
	str += sizeof(time_t); 
	g->hrs_commit = *(double*)str;
	str += sizeof(double);
	g->label = strdup(str);

	return g;
}

// returns dynamically allocated string
// WARNING this string is NOT null terminated
char* string_from_goal(goal_t* g) {
	int i1 = 0;
	int i2 = i1 + sizeof(bool);
	int i3 = i2 + sizeof(int);
	int i4 = i3 + sizeof(time_t);
	int i5 = i4 + sizeof(double);
	int i6 = i5 + strlen(g->label);

	char* buf = malloc(i5 + 1);
	*(bool*)(buf + i1) = g->is_banner;
	*(int*)(buf + i2) = g->priority;
	*(time_t*)(buf + i3) = g->due;
	*(double*)(buf + i4) = g->hrs_commit;
	strcpy((char*)(buf + i5), g->label);
	buf[i6] = '\0';

	return buf;
}

void write_goal(goal_t* g) {
	if (g == NULL) {
		printf("attempted to write null goal\n");
	}
	char* str = string_from_goal(g);
	FILE* f = fopen(goals_src, "a");
	int tok = 0;
	for (;tok < sizeof(bool) + sizeof(int) + sizeof(time_t) + sizeof(double);tok++) {
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
	if (fread(&(g->is_banner), 1, sizeof(bool), *f) < sizeof(bool) ||
			fread(&(g->priority), 1, sizeof(int), *f) < sizeof(int) ||
			fread(&(g->due), 1, sizeof(time_t), *f) < sizeof(time_t) ||
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
	FILE* f = fopen(goals_src, "r");

	glist_t* list = NULL;
	goal_t* g;
	int gID = 0;
	while ((g = read_goal(&f)) != NULL) {
		g->ID = gID;
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
		gID++;
	}
	return list;
}

