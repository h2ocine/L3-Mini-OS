#include "../header/joker.h"

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

char ** all_joker_fic(char *input ,char *dos, int *t){
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
        char **jok = all_joker_fic(newInput, newDos, &size_jok);
        if(jok != NULL){
            int tmp;
            res = cat_tabs(res, size_res, jok, size_jok, &tmp);  
            size_res = tmp;
        }
    }
    *t = size_res;
    return res;
}