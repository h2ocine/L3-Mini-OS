#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>


void free_StingArrayArray(char **s, int taille)
{
    for (int i = 0; i < taille; i++)
        free(s[i]);

    free(s);
}

void affiche_mat(char **tab, int taille)
{
    for (int i = 0; i < taille; i++)
    {
        printf("tab[%d]: %s\n", i, tab[i]);
    }
}

int isIn(char **s, int taille, char *elem){
    for(int i=0; i<taille; i++){
        if(strcmp(s[i], elem) == 0) return 1;
    }
    return 0;
}

char **get_tabredirection(char **tab,int taille,int *tailleredirec){
    char **res = NULL;
    int tailleRes = 0;
    for(int i = 0;i<taille;i++){
        if( is_redirection(tab[i]) ==1){
            tailleRes++;
            
            res = realloc(res, sizeof(char *) * tailleRes);
            if(res ==NULL) perror("realloc");

            res[tailleRes-1] = malloc(strlen(tab[i])+1);
            snprintf(res[tailleRes-1], strlen(tab[i])+1, "%s", tab[i]);
            res[tailleRes-1][strlen(tab[i])] = '\0';
        }
    }
    *tailleredirec = tailleRes;
    return res;
}


char **getCmd(char **tab, int taille, char **s, int tailleS, int *t){
    char **res = NULL;
    int tailleRes = 0;
    for(int i=0; i<taille; i++){
        if(isIn(s, tailleS, tab[i])){
            *t = tailleRes;
            return res;
        }
        tailleRes++;
        res = realloc(res, sizeof(char *) * tailleRes);
        if(res == NULL) perror("realloc");

        res[tailleRes-1] = malloc(strlen(tab[i])+1);
        snprintf(res[tailleRes-1], strlen(tab[i])+1, "%s", tab[i]);
        res[tailleRes-1][strlen(tab[i])] = '\0';
    }

    *t = tailleRes;
    return res;
}

char **apres(char **tab, int taille, char **s, int tailleS, int *t){
    char **res = NULL;
    int tailleRes = 0;

    for(int i=0; i<taille; i++){
        if(i>0 && isIn(s, tailleS, tab[i-1])){
            tailleRes++;
            res = realloc(res, sizeof(char *) * tailleRes);
            if(res == NULL) perror("realloc");

            res[tailleRes-1] = malloc(strlen(tab[i])+1);
            snprintf(res[tailleRes-1], strlen(tab[i])+1, "%s", tab[i]);
            res[tailleRes-1][strlen(tab[i])] = '\0';
        }
    }
    *t = tailleRes;
    return res;

}

int main(void){
    char *tab[] = {"ls", "-a", "2>", "test.txt", "2>", "error.txt"};
    char *redirections[] = {">", "<", ">|", ">>", "2>", "2>|", "2>>"};

    int taille;
    char **res = getCmd(tab, 6, redirections, 7, &taille);

    affiche_mat(res, taille);

    free_StingArrayArray(res, taille);

    int taille2;
    char **res2 = apres(tab, 6, redirections, 7, &taille2);

    affiche_mat(res2, taille2);

    free_StingArrayArray(res2, taille2);

    return 0;
}