#define ioptlen 4
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include "myshellVariables.h"
#include "bookmark.h"
#include "codesearch.h"
#include "set.h"
#include "print.h"

char **env;

/* The setup function below will not return any value, but it will just: read
in the next command line; separate it into distinct arguments (using blanks as
delimiters), and set the args array entries to point to the beginning of what
will become null-terminated, C-style strings. */
char *io_opt_o[];
char io_o[40]="->\0>>\0>\0<";
char *io_pt[ioptlen];

void init_io_pts(){
    io_pt[0]=&io_o[0];
    io_pt[1]=&io_o[3];
    io_pt[2]=&io_o[6];
    io_pt[3]=&io_o[8];
    /*int i;
    for(i=0;i<4;i++){
        printf("%s\n",io_pt[i]);
    }*/
}

int check_io(char *args[],int ct,int cur,int background){

    init_io_pts();
    int i=cur,op=0;
    int j=0,init_j=0,lim_j=4,tot_lim=2;
    char temp[80]="";
    while(args[i]!=NULL){


        if(args[i+1]==NULL){
            //printf("%d %s %d %d\n",args[i][0],io_pt[j],op,tot_lim);
            if(args[i][0]==38&&background==1){

                return 1;
            }
        }
        strcpy(temp,io_pt[j]);
        if(args[i][0]>=48&&args[i][0]<=57){
            temp[0]=args[i][0];
        }
        //printf("-----%s\n",temp);
        printf("1- %s %s %d %d %d\n",args[i],io_pt[j],op,tot_lim,i);
        if(!strcmp(args[i],temp)&&op==0&&tot_lim){
            printf("987977878798");
            op=1;
            if(j==0||j==1||j==2) {
                init_j=3;
                tot_lim=2;
            }
            else if(j==3){

                init_j=0;
                lim_j=3;
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
        printf("2- %s %s %d %d %d\n",args[i],io_pt[j],op,tot_lim,i);
        if(op==1&&access(args[i], F_OK)!=-1&&tot_lim) {
            printf("ok\n");
            op=0;
            tot_lim--;
            i++;
        }else if(op==1){
            printf("NO FILE EXISTS IN THAT NAME\n");
            return 5;
        }

    }
    return 1;
}

int setup(char inputBuffer[], char *args[],int *background)
{
    int length, /* # of characters in the command line */
            i,      /* loop index for accessing inputBuffer array */
            start,  /* index where beginning of next command parameter is */
            ct;     /* index of where to place the next parameter into args[] */

    ct = 0;
    /* read what the user enters on the command line */
    length = read(STDIN_FILENO,inputBuffer,MAX_LINE);

    /* 0 is the system predefined file descriptor for stdin (standard input),
       which is the user's screen in this case. inputBuffer by itself is the
       same as &inputBuffer[0], i.e. the starting address of where to store
       the command that is read, and length holds the number of characters
       read in. inputBuffer is not a null terminated C-string. */

    start = -1;
    if (length == 0)
        exit(0);            /* ^d was entered, end of user command stream */

/* the signal interrupted the read system call */
/* if the process is in the read() system call, read returns -1
  However, if this occurs, errno is set to EINTR. We can check this  value
  and disregard the -1 value */
    if ( (length < 0) && (errno != EINTR) ) {
        perror("error reading the command");
        exit(-1);           /* terminate with error code of -1 */
    }
    //printf(">>%s<<",inputBuffer);
    int isQuote=0,eq_sign_b=0;
    for (i=0;i<length;i++){ /* examine every character in the inputBuffer */
        switch (inputBuffer[i]){
            case '=':
                eq_sign_b=1;
            case ' ':
            case '\t' :               /* argument separators */
                if(isQuote==1) {
                    eq_sign_b=0;
                    break;
                }
                if(start != -1){
                    args[ct] = &inputBuffer[start];    /* set up pointer */
                    ct++;
                }
                inputBuffer[i] = '\0'; /* add a null char; make a C string */
                start = -1;
                if(eq_sign_b){
                    eq_sign_b=0;
                    args[ct]=&eq_sign[0];
                    ct++;
                }
                break;

            case '\n':                 /* should be the final char examined */
                if (start != -1){
                    args[ct] = &inputBuffer[start];
                    ct++;
                }
                inputBuffer[i] = '\0';
                args[ct] = NULL; /* no more arguments to this command */
                break;

            default :             /* some other character */
                if (start == -1)
                    start = i;
                if (inputBuffer[i] == '&'){
                    *background  = 1;
                    inputBuffer[i-1] = '\0';
                }else if(inputBuffer[i]==34){
                    isQuote=(isQuote+1)%2;
                }
        } /* end of switch */
    }    /* end of for */
    args[ct] = NULL; /* just in case the input line was > 80 */
    //printf("%s",inputBuffer);
    //for (i = 0; i < ct; i++) {
      //  int ret = checkArgs(args,ct,*background);
        //if(ret==-1) {
          //  printf("----\n");
            //return -1;
        //}
        //printf("args %d = %s ve %c ve %d bg=%d\n",i,args[i],args[i][strlen(args[i])-1],ret,*background);
    //}
    checkArgs(args,ct,*background);

} /* end of setup routine */

int checkArgs(char *args[],int ct,int background){
    ct-=1;
    int okay=0;
    if(ct>=0){
        if(!strcmp(cmm_bookmark,args[0])){
            if(check_for_bm(args,ct,background)==1){
                e_command(cmm_bookmark,args,background);
            }
        }else if(!strcmp(cmm_codesearch,args[0])){
            if(check_for_cs(args,ct,background)==1){
                e_command(cmm_codesearch,args,background);
            }
        }else if(!strcmp(cmm_print,args[0])){
            if(check_for_print(args,ct,background)==1){
                e_command(cmm_print,args,background);
            }
        }else if(!strcmp(cmm_set,args[0])){
            if(check_for_set(args,ct,background)==1){
                e_command(cmm_set,args,background);
            }
        }else if(!strcmp(cmm_exit,args[0])&&ct==0){
            return -1;
        }else okay=5; //wrong arguments
    }else okay=4;//arguments not enough
    return okay;
}
int check_if_bg(int background,int idx,int ct){
    if(background==1&&(idx+1)==ct){
        return 1;
    }
    return 2;
}
int check_for_print(char*args[],int ct,int background){

    int okay=5;
    if(ct==1) okay=1;
    else if(ct>1){
        if(ct==2){
            okay=1;
        }else okay=check_if_bg(background,2,ct);
    }
    /*if(ct>=1){
        okay=check_if_bg(background,0,ct);
        if(ct==2) okay=1;
        else okay=check_if_bg(background,1,ct);
    }*/

    return okay;
}
void e_process(char path[],char *args[],int background){
    int childpid;
    //args[0]=path;
    if((childpid=fork())==0)
        execv(path,args);
    if(!background) wait(NULL);
    //printf("%s\n",args[0]);
}
void sel_N_run(int c_name,char *args[]){

    int i=0,ct=0;

    while(args[i]!=NULL){

        ct++;
        i++;
    }
    //printf("%d %s",ct, args[0]);
    switch(c_name){
        case 0:
            printf("qwerqwer %d %s ,ct,args[0]\n");
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
    int childpid,c_name;
    //args[0]=path;
    if(!strcmp(name,cmm_bookmark)) c_name=0;
    if(!strcmp(name,cmm_codesearch)) c_name=1;
    if(!strcmp(name,cmm_print)) c_name=2;
    if(!strcmp(name,cmm_set)) c_name=3;
    int *er;
    if((childpid=fork())==0){
        sel_N_run(c_name,args);
        exit(0);
    }

    if(!background) wait(er);
    printf("%d\n",errno);
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
int check_for_set(char*args[],int ct,int background){
    int okay=0;
    if(ct>=3){
        int i;
        //printf("%d\n",okay);
        if(!strcmp(args[2],eq_sign)){
            if(ct==3) okay=1;
            else okay=check_if_bg(background,3,ct);

        }else okay=5; //wrong arguments
    }else okay=4; //arguments not enough

    return okay;
}
int check_for_cs(char*args[],int ct,int background){

    int okay=0;
    if(ct>=1){
           if(!strcmp(args[1],cs_rec)){
           if(ct>=2){
               if(args[2][0]==34 && args[2][strlen(args[2])-1]==34){
                   //printf("!!!!!!\n");
                   if(ct==2) okay=1;
                   else okay=check_if_bg(background,2,ct);
               }else okay=6; //not a valid string
           }else okay=4; //arguments not enough
       }else if(args[1][0]==34 && args[1][strlen(args[1])-1]==34){

           if(ct==1) okay=1;
           else okay=check_if_bg(background,1,ct);
       }else okay=5; //wrong arguments
    }else okay=4; //arguments not enough

    return okay;
}


int check_for_bm(char*args[],int ct,int background){
    int okay=0;

        if(ct>=1){
            if(!strcmp(bm_list,args[1])||(args[1][0]==34&&args[1][strlen(args[1])-1]==34)){
                if(ct==1) okay=1;
                else okay=check_if_bg(background,1,ct);
            }
            else if(!strcmp(bm_dlt,args[1])||!strcmp(bm_idx,args[1])){
                if(ct>=2){
                    if(check_if_int(args[2])){
                        if(ct==2) okay=1;
                        else okay=check_if_bg(background,2,ct);
                    }else okay=3;//value is not integer
                }else okay=4;//arguments not enough
            }else okay=5;//wrong arguments
        }else okay=4;//arguments not enough

    return okay;
}

void cp_arr(char *arr1[],char *arr2[],int arr1_idx,int arr2_idx,int arr2_lim){
    int x,y;
    for(x=arr1_idx,y=arr2_idx;y<arr2_lim;x++,y++){
        arr1[x]=arr2[y];
    }
}
void initialize(){
    cmm[0]=&cmm_bookmark[0];
    cmm[1]=&cmm_codesearch[0];
    cmm[2]=&cmm_print[0];
    cmm[3]=&cmm_set[0];
    cmm[4]=&cmm_exit[0];
}
int main(int x,char *y[],char **envp)
{
    char *asd[8];
    char str0[]="<";
    char str1[]="/home/berkay/Documents/asd.txt";
    char str2[]="5>";
    char str3[]="/home/berkay/Documents/asd.txt";
    char str4[]="&";
    asd[0]=str0;
    asd[1]=str1;
    asd[2]=str2;
    asd[3]=str3;
    asd[4]=str4;
    asd[5]=NULL;
    printf("%d",check_io(asd,3,0,1));
    exit(0);
    env=envp;
    initialize();
    int end =0;
    char inputBuffer[MAX_LINE]; /*buffer to hold command entered */
    int background; /* equals 1 if a command is followed by '&' */
    char *args[MAX_LINE/2 + 1]; /*command line arguments */
    while (1){
        background = 0;
        //printf("myshell: ");
        /*setup() calls exit() when Control-D is entered */
        end=setup(inputBuffer, args, &background);
        printf("---\n");
        if(end==-1) break;
        /** the steps are:
        (1) fork a child process using fork()
        (2) the child process will invoke execv()
        (3) if background == 0, the parent will wait,
        otherwise it will invoke the setup() function again. */
    }
    return end;
}