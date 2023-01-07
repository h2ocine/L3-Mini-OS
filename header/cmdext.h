/*
    Ce header comporte toute les méthodes en lien à l'execution de commandes externes
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "../header/util.h"
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>

void execCMD(char *cmd, char **args,int *last_exit);

void commande_externe(char **tab, int taille,int *last_exit);
 
 