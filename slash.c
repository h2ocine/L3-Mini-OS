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

    while (1)
    {
        // affichage du prompt :
        //---------------------
        // recupération du dossier courant
        char *cur = getenv("PWD");
        char dossier_courant[MAX_ARGS_NUMBER];
        if (snprintf(dossier_courant, strlen(cur) + 1, "%s", cur) < 0)
            perror("snprintf");

        // recupération de la valeur de retour dans prompt_exit
        //[TODO] : Faire le cas ou la valeur de retour > 9;

        //On convertit last_exit en chaine de caractères pour compter sa taille
        char last_exit_tab[30];
        sprintf(last_exit_tab,"%d",last_exit);

        // printf("Valeur de exit tab = %s \n",last_exit_tab);
        // printf("Longueur de la last_exit = %ld \n",strlen(last_exit_tab));

        //On crée un char_prompt_exit variable en fonctione de la longueur de last_exit

        char *prompt_exit_variable = malloc(strlen(last_exit_tab));
        if(prompt_exit_variable == NULL) perror("malloc");
        strcpy(prompt_exit_variable,last_exit_tab);
        // printf("Valeur du last_exit variable = %s et longueur de last_exit variable = %ld \n",prompt_exit_variable,strlen(prompt_exit_variable));


        // on crée le tableau final du prompt_exit avec le [...]
        char *prompt_exit_final = malloc(strlen(last_exit_tab)+3);
        strcpy(prompt_exit_final,"[");
        strcat(prompt_exit_final,prompt_exit_variable);
        strcat(prompt_exit_final+strlen(prompt_exit_variable)+1,"]");

        printf("Valeur du promp_final_variable = %s \n et longueur finale = %ld \n",prompt_exit_final,strlen(prompt_exit_final));

     
        // char prompt_exit[2];
        // prompt_exit[0] = last_exit + '0'; // recuperer le premier caractère
        // prompt_exit[1] = '\0';            // fin de la chaine de la valeur de retoure

        // creation du prompt -> il a 30 caractère au maximum + (.)caractère de couleurs
        char prompt[TAILLE_PROMPT];

        //if(strlen(prompt_exit_variable))

        // On ajoute la valeur de retour ([0] ou [1]) et s'occupe de la couleur du prompt
        formatage_couleur(last_exit, prompt, prompt_exit_final);

        // on supprime les carctère en trop du dossier courant et on les remplae par ... avec truncate_prompt
        int max_size = 30 - (strlen(prompt_exit_final) + 2) - 2; // on set le nombre de carctère max du chemin //-3 ([valeur retour]) -2 $ espace
        char *prompt_dir = truncate_prompt(dossier_courant, max_size);

        // on ajoute le dossier courant dans le prompt
        strcat(prompt, prompt_dir);
        free(prompt_dir);

        // On met la saisie d'utilisateur en blanc en on ajoute le dollar espace
        strcat(prompt, blanc);
        strcat(prompt, "$ ");
        // prompt[strlrn] = '\0';

        /*****************************************************************/
        // Lecture de la commande (On la récupére dans tab)
        /*****************************************************************/

        // lecture de la commande et affichage de p
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

        tab = explode(ligne, delimiter, &taille); /***************--------------------------*********/
        /*****************************************************************/
        /*****************************************************************/

        // Traitement de la commande
        /*****************************************************************/
        /*****************************************************************/
        if (taille == 0)
            continue;

        // On traite notre tableau
        if (taille > 0)
        {

            // On cherche si dans notre tableau
            recherche_commande_interne(tab, &last_exit, taille);
            // On verifie si il y a présence de true ou false

            // On vérifie la présence de true ou de false dans une commande du type .../.../true ou .../.../false
            int newtaille;

            // On initialise un tableau de String séparé de tout les délimiteurs
            char **tabvaleurprompt = explode(ligne, "/,;,", &newtaille);

            //On parcourt le tableau pour chercher la présence de true ou false 
            cherche_true_false(&last_exit,tabvaleurprompt,newtaille);
         
            //On free le tableau
            free_StingArrayArray(tabvaleurprompt, newtaille);
        }
        else
        {
            free(tab);
        }

        free_StingArrayArray(tab, taille);
        free(ligne);
        free(prompt_exit_variable);
        free(prompt_exit_final);

        /*---------------------------*/
    }
    return 0;
}
