#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include "gutils.h"
#include "goals.h"
#include "constants.h"
#include "interface.h"

int main(int argc, char* argv[]) {

    const char* home_directory = home_dir();

    if (argc < 2) {
        printf("\tspecify a command or help\n");
        return 0;
    }

    if (!strcmp(argv[1], "init")) {
        init_sequence(home_directory);
        return 0;
    }

    // check if goals is initialized before continuing
    char* goals_src = goals_path();

    errno = 0;
    int ret = access(goals_src, R_OK | W_OK);
    free(goals_src);

    if (ret == -1 && errno == ENOENT) {
        printf("\tinitialize project with 'track init'\n");
        return 0;
    } else if (ret == -1) {
        printf("\tan unknown error occurred\n");
        return 0;
    }

    // setup list for all utilities
    glist_t* goals = read_goals();


    if (!strcmp(argv[1], "display")) {
        display_goals(goals);
    } else if (!strcmp(argv[1], "add")) {
        add_goal(goals);
    }

    

    // clean up resources
    free_list(goals);
    return 0;

}