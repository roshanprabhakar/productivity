#ifndef CONSTANTS
#define CONSTANTS

/* PROJECT WIDE CONSTANTS */
extern const char* goals_ext;

extern const unsigned int MAX_INLINE_ARGS;
extern const int MAX_LABEL_SIZE;
extern const int MAX_PRINT_SIZE;
extern const int MAX_GOAL_SIZE;
extern const int START_ARG_LINE_SIZE;

extern const time_t WARNING_TIME;

// return user home directory
const char* home_dir();

// returns absolute path of goals file
char* goals_path();

// Priority levels
/* KEEP designated for goals intentially left incomplete
 */
enum priority_level {URGENT = 0, WORK = 10, PERSONAL = 20, MENIAL = 30, KEEP = 40};

/* given an array of command line arguments, extract the value of flag, parsed according
 * to a given type encoding. The result must be freed.
 */
void* extract_value(int argc, char* argv[], char* flag, char type);

/* Given a sequence a string to parse into arguments, create a dynamically allocated array
 * of dynamically allocated strings, each a component of argv.
 * - each element of argv is separated from adjacent elements with '*' characters.
 */
char** getargv(char* in, int* argc, char* delim); 

#endif