/**
 * IZP: lab 7.
 * Dynamic memory management: strings.
 * @author Roman Andriushchenko
 * @note Complete the code and submit the file as 'xloginxx_lab7_1.c' to WIS
 */
#include<stdio.h>
#include<stdlib.h>
// Note: you are not allowed to use <string.h>

/**
 * Read a line (line ends with '\n') from standard input.
 * Return a string (array ended with '\0') or NULL if any error occurs.
 */
char *read_line() {
    char *str=malloc(1*sizeof(char));
    if(str==NULL){
        fprintf(stderr," allocation error.\n");
        return NULL;
    }
    int input;
    int length=0;
    while((input=getchar())!='\n'){
        length++;
        char *reallocated=realloc(str,(length+1)*sizeof(char));
        if(reallocated==NULL){
            fprintf(stderr," allocation error.\n");
            free(str);
            return NULL;
        }
        str=reallocated;
        str[length-1]=input;
    }
    str[length]='\0';
    return str;
}

/** Compute a length of a string. Return NULL if any error occurs. */
int str_len(const char *str) {
    int length=0;
    while (str[length]!='\0'){
        length++;
    }
    return length;
}

/** Duplicate a string. Return NULL if any error occurs. */
char *str_copy(const char *str) {
    char *str_c=malloc((str_len(str)+1)*sizeof(char));
    if(str_c==NULL){
        fprintf(stderr," allocation error.\n");
        return NULL;
    }
    int i=0;
    while(i<=str_len(str)+1){
        str_c[i]=str[i];
        i++;
    }
    return str_c;
}

/** Concatenate two strings. Return NULL if any error occurs. */
char *str_cat(const char *prefix, const char *suffix) {
    int len1=str_len(prefix);
    int len2=str_len(suffix);
    char *str_ca=malloc((len1+len2+1)*sizeof(char));
    if(str_ca==NULL){
        fprintf(stderr," allocation error.\n");
        return NULL;
    }
    int i=0;
    while(i<str_len(prefix)){
        str_ca[i]=prefix[i];
        i++;
    }

    while(i<=(len1+len2)){
        str_ca[i]=suffix[i-len1];
        i++;
    }
    return str_ca;
}

// BONUS: implement this only after submitting the primary part of the program.
/**
 * Returns a pointer to the first occurrence of part in full, or NULL if part
 * is not a substring of full.
 */
char *str_locate(const char *full, const char *part) {
     int len1=str_len(full);
     int len2=str_len(part);
     int k=0;
     if(len1 >= len2){
        for(int i=0; i<len1; i++){
            if(full[i] == part[0]){
                k=i;
                for(int j=0; j<len2; j++) {
                    if(full[i+j] == part[j]) {
                        if(part[j] == '\0') {
                            return full[k];
                        }
                    }
                    else{
                        return NULL;
                    }
                }
            }
        }
     }
return NULL;
}


int main() {

    char *line1;
    char *line2;

    line1=read_line();
    line2=read_line();
    printf("Line 1: '%s'. Line 2: '%s'.\n", line1, line2);

    int len1, len2;
    len1=str_len(line1);
    len2=str_len(line2);
    printf("Line 1 is of length %d. Line 2 is of length %d.\n", len1, len2);

    char *line_dup;
    line_dup=str_copy(line1);
    printf("Line 1: '%s'. Duplicate of line 1: '%s'\n", line1, line_dup);

    char *line_cat;
    line_cat=str_cat(line1, line2);
    printf("Lines 1 & 2 concatenated: '%s'.\n", line_cat);

    printf("Line 2 is %s a substring of line 1.\n", str_locate(line1,line2) ? "\b" : "");

    free(line1);
    free(line2);
    free(line_dup);
    free(line_cat);
    // success
    return 0;
}
