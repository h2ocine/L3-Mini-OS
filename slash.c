#include "header/util.h"
#include "header/prompt.h"
#include "header/cmdext.h"
#include "header/cmdint.h"
#include "header/signal.h"
#include "header/redirections.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>


#define MAX_ARGS_STRLEN 4096
#define TAILLE_PROMPT 47 //7 (rouge ou vert) + 5 (bleu) + 4 (blanc) + 1 ('\0')
/*
    main
*/
int main(void)
{
    gestion_signeaux(0);

    setenv("OLDPWD", getenv("PWD"), 1);

    char **tab;
    rl_outstream = stderr;

    int last_exit = 0;

    while (1)
    {
        //affichage du prompt 
        char prompt[TAILLE_PROMPT];
        affichage_prompt(last_exit,prompt);

        // lecture de la commande 
        char *ligne = readline(prompt);

        // Cas du CTRL - D
        if (ligne == NULL)
        {
            // On appelle exit sans paramètres
            exits(NULL, last_exit);
        }

        // On ajoute la dernière commande à l'historique
        add_history(ligne);

        // On transforme la ligne en tableau (ici on recupere un tableau via la fonction explode qui découpe la ligne en mots)
        const char *delimiter = " ";
        int taille;
        tab = explode(ligne, delimiter, &taille);

        // Traitement de la commande 

        if (taille == 0)
            continue;

        // On traite notre tableau
        if (taille > 0)
        {
            int bool = 0;
            // On cherche si dans notre tableau
            if (bool == 0){
                if (check_redirection(tab,taille,&last_exit)== 0){
                     recherche_commande_interne(tab, &last_exit, taille);
                }
                bool = 1;

            }
           
            // On verifie si il y a présence de true ou false
            // On vérifie la présence de true ou de false dans une commande du type .../.../true ou .../.../false
            int newtaille;

            // On initialise un tableau de String séparé de tout les délimiteurs
            char **tabvaleurprompt = explode(ligne, "/,;,", &newtaille);

            //On parcourt le tableau pour chercher la présence de true ou false 
            if (bool == 1) cherche_true_false(&last_exit,tabvaleurprompt,newtaille);
            
            //On free le tableau
            free_StingArrayArray(tabvaleurprompt, newtaille);
        }
        else
        {
            free(tab);
        }

        free_StingArrayArray(tab, taille);
        free(ligne);
        

        /*---------------------------*/
    }
    return 0;
}
