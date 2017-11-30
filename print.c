//
// Created by erman on 30.11.2017.
//

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv, char **envp)
{
    if (argc == 2) {
        for (char **env = envp; *env != 0; env++) {
            char *thisEnv = *env;
            if (strstr(thisEnv,argv[1]) != NULL) {

                thisEnv += strlen(argv[1])+1;
                printf("%s",thisEnv);
                break;
            }
        }

    } else if (argc > 2) {
        printf("Too many arguments!");

    } else {
        for (char **env = envp; *env != 0; env++) {
            char *thisEnv = *env;
            printf("%s\n", thisEnv);
        }
    }
    return 0;
}