#ifndef GOALS
#define GOALS

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// Banner goals carry irrelevant data in hrs_commit, due, priority
// Banner goals are of priority Banner, no designated int value
typedef struct {
	char* label;
	double hrs_commit;
	time_t due;
	int priority;
	bool is_banner;
	int ID;
} goal_t;


/* The fundamental data structure behind goal management
 */
typedef struct list_elem {
	goal_t* cur;
	struct list_elem* next;
} glist_t;


/* * * parsing api * * */

/* converting a time interval to seconds, according to the implementation spec
 */
double commitment(char*);

/* convert a commitment time in hours to its string representation.
 * The returned string must be freed.
 */
char* commitment_to_str(double);

/* converts a given human-readable time (according to spec) to unix epoch time
 */
time_t readable_to_epoch(char*);

/* converts a given time_t to a human readable string (according to the spec)
 */
char* epoch_to_readable(time_t);

/* Generates dynamically allocated goal_t object from respective elements.
 */
goal_t* create_goal(char* goal, double commitment, time_t due, int priority, bool is_banner, int ID);
goal_t* parse_goal(char* goal, char* commitment, char* due, char* priority, char* is_banner, char* ID);


/* Creates dynamically allocated goal_t from a given string representing the 
 * the following sequential data with no null padding:
 * 1 int, 1 long, 1 double, n chars, terminator
 */
goal_t* goal_from_string(char*);


/* Generates dynamically allocated string representation of the given goal,
 * formatted according to the specification required by goal_from_string.
 */
char* string_from_goal(goal_t*);


/* Generates a list of goal objects sorted in priority order.
 */
glist_t* read_goals();

/* Insert the passed goal into the passed glist
 */
void insert_goal(goal_t*, glist_t**);

/* Free heap resources
 */
void free_goal(goal_t*);
void free_list(glist_t*);

/* File IO
 */
goal_t* read_goal(FILE**);
void write_goal(goal_t*);
void write_list(glist_t*);

/* --- debug tools --- */
void dump_goal(goal_t*);
void dump_glist(glist_t*);

#endif