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

int main(void){
    char *s = "123456789";

    char *res = truncate_start(s, 3);
    printf("res: %s\n", res);
    free(res);

    return 0;
}