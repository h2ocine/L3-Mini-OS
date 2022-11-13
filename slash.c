
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "slash.h"

/*
    Libere la mémoire de toute les chaines de caractères presente dans s
*/
void frees(char **s){
    while(*s){
        printf("%s\n", *s);
        free(*s);
        s++;
    }
}

/**
 * Retourne un tableau de chaînes de caractères, chacune d'elle étant une sous-chaîne du paramètre str extraite en utilisant le séparateur separators
*/
char ** explode(char *str, const char *separators){
    int i = 0;
    size_t size = 1;
    char **res = malloc(size);
    char * strToken = strtok ( str, separators );
    while ( strToken != NULL ) {

        // On alloue dynamiquement un char * avec pour valeur strToken
        char *s = malloc(strlen(strToken));
        if(s == NULL) perror("malloc");
        sprintf(s, "%s", strToken);

        // On ajoute la chaine de caractere au tableau
        size += sizeof(char *);
        res = realloc(res, size);
        if(res == NULL) perror("realloc");
        res[i] = s;
        i++;

        // On demande le token suivant.
        strToken = strtok ( NULL, separators );
    }
    return res;
}


DIR *cd (char *pathname, char *option, char *ref){
    return;
}

int exit(char *val){
    return 0;
}

char *pwd(char *arg){
    return;
}

int main(void){
    DIR *dir;
    if((dir = opendir(".")) < 0) exit(1);

    char *ligne;
    ligne = malloc(MAX_ARGS_STRLEN);
    if(ligne == NULL) perror("malloc");

   

    while(1){
        char *pre = "$ ";
        // TODO: On affiche le prompt (invite de commande)
        if(write(1, pre, strlen(pre)) < 0) exit(1);

        // TODO: On lit la ligne (entré standart) et on la stocke dans ligne
        if(read(0, ligne, MAX_ARGS_STRLEN) < 0) exit(1);

        // TODO: On utilise readline pour simplifier la lecture         
        ligne = readline(ligne);

        // TODO: On ajoute la dernière commande à l'historique
        

        // TODO: On transforme la ligne en tableau
        char *delimiter = " ";
        char **tab = explode(ligne, delimiter);
        
        switch (tab[0]){
            case "exit": break;
        }
    }

    free(ligne);

    return 0;
}