
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

/**
 * Renvoie le répertoire ref (s'il s'agit d'une référence valide), le précédent répertoire de travail si le paramètre est -
 * 
 */
DIR *cd (char *pathname, char *option, char *ref);

/**
 * Termine le processus slash et renvoie comme valeur de retour val ou par default la valeur de retour de la dernière commande exécutée
 * 
 */
int exits(char *val);

/**
 * Affiche la (plus précisément), une référence absolue du répertoire de travail courant 
 */
int pwd(char **arg);
