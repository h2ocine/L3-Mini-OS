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
int  explode(char *str, const char *separators, char **res){ 
    int i = 0;
    size_t size = 0;
    if(strlen(str) == 0) return 0;

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
    return i;
}

/*********************************  COMMANDES INTERNES *************************************************/

DIR *cd (char *pathname, char *option, char *ref){
    return NULL;
}

int exits(char *val){
    pid_t pid_p1;
    pid_p1 = getpid();
    if(kill(pid_p1,SIGKILL) == 0){
        
        return 100;
    }
    return 100;
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



/*
    main
*/
int main(void){
    DIR *dir;
    if((dir = opendir(".")) < 0) exits("1");
    char **tab;
    rl_outstream = stderr;
   

    while(1){
        tab = malloc(0);
        if(tab == NULL) perror("malloc");

        char *pre = "$ ";
        // TODO: On affiche le prompt (invite de commande)
        if(write(1, pre, strlen(pre)) < 0) exits("1");

        // TODO: On utilise readline pour simplifier la lecture     
            
        char *ligne = readline("");
        //Cas du CTRL - D 
        if (ligne == NULL) {
            //On appelle exit sans paramètres 
            exits("NULL");
            
        } 

        // TODO: On ajoute la dernière commande à l'historique

        // TODO: On ajoute la dernière commande à l'historique
        add_history(ligne);
        
        // TODO: On transforme la ligne en tableau
        char *delimiter = " ";

        //Ici on recupere un tableau via la fonction explode qui découpe la ligne en mots 
        int taille = explode(ligne,delimiter, tab);

        //On traite notre tableau 
        if (taille > 0){
            if(strcmp("exit",tab[0]) == 0){
                printf("On est dans exit ligne = %s et %s\n",tab[0],tab[1]);
                break;
            }else if(strcmp("cd",tab[0])==0){
                break;
            }else if(strcmp("pwd",tab[0]) == 0){
                pwd(tab);
            }else{
                //Faire la commande externe .
            }
            free_StingArrayArray(tab);
        }else{
            free(tab);
        }
       
    }
    closedir(dir);

    return 0;
}