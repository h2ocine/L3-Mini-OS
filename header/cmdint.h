/*
    Ce header comporte toute les méthodes consernant les commandes internes
*/

#include "../header/cdaux.h"
#include "../header/pwdaux.h"
#include "../header/util.h"
#include "../header/cmdext.h"
#include "joker.h"  
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>


void recherche_commande_interne(char **tab, int *last_exit, int taille);

int exits(char *val, int last_exit);

int cd(char *option, char *ref);	

int pwd(int argc, char **argv);