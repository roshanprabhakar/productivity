#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <errno.h>
#include "interface.h"

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

    char* line = malloc(0);
    size_t cap = 0;
    // ssize_t len = getline(&line, &cap, stdin);
    getline(&line, &cap, stdin);

    free(new_goal);

    return 0;
}