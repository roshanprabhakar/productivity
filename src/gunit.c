#include <stdio.h>
#include <string.h>
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
 
glist_t* unit_write_and_read_goals() {
	printf("--------- TESTING: read, write goals ----\n");
	goal_t* g1 = create_goal("example goal 1", 1.1, 123, 1);
	goal_t* g2 = create_goal("example goal 2", 2.2, 234, 2);
	goal_t* g3 = create_goal("example goal 3", 3.3, 345, 3);
	goal_t* g4 = create_goal("example goal 4", 4.4, 456, 3);
	
	write_goal(g1);
	write_goal(g2);
	write_goal(g3);
	write_goal(g4);

	FILE* f = fopen("res/goals", "r");
	
	goal_t* g1r = read_goal(&f);
	dump_goal(g1r);
	goal_t* g2r = read_goal(&f);
	dump_goal(g2r);
	goal_t* g3r = read_goal(&f);
	dump_goal(g3r);
	goal_t* g4r = read_goal(&f);
	dump_goal(g4r);
	
	printf("-----------------------------------------\n");
	return NULL;
}

int main() {
	goal_t* g1 = unit_create_goal();
	free(g1);

	char* s = unit_string_from_goal();
	free(s);

	goal_t* g2 = unit_goal_from_string();
	free(g2);

	unit_write_and_read_goals();

	return 0;
}
