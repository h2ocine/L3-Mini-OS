/*
    Ce header contient toute les méthodes auxiliaire à redirections
*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>





//fonction qui check quel type de redirection
int check_redirection(char **tab,int taille,int *last_exit);

