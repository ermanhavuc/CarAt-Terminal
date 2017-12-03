//
// Created by erman on 03.12.2017.
//

#include <bits/types/FILE.h>
#include <stdio.h>
#include <termios.h>            //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO

#ifndef PROJECT2_HISTORY_H
#define PROJECT2_HISTORY_H

#endif //PROJECT2_HISTORY_H

int history(){

    int c;
    static struct termios oldt, newt;

    /*tcgetattr gets the parameters of the current terminal
    STDIN_FILENO will tell tcgetattr that it should write the settings
    of stdin to oldt*/
    tcgetattr( STDIN_FILENO, &oldt);
    /*now the settings will be copied*/
    newt = oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    newt.c_lflag &= ~(ICANON | ECHO);

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    /*This is your part:
    I choose 'e' to end input. Notice that EOF is also turned off
    in the non-canonical mode*/

    int index = 0;
    int lineNumber = getLineNumber();
    int waitFlag = 0;
    index = lineNumber + 1;

    while((c=getchar())!= '\n'){
        if (c == '\033') { // if the first value is esc
            getchar(); // skip the [
            switch(getchar()) { // the real value
                case 'A':
                    if(index > 0){
                        index--;
                        waitFlag = 0;
                    }
                    break;

                case 'B':
                    if(index < lineNumber + 2){
                        if (index == lineNumber){
                            waitFlag = 1;
                            index++;
                        }
                        else{

                            waitFlag = 0;
                            index++;
                        }
                    }
                    break;
            }
        }

        if(!waitFlag){
            printf("\33[2K\r");
            showCommand(index);
        }
        else{
            printf("\33[2K\r");
        }

        if (index == (lineNumber+2)){
            printf("\33[2K\r");
            showCommand(0);
            waitFlag = 0;
        }
    }

    /*restore the old settings*/
    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);

}

void addToHistory(char **input){

    FILE *history;
    history = fopen("../history.txt", "a");

    fprintf(history, "%s", input);

    fclose(history);
}

void showCommand(int index) {

    FILE *history = fopen("history.txt", "r");
    int count = 0;
    char line[128]; // or other suitable maximum line size

    if (history != NULL){
        while (fgets(line, sizeof line, history) != NULL){
            if (count == index){
                strtok(line,"\n");
                printf("%s",line);
                break;
            }
            else {
                count++;
            }
        }
        fclose(history);
    }
    else {
        printf("History file does not exist!");
    }
}

int getLineNumber(){

    int lines = 0;
    int ch = 0;
    FILE *history = fopen("history.txt", "r");

    if(history != NULL){

        while(!feof(history)){

            ch = fgetc(history);
            if(ch == '\n')
            {
                lines++;
            }
        }
        fclose(history);
    }

    return lines;
}