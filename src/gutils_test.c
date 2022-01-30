#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "gutils.h"
#include "goals.h"
#include "constants.h"

void port_notes() {
	// goal, commit, due, priority
	goal_t* arr[12];
	arr[0] = create_goal("plan workout regimen", 0.5, 1, 3, 1);
	arr[1] = create_goal("Look into etherium block chain, how it differs from bitcoin", 3, 2, 3, 1);
	arr[2] = create_goal("Look into smart contracts (+Uniswap)", 2, 3, 3, 1);
	arr[3] = create_goal("Brush retainers", 0.5, 0, 2, 1);
	arr[4] = create_goal("Buy shaving cream", 1, 0, 2, 0);
	arr[5] = create_goal("Return genome book", 0.5, 50, 2, 0);
	arr[6] = create_goal("Chart 4 year requirements, not including cs-track specific reqs", 2, 50, 5, 1);
	arr[7] = create_goal("Look into pintos projects", 2, 50, 3, 1);
	arr[8] = create_goal("Watch for gdb and valgrind updates for mac", 1, 50, 2, 0);
	arr[9] = create_goal("Get book: Computer Systems: A Programmer's Perspective", 1, 50, 4, 0);
	arr[10] = create_goal("Buy merch from the bookstore", 2, 50, 3, 0);
	arr[11] = create_goal("Make appt for admission viewing", .25, 50, 3, 1);
	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++) {
		write_goal(arr[i]);
	}
}

// depends on res containing at least 5 goal entries
void unit_find_goal() {
    printf("find_goal in glist {\n");
     
    glist_t* list = read_goals();
    for (int i = 1; i < 6; i++) {
        printf("finding id %d: ", i);
		goal_t* g = find_goal(list, i);
		if (g != NULL) printf(" ...found!\n");
		else printf(" ...nonexistent entry\n");
    }

    printf("}\n");
}

void apply_change(goal_t* goal, int argc, char* args) {
	char** argv = getargv(args, argc);
	update_goal(goal, argc, argv);
	printf("applying change:\n");
	for (int i = 0; i < argc; i++) {
		printf("%s ", argv[i]);
		free(argv[i]);
	}
	printf("\n");
	printf("updated goal:\n");
	dump_goal(goal);
	printf("\n");
	free(argv);
}

void unit_update_goal() {
	printf("\nupdate_goal {\n");
	
	goal_t* g = create_goal("example goal 1", 1.5, (time_t) 10000000, 1, false);
	g->priority = 10;

	printf("INITIAL GOAL: \n");
	dump_goal(g);
	printf("\n");

	printf("setting time commitment...\n");
	apply_change(g, 3, "gunit*-ca*10d");
	apply_change(g, 3, "gunit*--commitment-append*-10d");
	apply_change(g, 3, "gunit*-cs*2mi");
	apply_change(g, 3, "gunit*--commitment-set*4mi");

	printf("setting is banner...\n");
	apply_change(g, 2, "gunit*-sb");
	apply_change(g, 2, "gunit*--remove-banner");
	apply_change(g, 2, "gunit*--set-banner");
	apply_change(g, 2, "gunit*-rb");

	printf("setting due date...\n");
	apply_change(g, 3, "gunit*-da*2days");
	apply_change(g, 3, "gunit*--date-set*10/10/2021 12:00");
	apply_change(g, 3, "gunit*--date-append*12hrs");
	apply_change(g, 3, "gunit*-ds*12/23/2021 18:00");

	printf("setting priority...\n");
	apply_change(g, 3, "gunit*-sp*1");
	apply_change(g, 3, "gunit*--set-priority*2");

	free(g);
	printf("}\n");
}

void unit_display_goals() {
	printf("\ndisplay_goals {\n");
	glist_t* goals = read_goals();
	display_goals(goals);
	printf("\n}\n");
}

int main() {
	// unit_find_goal(); // WORKS
	// unit_update_goal(); // WORKS
	unit_display_goals();
	//port_notes();
	return 0;
}