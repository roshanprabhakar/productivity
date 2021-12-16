#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_LABEL_SIZE 100
const extern int MAX_GOAL_SIZE;

typedef struct {
	char* label;
	double hrs_commit;
	unsigned long due;
	unsigned int priority;
} goal_t;

/* The fundamental data structure behind goal management: this structure 
 * stores the data needed for goal manipulation.
 */
typedef struct list_elem {
	goal_t* cur;
	struct list_elem* next;
} glist_t;

/* Generates dynamically allocated goal_t object from respective elements.
 */
goal_t* create_goal(char* goal, double commitment, long due, int priority);

/* Creates dynamically allocated goal_t from a given string representing the 
 * the following sequential data with no null padding:
 * 1 int, 1 long, 1 double, n chars, terminator
 */
goal_t* goal_from_string(char* str);

/* Generates dynamically allocated string representation of the given goal,
 * formatted according to the specification required by goal_from_string.
 */
char* string_from_goal(goal_t* g);

/* Generates a list of goal objects sorted in priority order. The priority
 * queue structure is internally supported with a linked list
 */
glist_t* readlist(FILE* f);

/* File IO
 */
goal_t* read_goal(FILE** f);
void write_goal(goal_t* g);

/* --- debug tools --- */
void dump_goal(goal_t* goal);
void dump_glist(glist_t* list);
