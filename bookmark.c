//
// Created by erman on 30.11.2017.
//

#include <bits/types/FILE.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void add(char line[]) ;
void delete(int index) ;
void list();
void run(int index);

int main(int argc, char *argv[]) {


}

void add(char line[]) {

    FILE *bookmarks;
    bookmarks = fopen("../bookmarks.txt", "a");

    fprintf(bookmarks, line);

    fclose(bookmarks);
}

void delete(int index) {

    FILE *original, *copy;
    char filename[17] = "../bookmarks.txt";
    char ch = ' ';
    int temp = 1;

    //open file in read mode
    original = fopen(filename, "r");

    //open new file in write mode
    copy = fopen("copy.txt", "w");

    while (ch != EOF)
    {
        ch = getc(original);
        //except the line to be deleted
        if (temp != index+1)
        {
            //copy all lines in file replica.c
            putc(ch, copy);
        }
        if (ch == '\n')
        {
            temp++;
        }
    }

    truncate(copy,1);

    if(temp <= index){
        printf("There is no command at %d !", index);
    }

    fclose(original);
    fclose(copy);

    remove(filename);

    //rename the file replica.c to original name
    rename("copy.txt", filename);
}

void list() {

    FILE *bookmarks = fopen("../bookmarks.txt", "r");
    int count = 0;
    char line[128]; // or other suitable maximum line size
    if ( bookmarks != NULL )
    {
        while (fgets(line, sizeof line, bookmarks) != NULL) // read a line
        {
            strtok(line, "\n");
            printf("%d \"%s\"\n",count,line);
            count++;
        }
        fclose(bookmarks);
    }
    else
    {
        printf("Bookmarks file does not exist!");
    }

}

void run(int index) {

    FILE *bookmarks = fopen("../bookmarks.txt", "r");
    int count = 0;
    char line[128]; // or other suitable maximum line size
    if ( bookmarks != NULL )
    {
        while (fgets(line, sizeof line, bookmarks) != NULL) // read a line
        {
            if (count == index)
            {
                strtok(line, "\n");
                break;
            }
            else
            {
                count++;
            }
        }
        fclose(bookmarks);
    }
    else
    {
        printf("Bookmarks file does not exist!");
    }

    /*
     * run
     * run
     * run
     */
}