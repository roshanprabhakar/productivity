#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "gutils.h"
#include "goals.h"
#include "constants.h"

/** 
 * Not in need of testing:
 * - update_goals
 * - clear_all
 * - 
 */

// depends on res containing at least 5 goal entries
void unit_find_goal() {
    printf("find_goal in glist {\n");
     
    glist_t* list = read_goals();
    for (int i = 1; i < 6; i++) {
        printf("finding id %d: ", i);
        if (find_goal(list, i) != NULL) {
            printf(" ...found!");
        } else {
            printf("NULL returned");
            exit(1);
        }
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
	free(argv);
}

void unit_update_goal() {
	printf("\nupdate_goal {\n");
	
	goal_t* g = create_goal("example goal 1", 1.5, (time_t) 10000000, 1, false);
	g->priority = 10;

	printf("INITIAL GOAL: \n");
	dump_goal(g);

	printf("time commitment...\n");
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
	display_goals();
	printf("}\n");
}

int main() {
	unit_find_goal();
	unit_update_goal();
	unit_display_goals();
	return 0;
}