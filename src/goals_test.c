#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "goals.h"
#include "gutils.h"
#include "constants.h"

void unit_convert_commitment() {
	printf("\ncommitment (unit convert) {\n");
	double out;
	out = commitment("1d");
	printf("%lf : %lf\n", out, (double)24 * 60);
	out = commitment("30min");
	printf("%lf : %lf\n", out, (double)30);
	out = commitment("1mo");
	printf("%lf : %lf\n", out, (double)30 * 24 * 60);
	out = commitment("2mi");
	printf("%lf : %lf\n", out, (double)2);
	out = commitment("1.5d");
	printf("%lf : %lf\n", out, 1.5 * 24 * 60);
	out = commitment("1.32h");
	printf("%lf : %lf\n", out, 1.32 * 60);
	out = commitment("-3d");
	printf("%lf : %lf\n", out, -3.0 * 24 * 60);
	printf("}\n");

	printf("\ndouble -> string: {\n");
	double list[4] = {1.1111, 24.55, 0.5, 0.1};
	for (int i = 0; i < sizeof(list) / sizeof(list[0]); i++) {
		char* str = commitment_to_str(list[i]);
		printf("%lf : %s\n", list[i], str);
		free(str);
	}
	printf("}\n");
}

// tests both ways
void unit_convert_due_date() {
	printf("\ndue (standard readable -> unix time_t) {\n");

	char* test1 = "06/18/2022 00:00";
	time_t te = readable_to_epoch(test1);
	char* tr = epoch_to_readable(te);
	printf("original: %s\nepoch time: %ld\nconverted readable: %s\n", test1, (long)te, tr);

	char* test2 = "06/01/2022 12:00";
	time_t te2 = readable_to_epoch(test2);
	char* tr2 = epoch_to_readable(te2);
	printf("original: %s\nepoch time: %ld\nconverted readable: %s\n", test2, (long)te2, tr2);

	printf("}\n");
}

goal_t* unit_create_goal() {
	printf("\ncreate_goal {\n");
	goal_t* g = create_goal("example goal 1", 1.5, (time_t) 10000000, 1, false);
	g->ID = 1;
	dump_goal(g);
	printf("}\n");
	return g;
}

goal_t* unit_goal_from_string() {
	printf("\ngoal_from_string {\nMust match:\n");
	goal_t* g = create_goal("example goal 1", 1.5, (time_t) 10000000, 1, false);
	dump_goal(g);
	char* str = string_from_goal(g);
	free(g);
	goal_t* g1 = goal_from_string(str);
	free(str);
	dump_goal(g1);
	printf("}\n");
	return g1;
}

char* unit_string_from_goal() {
	printf("\nstring_from_goal {\n");
	goal_t* g = create_goal("example goal 1", 1.5, (time_t) 10000000, 1, false);
	printf("recreated goal must match: \n");
	dump_goal(g);
	char* str = string_from_goal(g);
	printf("[NOT terminated, will appear shorter than expected] string: %s\n", str);
	printf("is_banner: %d \npriority: %d\ndue: %lu\ncommit: %lf\nlabel: %s\n", 
			*(bool*)str, *(int*)(str + sizeof(bool)), *(unsigned long*)(str + sizeof(bool) + sizeof(int)), 
			*(double*)(str + sizeof(bool) + sizeof(int) + sizeof(time_t)),
			str + sizeof(bool) + sizeof(int) + sizeof(time_t) + sizeof(double));
	printf("}\n");
	free(g);
	return str;
}

void unit_read_list() {
	printf("\nread_goals {\n");
	glist_t* g = read_goals();
	glist_t* n = g->next;
	dump_glist(g);

	while (g != NULL) {
		free(g);
		g = n;
		if (n != NULL) n = n->next;
	}

	printf("}\n");
}

void unit_write() {
	printf("\ncreate_goal {\n");

	printf("writing: \n");
	goal_t* g1 = create_goal("example goal 1", 1.1, (time_t) 123, 1, false);
	dump_goal(g1);

	goal_t* g2 = create_goal("example goal 2", 2.2, (time_t) 234, 2, false);
	dump_goal(g2);

	goal_t* g3 = create_goal("example goal 3", 3.3, (time_t) 345, 3, false);
	dump_goal(g3);

	goal_t* g4 = create_goal("example goal 4", 4.4, (time_t) 456, 3, false);
	dump_goal(g4);

	// wipe goals
	FILE* f = fopen(goals_src, "w");
	fclose(f);

	// write created goals
	write_goal(g2);
	free(g2);

	write_goal(g4);
	free(g4);

	write_goal(g1);
	free(g1);

	write_goal(g3);
	free(g3);

	glist_t* list = read_goals();
	glist_t* next = list->next;
	dump_glist(list);
	while (list != NULL) {
		free(list);
		list = next;
		if (next != NULL) next = next->next;
	}

	printf("}\n");
}

void unit_read() {
	printf("\nread_goal {\n");
	FILE* f = fopen(goals_src, "r");

	goal_t* g;
	while ((g = read_goal(&f)) != NULL) {
		dump_goal(g);
		free(g);
	}
	fclose(f);
	printf("}\n");
}

int main() {
	// unit_create_goal(); // WORKING
    unit_convert_commitment(); // WORKING
    // unit_convert_due_date(); // WORKING
    // unit_goal_from_string(); // WORKING
    // unit_string_from_goal(); // WORKING
    // unit_read_list(); // WORKING
	// unit_write(); // WORKING
	// unit_read(); // WORKING
	return 0;
}