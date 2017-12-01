//
// Created by erman on 01.12.2017.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void RemoveSpaces(char* source);

int main(int argc, char **argv)
{
    char *mainArg = "";
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
    printf("%s",mainArg);

    RemoveSpaces(mainArg);

    putenv(mainArg);
}

void RemoveSpaces(char *source) {
    char* i = source;
    char* j = source;
    while(*j != 0)
    {
        *i = *j++;
        if(*i != ' ')
            i++;
    }
    *i = 0;
}