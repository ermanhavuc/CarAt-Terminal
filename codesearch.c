#define PATH_NAME "/home/berkay/Documents/OPSYS/test"
#define LIM1 4
#define LIM2 12

#include <bits/types/FILE.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <_G_config.h>

char path[80]=PATH_NAME;
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
                for(int i=0;i<strlen(line);i++){
                    if(line[i]!=' '){
                        char spc[2]={line[i],NULL};
                        line=strstr(line,spc);
                        break;
                    }
                }
                printf("%d: %s -> %s \n",ct ,file_path, line);
            }
            ct++;
            //printf("OK\n");
        }
    }else printf("no files\n");
}
void findfiles(DIR *dr,struct dirent *de,char path[],char word[]){
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
            if(strcmp(de->d_name,"..")&&strcmp(de->d_name,".")){
                //printf("folder:%s path:%s\n",de->d_name,path);
                char temp_str[256]="";
                strcat(temp_str,path);
                strcat(temp_str,"/");
                strcat(temp_str,de->d_name);
                //temp=opendir(temp_str);
                findfiles(temp,temp_p,temp_str,word);

            }
        }else if(de->d_type==8&&valid_f){
           //printf("           source code:%s path:%s\n",de->d_name,path);
            char temp_str[256]="";
            strcat(temp_str,path);
            strcat(temp_str,"/");
            strcat(temp_str,de->d_name);
            printf("%s\n",temp_str);
            findwords(temp_str,word);
        }
    }
    //printf("find files close\n");
}
int main(int argc,char *args[]){
    struct dirent *de;
    DIR *dr;
    //scan_for_pos_vals("asd.c");
    //findwords("/home/berkay/Documents/OPSYS/test/main.c","for");
    //printf("%s\n",path);
    findfiles(dr,de,path,"if(");
    //closedir(dr);
    return 0;
}