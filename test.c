#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

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

// affiche un tableau de chaine de caractère
void affiche_mat(char **tab, int taille){
    for(int i=0; i<taille; i++){
        printf("tab[%d]: %s\n", i, tab[i]);
    }
}

// double la taille du tableau et en crée une symetrie; ex: ["a", "b", "c"] -> ["a", "b", "c", "a", "b", "c"]
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

void test_double_tab(){
    printf("okk\n");
    char *c1 = "Premiere ligne";
    char *c2 = "Seconde ligne";
    char *c3 = "Troisieme ligne";

    char **tab = malloc(sizeof(char *) * 3);

    tab[0] = malloc(strlen(c1)+1);
    snprintf(tab[0], strlen(c1)+1, "%s", c1);
    tab[0][strlen(c1)] = '\0';

    tab[1] = malloc(strlen(c2)+1);
    snprintf(tab[1], strlen(c2)+1, "%s", c2);
    tab[1][strlen(c2)] = '\0';

    tab[2] = malloc(strlen(c3)+1);
    snprintf(tab[2], strlen(c3)+1, "%s", c3);
    tab[2][strlen(c3)] = '\0';

    printf("okk\n");
    char **d = double_tab(tab, 3);
    affiche_mat(d, 6);

    free_StingArrayArray(tab, 3);
    free_StingArrayArray(d, 6);
}

// On ajoute à toute les elements de tab (à la fin) la chaine s; ex: add_last ["a", "b"] "/" -> ["a/", "b/"]
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

void test_add_last(){
    char *c1 = "Premiere ligne";
    char *c2 = "Seconde ligne";
    char *c3 = "Troisieme ligne";

    char **tab = malloc(sizeof(char *) * 3);

    tab[0] = malloc(strlen(c1)+1);
    snprintf(tab[0], strlen(c1)+1, "%s", c1);
    tab[0][strlen(c1)] = '\0';

    tab[1] = malloc(strlen(c2)+1);
    snprintf(tab[1], strlen(c2)+1, "%s", c2);
    tab[1][strlen(c2)] = '\0';

    tab[2] = malloc(strlen(c3)+1);
    snprintf(tab[2], strlen(c3)+1, "%s", c3);
    tab[2][strlen(c3)] = '\0';

    char **new = add_last(tab, 3, "/test");
    affiche_mat(new, 3);

    free_StingArrayArray(tab, 3);
    free_StingArrayArray(new, 3);
}

// renvoie sous forme de tableau tout les fichiers et dossier dans path ayant pour prefixe pre et met la taille de ce tableau dans taille
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

// renvoie sous forme de tableau tout les fichiers et dossier dans path ayant pour suffixe post et met la taille de ce tableau dans taille
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

// renvoie l'intersection de deux tableaux (tout les éléments présent à la fois dans tab1 et tab2)
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

void test_begin_with(){
    char *path = ".";
    char *pre = "test";

    int t;
    char **res = begin_with(path, pre, &t);
    affiche_mat(res, t);

    free_StingArrayArray(res, t);
}

void test_end_with(){
    char *path = ".";
    char *post = ".c";

    int t;
    char **res = end_with(path, post, &t);
    printf("t: %d\n", t);
    affiche_mat(res, t);

    free_StingArrayArray(res, t);
}

void test_begin_end_with(){
    char *path = ".";
    char *pre = "fonc";
    char *post = ".c";

    int t;
    char **res = begin_end_with(path, pre, post, &t);
    printf("%d\n", t);
    affiche_mat(res, t);
    free_StingArrayArray(res, t);

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

void test_all_fic(){
    char *path = ".";
    int taille;

    char **res = all_fic(path, &taille);
    affiche_mat(res, taille);

    free_StingArrayArray(res, taille);
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

char **trans(char *path, char *input, int *taille){
    if(strchr(input, '*') == NULL){
        char **res = malloc(sizeof(char *));
        
        char *cpy;
        cpy = malloc(strlen(input)+1);
        snprintf(cpy, strlen(input)+1, "%s", input);

        res[0] = cpy;
        *taille = 1;
        return res;
    }
   
    int t;
    // On commence par separer le chemin
    char **tab = explode(input, "*", &t);
    
    
    // cas où input = "*"
    if(t == 0){
        return all_fic(path, taille);

    }else if(t != 2){
        char *pre = malloc(strlen(tab[0])+1);
        snprintf(pre, strlen(tab[0])+1, "%s", tab[0]);
        pre[strlen(tab[0])] = '\0';
        
        if(input[0] == '*'){
            return end_with(path, pre, taille);
        }else{ 
            char **res = begin_with(path, pre, taille);
            return res;
        }
    }else{
        char *pre = malloc(strlen(tab[0])+1);
        snprintf(pre, strlen(tab[0])+1, "%s", tab[0]);
        pre[strlen(tab[0])] = '\0';

        char *post = malloc(strlen(tab[1])+1);
        snprintf(post, strlen(tab[1])+1, "%s", tab[1]);
        post[strlen(tab[1])] = '\0';

        return begin_end_with(path, pre, post, taille);
    }
    
}

void test_trans(){
    char *path = "./header";
    char *input = "cmd*";

    char *cpyInput = malloc(strlen(input)+1);
    snprintf(cpyInput, strlen(input)+1, "%s", input);
    cpyInput[strlen(input)] = '\0';

    int taille;
    char **res = trans(path, cpyInput, &taille);    
    affiche_mat(res, taille);

    free_StingArrayArray(res, taille);
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

char ** joker(char *input ,char *dos, int *t){
    int size_tab;
    // On commence par separer le input 
    char **tab = explode(input, "/", &size_tab);
    char *cpyDos;
    if(dos[strlen(dos)-1] != '/'){
        cpyDos = malloc(strlen(dos) + 2);
        snprintf(cpyDos, strlen(dos)+1, "%s", dos);

        cpyDos[strlen(dos)] = '/';
        cpyDos[strlen(dos)+1] = '\0';
    }else{
        cpyDos = malloc(strlen(dos)+1);
        snprintf(cpyDos, strlen(dos)+1, "%s", dos);
        cpyDos[strlen(dos)] = '\0';
    }

    int size_pos = 0;
    char **possibilite = trans(cpyDos, tab[0], &size_pos);
    
    char **res = NULL;
    int size_res = 0;

    // Dans le cas où l'on doit chercher dans le dossier dos
    if(size_tab == 1){
        *t = 0;
        res = trans(cpyDos, input, t);
        free_StingArrayArray(possibilite, size_pos);
        free_StingArrayArray(tab, size_tab);
        res = add_start(res, *t, cpyDos);
        return res;
    }
    

    for(int i=0; i<size_pos; i++){
        // Nouveau chemin
        char *newDos;
        if(dos[strlen(cpyDos)-1] != '/'){
            newDos = malloc(strlen(cpyDos) + strlen(possibilite[i]) + 1);
            snprintf(newDos, strlen(cpyDos)+1, "%s", cpyDos);
            newDos[strlen(cpyDos)] = '\0';
            strncat(newDos, possibilite[i], strlen(possibilite[i]));
        }else{
            newDos = malloc(strlen(cpyDos) + strlen(possibilite[i]) + 1);
            snprintf(newDos, strlen(cpyDos)+1, "%s", cpyDos);
            newDos[strlen(cpyDos)] = '\0';

            strncat(newDos, possibilite[i], strlen(possibilite[i]));  
        }
        

        // Nouveau input
        char *newInput = NULL;
        size_t size_newInput = 0;
        for(int j=1; j<size_tab; j++){
            if(j != size_tab-1){
                size_newInput += strlen(tab[j]) + 1 /* pour le '/' */;
            }else{
                size_newInput += strlen(tab[j]);
            }
            
            newInput = realloc(newInput, size_newInput + 1);

            if(j == 1){
                snprintf(newInput, strlen(tab[j])+1, "%s", tab[j]);
                if(j != size_tab-1){
                    newInput[strlen(tab[j])] = '/';
                    newInput[strlen(tab[j])+1] = '\0';
                }else{ 
                    newInput[strlen(tab[j])] = '\0';
                }
                
            }else{
                strncat(newInput,tab[j], strlen(tab[j]));
                if(j != size_tab-1) newInput[size_newInput] = '/';
            }
        }  
        
        // On regarde tout les resultats possible depuis une la possibilite possibilite[i]
        int size_jok;   // nombre de possibilite
        char **jok = joker(newInput, newDos, &size_jok);
        if(jok != NULL){
            int tmp;
            res = cat_tabs(res, size_res, jok, size_jok, &tmp);  
            size_res = tmp;
        }
    }
    *t = size_res;
    return res;
}

char **joker_init(char *input, int *taille){
    char *dos = "./";
    char *cpyDos = malloc(strlen(dos)+1);
    snprintf(cpyDos, strlen(dos)+1, "%s", dos);
    cpyDos[strlen(dos)] = '\0';

    char *cpyInput = malloc(strlen(input)+1);
    snprintf(cpyInput, strlen(input)+1, "%s", input);
    cpyInput[strlen(input)] = '\0';
    
    return joker(cpyInput, cpyDos, taille);
}

void test_joker(){
    char *input = "*/*/*";
    int taille;

    char **res = joker_init(input, &taille);
    printf(">\n");
    affiche_mat(res, taille);
    printf("<\n");

    free_StingArrayArray(res, taille);
}


int main(void){

    // test_double_tab();
    // test_add_last();
    // test_begin_with();
    // test_end_with();
    // test_begin_end_with();
    // test_trans();
    // test_joker();
    // test_all_fic();
    test_joker();
    return 0;
}