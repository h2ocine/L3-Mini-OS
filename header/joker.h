#include <stdio.h>
#include "util.h"
#include "cmdext.h"
#include <dirent.h>

char is_valid(char *chaine);
char *get_postfix(char *chaine);
int is_postfix(char *chaine, char *pre);
int nombre_mot_char(char *chaine, char separator);
char *get_balise(char *path);
char *get_pathtobalise(char *path);
int joker_1(char* path, char* argument_commande);