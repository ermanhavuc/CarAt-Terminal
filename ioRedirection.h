//
// Created by root on 02.12.2017.
//

#include <bits/types/FILE.h>
#include <stdio.h>
#include <zconf.h>
#include <fcntl.h>

#ifndef PROJECT2_IOREDIRECTION_H
#define PROJECT2_IOREDIRECTION_H
#endif //PROJECT2_IOREDIRECTION_H

#define MODE_FLAGS_RW_AP (O_RDWR | O_CREAT | O_APPEND)
#define MODE_FLAGS_OW (O_RDWR | O_CREAT | O_TRUNC)
#define PERMISSION (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

int io_file[3];
void rf_n_update_fd(char file_path[],int i0o1){

    switch (i0o1){
        case 1:
            io_file[0]= open(file_path,MODE_FLAGS_RW_AP, PERMISSION);//input
            if(io_file[0]==-1){
                perror("failed to open file\n");
            }
            if (dup2(io_file[0], STDIN_FILENO) == -1) {
                perror("Failed to redirect standard input\n");
            }else printf("GOOD!!!!!! %d\n",i0o1);
            break;
        case 23:
        case 2:
            io_file[1]= open(file_path,MODE_FLAGS_OW, PERMISSION);//output overwrite
            if (dup2(io_file[1], STDOUT_FILENO) == -1) {
                perror("Failed to redirect standard output\n");
            }else printf("GOOD!!!!!!%d\n",i0o1);
            break;
        case 20:
        case 3:
            io_file[2]= open(file_path,MODE_FLAGS_RW_AP, PERMISSION);//output append
            if (dup2(io_file[2], STDOUT_FILENO) == -1) {
                perror("Failed to redirect standard output\n");
            }else printf("GOOD!!!!!!%d\n",i0o1);
            break;
        case 33:
            io_file[3]= open(file_path,MODE_FLAGS_OW, PERMISSION);//error overwrite
            if (dup2(io_file[3], STDERR_FILENO) == -1) {
                perror("Failed to redirect standard error\n");
            }else printf("GOOD!!!!!!%d\n",i0o1);
            break;
        case 30:
            io_file[4]= open(file_path,MODE_FLAGS_RW_AP, PERMISSION);//error overwrite
            if (dup2(io_file[4], STDERR_FILENO) == -1) {
                perror("Failed to redirect standard error\n");
            }else printf("GOOD!!!!!!%d\n",i0o1);
            break;
    }
}

void scan_f_name(char *args[]){
    int i=0,k=0,send=0;
    char temp[5]="";
    /*while(args[k]!=NULL){
        printf("SEND- %d %s\n",send,args[k]);
        k++;
    }*/
    while(args[i]!=NULL){
        send = 0;
        ssize_t len=strlen(args[i]);
        if(args[i][len-1]=='>'){//outputs
            int add=0;
            strcpy(temp,args[i]);
            if(args[i][0]>=48&&args[i][0]<=57&&temp[0]=='-'){
                temp[0]=args[i][0];
                temp[1]=NULL;
                add=atoi(temp);
                add=(add*10)+10;
            }
            if(len-1>=1){
                if(len-1==1){
                    send=3;//just append
                    send+=add;//overwrite redirection 13/23/33
                }else if(len-1==2){
                    send+=add;//append redirection 10/20/30
                }
            }else if(len-1==0){
                send=2;//overwrite
            }
            rf_n_update_fd(args[i+1],send);
        }
        if(args[i][len-1]=='<'){ //input
            send=1;
            rf_n_update_fd(args[i+1],send);
        }
        printf("SEND- %d %s\n",send,args[i]);
        i++;
    }
}