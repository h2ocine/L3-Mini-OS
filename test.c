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

int cd_logique(char *path){

    // On cree une copie de path car pour utiliser la fonction explode on a besoin d'une chaine de caractère alouer dynamiquement
    char *c = malloc(strlen(path));
    if(sprintf(c, "%s", dest) < 0) {
        perror("sprintf erreur ");
        exit(1);
    }
    c[strlen(path)] = '\0';

    
    char *realpath = realLogiquePath(c);
    // On vérifie le chemin physique si le chemin realpath n'a pas de sens(càd qu'il existe)
    if(chdir(realpath) < 0) return cd(pathname, p, ref);
    setenv("PWD", realpath, 1); // On change la var d'environnement PWD car elle est utiliser dans la commande 
    
    // On vide la chaine de caractere oldPath et on lui donne la valeur de dossier_courant
    memset(oldPath, 0, MAX_ARGS_NUMBER);
    strcpy(oldPath, dossier_courant);
    oldPath[strlen(dossier_courant)] = '\0';

    // On vide la chaine de caractere dossier_courant et on lui donne la valeur de path
    memset(dossier_courant, 0, MAX_ARGS_NUMBER);
    strcpy(dossier_courant, realpath);
    dossier_courant[strlen(realpath)] = '\0';

    return 0;
}

int cd_physique(char *path){
    char real[MAX_ARGS_NUMBER];
    realpath(dest, real);

    //CAS : fichier inexistant 
    if(chdir(real) < 0) 
    {
        printf("\033[36mbash: cd: %s: No such file or directory\n", ref);
        return 1;
    }
    setenv("PWD", real, 1);

    // On vide la chaine de caractere oldPath et on lui donne la valeur de dossier_courant
    memset(oldPath, 0, MAX_ARGS_NUMBER);
    strcpy(oldPath, dossier_courant);
    oldPath[strlen(dossier_courant)] = '\0';

    // On vide la chaine de caractere dossier_courant et on lui donne la valeur de path
    memset(dossier_courant, 0, MAX_ARGS_NUMBER);
    strcpy(dossier_courant, realpath);
    dossier_courant[strlen(realpath)] = '\0';

    return 0;
}

int cd (char *pathname, char *option, char *ref){
    char *l = "-L"; // Represente l'option -L
    char *p = "-P"; // Represente l'option -P
    char dest[MAX_ARGS_NUMBER] = "";


    if(ref != NULL)
    {   
        // Si la ref est un chemin relatif
        if(ref[0] == '/'){
            dest = ref;
        }

        // Si la ref est "-" dest vaut oldpath 
        else if(strcmp(ref, "-") == 0)
        {   
            if(sprintf(dest, "%s", oldPath) < 0) {
                perror("sprintf erreur");
                exit(1);
            }
            dest[strlen(oldPath)] = '\0';
            
        }
        else
        {
            if(sprintf(dest, "%s", pathname) < 0) {
                perror("sprintf erreur ");
                exit(1);
            }
            dest[strlen(pathname)] = '/';
            dest[strlen(pathname) + 1] = '\0';
            strcat(dest, ref);
            dest[strlen(pathname) + 1 + strlen(ref)] = '\0';
        }
    }
    else
    {
        if(sprintf(dest, "%s", getenv("HOME")) < 0) {perror("sprintf erreur ");exit(1);} 
        dest[strlen(getenv("HOME"))] = '\0';
    }

    // Cas lien logique
    if(option == NULL || strcmp(option, l) == 0) cd_logique(dest);
    else if(strcmp(option, p) == 0) cd_physique(dest);
    else return 1;  // Cas d'une autre option

    return 0;
}


int main(void){
    
    
}