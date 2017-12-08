//
// Created by erman on 01.12.2017.
//

#ifndef PROJECT2_CODESEARCH_H
#define PROJECT2_CODESEARCH_H

#endif //PROJECT2_CODESEARCH_H

#define PATH_NAME "."
#define LIM1 4
#define LIM2 12

#include <bits/types/FILE.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <_G_config.h>


char path[80]=PATH_NAME;
char word_arg[80]="";
int scan_for_pos_vals(char src[]){
    int ct1,ct2;
    char *pos_val[LIM1];
    char needed_ext[LIM2]=".c\0.C\0.h\0.H";
    for(ct1=0,ct2=0;ct1<LIM1&&ct2<LIM2;ct1++,ct2+=3){
        pos_val[ct1]=strstr(src,&needed_ext[ct2]);
        if(pos_val[ct1]!=NULL) return 1;
    }
    return 0;


}

void findwords(char file_path[128],char word[80]){
    char *line,*ret;
    ssize_t len=0;
    fpos_t pos;
    int i=0,ct=1;
    FILE *file = fopen(file_path,"r");
    if(file){
        while(getline(&line, &len, file)!=-1){
            ret=strstr(line,word);
            int i;
            if(ret!=NULL) {
                for(int i=0;i<strlen(line);i++){
                    if(line[i]!=' '){
                        char spc[2]={line[i],NULL};
                        line=strstr(line,spc);
                        break;
                    }

                }

            }
            ct++;
        }
    }else fprintf(stderr,"No files\n");
}
void findfiles(DIR *dr,struct dirent *de,char path[],char word[],int rec){

    dr=opendir(path);

    if (dr == NULL){ //no directory
        fprintf(stderr,"Could not open given directory.\n");
        return 10;
    }
    DIR *temp;
    struct dirent *temp_p;
    while ((de = readdir(dr)) != NULL){
        int valid_f=scan_for_pos_vals(de->d_name);
        if(de->d_type==4){
            if(strcmp(de->d_name,"..")&&strcmp(de->d_name,".")&&rec){
                char temp_str[256]="";
                strcat(temp_str,path);
                strcat(temp_str,"/");
                strcat(temp_str,de->d_name);
                findfiles(temp,temp_p,temp_str,word,rec);
            }
        }else if(de->d_type==8&&valid_f){
            char temp_str[256]="";
            strcat(temp_str,path);
            strcat(temp_str,"/");
            strcat(temp_str,de->d_name);
            findwords(temp_str,word);
        }
    }
}


int test_args(int argc,char *args[]){
    if(argc<=3){
        if(argc==2){
                size_t len=strlen(args[1]);
                if(args[1][0]==34&&args[1][len-1]==34){
                    strcpy(word_arg,&args[1][1]);
                    strcpy(&word_arg[len-2],&args[1][len]);
                    return 0;
                }else{
                    fprintf(stderr,"Arguments not valid.\n");
                    return 10;
                }
        }else if(argc==3){
            size_t len=strlen(args[2]);
            if(!strcmp(args[1],"-r")&&args[2][0]==34&&args[2][len-1]==34){
                strcpy(word_arg,&args[2][1]);
                strcpy(&word_arg[len-2],&args[2][len]);
                return 1;
            }
        }else{
            fprintf(stderr,"Argument count is not enough.\n");
            return 10;
        }
    }else{
        fprintf(stderr,"Argument count is too much.");
        return 10;
    }

}
int codesearch(int argc,char *args[]){


    struct dirent *de;
    int rec=0;
    DIR *dr;
    if(argc>=2){
        rec=test_args(argc,args);
    }else{
        fprintf(stderr,"Argument count is not enough.\n");
        return 10;
    }
    char ar[45];
    strcpy(ar,"");
    int i=0;
    while(args[i]!=NULL){
        i++;
    }
    findfiles(dr,de,path,word_arg,rec);
    return 0;
}