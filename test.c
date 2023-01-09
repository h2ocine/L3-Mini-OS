#include <stdio.h>
#include <stdlib.h>


int main(){
    setenv("MY_VAR", "hello", 1);
    char *value = getenv("MY_VAR");
    printf("MY_VAR = %s\n", value);

return 0;

}

