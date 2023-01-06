#include "../header/redirections.h"

// Fonction qui va dispatcher les différents cas
void check_redirection(char **tab, int taille)
{
    for (int i = 0; i < taille; i++)
    {
        if (strcmp(tab[i], "<") == 0)
        {
            printf("redirection entreée standard \n");
        }
        else if (strcmp(tab[i], ">") == 0)
        {
            printf("redirection sortie standard \n");
        }
        else if (strcmp(tab[i], ">|") == 0)
        {
            printf("redirection sortie standard avec ecrasement \n");
        }
        else if (strcmp(tab[i], ">>") == 0)
        {
            printf("redirection sortie standard en en concatenation \n");
        }
        else if (strcmp(tab[i], "2>") == 0)
        {
            printf("redirection de la sortie erreur standard sans ecrasement ");
        }
        else if (strcmp(tab[i], "2>>") == 0)
        {
            printf("redirection sortie erreur standard en concatenation \n");
        }
        else if (strcmp(tab[i], "|") == 0)
        {
            printf("redirection de la sortie standard de cmd1 et l'entrée standard cm2 tube \n");
        }
        else
        {
            // Il n'y aucune redirection
            continue;
        }
    }
}