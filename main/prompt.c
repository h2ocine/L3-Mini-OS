#include "../header/prompt.h"

// Affiche le prompt avec la valeur de retour last_exit et le retourne dans le second paramètre
void affichage_prompt(int last_exit, char* prompt)
{
        char *blanc = "\033[0m";
        // recupération du dossier courant
        char *cur = getenv("PWD");
        char dossier_courant[MAX_ARGS_NUMBER];
        if (snprintf(dossier_courant, strlen(cur) + 1, "%s", cur) < 0){
            perror("snprintf");
            exit(1);
        }

        // recupération de la valeur de retour dans prompt_exit
        //On convertit last_exit en chaine de caractères pour compter sa taille
        char last_exit_tab[30];
        sprintf(last_exit_tab,"%d",last_exit);
        //sprintf(last_exit_tab,strlen(last_exit)+1, "%s", last_exit);
        last_exit_tab[strlen(last_exit_tab)] = '\0';

        char *prompt_exit_variable = malloc(strlen(last_exit_tab)+1);
        if(prompt_exit_variable == NULL) perror("malloc");
         //sprintf(prompt_exit_variable,"%s",last_exit_tab);
        strcpy(prompt_exit_variable,last_exit_tab);
        strcpy(prompt_exit_variable + strlen(last_exit_tab),"\0");
        // printf("Valeur du last_exit variable = %s et longueur de last_exit variable = %ld \n",prompt_exit_variable,strlen(prompt_exit_variable));

        // on crée le tableau final du prompt_exit avec le [...]
        char *prompt_exit_final = malloc(strlen(last_exit_tab)+3);
        strcpy(prompt_exit_final,"[");

        if(last_exit == 255)
            strcat(prompt_exit_final,"SIG\0");
        else
            strcat(prompt_exit_final,prompt_exit_variable);
        
        strcat(prompt_exit_final+strlen(prompt_exit_variable)+1,"]");

        // On ajoute la valeur de retour ([0] ou [1]) et s'occupe de la couleur du prompt
        formatage_couleur(last_exit, prompt, prompt_exit_final);

        // on supprime les carctère en trop du dossier courant et on les remplae par ... avec truncate_prompt
        int max_size =  32 - (strlen(prompt_exit_final) + 2) - 2; // on set le nombre de caractère max du chemin //-3 ([valeur retour]) -2 $ espace
        char *prompt_dir = truncate_prompt(dossier_courant, max_size);

        // on ajoute le dossier courant dans le prompt
        strcat(prompt, prompt_dir);
        free(prompt_dir);

        // On met la saisie d'utilisateur en blanc en on ajoute le dollar espace
        strcat(prompt, blanc);
        strcat(prompt, "$ ");
        // prompt[strlrn] = '\0';

        free(prompt_exit_variable);
        free(prompt_exit_final);

}

void formatage_couleur(int last_exit, char *prompt, char *prompt_exit)
{
    char *rouge = "\033[0;31m";
    char *vert = "\033[0;32m";
    char *cyan = "\033[36m";
    
    if (last_exit == 0)
    {
        strcpy(prompt, vert);
        //strcat(prompt, "[");
        //strcat(prompt, "[");
        strcat(prompt, prompt_exit);
        //strcat(prompt, "]");
        //strcat(prompt, "]");
        strcat(prompt, cyan);
        // Pas d'erreur à la derniere commande
    }
    else if (last_exit == 1)
    {
        strcpy(prompt, rouge);
        strcat(prompt, prompt_exit);
        strcat(prompt, cyan);
    }
    else{
        strcpy(prompt, rouge);
        strcat(prompt, prompt_exit);
        //strcat(prompt, "]");
        strcat(prompt, cyan);
    }
}

char *truncate_prompt(char *prompt, int max_size)
{
    int size = strlen(prompt);
    char *res;
    if (size > max_size)
    {
        res = malloc(max_size + 1);
        if (res == NULL)
            perror("malloc");
        strcpy(res, "...");
        for (int i = 3; i < max_size; i++)
        {
            int ind = size - max_size + i;
            res[i] = prompt[ind];
        }
        res[max_size] = '\0';
    }
    else
    {
        res = malloc(size + 1);
        if (res == NULL)
            perror("malloc");
        snprintf(res, strlen(prompt) + 1, "%s", prompt);
        res[size] = '\0';
    }
    return res;
}
