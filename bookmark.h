//
// Created by erman on 01.12.2017.
//

#ifndef PROJECT2_BOOKMARK_H
#define PROJECT2_BOOKMARK_H

#endif //PROJECT2_BOOKMARK_H

#include <bits/types/FILE.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void add(char line[]) ;
void delete(int index) ;
void list();
void run(int index);
void removeDQuotes(char *source);

int bookmark(char **args) {

    if(args[1] == '\0'){
        printf("asd");
    }
    else if(strcmp(args[1], "-l") == 0){
        list();
    }
    else if(strcmp(args[1], "-i") == 0){
        run(strtol(args[2], NULL, 10));
    }
    else if(strcmp(args[1], "-d") == 0){
        delete(strtol(args[2], NULL, 10));
    }
    else {
        removeDQuotes(args[1]);
        add(args[1]);
    }

}

void add(char line[]) {

    FILE *bookmarks;
    bookmarks = fopen("../bookmarks.txt", "a");

    fprintf(bookmarks, "%s\n", line);

    fclose(bookmarks);
}

void delete(int index){

    FILE *file = fopen("../bookmarks.txt", "r");
    FILE *filetemp = fopen("__tempfile__", "w");

    char line[128];
    int countline = 0;

    while(fgets(line, 255, file) != NULL){
        if(countline != index){
            fputs(line, filetemp);
        }

        countline++;
    }

    fclose(file);
    fclose(filetemp);

    rename("__tempfile__", "../bookmarks.txt");
}

void list() {

    FILE *bookmarks = fopen("../bookmarks.txt", "r");
    int count = 0;
    char line[128]; // or other suitable maximum line size

    if ( bookmarks != NULL ) {
        while (fgets(line, sizeof line, bookmarks) != NULL) {
            strtok(line, "\n");
            printf("%d \"%s\"\n",count,line);
            count++;
        }
        fclose(bookmarks);
    }
    else {
        printf("Bookmarks file does not exist!");
    }

}

void run(int index) {

    FILE *bookmarks = fopen("../bookmarks.txt", "r");
    int count = 0;
    char line[128]; // or other suitable maximum line size

    if ( bookmarks != NULL ){
        while (fgets(line, sizeof line, bookmarks) != NULL){
            if (count == index){
                strtok(line, "\n");
                break;
            }
            else {
                count++;
            }
        }
        fclose(bookmarks);
    }
    else {
        printf("Bookmarks file does not exist!");
    }

    /*
     * run
     */
}

void removeDQuotes(char *source) {

    char* i = source;
    char* j = source;

    while(*j != 0) {
        *i = *j++;
        if(*i != '"')
            i++;
    }
    *i = 0;
}