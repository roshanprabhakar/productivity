#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "constants.h"
#include "gutils.h"

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

goal_t* find_goal(glist_t* list, int id) {
	while (list != NULL && list->cur->ID != id) list = list->next;
	return list->cur;
}

void close_goal(glist_t* goals, int id) {
	/* TODO */
	return;
}

// doesn't include label, total len 66
void pretty_print(goal_t* goal, int width) {
	printf("| %s", goal->label);
	for (int i = 0; i < width - strlen(goal->label) - 1; i++) printf(" ");
	printf("|\n");
	
	char out[width + 2];
	
	int end = sprintf(out, "%s", "| hrs: ");
	out[end] = ' ';

	end += sprintf(out + end, "%03.2f", goal->hrs_commit);
	out[end] = ' ';

	end += sprintf(out + 7 + 6, "%s", "   due: ");
	out[end] = ' ';

	char* r_due = epoch_to_readable((time_t)goal->due); 
	r_due[strlen(r_due) - 1] = ' ';
	int len = strlen(r_due);
	end += sprintf(out + 7 + 6 + 8, "%s", r_due);
	out[end - 1] = ' ';
	
	end += sprintf(out + 7 + 6 + 8 + len, "%s", "    priority: ");
	out[end] = ' ';

	end += sprintf(out + 7 + 6 + 8 + len + 14, "%02d", goal->priority);
	out[end] = '\0';

	printf("%s", out);
	for (int i = 0; i < width - strlen(out) + 1; i++) printf(" ");
	printf("|\n");
}

void display_goals() {
	glist_t* list = read_goals();
	glist_t* cur;
	
	dump_glist(list);

	// search through all goals for banners
	cur = list;
	
	while (cur != NULL) {
		if (cur->cur->is_banner) {
			pretty_print(cur->cur, MAX_PRINT_SIZE);
		}
		cur = cur->next;
	}



	// glist_t* list = read_goals();
	// glist_t* goal;

	// goal = list;
	// int max_width = 80;
	// while (goal != NULL) {
	// 	int len = strlen(goal->cur->label);
	// 	if (len > max_width) {
	// 		max_width = len;
	// 	}
	// 	goal = goal->next;
	// }
	// printf("+");
	// for (int i = 0; i < max_width; i++) printf("-");
	// printf("+\n");
	// goal = list;
	// while (goal != NULL) {
	// 	if (goal->cur->is_banner) {
	// 		printf("|%s", goal->cur->label);
	// 		for (int i=0;i<MAX_LABEL_SIZE-strlen(goal->cur->label);i++) printf(" ");
	// 		printf("|\n");
	// 	}
	// 	goal = goal->next;
	// }
	// printf("+");
	// for (int i = 0; i < max_width; i++) printf("-");
	// printf("+\n");

	// goal = list;
	// while (goal != NULL) {
	// 	if (!goal->cur->is_banner) {
	// 		pretty_print(goal->cur, max_width);
	// 	}
	// 	goal = goal->next;
	// }
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
		goal->due += commitment(a_arg1);
		free(a_arg1);
	}

	char* a_arg2 = extract_value(argc, argv, "--date-append", 's');
	if (a_arg2) {
		goal->due += commitment(a_arg2);
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
		goal->hrs_commit = commitment(l_sarg1);
		free(l_sarg1);
	}

	char* l_sarg2 = extract_value(argc, argv, "--commitment-set", 's');
	if (l_sarg2) {
		goal->hrs_commit = commitment(l_sarg2);
		free(l_sarg2);
	}

	char* l_arg1 = extract_value(argc, argv, "-ca", 's');
	if (l_arg1) {
		goal->hrs_commit += commitment(l_arg1);
		free(l_arg1);
	}

	char* l_arg2 = extract_value(argc, argv, "--commitment-append", 's');
	if (l_arg2) {
		goal->hrs_commit += commitment(l_arg2);
		free(l_arg2);
	}
}
