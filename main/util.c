#include "../header/util.h"

char *supString(char *s, char *sup){
    int size_sup = strlen(sup);

    // prefixe de s
    char *pre = malloc(size_sup+1);
    snprintf(pre, size_sup+1, "%s", s);
    pre[size_sup] = '\0';

    if(strcmp(pre, sup) == 0){
        free(pre);
        size_t len = strlen(s) - size_sup;
        char *res = malloc(len+1);
        snprintf(res, len + 1, "%s", &s[0+size_sup]);
        
        return res;
    }
    free(pre);

    char *cpy = malloc(strlen(s)+1);
    snprintf(cpy, strlen(s)+1, "%s", s);
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
    if(res == NULL) perror("malloc");

    if(snprintf(res, stop + 1, "%s", s) < 0) perror("snprintf");
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
    struct dirent *entry;

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

void affiche_mat(char **tab, int taille){
    for(int i=0; i<taille; i++){
        printf("tab[%d]: %s\n", i, tab[i]);
    }
}

char ** double_tab(char **tab, int taille){
    affiche_mat(tab, taille);
    char ** res = malloc(sizeof(char *) * (taille*2));
    if(res == NULL) perror("malloc");
    int i;
    
    for(i=0; i<taille; i++){
        char *cpy = malloc(strlen(tab[i])+1);
        snprintf(cpy, strlen(tab[i]), "%s", tab[i]);
        cpy[strlen(tab[i])] = '\0';

        char *cpy2 = malloc(strlen(tab[i])+1);
        snprintf(cpy2, strlen(tab[i]), "%s", tab[i]);
        cpy2[strlen(tab[i])] = '\0';

        res[i] = cpy;
        res[i + taille] = cpy2;
    }
    return res;

}

char ** add_last(char **tab, int taille, char *s){
    char **res = malloc(sizeof(char *) * taille);

    for(int i=0; i<taille; i++){
        char *cpy = malloc(strlen(tab[i]) + strlen(s) +1);
        snprintf(cpy, strlen(tab[i])+1, "%s", tab[i]);
        strncat(cpy, s, strlen(s));
        cpy[strlen(tab[i]) + strlen(s)] = '\0';

        res[i] = cpy;
    }

    return res;
}

char **add_start(char **tab, int taille, char *s){
    char **res = malloc(sizeof(char *) * taille);
    for(int i=0; i<taille; i++){
        char *cpy = malloc(strlen(tab[i]) + strlen(s) +1);
        snprintf(cpy, strlen(s)+1, "%s", s);
        cpy[strlen(s)] = '\0';
        strncat(cpy, tab[i], strlen(tab[i]));
    
        res[i] = cpy;
    }

    return res;
}

char **begin_with(char *path, char *pre, int *taille){
    char **res = NULL;
    int res_size = 0;
    
    size_t size_pre = strlen(pre);

    DIR *dir = opendir(path);
    struct dirent *entry;

    if(dir == NULL){
        taille = 0;
        return NULL;
    } 

    while ((entry = readdir(dir)))
    {   
        
        char *pre_entry = malloc(strlen(pre)+1);
        snprintf(pre_entry, size_pre+1, "%s", entry->d_name);
        pre_entry[size_pre] = '\0';

        if(strcmp(pre_entry, pre) == 0){
            size_t size_entry = strlen(entry->d_name);

            res_size++;
            res = realloc(res, sizeof(char *) * res_size);
            res[res_size-1] = malloc(size_entry +1);
            snprintf(res[res_size-1], size_entry+1, "%s", entry->d_name);
            res[res_size-1][size_entry] = '\0';
        }
    }
    closedir(dir);
    *taille = res_size;
    return res;

}

char **end_with(char *path, char *post, int *taille){
    char **res = NULL;
    int res_size = 0;
    
    size_t size_post = strlen(post);
    DIR *dir = opendir(path);
    struct dirent *entry;
    if(dir == NULL){
        taille = 0;
        return NULL;
    } 

    while ((entry = readdir(dir)))
    {   
        size_t size_entry = strlen(entry->d_name);
        char *post_entry = malloc(size_post+1);
        snprintf(post_entry, size_post+1, "%s", &entry->d_name[size_entry-size_post]);
        post_entry[size_post] = '\0';

        if(strcmp(post_entry, post) == 0){

            res_size++;
            res = realloc(res, sizeof(char *) * res_size);
            res[res_size-1] = malloc(size_entry +1);
            snprintf(res[res_size-1], size_entry+1, "%s", entry->d_name);
            res[res_size-1][size_entry] = '\0';
        }
    }
    closedir(dir);
    *taille = res_size;
    return res;

}

char **inter(char **tab1, int taille1, char **tab2, int taille2, int *taille){
    char **res = NULL;
    int size_res = 0;

    for(int i=0; i<taille1; i++){
        for(int j=0; j<taille2; j++){
            if(strcmp(tab1[i], tab2[j]) == 0){
                size_res++;
                res = realloc(res, sizeof(char *) * size_res);

                char *cpy = malloc(strlen(tab1[i]) +1);
                snprintf(cpy, strlen(tab1[i])+1, "%s", tab1[i]);
                cpy[strlen(tab1[i])] = '\0';

                res[size_res-1] = cpy;
                break;
            }
        }
    }
    *taille = size_res;
    return res;
}

char **begin_end_with(char *path, char *pre, char *post, int *taille){
    int taille1;
    char **tabPre = begin_with(path, pre, &taille1);

    int taille2;
    char **tabPost = end_with(path, post, &taille2);

    char **res = inter(tabPre, taille1, tabPost, taille2, taille);

    free_StingArrayArray(tabPre, taille1);
    free_StingArrayArray(tabPost, taille2);
    
    return res;

}

char **all_fic(char *path, int *taille){

    char **res = NULL;
    size_t size_res = 0;
    DIR *dir = opendir(path);
    struct dirent *entry;

    if(dir == NULL) return res;
    
    while((entry = readdir(dir))){
        if(entry->d_name[0] == '.') continue;
        size_res ++;
        res = realloc(res, size_res * sizeof(char *));
        res[size_res-1] = malloc(strlen(entry->d_name)+1);

        if(size_res == 1){
            snprintf(res[0], strlen(entry->d_name)+1, "%s", entry->d_name);
            res[0][strlen(entry->d_name)] = '\0';
        }else{
            snprintf(res[size_res - 1], strlen(entry->d_name)+1,"%s", entry->d_name);
        }
    }
    *taille = size_res;
    return res;
}

char *with_slash(char *path){
    if(path[strlen(path)-1] != '/'){
        char *new = malloc(strlen(path)+2);
        snprintf(new, strlen(path)+1, "%s", path);
        new[strlen(path)] = '/';
        new[strlen(path)+1] = '\0';
        return new;
    }
    return path;
}

char **cat_tabs(char **tab1, int taille1, char **tab2, int taille2, int *taille){
    char **res = malloc(sizeof(char *) * (taille1 + taille2));
    for(int i=0; i<taille1; i++){
        char *cpy = malloc(strlen(tab1[i]) +1);
        snprintf(cpy, strlen(tab1[i])+1, "%s", tab1[i]);
        cpy[strlen(tab1[i])] = '\0';
    
        res[i] = cpy;
    }

    for(int j=0; j<taille2; j++){
        char *cpy = malloc(strlen(tab2[j]) +1);
        snprintf(cpy, strlen(tab2[j])+1, "%s", tab2[j]);
        cpy[strlen(tab2[j])] = '\0';

        res[j + taille1] = cpy;
    }
   
    *taille = taille1 + taille2;
    return res;

}