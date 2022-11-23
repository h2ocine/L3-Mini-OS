#include "fonctions_internes.c"

/*
    main
*/
int main(void)
{
    setenv("OLDPWD", getenv("PWD"), 1);

    char **tab;
    int last_exit = 0;
    rl_outstream = stderr;

    char *blanc = "\033[0m";

    while(1){
        //affichage du prompt :
        //---------------------
        //recupération du dossier courant 
        char *cur = getenv("PWD");
        char dossier_courant[MAX_ARGS_NUMBER]; 
        if(snprintf(dossier_courant, strlen(cur)+1, "%s", cur)<0) perror("snprintf");

        //recupération de la valeur de retour dans prompt_exit
        //[TODO] : Faire le cas ou la valeur de retour > 9; 
        char prompt_exit[2];
        prompt_exit[0] = last_exit + '0'; //recuperer le premier caractère
        prompt_exit[1] = '\0'; //fin de la chaine de la valeur de retoure

        //creation du prompt -> il a 30 caractère au maximum + (.)caractère de couleurs
        char prompt[TAILLE_PROMPT];

        //On ajoute la valeur de retour ([0] ou [1]) et s'occupe de la couleur du prompt
        formatage_couleur(last_exit,prompt,prompt_exit);

        //on supprime les carctère en trop du dossier courant et on les remplae par ... avec truncate_prompt
        int max_size = 30 - (strlen(prompt_exit) + 2 ) - 2; //on set le nombre de carctère max du chemin //-3 ([valeur retour]) -2 $ espace
        char *prompt_dir = truncate_prompt(dossier_courant, max_size);
        
        //on ajoute le dossier courant dans le prompt
        strcat(prompt, prompt_dir);
        free(prompt_dir);

        //On met la saisie d'utilisateur en blanc en on ajoute le dollar espace
        strcat(prompt,blanc);
        strcat(prompt, "$ ");
        //prompt[strlrn] = '\0';

        /*****************************************************************/
        //Lecture de la commande (On la récupére dans tab)
        /*****************************************************************/

        //lecture de la commande et affichage de p
        char *ligne = readline(prompt);

        //Cas du CTRL - D 
        if (ligne == NULL) {
            //On appelle exit sans paramètres 
            exits(NULL,last_exit);
            
        } 

        // On ajoute la dernière commande à l'historique
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
            
            //On cherche si dans notre tableau
            recherche_commande_interne(tab,&last_exit,taille);
        }
        else
        {
            free(tab);
        }
        free_StingArrayArray(tab,taille);
        free(ligne);


        /*---------------------------*/

    }
    return 0;
}