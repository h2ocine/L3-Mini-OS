#include "../header/util.h"

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

char *supString(char *s, char *sup)
{
    int size_sup = strlen(sup);

    // prefixe de s
    char *pre = malloc(size_sup + 1);
    snprintf(pre, size_sup + 1, "%s", s);
    pre[size_sup] = '\0';

    if (strcmp(pre, sup) == 0)
    {
        free(pre);
        size_t len = strlen(s) - size_sup;
        char *res = malloc(len + 1);
        snprintf(res, len + 1, "%s", &s[0 + size_sup]);

        return res;
    }
    free(pre);

    char *cpy = malloc(strlen(s) + 1);
    snprintf(cpy, strlen(s) + 1, "%s", s);
    cpy[strlen(s)] = '\0';

    return cpy;
}

// int main(void){
//     char *sup = "./";

//     char *p = "./a/b";
//     char *path = malloc(strlen(p)+1);
//     snprintf(path, strlen(p)+1, "%s", p);

//     char *res = supString(path, sup);
//     free(path);

//     printf("%s\n", res);
//     free(res);
// }

char *truncate_str(char *s, char spr)
{

    int stop = strlen(s);

    for (int i = stop - 1; i >= 0; i--)
    {
        stop--;
        if (s[i] == spr)
            break;
    }

    char *res = malloc(stop + 1);
    if (res == NULL)
        perror("malloc");

    if (snprintf(res, stop + 1, "%s", s) < 0)
        perror("snprintf");
    res[stop] = '\0';
    return res;
}

void videString(char *s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        s[i] = '\0';
    }
}

int isIn(char *path, char *fic)
{
    DIR *dir = opendir(path);
    if (dir == NULL) return 0;
    struct dirent *entry;

    if(dir == NULL) return 0;

    while ((entry = readdir(dir)))
    {
        if (strcmp(entry->d_name, fic) == 0)
        {
            closedir(dir);
            return 1;
        }
    }
    closedir(dir);
    return 0;
}

char **explode(char *str, const char *separators, int *taille)
{   
    int i = 0;
    int size = 0;
    char *s = NULL;
    char **res = NULL;

    // Cas chaine vide
    if (strlen(str) == 0)
    {
        *taille = 0;
        return NULL;
    }

    // Séparer la chaine en plusieurs sous chaines :
    char *strToken = strtok(str, separators);
    while (strToken != NULL)
    {
        // On copie strToken dans une chaine de caractère s (pour avoir utiliser la taille exact)
        if (!(s = malloc(strlen(strToken) + 1)))
            perror("malloc");
        if (snprintf(s, strlen(strToken) + 1, "%s", strToken) < 0)
        {
            perror("explode snprintf error ");
            exit(1);
        }
        s[strlen(strToken)] = '\0';
        // On ajoute la chaine de caractere s au tableau res
        size += 1;
        res = realloc(res, size * sizeof(char *)); // +1

        if (res == NULL)
            perror("fonction explode : realloc erreur ");

        res[i] = s;
        // res[i+1][] = '\0';
        i++;

        // On demande le token suivant.
        strToken = strtok(NULL, separators);
    }

    if (!s)
        free(s);
    free(strToken);

    *taille = i; // ici on retourne la taille de res
    return res;
}

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

// char **double_tab(char **tab, int taille)
// {
//     affiche_mat(tab, taille);
//     char **res = malloc(sizeof(char *) * (taille * 2));
//     if (res == NULL)
//         perror("malloc");
//     int i;

//     for (i = 0; i < taille; i++)
//     {
//         char *cpy = malloc(strlen(tab[i]) + 1);
//         snprintf(cpy, strlen(tab[i]), "%s", tab[i]);
//         cpy[strlen(tab[i])] = '\0';

//         char *cpy2 = malloc(strlen(tab[i]) + 1);
//         snprintf(cpy2, strlen(tab[i]), "%s", tab[i]);
//         cpy2[strlen(tab[i])] = '\0';

//         res[i] = cpy;
//         res[i + taille] = cpy2;
//     }
//     return res;
// }

char **add_last(char **tab, int taille, char *s)
{
    char **res = malloc(sizeof(char *) * taille);

    for (int i = 0; i < taille; i++)
    {
        res[i] = malloc(strlen(tab[i]) + strlen(s) + 1);
        snprintf(res[i], strlen(tab[i]) + 1, "%s", tab[i]);
        strncat(res[i], s, strlen(s));
        res[i][strlen(tab[i]) + strlen(s)] = '\0';
    }

    return res;
}

char **add_start(char **tab, int taille, char *s)
{
    char **res = malloc(sizeof(char *) * taille);
    for (int i = 0; i < taille; i++)
    {
        res[i] = malloc(strlen(tab[i]) + strlen(s) + 1);
        strncpy(res[i], s, strlen(s)+1);

        char *cpy = malloc(strlen(tab[i])+1);
        strncpy(cpy, tab[i], strlen(tab[i])+1);

        strncat(res[i], cpy, strlen(cpy));
        free(cpy);
    }
    return res;
}

char **begin_with(char *path, char *pre, int *taille)
{
    char **res = NULL;
    int res_size = 0;

    size_t size_pre = strlen(pre);

    DIR *dir = opendir(path);
    struct dirent *entry;

    if (dir == NULL)
    {
        taille = 0;
        return NULL;
    }

    while ((entry = readdir(dir)))
    {
        char *pre_entry = malloc(strlen(pre) + 1);
        snprintf(pre_entry, size_pre + 1, "%s", entry->d_name);
        pre_entry[size_pre] = '\0';

        if (strcmp(pre_entry, pre) == 0)
        {
            size_t size_entry = strlen(entry->d_name);

            res_size++;
            res = realloc(res, sizeof(char *) * res_size);
            res[res_size - 1] = malloc(size_entry + 1);
            snprintf(res[res_size - 1], size_entry + 1, "%s", entry->d_name);
            res[res_size - 1][size_entry] = '\0';
        }
        free(pre_entry);
    }
    closedir(dir);
    *taille = res_size;
    return res;
}

char **end_with(char *path, char *post, int *taille)
{
    char **res = NULL;
    int res_size = 0;

    size_t size_post = strlen(post);
    DIR *dir = opendir(path);
    struct dirent *entry;
    if (dir == NULL)
    {
        taille = 0;
        return NULL;
    }

    while ((entry = readdir(dir)))
    {   

        size_t size_entry = strlen(entry->d_name);
        char *post_entry = malloc(size_post + 1);
        snprintf(post_entry, size_post + 1, "%s", &entry->d_name[size_entry - size_post]);
        post_entry[size_post] = '\0';

        if (strcmp(post_entry, post) == 0)
        {   
            res_size++;
            res = realloc(res, sizeof(char *) * res_size);
            if(res == NULL){
                perror("realloc");
            }
    
            res[res_size - 1] = malloc(size_entry + 1);
            snprintf(res[res_size - 1], size_entry + 1, "%s", entry->d_name);
            res[res_size - 1][size_entry] = '\0';
        }
        free(post_entry);
    }
    closedir(dir);
    *taille = res_size;
    return res;
}

char **inter(char **tab1, int taille1, char **tab2, int taille2, int *taille)
{
    char **res = NULL;
    int size_res = 0;

    for (int i = 0; i < taille1; i++)
    {
        for (int j = 0; j < taille2; j++)
        {
            if (strcmp(tab1[i], tab2[j]) == 0)
            {
                size_res++;
                res = realloc(res, sizeof(char *) * size_res);

                res[size_res - 1] = malloc(strlen(tab1[i]) + 1);
                snprintf(res[size_res - 1], strlen(tab1[i]) + 1, "%s", tab1[i]);
                res[size_res - 1][strlen(tab1[i])] = '\0';
                break;
            }
        }
    }
    *taille = size_res;
    return res;
}

char **begin_end_with(char *path, char *pre, char *post, int *taille)
{
    int taille1;
    char **tabPre = begin_with(path, pre, &taille1);

    int taille2;
    char **tabPost = end_with(path, post, &taille2);

    char **res = inter(tabPre, taille1, tabPost, taille2, taille);

    free_StingArrayArray(tabPre, taille1);
    free_StingArrayArray(tabPost, taille2);

    return res;
}

char **all_fic(char *path, int *taille)
{

    char **res = NULL;
    size_t size_res = 0;
    DIR *dir = opendir(path);
    struct dirent *entry;

    if (dir == NULL)
        return res;

    while ((entry = readdir(dir)))
    {
        if (entry->d_name[0] == '.')
            continue;
        size_res++;
        res = realloc(res, size_res * sizeof(char *));
        res[size_res - 1] = malloc(strlen(entry->d_name) + 1);

        if (size_res == 1)
        {
            snprintf(res[0], strlen(entry->d_name) + 1, "%s", entry->d_name);
            res[0][strlen(entry->d_name)] = '\0';
        }
        else
        {
            snprintf(res[size_res - 1], strlen(entry->d_name) + 1, "%s", entry->d_name);
        }
    }
    *taille = size_res;
    return res;
}

char *with_slash(char *path)
{   
    if(strlen(path) == 0){
        char *res = malloc(1);
        res[0] = '\0';
        return res;
    }
    if (path[strlen(path) - 1] != '/')
    {
        char *new = malloc(strlen(path) + 2);
        snprintf(new, strlen(path) + 1, "%s", path);
        new[strlen(path)] = '/';
        new[strlen(path) + 1] = '\0';
        return new;
    }
    char *cpy = malloc(strlen(path)+1);
    strncpy(cpy, path, strlen(path)+1);

    return cpy;
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

char *truncate_start(char *s, int n){
    size_t size =((strlen(s) - n) < 0)? 0 : strlen(s) - n;
    char *res = malloc(size + 1);

    for(int i=0; i<size; i++){
        res[i] = s[i+n];
    }
    res[size] = '\0';
    return res;
}

char **delete_pre(char **tab, int taille, char *pre){
    char **res = malloc(sizeof(char *) * taille);

    for(int i=0; i<taille; i++){
        res[i] = supString(tab[i], pre);
    }
    return res;
}

char *good_path(char *path){
    char *res = malloc(1);
    int ind = 0;

    for(int i=0; i<strlen(path); i++){
        if(i != strlen(path)-1 && path[i] == '/' && path[i+1] == '/') continue;
        ind++;
        res = realloc(res, ind+1);
        res[ind-1] = path[i];
    }
    res[ind] = '\0';
    return res;
}


void cherche_true_false(int *last_exit, char **tabvaleurprompt, int newtaille)
{
    for (int i = 0; i < newtaille; i++)
    {
        if (strcmp(tabvaleurprompt[i], "true") == 0)
        {
            *last_exit = 0;
        }
        else if (strcmp(tabvaleurprompt[i], "false") == 0)
        {
            *last_exit = 1;
        }  
    }
}

char ** push(char **tab, int taille, char *elem){
    char **res = malloc(sizeof(char *) * (taille+1));
    if(res==NULL) perror("malloc");

    for(int i=0; i<taille; i++){
        res[i] = malloc(strlen(tab[i])+1);
        snprintf(res[i], strlen(tab[i])+1, "%s", tab[i]);
        res[i][strlen(tab[i])] = '\0';
    }
    res[taille] = malloc(strlen(elem)+1);
    snprintf(res[taille], strlen(elem)+1, "%s", elem);
    res[taille][strlen(elem)] = '\0';

    return res;
}
