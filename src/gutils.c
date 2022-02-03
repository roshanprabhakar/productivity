#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "constants.h"
#include "gutils.h"
#include "goals.h"

void clear_all() {
	FILE* f = fopen(goals_src, "w");
	if (fclose(f)) {
		printf("Could not clear %s\n", goals_src);
	}
}
 
void update_goals(glist_t* new_goals) {
	clear_all();
	glist_t* cur = new_goals;
	while (cur != NULL) {
		write_goal(cur->cur);
		cur = cur->next;
	}
}

/**
 * pointer to the goal with the specified id on found, otherwise NULL
 */
goal_t* find_goal(glist_t* list, int id) {
	while (list != NULL && list->cur->ID != id) list = list->next;
	if (list != NULL) return list->cur;
	return NULL;
}

void close_goal(glist_t* goals, int id) {
	/* TODO */
	return;
}

void divide(int width, int color) {
	printf("\x1b[%dm", color);
	printf("+");
	for (int i = 0; i < width; i++) printf("-");
	printf("+\n");
	printf("\033[0m");
}

// banner time commitments rounded down to the nearest largest denominator time measurement
void display_goals(glist_t* list) {
	time_t now = time(0);
	divide(MAX_PRINT_SIZE - 2, 34);
	glist_t* cur = list;
	while (cur != NULL) {
		if (cur->cur->is_banner) {
			char* commit = commitment_to_str(cur->cur->hrs_commit);
			printf("\x1b[%dm", 32);
			printf("   %d\t", cur->cur->ID);
			printf("\x1b[%dm", 36);
			printf("%.6s\t", commit);
			printf("\033[0m");
			printf("%.80s\n", cur->cur->label);
			free(commit);
		}
		cur = cur->next;
	}
	divide(MAX_PRINT_SIZE - 2, 34);
	printf("\n");
	cur = list;
	while (cur != NULL) {
		if (!cur->cur->is_banner) {
			char* commit = commitment_to_str(cur->cur->hrs_commit);
			printf("\x1b[%dm", 32);
			printf("   %d\t", cur->cur->ID);
			if (cur->cur->due - now < WARNING_TIME) {
				printf("\x1b[%dm", 31);
			} else {
				printf("\x1b[%dm", 35);
			}
			printf("%s\t ", epoch_to_readable(cur->cur->due));
			printf("\x1b[%dm", 36);
			printf("%s\n", commit);
			printf("\033[0m");
			printf("\t%.100s\n", cur->cur->label);
			free(commit);
			// divide(MAX_PRINT_SIZE / 2 - 1);
			printf("\n");
		}
		cur = cur->next;
	}
	divide(MAX_PRINT_SIZE - 2, 34);
}

void display_goal(goal_t* goal) {
	divide(MAX_PRINT_SIZE / 2 - 1, 34);
	printf("\x1b[%dm", 32);
	printf("ID: \t\t%d\n", goal->ID);
	printf("\x1b[%dm", 31);
	printf("Priority: \t%d\n", goal->priority);
	printf("\x1b[%dm", 35);
	printf("Due date: \t%s\n", epoch_to_readable(goal->due));
	printf("\x1b[%dm", 36);
	char* commit = commitment_to_str(goal->hrs_commit);
	printf("Commitment: \t%s\n\n", commit);
	printf("\033[0m");
	size_t len = strlen(goal->label);
	for (int i = 0; i < len; i++) {
		printf("%c", goal->label[i]);
		if (i % (MAX_PRINT_SIZE / 2) == 0 && i != 0) {
			printf("\n");
		}
	}
	printf("\n");
	free(commit);
	divide(MAX_PRINT_SIZE / 2 - 1, 34);
}


/* SUPPORTED UPDATES
 * - update time commitment (add, remove, set) (-ca) (--commitment-append), (-cs) (--commitment-set)
 * - update banner status (move to/from banners) (-sb) (--set-banner), (-rb) (--remove-banner)
 * - update due date (add time unit, remove time unit, reset time unit) (-da) (--date-append), (-ds) (--date-set)
 * - set priority (-sp) (--set-priority)
 */

void update_hrs_commit(goal_t* goal, int argc, char* argv[]);
void update_is_banner(goal_t* goal, int argc, char* argv[]);
void update_due(goal_t* goal, int argc, char* argv[]);
void update_priority(goal_t* goal, int argc, char* argv[]);

void update_goal(goal_t* goal, int argc, char* argv[]) {
	update_hrs_commit(goal, argc, argv);
	update_is_banner(goal, argc, argv);
	update_due(goal, argc, argv);
	update_priority(goal, argc, argv);
}

void update_priority(goal_t* goal, int argc, char* argv[]) {
	int* s_arg1 = extract_value(argc, argv, "-sp", 'i');
	if (s_arg1) {
		goal->priority = *s_arg1;
		free(s_arg1);
	}

	int* s_arg2 = extract_value(argc, argv, "--set-priority", 'i');
	if (s_arg2) {
		goal->priority = *s_arg2;
		free(s_arg2);
	}
}

void update_due(goal_t* goal, int argc, char* argv[]) {
	char* a_arg1 = extract_value(argc, argv, "-da", 's');
	if (a_arg1) {
		goal->due += commitment(a_arg1) * 60;
		free(a_arg1);
	}

	char* a_arg2 = extract_value(argc, argv, "--date-append", 's');
	if (a_arg2) {
		goal->due += commitment(a_arg2) * 60;
		free(a_arg2);
	}

	char* s_arg1 = extract_value(argc, argv, "-ds", 's');
	if (s_arg1) {
		goal->due = readable_to_epoch(s_arg1);
		free(s_arg1);
	}

	char* s_arg2 = extract_value(argc, argv, "--date-set", 's');
	if (s_arg2) {
		goal->due = readable_to_epoch(s_arg2);
		free(s_arg2);
	}
}

void update_is_banner(goal_t* goal, int argc, char* argv[]) {
	bool* b_sarg1 = extract_value(argc, argv, "-sb", 'b');
	if (b_sarg1) {
		if (*b_sarg1) goal->is_banner = true;
		free(b_sarg1);
	}

	bool* b_sarg2 = extract_value(argc, argv, "--set-banner", 'b');
	if (b_sarg2) {
		if (*b_sarg2) goal->is_banner = true;
		free(b_sarg2);
	}

	bool* b_rarg1 = extract_value(argc, argv, "-rb", 'b');
	if (b_rarg1) {
		if (*b_rarg1) goal->is_banner = false;
		free(b_rarg1);
	}

	bool* b_rarg2 = extract_value(argc, argv, "--remove-banner", 'b');
	if (b_rarg2) {
		if (*b_rarg2) goal->is_banner = false;
		free(b_rarg2);
	}
}

void update_hrs_commit(goal_t* goal, int argc, char* argv[]) {
	char* l_sarg1 = extract_value(argc, argv, "-cs", 's');
	if (l_sarg1) {
		goal->hrs_commit = commitment(l_sarg1) / 60.0;
		free(l_sarg1);
	}

	char* l_sarg2 = extract_value(argc, argv, "--commitment-set", 's');
	if (l_sarg2) {
		goal->hrs_commit = commitment(l_sarg2) / 60.0;
		free(l_sarg2);
	}

	char* l_arg1 = extract_value(argc, argv, "-ca", 's');
	if (l_arg1) {
		double new_commit = goal->hrs_commit + commitment(l_arg1) / 60.0;
		if (new_commit < 0) new_commit = 0;
		goal->hrs_commit = new_commit;
		free(l_arg1);
	}

	char* l_arg2 = extract_value(argc, argv, "--commitment-append", 's');
	if (l_arg2) {
		double new_commit = goal->hrs_commit + commitment(l_arg2) / 60.0;
		if (new_commit < 0) new_commit = 0;
		goal->hrs_commit = new_commit;
		free(l_arg2);
	}
}
