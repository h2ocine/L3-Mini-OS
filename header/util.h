/* 
    Ce header rassemble toute les methodes dites 'utile' pour notre projet
*/
#include <string.h> 
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

/**
 *  raccourci une chaine de caractere jusqua la premiere occurence (comprise) d'un caractere donné
 */
char *truncate_str(char *s, char spr);

/**
 * @brief vide une chaine de caractere alloué par un malloc ; ex: videString(s) -> "" 
 * 
 * @param s 
 */
void videString(char *s);

/**
 * Retourne 1 si le fichier fic est present dans path
 */
int isIn(char *path, char *fic);

/**
 * @brief retourne un tableau (aloue dynamiquement) de chaînes de caractères, chacune d'elle étant une sous-chaîne du 
 * paramètre str extraite en utilisant le séparateur separators et met sa taille dans la variable taille
 */
char **explode(char *str, const char *separators, int *taille);

/* libere la memoire d'un tableau de chaine de caracteres et l'ensemble de ses chaines de caractères */
void free_StingArrayArray(char **s, int taille);