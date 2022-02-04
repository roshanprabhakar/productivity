#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "interface.h"
#include "constants.h"
#include "gutils.h"
#include "goals.h"

int init_sequence(const char* home_dir) {
    char config_path[strlen(home_dir) + 9 + 6 + 6];
    strcpy(config_path, home_dir);
    
    strcat(config_path, "/.config");
    // printf("%s\n", config_path);

    errno = 0;
    int ret = mkdir(config_path, 0755);
    if (ret != 0 && errno != EEXIST) { // the error is something besides dir exists
        printf("\tfailed to initialize project\n\tconsult help for manual init instructions\n");
        return -1;
    }

    strcat(config_path, "/track");
    // printf("%s\n", config_path);

    errno = 0;
    ret = mkdir(config_path, 0755);
    if (ret != 0 && errno != EEXIST) {
        printf("\tfailed to initialize project\n\tconsult help for manual init instructions\n");
        return -1;
    }

    strcat(config_path, "/goals");

    errno = 0;
    ret = access(config_path, R_OK | W_OK);
    if (ret == -1 && errno == ENOENT) {
        fopen(config_path, "w");
    } else if (ret == -1) {
        printf("\tfailed to initialize project\n\tconsult help for manual init instructions\n");
        return -1;
    } else {
        printf("\tproject already initialized\n");
        return 0;
    }

    printf("\tproject successfully initialized\n");
    return 0;
}

int add_goal(glist_t* list) {

    goal_t* new_goal = calloc(sizeof(goal_t), 1);

    char* label = malloc(0);
    size_t label_cap = 0;
    getline(&label, &label_cap, stdin);
    new_goal->label = label;

    char* args = malloc(0);
    size_t args_cap = 0;
    getline(&args, &args_cap, stdin);

    int argc;
    char** argv = getargv(args, &argc, " ");
    free(args);
    update_goal(new_goal, argc, argv);
    
    insert_goal(new_goal, &list);

    return 0;
}