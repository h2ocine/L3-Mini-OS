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
int exit(char *val);

/**
 * Affiche la (plus précisément), une référence absolue du répertoire de travail courant 
 */
char *pwd(char *arg);
