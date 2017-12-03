//
// Created by erman on 01.12.2017.
//

#ifndef PROJECT2_SET_H
#define PROJECT2_SET_H

#endif //PROJECT2_SET_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void removeSpaces(char* source);

int set(int argc, char **argv)
{
    if(argv[1] == '\0' || strcmp(argv[2],"=") || argv[3] == '\0' || check_if_int(argv[1]) || check_if_int(argv[3])){
        printf("Wrong arguments!");
    }
    else {
        setenv(argv[1],argv[3],0);
    }
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

int check_if_int(char str[]){
    int i,lnt=strlen(str);
    for(i=0;i<lnt;i++){
        if(!(str[i]>=48&&str[i]<=57)){
            return 0;
        }
    }
    return 1;
}