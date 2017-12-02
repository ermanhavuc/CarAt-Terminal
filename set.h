//
// Created by erman on 01.12.2017.
//

#ifndef PROJECT2_SET_H
#define PROJECT2_SET_H

#endif //PROJECT2_SET_H

//
// Created by erman on 01.12.2017.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void removeSpaces(char* source);

int set(int argc, char **argv)
{
    char *mainArg = "export ";
    int i = 1;

    while(argv[i] != NULL){
        char * new_str ;
        if((new_str = malloc(strlen(mainArg)+strlen(argv[i])+1)) != NULL){
            new_str[0] = '\0';
            strcat(new_str,mainArg);
            strcat(new_str,argv[i]);
        }
        mainArg = new_str;
        i++;
    }

    removeSpaces(mainArg);

    char **input = mainArg;

    if (execvp(*input, input) < 0) {     /* execute the command  */
        printf("*** ERROR: exec failed\n");
        exit(1);
    }

    //printf("%s\n",mainArg);

    //execute(mainArg);
    printf("asd\n");

    /*printf("%s\n",mainArg);

    printf("%s %s",argv[1],argv[3]);

    printf("%d",setenv(argv[1],argv[3],0));
    printf("%s",getenv(argv[1]));
    */
}

void removeSpaces(char *source) {
    char* i = source;
    char* j = source;
    int c = 0;

    while(*j != 0)
    {
        *i = *j++;
        if(*i != ' ')
            i++;
        else if( c == 0 ){
            i++;
            c = 1;
        }
    }
    *i = 0;
}

void execute(char **argv)
{
    pid_t  pid;
    int    status;

    if ((pid = fork()) < 0) {     /* fork a child process           */
        printf("*** ERROR: forking child process failed\n");
        exit(1);
    }
    else if (pid == 0) {          /* for the child process:         */
        if (execvp(*argv, argv) < 0) {     /* execute the command  */
            printf("*** ERROR: exec failed\n");
            exit(1);
        }
    }
    else {                                  /* for the parent:      */
        while (wait(&status) != pid)       /* wait for completion  */
            ;
    }
}