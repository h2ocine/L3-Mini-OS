#include "fonctions.h"
#include <errno.h>


void *cd (char *pathname, char *option, char *ref){
    return NULL;
}

int exits(char *val,int last_exit){
    if (val != NULL){
        if (atoi(val)!= 0){
            exit(atoi(val));
        }
    }
    //On exit sans paramètres
    //exit(atoi(getenv("var_env")));
    //Faire le cas de la variable d'environement
    //printf("ca rentre ici");
    exit(last_exit);


}


//fonction pwd : //

//write
int pwd_affichage(char* ref)
{
    if(write(1,ref,strlen(ref)) == -1)
    {
        perror("Erreur ");
        free(ref);
        return 1;
    }
    return 0;
}

//Recuperation de la référence absolu logique
int pwd_logique(char* ref)
{
    if(!strcpy(ref,getenv("PWD")))   
    {
        perror("getenv - Erreur ");
        free(ref);
        return 1;
    }
    strcat(ref,"\n");
    if(pwd_affichage(ref) == 1)
        return 1;
    free(ref);
    return 0;
}

//Recuperation de la référence absolu phyique
int pwd_physique(char* ref)
{
    if(!getcwd(ref,MAX_ARGS_STRLEN))   
    {
        perror("getcwd - Erreur ");
        free(ref);
        return 1;
    }
    strcat(ref,"\n");
    if(pwd_affichage(ref) == 1)
        return 1;
    free(ref);
    return 0;
}

int pwd(int argc, char **argv){
    char *ref = malloc(MAX_ARGS_STRLEN);

    //Cas par défault : (Référence absolu logique)
    if(argc == 1)
    {
        return pwd_logique(ref);
    }
    else
    {
        //Cas -L : (Référence absolu logique)
        if(strcmp(argv[1],"-L") == 0 || strcmp(argv[1],"-l") == 0)
            return pwd_logique(ref);
        else
        {
            //Cas -P : (Référence absolu phyique)
            if(strcmp(argv[1],"-P") == 0 || strcmp(argv[1],"-p") == 0)
                return pwd_physique(ref);
            //Cas paramètre faux
            else
                return 1; 
        }

    }   
}

