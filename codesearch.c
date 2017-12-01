#define PATH_NAME "/home/berkay/Documents/OPSYS/test"
//#define PATH_NAME "."
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
    //printf("char=%c\n",c);
    FILE *file = fopen(file_path,"r");
    if(file){
        while(getline(&line, &len, file)!=-1){
            //printf("%s",line);
            ret=strstr(line,word);
            int i;
            if(ret!=NULL) {
                printf("%d: %s -> %s \n",ct ,file_path, line);
                for(int i=0;i<strlen(line);i++){
                    if(line[i]!=' '){
                        char spc[2]={line[i],NULL};
                        line=strstr(line,spc);
                        break;
                    }

                }

            }
            ct++;
            //printf("OK\n");
        }
    }else printf("no files\n");
}
void findfiles(DIR *dr,struct dirent *de,char path[],char word[],int rec){
    dr=opendir(path);
    if (dr == NULL){ //no directory
        printf("Could not open current directory \n");
    }
    DIR *temp;
    struct dirent *temp_p;
    //printf("findfiles  ");
    while ((de = readdir(dr)) != NULL){
        int valid_f=scan_for_pos_vals(de->d_name);
        if(de->d_type==4){
            if(strcmp(de->d_name,"..")&&strcmp(de->d_name,".")&&rec){
                //printf("folder:%s path:%s\n",de->d_name,path);
                char temp_str[256]="";
                strcat(temp_str,path);
                strcat(temp_str,"/");
                strcat(temp_str,de->d_name);
                //temp=opendir(temp_str);
                findfiles(temp,temp_p,temp_str,word,rec);
            }
        }else if(de->d_type==8&&valid_f){
            //printf("           source code:%s path:%s\n",de->d_name,path);
            char temp_str[256]="";
            strcat(temp_str,path);
            strcat(temp_str,"/");
            strcat(temp_str,de->d_name);
            //printf("%s\n",temp_str);
            findwords(temp_str,word);
        }
    }
    //printf("find files close\n");
}
int ipt_args(char *args[]){
    int i=0,ret_val=0;
    while(args[i]!=NULL){
        if(!strcmp(args[i],"-r")){
            ret_val=1;
        }
        size_t len=strlen(args[i]);
        if(args[i][0]==34&&args[i][len-1]==34){
            strcpy(word_arg,&args[i][1]);
            strcpy(&word_arg[len-2],&args[i][len]);
            //printf("word is%s\n",word_arg);
        }

        i++;
    }
    return ret_val;
}
int main(int argc,char *args[]){
    struct dirent *de;
    int rec=0;
    DIR *dr;
    //char *path_arg;

    //args[1]="\"if(\"";
    //args[2]=NULL;
    if(argc>=2){
        //path_arg=args[0];
        //printf("argc=%d arg1=%s\n",argc,arg[0]);
        rec=ipt_args(args);


    }else{
        perror("Arguments not enough\n");
    }
    //scan_for_pos_vals("asd.c");
    //findwords("/home/berkay/Documents/OPSYS/test/main.c","for");
    //printf("%s\n",path);
    findfiles(dr,de,path,word_arg,rec);
    //closedir(dr);
    return 0;
}