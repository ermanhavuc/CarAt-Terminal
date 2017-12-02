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
    int b = 0;

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

                    char * new_str ;

                    if((new_str = malloc(strlen(s)+strlen(args[0])+2)) != NULL){
                        new_str[0] = '\0';
                        strcat(new_str,s);
                        strcat(new_str,"/");
                        strcat(new_str,args[0]);
                    }

                    s = new_str;

                    args += 1;

                    char *const parmList[] = {s, argv, NULL};

                    e_process(s,parmList,background);

                    b = 1;

                    break;
                }
            }
            closedir (dir);

        } else {

            perror ("");
            return EXIT_FAILURE;
        }

        if(b == 1) {
            break;
        }

        s = p + 1;

    } while (p != NULL);
}