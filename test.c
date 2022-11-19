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

char dossier_courant[4000]; 
char oldPath[4000];

int cd (char *pathname, char *option, char *ref){
    char *l = "-L";
    char *p = "-P";
    char dest[4000];
    if(ref != NULL){
        if(strcmp(ref, "-") == 0){
            sprintf(dest, "%s", oldPath);
        }else{
            sprintf(dest, "%s", oldPath);
        }
    }else{
        sprintf(dest, "%s", getenv("HOME"));
    }
    
    if(option == NULL || strcmp(option, l) == 0){
        strcpy(oldPath, dossier_courant);
        strcpy(dossier_courant, dest);
        if(chdir(dest) < 0){
            printf("probleme\n");
             return 1;
        }
         
        
    }else if(strcmp(option, p) == 0){
        struct stat s;
        if(lstat(pathname, &s) < 0) return 1;
        if(S_ISLNK(s.st_mode)){
            if(chdir(pathname) < 0) return 1;
            char chemin_physique[4000];
            getcwd(chemin_physique, 4000); 
            sprintf(dest, "%s", chemin_physique);
            
        }
        if(chdir(dest) < 0) return 1;
        strcpy(oldPath, dossier_courant);
        strcpy(dossier_courant, dest);
    }

    char res[4000];
    getcwd(res, 4000);
    return 0;
}

void free_StingArrayArray(char **s,int taille){
    for(int i = 0; i < taille ; i++)
        free(s[i]);
    
    free(s);
}

/*
 * Retourne un tableau de chaînes de caractères, chacune d'elle étant une sous-chaîne du paramètre str extraite en utilisant le séparateur separators
*/
char**  explode(char *str, const char *separators, int* taille)
{ 
    int i = 0;
    size_t size = 0;
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
        if(!(s = malloc(strlen(strToken)))) 
            perror("malloc");
        if(snprintf(s, strlen(strToken) + 1, "%s", strToken) < 0)
        {
            perror("explode snprintf error ");
            exit(1);
        }

        //On ajoute la chaine de caractere s au tableau res
        size += sizeof(char *);
        res = realloc(res, size);
        if(res == NULL) 
            perror("realloc");
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

int pop(int *tab, int taille){
    for(int i=taille-1; i>= 0 ; i--){
        if(tab[i] != 0){
            return tab[i];
        }
    }
    return 0;
}

void deleteLast(int *tab, int taille){
    for(int i=taille-1; i>= 0 ; i--){
        if(tab[i] != 0){
            tab[i] = 0;
            return;
        }
    }
}

void push(int *tab, int taille, int n){
    for(int i=0; i<taille; i++){
        if(tab[i] == 0){
            tab[i] = n;
            return;
        }
    }
    printf("tableau pas assez grand\n");
}

char *realLogiquePath(char *path){
    int taille;
    char **tab = explode(path, "/", &taille);
    free(path);

    char *res = malloc(4000);
    sprintf(res, "%s", "/");
    int *size = calloc(taille, sizeof(int));
    

    for(int i=0; i<taille; i++){
        int len = strlen(res);
        int last_size = pop(size, taille);
        printf("len: %d, last: %d, tab: %s\n", len, last_size, tab[i]);

        // on ignore
        if(strcmp(tab[i], ".") == 0){
            printf("if 1\n");
            continue;

        // On revient au dossier parent
        }else if(strcmp(tab[i], "..") == 0){
            printf("if 2\n");
            res[len - last_size] =  '\0';
            deleteLast(size, taille);
            
        }else{
            printf("if 3\n");
            sprintf(&res[len], "%s", tab[i]);
            push(size, taille, strlen(tab[i]));
        }
    }
    
    free(size);
    free_StingArrayArray(tab, taille);
    return res;
}


int main(void){
    char *c = malloc(100);
    char *ch = "/d1/d3/d451/../..";
    sprintf(c, "%s", ch);

    
    char *res = realLogiquePath(c);
    printf("res: %s\n", res);

    free(res);
    
    
}