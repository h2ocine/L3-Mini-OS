/*
    Ce header comporte toute les méthodes auxiliare à la commande cd
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"
#include <limits.h>

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

char *logiquePath(char *path);

int cd_physique(char *path, char *ref);

int cd_logique(char *path, char *ref);