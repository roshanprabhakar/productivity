#ifndef INTERFACE
#define INTERFACE

#include "goals.h"

// Local project setup
int init_sequence(const char* home_dir);

// Interface to add to the todo list
/* Goal initialization is as follows:
 * - goal is initialized with NULL values
 * - line 1 stdin: update rules from null
 * - line 2 stdin: label
 */
int add_goal(glist_t* list);

#endif