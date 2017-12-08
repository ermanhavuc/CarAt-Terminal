#ifndef PROJECT2_MYSHELLVARIABLES_H
#define PROJECT2_MYSHELLVARIABLES_H

#endif //PROJECT2_MYSHELLVARIABLES_H

#define MAX_LINE 128 /* 80 chars per line, per command, should be enough. */
#define NUM_OF_CMM 5 //defines total numbers of commands
#define MSGSIZE 6

char cmm_bookmark[80]="bookmark";
char bm_list[3]="-l";
char bm_idx[3]="-i";
char bm_dlt[3]="-d";
char cmm_codesearch[80]="codesearch";
char cs_rec[3]="-r";
char cmm_print[80]="print";
char cmm_set[80]="set";
char cmm_exit[80]="exit";
char *cmm[NUM_OF_CMM];
char eq_sign[2]="=";
char **env;
int io_place=-1,bm_exe=0;
ssize_t bm_len=0;

