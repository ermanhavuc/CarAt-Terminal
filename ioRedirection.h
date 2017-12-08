#include <bits/types/FILE.h>
#include <stdio.h>
#include <fcntl.h>


#ifndef PROJECT2_IOREDIRECTION_H
#define PROJECT2_IOREDIRECTION_H
#endif //PROJECT2_IOREDIRECTION_H

#define MODE_FLAGS_RW_AP (O_RDWR | O_CREAT | O_APPEND)
#define MODE_FLAGS_OW (O_RDWR | O_CREAT | O_TRUNC)
#define PERMISSION (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define POS_FILE_CNT 5

int io_file[POS_FILE_CNT],s_stdout,s_stderr,s_stdin,i_red_flag=0;
void rf_n_update_fd(char file_path[],int i0o1){
    s_stdout=dup(1);
    s_stderr=dup(2);
    s_stdin=dup(0);


    switch (i0o1){

        case 1:
            i_red_flag=1;
            io_file[0]= open(file_path,MODE_FLAGS_RW_AP, PERMISSION);//input

            if(io_file[0]==-1){
                fprintf(stderr,"Failed to open file.\n");
            }
            if (dup2(io_file[0], STDIN_FILENO) == -1) {
                fprintf(stderr,"Failed to redirect standard input.\n");
            }
            break;
        case 23:
        case 2:
            io_file[1]= open(file_path,MODE_FLAGS_OW, PERMISSION);//output overwrite
            if (dup2(io_file[1], STDOUT_FILENO) == -1) {
                fprintf(stderr,"Failed to redirect standard output\n");
            }
            break;
        case 20:
        case 3:
            io_file[2]= open(file_path,MODE_FLAGS_RW_AP, PERMISSION);//output append
            if (dup2(io_file[2], STDOUT_FILENO) == -1) {
                fprintf(stderr,"Failed to redirect standard output\n");
            }
            break;
        case 33:
            io_file[3]= open(file_path,MODE_FLAGS_OW, PERMISSION);//error overwrite
            if (dup2(io_file[3], STDERR_FILENO) == -1) {
                fprintf(stderr,"Failed to redirect standard error\n");
            }
            break;
        case 30:
            io_file[4]= open(file_path,MODE_FLAGS_RW_AP, PERMISSION);//error overwrite
            if (dup2(io_file[4], STDERR_FILENO) == -1) {
                fprintf(stderr,"Failed to redirect standard error\n");
            }
            break;
    }
}

void scan_f_name(char *args[]){
    int i=0,k=0,send=0;
    char temp[5]="";
    while(args[i]!=NULL){

        send = 0;
        ssize_t len=strlen(args[i]);
        if(args[i][len-1]=='>'){//outputs

            int add=0;

            if(args[i][0]>=48&&args[i][0]<=57){
                char app[3]="->",tru[4]="->>",*arr[2];
                arr[0]=app;
                arr[1]=tru;
                int k;
                for(k=0;k<2;k++){
                    strcpy(temp,arr[k]);
                    temp[0]=args[i][0];
                    if(!strcmp(args[i],temp)){
                        temp[1]=NULL;
                        add=atoi(temp);
                        add=(add*10)+10;
                    }
                }
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
        i++;
    }
}

void close_redirections(){

    int i=0;
    while(i<POS_FILE_CNT){
        i++;
    }
    i_red_flag=0;
    dup2(s_stderr,2);
    dup2(s_stdout,1);
    dup2(s_stdin,0);

}
