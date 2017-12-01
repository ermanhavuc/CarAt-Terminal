//
// Created by erman on 01.12.2017.
//

#ifndef PROJECT2_MYSHELLVARIABLES_H
#define PROJECT2_MYSHELLVARIABLES_H

#endif //PROJECT2_MYSHELLVARIABLES_H

#define MAX_LINE 128 /* 80 chars per line, per command, should be enough. */
#define NUM_OF_CMM 5 //defines total numbers of commands

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