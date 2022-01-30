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

char* commitment_to_str(double hrs) {
	int day, hr, min;
	char* str = NULL;
	if (hrs < 24) {
		hr = (int) hrs;
		min = (int) ((hrs - (int) hrs) * 60);
	
		int hr_len = snprintf(NULL, 0, "%d", hr);
		int min_len = snprintf(NULL, 0, "%d", min);

		str = calloc(hr_len + min_len + 9, 1);
		char* write_i = str;

		if (hr) {
			snprintf(write_i, hr_len + 1, "%d", hr);
			write_i += hr_len;
			strcpy(write_i, " hr ");
			write_i += 4;
		} if (min) {
			snprintf(write_i, min_len + 1, "%d", min);	
			write_i += min_len;
			strcpy(write_i, " min");
		}

	} else {
		day = (int) (hrs / 24);
		hrs -= day * 24;
		hr = (int) hrs;
		min = (int) ((hrs - hr) * 60);

		int day_len = snprintf(NULL, 0, "%d", day);
		int hr_len = snprintf(NULL, 0, "%d", hr);
		int min_len = snprintf(NULL, 0, "%d", min);

		str = calloc(day_len + hr_len + min_len + 20, 1);
		char* write_i = str;

		if (day) {
			snprintf(write_i, day_len + 1, "%d", day);	
			write_i += day_len;
			strcpy(write_i, " day ");
			write_i += 5;
		} if (hr) {
			snprintf(write_i, hr_len + 1, "%d", hr);
			write_i += hr_len;
			strcpy(write_i, " hr ");
			write_i += 4;
		} if (min) {
			snprintf(write_i, min_len + 1, "%d", min);
			write_i += min_len;
			strcpy(write_i, " min");
		}
	}
	if (!strcmp(str, "")) {
		strcpy(str, "0 min");
	}
	return str;
}

//str: MO/DA/YEAR HR:MI
time_t readable_to_epoch(char* str) {
	int mo, da, yr, hr, mi;
	sscanf(str, "%d/%d/%d %d:%d", &mo, &da, &yr, &hr, &mi);
	
	struct tm timeinfo;
	time_t out;
	timeinfo.tm_mon = mo - 1;
	timeinfo.tm_mday = da;
	timeinfo.tm_year = yr - 1900;
	timeinfo.tm_hour = hr;
	timeinfo.tm_min = mi;
	timeinfo.tm_sec = 0;
	timeinfo.tm_isdst = (yr % 4 == 0) ? 1 : 0;
	timeinfo.tm_yday = -1;
	timeinfo.tm_wday = -1;

	setenv("TZ", "", 1);
	tzset();
	out = mktime(&timeinfo);
	return out;
}

// WARNING returned string must NOT be freed;
// returns size 24 string
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

