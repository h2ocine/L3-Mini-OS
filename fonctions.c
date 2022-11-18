#include "fonctions.h"



void *cd (char *pathname, char *option, char *ref){
    return NULL;
}

int exits(char *val){
    if (val != NULL){
        if (atoi(val)!= 0){
            exit(atoi(val));
        }
    }
        //On exit sans paramètres
    exit(atoi(getenv("var_env")));

}


int pwd(char **arg){
    const int pathsize = 100; //TODO : set pathsize
    char *ref = malloc(pathsize);
    //Recuperer le chemin d'accès du répertoire de travail courant
    
    if(arg[1] == NULL)
    {
        //Cas par défault : 
        //Référence absolu logique (avec getenv) :
        ref = getenv("PWD");
        if(!ref)   
        {
            perror("getcwd - Erreur ");
            free(ref);
            return 1;
        }
    }
    else
    {
        //Cas -L
        //Référence absolu logique (avec getenv) :
        if( strcmp(arg[1],"-L") == 0 || strcmp(arg[1],"-l") == 0)
        {
            ref = getenv("PWD");
            if(!ref)   
            {
                perror("getcwd - Erreur ");
                free(ref);
                return 1;
            }
        }
        else
        {
            //Cas -P
            //Référence absolue physique (avec getcwd)
            if(strcmp(arg[1],"-P") == 0 || strcmp(arg[1],"-p") == 0)
            {
                if(!getcwd(ref,sizeof(ref)))   
                {
                    perror("getcwd - Erreur ");
                    free(ref);
                    return 1;
                }
            }
            else
            {
                //Cas paramètre faux
                return 1; 
            }
        }

    }

    if(write(1,ref,strlen(ref)) == -1)
    {
        perror("Erreur ");
        free(ref);
        return 1;
    }    
    strcat(ref,"\n");
    free(ref);
    return 0;
}