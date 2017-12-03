//
// Created by erman on 01.12.2017.
//

#ifndef PROJECT2_PRINT_H
#define PROJECT2_PRINT_H

#endif //PROJECT2_PRINT_H

//
// Created by erman on 30.11.2017.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int print(int argc, char **argv, char **envp)
{
    if (argc == 1 ) {
        if (getenv(argv[1]) == NULL) {
            printf("There is no environment variable named as %s.", argv[1]);
        } else {
            printf("%s",getenv(argv[1]));
        }

    } else if (argc > 1) {
        printf("Too many arguments!");

    } else {
        for (char **env = envp; *env != 0; env++) {
            char *thisEnv = *env;
            printf("%s\n", thisEnv);
        }
    }
    return 0;
}