#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

char *truncate_start(char *s, int n){
    size_t size =((strlen(s) - n) < 0)? 0 : strlen(s) - n;

    char *res = malloc(size + 1);

    for(int i=0; i<size; i++){
        res[i] = s[i+n];
    }
    res[size] = '\0';
    return res;
}

char *good_path(char *path){
    char *res = malloc(1);
    int ind = 0;

    for(int i=0; i<strlen(path); i++){
        if(i != strlen(path)-1 && path[i] == '/' && path[i+1] == '/') continue;
        ind++;
        res = realloc(res, ind+1);
        res[ind-1] = path[i];
        printf("res[%d]: %c\n",ind-1 , res[ind-1]);
        printf("path[%d]: %c\n",i , path[i]);
    }
    res[ind-1] = '\0';
    return res;
}

int main(void){
    char *sal = "tmp//a/b///c//";
    char *res = good_path(sal);
    printf("res: %s\n", res);

    free(res);

    return 0;
}