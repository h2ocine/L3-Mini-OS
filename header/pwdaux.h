/*
    Ce header contient toute les méthodes auxiliaire à pwd
*/

#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

int pwd_affichage(char *ref);

int pwd_logique(char *ref);

int pwd_physique(char *ref);