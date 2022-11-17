#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>

void frees(char **s){
    int i=0;
    while(*s){
        free(*s);
        i++;
        s++;
    }
    s-=i;
    free(s);
}

/**
 * Retourne un tableau de chaînes de caractères, chacune d'elle étant une sous-chaîne du paramètre str extraite en utilisant le séparateur separators
*/
int explode(char *str, const char *separators, char **res){
    int i = 0;
    size_t size = 0;
    if(strlen(str) == 0) return 0;

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
    return i;
}

int main(void){
    char **tab = malloc(0);

    while(1){
        printf("$");
        char *ligne = readline("");
        int taille = explode(ligne, " ", tab);
        printf("taille: %d\n", taille);
    }
}