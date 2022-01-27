#ifndef GUTILS
#define GUTILS

#include <stdlib.h>
#include "goals.h"

/* * File IO * */
void update_goals(glist_t* new_goals);
void clear_all();

/* Find the goal associated with id in list
 */
goal_t* find_goal(glist_t* list, int id);

/* Update a goal according to argv configuration.
 */
void update_goal(goal_t* goal, int arc, char* argv[]);

/* Mark a goal as comlete
 * - update progress
 * - remove goal from goal tracking
 */
void close_goal(glist_t* goals, int id);

/* Writing goal data to console */
void display_goals();
void pretty_print(goal_t*, int);

#endif