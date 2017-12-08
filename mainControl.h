#ifndef PROJECT2_MAINCONTROL_H
#define PROJECT2_MAINCONTROL_H
#endif //PROJECT2_MAINCONTROL_H
#define ioptlen 5

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <dirent.h>
#include <_G_config.h>
#include <bits/types/FILE.h>
#include "myshellVariables.h"
#include "execCommand.h"
#include "ioRedirection.h"
#include "bookmark.h"
#include "codesearch.h"
#include "print.h"
#include "set.h"



/* The setup function below will not return any value, but it will just: read
in the next command line; separate it into distinct arguments (using blanks as
delimiters), and set the args array entries to point to the beginning of what
will become null-terminated, C-style strings. */
char io_o[40]="->\0->>\0>>\0>\0<";
char *io_pt[ioptlen];
void e_process(char path[],char *args[],int background){
    if(fork()==0){
        execv(path,args);
    }
    if(!background) wait(NULL);

}

void sel_N_run(int c_name,char *args[]){

    int i=0,ct=0;

    while(args[i]!=NULL){

        ct++;
        i++;
    }
    pid_t cp;
    switch(c_name){
        case 0:
            bookmark(args);
            break;
        case 1:

            codesearch(ct,args);
            break;
        case 2:
            print(ct,args,env);
            break;
        case 3:
            set(ct,args);
            break;
        case 4:
            cp=waitpid(-1,NULL,WNOHANG);
            if(cp==0){
                fprintf(stderr,"Before exit, please kill child processes first.\n");
            }else exit(0);
            break;
    }
}
void e_command(char name[],char *args[],int background){
    int c_name;
    if(!strcmp(name,cmm_bookmark)) c_name=0;
    if(!strcmp(name,cmm_codesearch)) c_name=1;
    if(!strcmp(name,cmm_print)) c_name=2;
    if(!strcmp(name,cmm_set)) c_name=3;
    if(!strcmp(name,cmm_exit)) c_name=4;
    sel_N_run(c_name,args);

}

void read_from_stdin(char cmm[],char *args[],int background,int withQ){
    int ct=0;
    char st[1024] = "",buff[1024];
    while (NULL!=read(STDIN_FILENO,buff,1024)){//fgets(buff, sizeof(buff)
        int i=0,j=0;
        while(buff[i]!=NULL){
            if(buff[i]==34){
                do{
                    st[j]=buff[i];
                    i++;
                    j++;

                }while (buff[i]!=34&&buff[i]!='\n'&&buff[i]!=NULL);
                st[j]=buff[i];

                if(withQ) args[io_place]=&st[0];
                else{
                    args[io_place]=&st[1];
                    st[j]=NULL;
                }
                if(args[io_place+1]!=NULL) args[io_place+1]=NULL;
                if(!strcmp(cmm,"set")) {
                    char ib[80]="";
                    char *ags[80];
                    int b;
                    int k=0;
                    while(args[k]!=NULL){
                        strcat(ib,args[k]);
                        strcat(ib," ");

                        k++;
                    }
                    bm_exe=4;
                    i_red_flag=0;
                    setup(ib,ags,&b);
                    i_red_flag=1;
                    bm_exe=0;
                }
                else e_command(cmm,args,background);

                j=0;
            }
            i++;
        }
    }
}
void initialize(){
    cmm[0]=&cmm_bookmark[0];
    cmm[1]=&cmm_codesearch[0];
    cmm[2]=&cmm_print[0];
    cmm[3]=&cmm_set[0];
    cmm[4]=&cmm_exit[0];
}

int scan_io(char *args[],int background){

    int i=1;
    io_place=-1;
    char temp[5]="";
    init_io_pts();
    while(args[i]!=NULL){

        int j=0;
        while(j<ioptlen){

            strcpy(temp,io_pt[j]);
            if(args[i][0]>=48&&args[i][0]<=57&&temp[0]=='-'){
                temp[0]=args[i][0];
            }
            if(!strcmp(args[i],temp)){
                io_place = i;

                return check_io(args,i,background);
            }
            j++;
        }
        j=0;
        i++;
    }
    if(background==1&&strcmp(args[i-1],"&")) return 5;
    return 1;
}

int check_Args(char *args[],int ct,int background){
    ct-=1;
    int okay=1;

    if(ct>=0){

        if(!strcmp(cmm_bookmark,args[0])){

            if((okay=scan_io(args,background))==1){

                if(background==1) args[ct]=NULL;
                if(io_place!=-1) {
                    scan_f_name(args);
                    args[io_place]=NULL;
                }
                if(i_red_flag==1){
                    args[io_place+1]=NULL;
                    read_from_stdin(cmm_bookmark,args,background,1);
                } else e_command(cmm_bookmark,args,background);
                close_redirections();
            }
        }else if(!strcmp(cmm_codesearch,args[0])){
            if((okay=scan_io(args,background))==1){
                if(background==1) args[ct]=NULL;
                if(io_place!=-1) {
                    scan_f_name(args);
                    args[io_place]=NULL;
                }
                if(i_red_flag==1){
                    args[io_place+1]=NULL;
                    read_from_stdin(cmm_codesearch,args,background,1);
                }else e_command(cmm_codesearch,args,background);
                close_redirections();
            }
        }else if(!strcmp(cmm_print,args[0])){
            if((okay=scan_io(args,background))==1){
                if(background==1) args[ct]=NULL;
                if(io_place!=-1) {
                    scan_f_name(args);
                    args[io_place]=NULL;
                }
                if(i_red_flag==1){
                    args[io_place+1]=NULL;
                    read_from_stdin(cmm_print,args,background,0);
                }else e_command(cmm_print,args,background);
                close_redirections();
            }
        }else if(!strcmp(cmm_set,args[0])){

            if((okay=scan_io(args,background))==1){
                if(background==1) args[ct]=NULL;
                if(io_place!=-1) {
                    scan_f_name(args);
                    args[io_place]=NULL;
                }
                if(i_red_flag==1){
                    args[io_place+1]=NULL;
                    read_from_stdin(cmm_set,args,background,0);
                }else e_command(cmm_set,args,background);
                if(bm_exe!=4) close_redirections();
            }
        }else if(!strcmp(cmm_exit,args[0])&&ct==0){
            e_command(cmm_exit,args,background);
        }else if(scan_io(args,background)==1){

            if(background==1) args[ct]=NULL;
            if(io_place!=-1) {
                scan_f_name(args);
                args[io_place]=NULL;
            }
            execCommand(args,background);
            close_redirections();


        }else{
            fprintf(stderr,"Arguments are not valid.\n");
        }
    }else {
        fprintf(stderr,"Argument count is not enough.\n");
        okay=4;
    }
    return okay;
}
void init_io_pts(){
    io_pt[0]=&io_o[0];
    io_pt[1]=&io_o[3];
    io_pt[2]=&io_o[7];
    io_pt[3]=&io_o[10];
    io_pt[4]=&io_o[12];
}

int check_io(char *args[],int cur,int background){

    int i=cur,op=0,start=0;

    int j=0,init_j=0,lim_j=5,tot_lim=2;
    char temp[80]="";
    while(args[i]!=NULL){

        if(args[i+1]==NULL&&start==0){
            if(args[i][0]==38&&background==1){

                return 1;
            }
        }
        strcpy(temp,io_pt[j]);
        if(args[i][0]>=48&&args[i][0]<=57&&temp[0]=='-'){
            temp[0]=args[i][0];
        }
        if(!strcmp(args[i],temp)&&op==0&&tot_lim){

            op=1;
            start=1;
            if(j==0||j==1||j==2||j==3) {
                init_j=4;
                tot_lim=2;
            }
            else if(j==4){

                init_j=0;
                lim_j=4;
                tot_lim=4;
            }
            tot_lim--;
            j=init_j;
            i++;
        }else if(op==0){
            j++;
            if(j==lim_j){

                return 5;
            }
        }
        if(op==1&&access(args[i], F_OK)!=-1&&tot_lim) {
            op=0;
            tot_lim--;
            i++;
            start=0;
        }else if(op==1){
            fprintf(stderr,"No file exists in that name: %s\n",args[i]);
            return 5;
        }

    }

    return 1;
}
