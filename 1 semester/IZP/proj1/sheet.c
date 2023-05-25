#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>

int num_of_c(char ret[10241], char delim){
    int k=1, i=0;
    while(ret[i]!='\n' && ret[i]!='\0'){
        if(ret[i]==delim){
            k++;
        }
        i++;
    }
    return k;
}

void icol(char ret[10241], char delim, int r){
    char loc_ret[10241]={'\0'};
    int k=0;
    size_t ir=0, il=0;
    for(il=0; il<strlen(ret); il++){
        if (k==r){
            loc_ret[il]=delim;
            il++;
            k++;
        }
        if (ret [ir]==delim){
            k++;
        }
        loc_ret[il]=ret[ir];
        ir++;
    }
    if (!(strchr(loc_ret,'\n')!=NULL)){
        strcat(loc_ret,"\n");
        }
    strcpy(ret, loc_ret);
}

void acol(char ret[10241], char delim){

    if (strchr(ret,'\n')!=NULL){
        strchr(ret,'\n')[0]=delim;
        strcat(ret,"\n");
    }
    else {
        ret[strlen(ret)+1]='\0';
        ret[strlen(ret)]=delim;
    }
}

void dcol(char ret[10241], char delim, int n){
    char loc_ret1[10241]={'\0'};
    int i=0, k=0;
    if (n!=1){
        i=-1;
        while(k!=n-1){
            i++;
            if(ret[i]==delim){
                k++;
            }
        }

        strncpy(loc_ret1, ret, i);
        if(strchr(ret+(i+1)*sizeof(char),delim)!=NULL){
            strcat(loc_ret1, strchr(ret+(i+1)*sizeof(char),delim));
        }
    }
    else{
        strcat(loc_ret1, strchr(ret,delim)+sizeof(char));
    }
    if (strchr(ret,'\n')!=NULL){
        strcpy(ret, loc_ret1);
        strcat(ret,"\n");
    }
    else{
        strcpy(ret, loc_ret1);
    }
}

void irow(char ret[10241], char delim){
    int i=0, k=0;
    while(ret[i]!='\n'&&ret[i]!='\0'){
        if(ret[i] == delim){
            k++;
        }
        i++;
    }
    i=0;
    while(i<k){
        printf("%c",delim);
        i++;
    }
}

void arow(char ret[10241], char *ar, char *delim){
    int i=0, k=0;
    char loc_ret[10241]={'\0'};
    while(ret[i]!='\n'&&ret[i]!='\0'){
        if(ret[i] == delim[0]){
            k++;
        }
        i++;
    }
    i=0;
    while(i<k){
        strncat(loc_ret, delim, 1);
        i++;
    }
    strcpy(ar,loc_ret);
}

void lower(char ret[10241], char delim, int c){
    int i=0, k=1;
    while(k<c){
        if(ret[i]==delim){
            k++;
        }
        i++;
    }
    while(ret[i]!='\n' && ret[i]!=':' && ret[i]!='\0'){
        if(ret[i]>=65 && ret[i]<=90){
            ret[i]=ret[i]+32;
        }
        i++;
    }
}

void upper(char ret[10241], char delim, int c){
    int i=0, k=1;
    while(k<c){
        if(ret[i]==delim){
            k++;
        }
        i++;
    }
    while(ret[i]!='\n' && ret[i]!=':' && ret[i]!='\0'){
        if(ret[i]>=97 && ret[i]<=122){
            ret[i]=ret[i]-32;
        }
        i++;
    }
}

void cset(char ret[10241], char delim, int c, char *str){
    char loc_ret[10241]={'\0'};
    int k=1, i=0;
    while(k!=c){
        if(ret[i]==delim){
            k++;
        }
        i++;
    }
    if (k!=1){
    strncpy(loc_ret,ret,i-1);
    loc_ret[strlen(loc_ret)+1]='\0';
    loc_ret[strlen(loc_ret)]=delim;
    }
    strcat(loc_ret, str);
    if(strchr(ret+i*sizeof(char), delim)!=NULL){
        strcat(loc_ret, strchr(ret+i*sizeof(char), delim));
    }
    if(ret[strlen(ret)-1]=='\n' && loc_ret[strlen(loc_ret)-1]!='\n'){
        loc_ret[strlen(loc_ret)+1]='\0';
        loc_ret[strlen(loc_ret)]='\n';
    }
    strcpy(ret, loc_ret);

}

int main(int argc, char **argv)
{
    char retezec[10241]={'\0'}, ar[10241]={'\0'};
    char delim=' ';
    int arow_u = 0, nr, argp = 1, argr, a=0;
    if (argc > 1 ){
        if ((argc > 2) && (argv[1][0] == '-') && (argv[1][1] == 'd')){
            delim = argv[2][0];
            argp = 3;
            }
    }
    nr=0;
    while(fgets(retezec, 10241 , stdin) != NULL){
        nr++;
        //TODO unknown arg
        int argo=argp;
        while (argo < argc){
            int nothing=0;
            if(argo < argc){
                if (strcmp(argv[argo],"icol") == 0){
                    if(argo+1<argc && atoi(argv[argo+1])>0 && num_of_c(retezec,delim)>=atoi(argv[argo+1])){
                        icol(retezec, delim, atoi(argv[argo+1])-1);
                        argo=argo+2;
                        nothing=1;
                    }
                    else{
                        fprintf(stderr,"error: argument %d=icol column not specified, 1 <= N <= number of columns\n",argo);
                        return 0;
                    }
                }
            }
            if(argo < argc){
                if (strcmp(argv[argo],"acol") == 0){
                acol(retezec,delim);
                argo=argo+1;
                nothing=1;
                }
            }
            if(argo < argc){
                if (strcmp(argv[argo],"dcol") == 0){
                    if(argo+1<argc && atoi(argv[argo+1])>0 && num_of_c(retezec,delim)>=atoi(argv[argo+1])){
                        dcol(retezec, delim, atoi(argv[argo+1]));
                        argo=argo+2;
                        nothing=1;
                    }
                    else{
                        fprintf(stderr,"error: argument %d=dcol column not specified, 1 <= N <= number of columns\n",argo);
                        return 0;
                    }
                }
            }
            if(argo < argc){
                if (strcmp(argv[argo],"dcols") == 0){
                    if(argo+2<argc && atoi(argv[argo+1])>0 && atoi(argv[argo+2])-atoi(argv[argo+1])>=0 && num_of_c(retezec,delim)>=atoi(argv[argo+2])){
                        for(int i=1; i<=atoi(argv[argo+2])-atoi(argv[argo+1])+1; i++){
                            dcol(retezec, delim, atoi(argv[argo+1]));
                        }
                        argo=argo+3;
                        nothing=1;
                    }
                    else{
                        fprintf(stderr,"error: argument %d=dcols N M, 1 <= N <= M <= number of columns\n",argo);
                        return 0;
                    }
                }
            }
            if(argo < argc){
                if (strcmp(argv[argo],"irow") == 0){
                    if(atoi(argv[argo+1])<=0){
                        fprintf(stderr,"error: argument %d=irow N, 1 <= N\n",argo);
                        return 0;
                    }
                    if((atoi(argv[argo+1])==nr) || (atoi(argv[argr+1]) == nr)){
                        if(argo+1<argc && atoi(argv[argo+1])>0){
                            irow(retezec, delim);
                            printf("\n");
                            argo=argo+2;
                            argr=argo;
                            nr++;
                            nothing=1;
                        }
                        else{
                            fprintf(stderr,"error: argument %d=irow N, 1 <= N\n",argo);
                            return 0;
                        }
                    }
                    else{
                        nothing=2;
                    }
                }
            }
            if(argo < argc){
                if (strcmp(argv[argo],"drow") == 0){
                    if(atoi(argv[argo+1])<=0){
                        fprintf(stderr,"error: argument %d=drow N, 1 <= N\n",argo);
                        return 0;
                    }
                    if((atoi(argv[argo+1]) == nr) || (atoi(argv[argr+1]) == nr)){
                        if(argo+1<argc && atoi(argv[argo+1])>0){
                            strcpy(retezec,"\0");
                            argo=argo+2;
                            argr=argo;
                            nothing=1;
                            break;
                        }
                        else{
                            fprintf(stderr,"error: argument %d=drow N, 1 <= N\n",argo);
                            return 0;
                        }
                    }
                    else{
                        nothing=2;
                    }
                }
            }
            if(argo < argc){
                if (strcmp(argv[argo],"drows") == 0){
                    if(atoi(argv[argo+1])<=0){
                        fprintf(stderr,"error: argument %d=drows N, 1 <= N\n",argo);
                        return 0;
                    }
                    if((atoi(argv[argo+1]) == nr) || (atoi(argv[argr+1]) == nr)){
                        if(argo+2<argc && atoi(argv[argo+1])>0 && atoi(argv[argo+2])-atoi(argv[argo+1])>=0){
                            for(int i=1; i<=(atoi(argv[argo+2])-atoi(argv[argo+1])); i++){
                                fgets(retezec, 10241 , stdin);
                            }
                            argo=argo+3;
                            argr=argo;
                            strcpy(retezec,"\0");
                            nothing=1;
                            break;
                        }
                        else{
                            fprintf(stderr,"error: argument %d=drows N M, 1 <= N <= M <= number of columns\n",argo);
                            return 0;
                        }
                    }
                    else{
                        nothing=2;
                    }
                }
            }
            if(argo < argc){
                if ((strcmp(argv[argo],"arow") == 0)){
                    arow(retezec,ar,&delim);
                    argo=argo+1;
                    arow_u=1;
                    nothing=1;
                    }
            }
            if(argo < argc){
                if (strcmp(argv[argo],"tolower") == 0){
                    if(argo+1<argc && atoi(argv[argo+1])>0 && num_of_c(retezec,delim)>=atoi(argv[argo+1])){
                        lower(retezec, delim, atoi(argv[argo+1]));
                        argo=argo+2;
                        nothing=1;
                    }
                    else{
                            fprintf(stderr,"error: argument %d=tolower N, 1 <= N\n",argo);
                            return 0;
                    }
                }
            }
            if(argo < argc){
                if (strcmp(argv[argo],"toupper") == 0){
                    if(argo+1<argc && atoi(argv[argo+1])>0 && num_of_c(retezec,delim)>=atoi(argv[argo+1])){
                        upper(retezec, delim, atoi(argv[argo+1]));
                        argo=argo+2;
                        nothing=1;
                        }
                    else{
                            fprintf(stderr,"error: argument %d=toupper N, 1 <= N\n",argo);
                            return 0;
                    }
                }
            }
            if(argo < argc){
                if (strcmp(argv[argo],"cset") == 0){
                    if(argo+2<argc && atoi(argv[argo+1])>0 && num_of_c(retezec,delim)>=atoi(argv[argo+1])){
                        if(strchr(argv[argo+2], delim)!=NULL){
                            fprintf(stderr,"error: argument %d=%s cannot contain a cell separator",argo+2,argv[argo+2]);
                            return 0;
                        }
                        cset(retezec, delim, atoi(argv[argo+1]), argv[argo+2]);
                        argo=argo+3;
                        nothing=1;
                        }
                    else{
                            fprintf(stderr,"error: argument %d=toupper N, 1 <= N\n",argo);
                            return 0;
                    }
                }
            }
            if (nothing==2){
                break;
            }
            else{
                if(nothing==0){
                    fprintf(stderr,"unknown argument %d=\"%s\"",argo,argv[argo]);
                    return 0;
                }
            }
        }
        printf("%s",retezec);
        a++;
    }
    if(arow_u==1){
        printf("\n%s",ar);
    }
    return 0;
}
