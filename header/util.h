/* 
    Ce header rassemble toute les methodes dites 'utile' pour notre projet
*/
#include <string.h> 
#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>

char **copie_tab(char **tab, int taille);

// renvoie une chaine de caractere sans le prefixe sup si il existe sinon une copie de s
char *supString(char *s, char *sup);

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

// affiche un tableau de chaine de caractère
void affiche_mat(char **tab, int taille);

// double la taille du tableau et en crée une symetrie; ex: ["a", "b", "c"] -> ["a", "b", "c", "a", "b", "c"]
char ** double_tab(char **tab, int taille);

    // On ajoute à toute les elements de tab (à la fin) la chaine s; ex: add_last ["a", "b"] "/" -> ["a/", "b/"]
char ** add_last(char **tab, int taille, char *s);

// ajoute au debut de toute les chaines de caracteres de tab la chaine de caractere s
char **add_start(char **tab, int taille, char *s);

// renvoie sous forme de tableau tout les fichiers dans path ayant pour préfixe pre et met le nombre d'élément dant $taille
char **begin_with(char *path, char *pre, int *taille);

// renvoie sous forme de tableau tout les fichiers et dossier dans path ayant pour suffixe post et met la taille de ce tableau dans taille
char **end_with(char *path, char *post, int *taille);

// renvoie l'intersection de deux tableaux (tout les éléments présent à la fois dans tab1 et tab2)
char **inter(char **tab1, int taille1, char **tab2, int taille2, int *taille);

// renvoie sous forme de tableau tout les fichiers ayant pour prefiche $pre et suffixe $post et met la taille de ce tableau dans taille
char **begin_end_with(char *path, char *pre, char *post, int *taille);

// renvoie tout les fichiers (sauf ceux commencant par un point) dans path
char **all_fic(char *path, int *taille);

// renvoie path si elle termine par le caractère '/' sinon elle renvoie une nouvelle chaine composer de path et un slash à la fin
char *with_slash(char *path);

// renvoie un nouveau tableau resultant de l'ajout de tout les éléments de tab1 puis l'ajout de tout les éléments de tab2
char **cat_tabs(char **tab1, int taille1, char **tab2, int taille2, int *taille);

// renvoie une chaine de caractère sans les n premiers charactere
char *truncate_start(char *s, int n);

// renvoie un tableau former des elements de tab sans le prefixe pre; ex: ["./a", "./b/c", "./m"] "./" -> ["a", "b/c", "m"]
char **delete_pre(char **tab, int taille, char *pre);

// Enleve tout les slash en trop; /tmp//a/b//c/// -> /tmp/a/b/c
char *good_path(char *path);

void cherche_true_false(int *last_exit, char **tabvaleurprompt, int newtaille);