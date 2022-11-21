#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void truncateString(char *s, int n){
    int len = strlen(s);
    for(int i= len-1; i>= len - n; i--){
        s[i] = '\0';
    }
}

char**  explode(char *str, const char *separators, int* taille)
{
    int i = 0;
    int size = 0;
    char* s = NULL;
    char** res  = malloc(0);
    if(res == NULL) 
        perror("malloc");

    //Cas chaine vide
    if(strlen(str) == 0)
    {
        *taille = 0;
        return NULL;
    } 

    //Séparer la chaine en plusieurs sous chaines :
    char * strToken = strtok (str, separators);
    while ( strToken != NULL ) 
    {
        // On copie strToken dans une chaine de caractère s (pour avoir utiliser la taille exact)
        if(!(s = malloc(strlen(strToken) + 1))) 
            perror("malloc");
        if(snprintf(s, strlen(strToken)+1, "%s", strToken) < 0)
        {
            perror("explode snprintf error ");
            exit(1);
        }

        //On ajoute la chaine de caractere s au tableau res
        size += 1;
        res = realloc(res, size * sizeof(char *));

        if(res == NULL) 
            perror("fonction explode : realloc erreur ");

        res[i] = s;
        i++;

        // On demande le token suivant.
        strToken = strtok ( NULL, separators );
    }
    
    if(!s)  
        free(s);
    free(strToken);

    *taille = i;//ici on retourne la taille de res
    return res;
}


char * truncate_str(char *s, char spr){

    int stop = strlen(s);

    for(int i=stop-1; i>=0; i--){
        stop--;
        if(s[i] == spr) break;
    }
    printf("stop: %ld\n", strlen(s) - stop);

    // printf("resultat: %s\n", res);
    char *res = malloc(stop + 1);
    snprintf(res, stop + 1 , "%s", s);
    res[stop] = '\0';
    return res;
}

void videString(char *s){
    for(int i=0; i<strlen(s); i++){
        s[i] = '\0';
    }
}

char *logiquePath(char *path){
    if(path[strlen(path)-1] == '/') perror("mauvais format");
    int taille;
    char **tab = explode(path, "/", &taille);
    free(path);

    char *res = malloc(1);
    res[0] = '\0';
    for(int i=0; i<taille; i++){
        if(strcmp(tab[i], ".") == 0){
            continue;

        // On revient au dossier parent
        }
        else if(strcmp(tab[i], "..") == 0)
        {   
            if(strlen(res) != 0){
                char *s = truncate_str(res, '/');
                videString(res);
                res = realloc(res, strlen(s) + 1);
                snprintf(res, strlen(s)+1, "%s", s);
                res[strlen(s)] = '\0';
                free(s);
            }

        }
        else
        {   
            int t = strlen(res) + strlen(tab[i]) + 2;
            res = realloc(res, strlen(res) + strlen(tab[i]) + 2);

            strcat(res, "/");
            strcat(res, tab[i]);
            res[strlen(res)] = '\0';
        }
    }
    if(strlen(res) == 0){
        res = realloc(res, 2);
        res[0] = '/';
        res[1] = '\0';
    }
    return res;

}

int main(void){
    char *s = "/home/adam/Cours/L3/SY/projet-systeme/a/b/../../../Test/././../Test/.";
    char *path = malloc(strlen(s)+1);
    snprintf(path, strlen(s)+1, "%s", s);
    path[strlen(s)] = '\0';

    char *real = logiquePath(path);

    printf("real: %s\n", real);

    free(real);
}