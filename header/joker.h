#include "util.h"

// renvoie sous forme de tableau le liste des fichiers dans path selon l'input et met la taille du tableau dans $taille;
// * -> represente tout les fichers, *.c -> tout les fichers ayant pour suffixe .c, a* -> tout les fichiers commancant par a ...
char **trans(char *path, char *input, int *taille);

// renvoie un tableau de tout les fichiers selon les jokers input en partant du fichier racine dos
char ** all_joker_fic(char *input ,char *dos, int *t);