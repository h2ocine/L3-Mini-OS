#include "fonctions.c"



void formatage_couleur(int last_exit,char *p,char *prompt){
    char *rouge = "\033[0;31m";
    char *blanc = "\033[0m";
    char *vert = " \033[0;32m";

    if(last_exit == 0){
        strcpy(p, vert);
        strcat(p, prompt);
        strcat(p, blanc);
            //Pas d'erreur à la derniere commande 
    }else if(last_exit == 1){
        strcpy(p, rouge);
        strcat(p, prompt);
        strcat(p, blanc);
    }
}
/*
    Cette fonction raccourcis si il le faut la chaine prompt à 30 caractères 
*/
char *truncate_prompt(char *prompt, int max_size){
    int size = strlen(prompt);
    char *res;
    if(size > max_size){
        res = malloc(max_size + 1);
        if(res == NULL) perror("malloc");
        strcpy(res, "...");
        for(int i=3; i<max_size; i++){
            int ind = size - max_size + i;
            res[i] = prompt[ind];
        }
        res[max_size] = '\0';
    }else{
        res = malloc(size + 1);
        if(res == NULL) perror("malloc");
        sprintf(res, "%s", prompt);
    }
    return res;

}

/*
    main
*/
int main(void){
    getcwd(dossier_courant, MAX_ARGS_NUMBER);

    strcpy(oldPath, dossier_courant);

    dir = opendir(dossier_courant);
    
    char **tab;
    int last_exit = 0;
    rl_outstream = stderr;
    
    //Initialisation variable d'environnement 
    //setenv("var_env","255",1);


    while(1){
        //Recupération du dosier prompt
        /*****************************************************************/
        /*****************************************************************/
        char prompt_exit[3];
        sprintf(prompt_exit, "%d", last_exit);

        char prompt[30];
        strcpy(prompt, "[");
        strcat(prompt, prompt_exit);
        strcat(prompt, "]");

        int max_size = 30 - (strlen(prompt_exit) + 2) - 2; // 30 - taille de l'affichage du exit ([0] = 3) - 2 (taille du dollar et espace)
        char *prompt_dir = truncate_prompt(dossier_courant, max_size);
        
        
        strcat(prompt, prompt_dir);

        free(prompt_dir);

        strcat(prompt, "$ ");

        /*****************************************************************/
        /*****************************************************************/

        //Lecture de la commande (On la récupére dans tab)
        /*****************************************************************/
        /*****************************************************************/


        //Formatage couleur prompt
        char*p = malloc(sizeof(char)*255);
        if(p== NULL) perror("malloc");

        formatage_couleur(last_exit,p,prompt);

        char *ligne = readline(p);
    
        //Cas du CTRL - D 
        if (ligne == NULL) {
            //On appelle exit sans paramètres 
            exits(NULL,last_exit);
            
        } 

        // TODO: On ajoute la dernière commande à l'historique
        add_history(ligne);
        
        //On transforme la ligne en tableau (ici on recupere un tableau via la fonction explode qui découpe la ligne en mots)
        const char* delimiter = " ";
        int taille;
        tab = explode(ligne,delimiter, &taille);
        /*****************************************************************/
        /*****************************************************************/
        
        //Traitement de la commande
        /*****************************************************************/
        /*****************************************************************/
        if(taille == 0) 
            continue;
        
        //On traite notre tableau 
        if (taille > 0){

            if(strcmp("exit",tab[0]) == 0)
            {   
                char t[MAX_ARGS_NUMBER];
                strcpy(t, tab[1]);
                free(p);
                free_StingArrayArray(tab,taille);
                
                //Pas besoin de la variable d'environnement ( voir avec adam)
                last_exit = exits(t,last_exit);
            }
            else if(strcmp("cd",tab[0])==0)
            {   
                char *arg;
                char *ref;
                if(taille == 1){
                    arg = NULL;
                }else if(taille == 2){
                    arg = NULL;
                    ref = tab[1];
                }else{
                    arg = tab[1];
                    ref = tab[2];
                }
                cd(dossier_courant, arg, ref);
            }
            else if(strcmp("pwd",tab[0]) == 0)
            {
                last_exit = pwd(taille,tab);
            }
            else
            {
                //Faire la commande externe .
            }
        
        }
        else
        {
            free(tab);
        }

        free(p);
        free_StingArrayArray(tab,taille);
        /*****************************************************************/
        /*****************************************************************/
    }
    free(dossier_courant);
    free(oldPath);
    closedir(dir);
    return 0;
}