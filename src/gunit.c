#include <stdio.h>
#include <string.h>
#include <time.h>
#include "goals.h"

// testing create_goal
goal_t* unit_create_goal() {
	printf("----------- TESTING: create_goal --------\n");
	goal_t* g = create_goal("example goal 1", 1.5, 10000000, 1);
	printf("priority: %d\ndue: %lu\ncommit: %lf\nlabel: %s\n", 
			g->priority, g->due, g->hrs_commit, g->label);
	printf("-----------------------------------------\n");
	return g;
}

char* unit_string_from_goal() {
	printf("----------- TESTING: goal->string -------\n");
	goal_t* g = unit_create_goal();
	char* str = string_from_goal(g);
	printf("string: %s\n", str);
	printf("priority: %d\ndue: %lu\ncommit: %lf\nlabel: %s\n", 
			*(int*)str, *(unsigned long*)(str + sizeof(int)), *(double*)(str + sizeof(int) + sizeof(long)),
			str + sizeof(int) + sizeof(long) + sizeof(double));
	printf("-----------------------------------------\n");
	return str;
}


goal_t* unit_goal_from_string() {
	printf("----------- TESTING: string->goal -------\n");
	goal_t* g = unit_create_goal();
	char* str = string_from_goal(g);
	free(g);
	goal_t* g1 = goal_from_string(str);
	free(str);
	dump_goal(g1);
	printf("-----------------------------------------\n");
	return g1;
}

void unit_write() {
	printf("--------- TESTING: write goals ----------\n");

	goal_t* g1 = create_goal("example goal 1", 1.1, 123, 1);
	goal_t* g2 = create_goal("example goal 2", 2.2, 234, 2);
	goal_t* g3 = create_goal("example goal 3", 3.3, 345, 3);
	goal_t* g4 = create_goal("example goal 4", 4.4, 456, 3);

	// wipe goals
	FILE* f = fopen("res/goals", "w");
	fclose(f);

	// write created goals
	write_goal(g2);
	write_goal(g4);
	write_goal(g1);
	write_goal(g3);

	printf("-----------------------------------------\n");
}

void unit_read() {
	printf("------ TESTING: read goals from file ----\n");
	FILE* f = fopen("res/goals", "r");

	goal_t* g;
	while ((g = read_goal(&f)) != NULL) {
		dump_goal(g);
		free(g);
	}

	printf("-----------------------------------------\n");
	fclose(f);
}

void unit_read_list() {
	printf("------- TESTING: read list --------------\n");
	glist_t* g = read_goals();
	dump_glist(g);
	printf("-----------------------------------------\n");
}

void port_notes() {
	// goal, commit, due, priority
	goal_t* arr[12];
	arr[0] = create_goal("plan workout regimen", 0.5, 1, 3);
	arr[1] = create_goal("Look into etherium block chain, how it differs from bitcoin", 3, 2, 3);
	arr[2] = create_goal("Look into smart contracts (+Uniswap)", 2, 3, 3);
	arr[3] = create_goal("Brush retainers", 0.5, 0, 2);
	arr[4] = create_goal("Buy shaving cream", 1, 0, 2);
	arr[5] = create_goal("Return genome book", 0.5, 50, 2);
	arr[6] = create_goal("Chart 4 year requirements, not including cs-track specific reqs", 2, 50, 5);
	arr[7] = create_goal("Look into pintos projects", 2, 50, 3);
	arr[8] = create_goal("Watch for gdb and valgrind updates for mac", 1, 50, 2);
	arr[9] = create_goal("Get book: Computer Systems: A Programmer's Perspective", 1, 50, 4);
	arr[10] = create_goal("Buy merch from the bookstore", 2, 50, 3);
	arr[11] = create_goal("Make appt for admission viewing", .25, 50, 3);
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
		write_goal(arr[i]);
	}
}

void unit_convert_commitment() {
	printf("---- Testing: convert commit -----------\n");
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
	printf("----------------------------------------\n");
}

void unit_convert_due_date() {
	printf("---- TESTING: convert due date ---------\n");
	printf("%lu : %lu\n", due("-d 1d"), (long)(time(0) + commitment("1d") * 60));
	printf("%lu : %lu\n", due("12/18/2021 03:39"), (long)(time(0) + commitment("2d") * 60));
	printf("----------------------------------------\n");
}

int main() {
	goal_t* g1 = unit_create_goal();
	free(g1);

	char* s = unit_string_from_goal();
	free(s);

	goal_t* g2 = unit_goal_from_string();
	free(g2);

	//unit_write();
	//unit_read();

	//port_notes();
	unit_read_list();
	
	unit_convert_commitment();
	unit_convert_due_date();

	return 0;
}
