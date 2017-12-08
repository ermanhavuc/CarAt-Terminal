//
// Created by erman on 02.12.2017.
//

#ifndef PROJECT2_EXECCOMMAND_H
#define PROJECT2_EXECCOMMAND_H

#endif //PROJECT2_EXECCOMMAND_H

void execCommand(char **args, int background){

    char *dup = strdup(getenv("PATH"));
    char *s = dup;
    char *p = NULL;
    int flag = 0;

    do {
        p = strchr(s, ':');

        if (p != NULL) {
            p[0] = 0;
        }
        DIR *dir;
        struct dirent *ent;

        if ((dir = opendir (s)) != NULL) {

            while ((ent = readdir (dir)) != NULL) {

                if(strcmp(ent->d_name,args[0]) == 0){

                    char * new_str;

                    if((new_str = malloc(strlen(s)+strlen(args[0])+2)) != NULL){
                        new_str[0] = '\0';
                        strcat(new_str,s);
                        strcat(new_str,"/");
                        strcat(new_str,args[0]);
                    }

                    s = new_str;

                    char **argv = args + 1;

                    //printf("%s", argv[1]);

                    char *const parmList[] = {s, *argv, NULL};


                    e_process(s,parmList,background);

                    flag = 1;

                    break;
                }
            }
            closedir (dir);

        } else {

            perror ("");
            return EXIT_FAILURE;
        }

        if(flag == 1) {
            break;
        }

        s = p + 1;

    } while (p != NULL);

    if( flag == 0){
        printf("There is no command named as %s",args[0]);
    }
}