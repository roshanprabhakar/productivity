#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <stdio.h>
#include <errno.h>
#include "gutils.h"
#include "goals.h"
#include "constants.h"

int main(int argc, char* argv[]) {

    struct passwd *pw = getpwuid(getuid());
    const char *home_dir = pw->pw_dir;

    if (argc < 2) {
        printf("\tspecify a command or help\n");
        return 0;
    }

    if (!strcmp(argv[1], "init")) {
        
        char config_path[strlen(home_dir) + 9 + 6 + 6];
        strcpy(config_path, home_dir);
        
        strcat(config_path, "/.config");
        // printf("%s\n", config_path);

        errno = 0;
        int ret = mkdir(config_path, 0755);
        if (ret != 0 && errno != EEXIST) { // the error is something besides dir exists
            printf("\tfailed to initialize project\n\tconsult help for manual init instructions\n");
            return 0;
        }

        strcat(config_path, "/track");
        // printf("%s\n", config_path);

        errno = 0;
        ret = mkdir(config_path, 0755);
        if (ret != 0 && errno != EEXIST) {
            printf("\tfailed to initialize project\n\tconsult help for manual init instructions\n");
            return 0;
        }

        strcat(config_path, "/goals");

        errno = 0;
        ret = access(config_path, R_OK | W_OK);
        if (ret == -1 && errno == ENOENT) {
            fopen(config_path, "w");
        } else if (ret == -1) {
            printf("\tfailed to initialize project\n\tconsult help for manual init instructions\n");
            return 0;
        } else {
            printf("\tproject already initialized\n");
            return 0;
        }

        printf("\tproject successfully initialized\n");
        return 0;
    }

    // check if goals is initialized before continuing
    char goals_path[strlen(home_dir) + strlen("/.config/track/goals") + 1];
    strcpy(goals_path, home_dir);
    strcat(goals_path, "/.config/track/goals");

    errno = 0;
    int ret = access(goals_path, R_OK | W_OK);
    if (ret == -1 && errno == ENOENT) {
        printf("\tinitialize project with 'track init'\n");
        return 0;
    } else if (ret == -1) {
        printf("\tan unknown error occurred\n");
        return 0;
    }

    glist_t* goals = read_goals();
    if (!strcmp(argv[1], "display")) {
        display_goals(goals);
    }
    close_list(goals);
    return 0;

}