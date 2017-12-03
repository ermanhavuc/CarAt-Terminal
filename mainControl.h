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
    //printf("%d %s\n",ct, args[0]);
    switch(c_name){
        case 0:
            //printf("qwerqwer %d %s ,ct,args[0]\n");
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
            break;
    }
}
void e_command(char name[],char *args[],int background){
    int c_name;
    if(!strcmp(name,cmm_bookmark)) c_name=0;
    if(!strcmp(name,cmm_codesearch)) c_name=1;
    if(!strcmp(name,cmm_print)) c_name=2;
    if(!strcmp(name,cmm_set)) c_name=3;
    sel_N_run(c_name,args);

}

void read_from_stdin(char cmm[],char *args[],int background){
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

                args[io_place]=&st[0];
                printf("\nargs= %s %d\n",args[0],0);
                printf("\nargs= %s %d\n",args[io_place],io_place);
                e_command(cmm,args,background);
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
            //printf("!!!!!!!!!!!!!!!!!!!%s %s\n",args[i],temp);
            if(!strcmp(args[i],temp)){
                io_place = i;

                return check_io(args,i,background);
            }
            j++;
        }
        j=0;
        i++;
    }
    printf("OKAY!!!\n");
    if(background==1&&strcmp(args[i-1],"&")) return 5;
    return 1;
}

int check_Args(char *args[],int ct,int background){
    ct-=1;
    int okay=1;
    if(ct>=0){

        if(!strcmp(cmm_bookmark,args[0])){

            if((okay=scan_io(args,background))==1){

                if(args[ct]!=NULL) printf("%s\n",args[ct]);
                if(background==1) args[ct]=NULL;
                //printf("!!!!!!!!!!!!!!!!!!! %d\n");
                if(io_place!=-1) {
                    scan_f_name(args);
                    args[io_place]=NULL;
                }
                if(i_red_flag==1){
                    args[io_place+1]=NULL;
                    read_from_stdin(cmm_bookmark,args,background);
                } else e_command(cmm_bookmark,args,background);
                close_redirections();
                printf("Redirs closed\n");
            }
        }else if(!strcmp(cmm_codesearch,args[0])){
            if((okay=scan_io(args,background))==1){
                if(args[ct]!=NULL) printf("%s\n",args[ct]);
                if(background==1) args[ct]=NULL;
                if(io_place!=-1) {
                    scan_f_name(args);
                    args[io_place]=NULL;
                }
                if(i_red_flag==1){
                    args[io_place+1]=NULL;
                    read_from_stdin(cmm_codesearch,args,background);
                }else e_command(cmm_codesearch,args,background);
                close_redirections();
                printf("Redirs closed\n");
            }
        }else if(!strcmp(cmm_print,args[0])){
            if((okay=scan_io(args,background))==1){
                if(args[ct]!=NULL) printf("%s\n",args[ct]);
                if(background==1) args[ct]=NULL;
                if(io_place!=-1) {
                    scan_f_name(args);
                    args[io_place]=NULL;
                }
                e_command(cmm_print,args,background);
                close_redirections();
                printf("Redirs closed\n");
            }
        }else if(!strcmp(cmm_set,args[0])){
            if((okay=scan_io(args,background))==1){
                if(args[ct]!=NULL) printf("%s\n",args[ct]);
                if(background==1) args[ct]=NULL;
                if(io_place!=-1) {
                    scan_f_name(args);
                    args[io_place]=NULL;
                }
                //printf("dasdasdasdasdasdas\n");
                e_command(cmm_set,args,background);
                close_redirections();
                printf("Redirs closed\n");
            }
        }else if(!strcmp(cmm_exit,args[0])&&ct==0){
            return -1;
        }else if(scan_io(args,background)==1){
            if(args[ct]!=NULL) printf("%s\n",args[ct]);
            if(background==1) args[ct]=NULL;
            if(io_place!=-1) {
                scan_f_name(args);
                args[io_place]=NULL;
            }
            execCommand(args,background);
            close_redirections();

        }else perror("Wrong Arguments!\n");
        //printf("dasdasdasdasdasdas %d\n",okay);
    }else okay=4;//arguments not enough
    return okay;
}
void init_io_pts(){
    io_pt[0]=&io_o[0];
    io_pt[1]=&io_o[3];
    io_pt[2]=&io_o[7];
    io_pt[3]=&io_o[10];
    io_pt[4]=&io_o[12];
    /*int i;
    for(i=0;i<4;i++){
        printf("%s\n",io_pt[i]);
    }*/
}

int check_io(char *args[],int cur,int background){
    //init_io_pts();
    //printf("!!!!!!!!!!!!!!!!!!! %d\n",io_place);
    int i=cur,op=0,start=0;

    int j=0,init_j=0,lim_j=5,tot_lim=2;
    char temp[80]="";
    while(args[i]!=NULL){
        //printf("!!!!!!!!!!!!!!!!!!! %d\n",io_place);
        //printf("---------%s\n",args[i]);

        if(args[i+1]==NULL&&start==0){
            //printf("%d %s %d %d\n",args[i][0],io_pt[j],op,tot_lim);
            if(args[i][0]==38&&background==1){

                return 1;
            }
        }
        strcpy(temp,io_pt[j]);
        if(args[i][0]>=48&&args[i][0]<=57&&temp[0]=='-'){
            temp[0]=args[i][0];
        }
        //printf("-----%s\n",temp);
        //printf("1- %s %s %d %d %d\n",args[i],io_pt[j],op,tot_lim,i);
        if(!strcmp(args[i],temp)&&op==0&&tot_lim){
            //printf("987977878798");

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
        //printf("%s\n",args[i]);
        //printf("2- %s %s %d %d %d\n",args[i],io_pt[j],op,tot_lim,i);
        if(op==1&&access(args[i], F_OK)!=-1&&tot_lim) {
            printf("ok\n");
            op=0;
            tot_lim--;
            i++;
            start=0;
        }else if(op==1){
            printf("NO FILE EXISTS IN THAT NAME %s\n",args[i]);
            printf("no\n");
            return 5;
        }

    }
    printf("yes\n");

    return 1;
}
