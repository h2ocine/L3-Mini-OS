#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(void){
    int n = open("hoho.txt", O_RDONLY);
    printf("n: %d\n", n);
    close(n);
    return 0;
}