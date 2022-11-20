#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <signal.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096
#define TAILLE_PROMPT 47 //7 (rouge ou vert) + 5 (bleu) + 4 (blanc) + 1 ('\0')

/**
 * 
 * 
 */
int cd (char *pathname, char *option, char *ref);

/**
 * Termine le processus slash et renvoie comme valeur de retour val ou par default la valeur de retour de la dernière commande exécutée
 * 
 */
int exits(char *val,int last_exit);

/**
 * Affiche la (plus précisément), une référence absolue du répertoire de travail courant 
 */
int pwd(int argc, char **argv);

DIR *dir;
char dossier_courant[MAX_ARGS_NUMBER]; 
char oldPath[MAX_ARGS_NUMBER];