
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include "mainControl.h"
#include "history.h"


int setup(char inputBuffer[], char *args[],int *background) {
    int length=0, /* # of characters in the command line */
            i,      /* loop index for accessing inputBuffer array */
            start,  /* index where beginning of next command parameter is */
            ct;     /* index of where to place the next parameter into args[] */

    ct = 0;


    /* read what the user enters on the command line */
    if(bm_exe==0){
        history();

        return 0;
    }else if(bm_exe==2){
        //length=addToHistory(inputBuffer);
        length=sk;
    }else if(bm_exe==3){
        //length = read(STDIN_FILENO,inputBuffer,MAX_LINE);
        fgets(inputBuffer,MAX_LINE,stdin);
        length=strlen(inputBuffer);
        //printf(">>>>%s %d",inputBuffer,length);
        bm_exe=0;
        addToHistory(inputBuffer);
    }
    else length=bm_len;
    //printf("\n%s %d\n",inputBuffer,sk);


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
        //printf("--%s",args[0]);
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

    check_Args(args,ct,*background);

    //printf("%d\n",adf);
    //printf("%d\n",adf);
} /* end of setup routine */



int main(int x,char *y[],char **envp) {
    /*char *asd[8];
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
    printf("%d",check_io(asd,3,0,1));*/

    env=envp;
    initialize();
    int end =0;
    char inputBuffer[MAX_LINE]; /*buffer to hold command entered */
    int background; /* equals 1 if a command is followed by '&' */
    char *args[MAX_LINE/2 + 1]; /*command line arguments */
    while (1){

        background = 0;
        printf("myshell: ");
        /*setup() calls exit() when Control-D is entered */
        end=setup(inputBuffer, args, &background);
        printf("\n---\n");
        if(end==-1) break;
        /** the steps are:
        (1) fork a child process using fork()
        (2) the child process will invoke execv()
        (3) if background == 0, the parent will wait,
        otherwise it will invoke the setup() function again. */
    }
    return end;
}