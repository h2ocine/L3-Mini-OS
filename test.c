#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>


void push(char **tab, int taille, char *elem){
    tab = realloc(tab, sizeof(char *) * (taille+1));
    if(tab == NULL) perror("realloc");

    tab[taille] = malloc(strlen(elem)+1);
    snprintf(tab[taille], strlen(elem)+1, "%s", elem);
    tab[taille][strlen(elem)] = '\0';
}

void free_StingArrayArray(char **s, int taille)
{
    for (int i = 0; i < taille; i++)
        free(s[i]);

    free(s);
}

char *transformeEnChemin(char *dossier, char *sousDossier){
    size_t size;
    char *res = NULL;
    if(dossier[strlen(dossier)-1] == '/'){
        size = strlen(dossier) + strlen(sousDossier)+1;
        res = malloc(size);
        if(res == NULL) perror("malloc");

        snprintf(res, strlen(dossier)+1, "%s", dossier);
        res[strlen(dossier)] = '\0';
        strncat(res, sousDossier, strlen(sousDossier));
    }else{
        size = strlen(dossier) + 1 + strlen(sousDossier) + 1;
        res = malloc(size);
        if(res == NULL) perror("malloc");
        
        snprintf(res, strlen(dossier)+1, "%s", dossier);
        res[strlen(dossier)] = '/';
        res[strlen(dossier)+1] = '\0';
        strncat(res, sousDossier, strlen(sousDossier));
    }
    return res;
}

char **copie_tab(char **tab, int taille)
{
    char **cpy = malloc(taille * sizeof(char *));
    for (int i = 0; i < taille; i++)
    {
        cpy[i] = malloc(strlen(tab[i]) + 1);
        snprintf(cpy[i], strlen(tab[i]) + 1, "%s", tab[i]);
        cpy[i][strlen(tab[i])] = '\0';
    }
    return cpy;
}

void affiche_mat(char **tab, int taille)
{
    for (int i = 0; i < taille; i++)
    {
        printf("tab[%d]: %s\n", i, tab[i]);
    }
}

char **cat_tabs(char **tab1, int taille1, char **tab2, int taille2, int *taille)
{   
    char **res = malloc(sizeof(char *) * (taille1 + taille2));
    
    for (int i = 0; i < taille1; i++)
    {   
        res[i] = malloc(strlen(tab1[i]) + 1);
        snprintf(res[i], strlen(tab1[i]) + 1, "%s", tab1[i]);
        res[i][strlen(tab1[i])] = '\0';
    }
    for (int j = 0; j < taille2; j++)
    {   
        res[j + taille1] = malloc(strlen(tab2[j]) + 1);
        snprintf(res[j + taille1], strlen(tab2[j]) + 1, "%s", tab2[j]);
        res[j + taille1][strlen(tab2[j])] = '\0';
    }

    *taille = taille1 + taille2;
    return res;
}

char **doubleEtoile(char *dossierCourant, int *taille){
    
    DIR *dir = opendir(dossierCourant);
    if(dir == NULL) perror("opendir");
    struct dirent *entry;

    int tailleRes = 1;
    char **res = malloc(sizeof(char *));
    if(res == NULL) perror("malloc");

    res[0] = malloc(strlen(dossierCourant)+1);
    if(res[0] == NULL) perror("malloc");
    

    snprintf(res[0], strlen(dossierCourant)+1, "%s", dossierCourant);
    res[0][strlen(dossierCourant)] = '\0';

    while((entry = readdir(dir))){
        if(strcmp(entry -> d_name, ".") == 0 || strcmp(entry -> d_name, "..") == 0 || entry->d_name[0] == '.') continue;

        char *path = transformeEnChemin(dossierCourant, entry->d_name);
        struct stat st;
        if(stat(path, &st) <0) perror("stat");

        // On vérifie que l'entrée est un dossier et n'est pas un lien symbolique
        if(S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode)){

            char **cpyRes = copie_tab(res, tailleRes);
            int tailleCpyRes = tailleRes;

            int tailleRecherche;
            char **recherche = doubleEtoile(path, &tailleRecherche);

            free_StingArrayArray(res, tailleRes);

            res = cat_tabs(cpyRes, tailleRes, recherche, tailleRecherche, &tailleRes);

            free_StingArrayArray(cpyRes, tailleCpyRes);
            free_StingArrayArray(recherche, tailleRecherche);

        }
    }
    closedir(dir);

    *taille = tailleRes;
    return res;
}

char **doubleEtoileInit(int *taille){
    return doubleEtoile("./", taille);
}

void testDoubleEtoile(){
    int taille;
    char *dossierCourant = "./";

    char **dossiers = doubleEtoile(dossierCourant, &taille);
    affiche_mat(dossiers, taille);
    free_StingArrayArray(dossiers, taille);
}

int main(void){
    testDoubleEtoile();
    return 0;
}