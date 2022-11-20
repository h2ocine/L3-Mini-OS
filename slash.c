#include "fonctions.c"



void formatage_couleur(int last_exit,char *prompt,char *prompt_exit){
    char *rouge = "\033[0;31m";
    //char *blanc = "\033[0m";
    char *vert = "\033[0;32m";
    char *cyan = "\033[36m";

    if(last_exit == 0){
        strcpy(prompt,vert);
        strcat(prompt, "[");
        strcat(prompt, prompt_exit);
        strcat(prompt, "]");
        strcat(prompt,cyan);
            //Pas d'erreur à la derniere commande 
    }else if(last_exit == 1){
        strcpy(prompt,rouge);
        strcat(prompt, "[");
        strcat(prompt, prompt_exit);
        strcat(prompt, "]");
        strcat(prompt,cyan);
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
        for(int i = 3; i < max_size; i++)
        {
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

    // char *rouge = "\033[0;31m";
    char *blanc = "\033[0m";
    // char *vert = "\033[0;32m";

    //char *cyan = "\033[36m";

    
    //Initialisation variable d'environnement 
    //setenv("var_env","255",1);


    while(1){
        //Recupération du dosier prompt
        /*****************************************************************/
        /*****************************************************************/
        char prompt_exit[2];

        prompt_exit[0] = last_exit + '0'; //recuperer le premier caractère
        prompt_exit[1] = '\0'; //fin de la chaine de la valeur de retoure


        char prompt[33];

        // strcpy(prompt,vert);
        // strcat(prompt, "[");
        // strcat(prompt, prompt_exit);
        // strcat(prompt, "]");
        // strcat(prompt,blanc);

        //On s'occupe de la couleur du prompt ( [0] && [1])
        formatage_couleur(last_exit,prompt,prompt_exit);

        int max_size = 30 - (strlen(prompt_exit) + 2) - 2; // 30 - taille de l'affichage du exit ([0] = 3) - 2 (taille du dollar et espace)
        char *prompt_dir = truncate_prompt(dossier_courant, max_size);
        
        
        strcat(prompt, prompt_dir);

        free(prompt_dir);


        strcat(prompt, "$ ");

        //On met la saisie d'utilisateur en blanc

        strcat(prompt,blanc);

        /*****************************************************************/
        /*****************************************************************/

        //Lecture de la commande (On la récupére dans tab)
        /*****************************************************************/
        /*****************************************************************/

        
        

        //Formatage couleur prompt
        char*p = malloc(sizeof(char)*255);
        if(p== NULL) perror("malloc");

        //formatage_couleur(last_exit,p,prompt);
        strcat(p, prompt);
       
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

        tab = explode(ligne,delimiter, &taille); /***************--------------------------*********/

        
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
                last_exit = 10;

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