#include "slash.h"

/*
    Libere la mémoire de toute les chaines de caractères presente dans s
*/
void free_StingArrayArray(char **s){
    int i=0;
    while(*s){
        free(*s);
        i++;
        s++;
    }
    s-=i;
    free(s);
}

/**
 * Retourne un tableau de chaînes de caractères, chacune d'elle étant une sous-chaîne du paramètre str extraite en utilisant le séparateur separators
*/
int explode(char *str, const char *separators){ 
    int i = 0;
    size_t size = 0;
    char **res = malloc(size);
    if(res == NULL) perror("malloc");
    if(strlen(str) == 0) return res;

    char * strToken = strtok ( str, separators );
    while ( strToken != NULL ) {

        // On alloue dynamiquement un char * avec pour valeur strToken
        char *s = malloc(strlen(strToken));
        if(s == NULL) perror("malloc");
        sprintf(s, "%s", strToken);

        // On ajoute la chaine de caractere au tableau
        size += sizeof(char *);
        res = realloc(res, size);
        if(res == NULL) perror("realloc");
        res[i] = s;
        i++;

        // On demande le token suivant.
        strToken = strtok ( NULL, separators );
    }
    return res;
}

/*********************************  COMMANDES INTERNES *************************************************/

DIR *cd (char *pathname, char *option, char *ref){
    return NULL;
}

int exits(char *val){
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
    if((dir = opendir(".")) < 0) exits("1");
    char **tab;
    rl_outstream = stderr;
   

    while(1){
        char *pre = "$ ";
        // TODO: On affiche le prompt (invite de commande)
        if(write(1, pre, strlen(pre)) < 0) exits("1");

        // TODO: On utilise readline pour simplifier la lecture     
            
        char *ligne = readline("");
        printf("ligne: %s\n", ligne);

        // TODO: On ajoute la dernière commande à l'historique

        add_history(ligne);
        
        // TODO: On transforme la ligne en tableau
        char *delimiter = " ";

        //Ici on recupere un tableau via la fonction explode qui découpe la ligne en mots 
        tab = explode(ligne,delimiter);

        printf("tab[0]: %s\n", tab[0]);

        //On traite notre tableau 
        
        if(strcmp("exit",tab[0]) == 0){
            break;
        }else if(strcmp("cd",tab[0])==0){
            break;
        }else if(strcmp("pwd",tab[0]) == 0){
            break;
        }else{
            //Faire la commande externe .
        }
    }

    free_StingArrayArray(tab);
    closedir(dir);

    return 0;
}