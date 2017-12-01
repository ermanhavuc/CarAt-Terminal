#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_LINE 128 /* 80 chars per line, per command, should be enough. */
#define NUM_OF_CMM 5 //defines total numbers of commands

/* The setup function below will not return any value, but it will just: read
in the next command line; separate it into distinct arguments (using blanks as
delimiters), and set the args array entries to point to the beginning of what
will become null-terminated, C-style strings. */
char cmm_bookmark[80]="bookmark";
char bookmark_path[80]="/home/berkay/Documents/OPSYS/test/bookmark.out";
char bm_list[3]="-l";
char bm_idx[3]="-i";
char bm_dlt[3]="-d";
char cmm_codesearch[80]="codesearch";
char codesearch_path[80]="/home/berkay/Documents/OPSYS/test/codesearch.out";
char cs_rec[3]="-r";
char cmm_print[80]="print";
char cmm_set[80]="set";
char cmm_exit[80]="exit";
char *cmm[NUM_OF_CMM];
char eq_sign[2]="=";

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
                if(isQuote==1) break;
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
                e_process(bookmark_path,args,background);
            }
        }else if(!strcmp(cmm_codesearch,args[0])){
            if(check_for_cs(args,ct,background)==1){
                e_process(codesearch_path,args,background);
            }
        }else if(!strcmp(cmm_print,args[0])){
            return check_for_print(args,ct,background);
        }else if(!strcmp(cmm_set,args[0])){
            return check_for_set(args,ct,background);
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
    if(ct>=1){
        okay=check_if_bg(background,0,ct);
        int i;
        for(i=0;i<NUM_OF_CMM;i++){
            if(!strcmp(args[1],cmm[i])){
                if(ct==1) okay=1;
                else okay=check_if_bg(background,1,ct);
            }else if(okay!=1) okay=5;
        }
    }else okay=1;
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

        if(!strcmp(args[2],eq_sign)){
            if(ct==3) okay=1;
            else okay=check_if_bg(background,3,ct);
        }else okay=5; //wrong arguments

        for(i=0;i<NUM_OF_CMM;i++){
            if(!strcmp(args[1],cmm[i])){
                okay=0;
                break;
            }else okay=5;//wrong arguments
        }
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
int main(void)
{
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