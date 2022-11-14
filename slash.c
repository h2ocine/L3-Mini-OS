#include "slash.h"


DIR *cd (char *pathname, char *option, char *ref){
    return;
}

int exit(char *val){
    return 0;
}




//TODO : retourner la référence absolue physique : FAIT
//TODO : retourner la référence absolue logique
int pwd(char **arg){
    const int pathsize = 100; //TODO : set pathsize
    char *ref = malloc(pathsize);

    //Recuperer le chemin d'accès absolu du répertoire de travail courant 
    if(!getcwd(ref,sizeof(ref)))   
    {
        perror("getcwd - Erreur ");
        free(ref);
        return 1;
    }
    strcat(ref,"\n");

    //TODO : ption par défaut (référence absolue logique)
    // if( arg[1] == NULL)

    
    //-P (référence absolue physique)
    if( strcmp(arg[1],"-P") == 0 || strcmp(arg[1],"-p") == 0)
    {
        if(write(1,ref,strlen(ref)) == -1)
        {
            perror("Erreur ");
            free(ref);
            return 1;
        }
        free(ref);
        return 0;
    }
    
    //TODO : -L (référence absolue logique)
    //if( strcmp(arg[1],"-L") == 0 || strcmp(arg[1],"-l") == 0)

    //TODO : Paramettre faux (different de -p et -l) = Option par défaut (référence absolue logique) ???

    free(ref);
    return 0;
    
}



/*
    main
*/
int main(void){
    DIR *dir;
    if((dir = opendir(".")) < 0) exit(1);

    char *ligne;
    ligne = malloc(MAX_ARGS_STRLEN);
    if(ligne == NULL) perror("malloc");

   

    while(1){
        char *pre = "$ ";
        // TODO: On affiche le prompt (invite de commande)
        if(write(1, pre, strlen(pre)) < 0) exit(1);

        // TODO: On lit la ligne (entré standart) et on la stocke dans ligne
        if(read(0, ligne, MAX_ARGS_STRLEN) < 0) exit(1);

        // TODO: On utilise readline pour simplifier la lecture         
        ligne = readline(ligne);

        // TODO: On ajoute la dernière commande à l'historique
        

        // TODO: On transforme la ligne en tableau
        char *delimiter = " ";
        char tab[] = strtok(ligne, delimiter);
        
        switch (tab[0]){
            case "exit": break;
        }
    }

    free(ligne);

    return 0;
}