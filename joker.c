#include "fonctions_auxiliaires.c"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <signal.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>

// retourne 1 si la chaine est valide en tant que balise, 0 sinon
char is_valid(char *chaine)
{
    if (chaine == NULL)
        return 0;

    if (chaine[0] == '.')
        return 0;

    for (int i = 0; i < strlen(chaine); i++)
    {
        if (chaine[i] == '*')
        {
            if (i == strlen(chaine) - 1) // chaine est sous la forme prefix*
                return 1;

            if (chaine[i + 1] != '.') // si on a un autre charactère que '.' après * -> return 0
                return 0;

            for (int j = i + 2; j < strlen(chaine); j++) // si il y'a deux . dans le suffix -> return 0
                if (chaine[j] == '.')
                    return 0;

            return 1;
        }
    }
    return 0; // pas de '*' dans la chaine
}

char *get_prefix(char *chaine)
{
    char *resultat = malloc(1);
    resultat[0] = '\0';
    int compteur = 1;
    for (int i = 0; strlen(chaine); i++)
    {
        if (chaine[i] == '*')
            break;

        compteur++;
        resultat = realloc(resultat, compteur);
        resultat[i] = chaine[i];
        resultat[i + 1] = '\0';
    }

    return resultat;
}

char *get_postfix(char *chaine)
{
    char *resultat = malloc(1);
    resultat[0] = '\0';
    for (int i = 0; i < strlen(chaine); i++)
    {
        if (chaine[i] == '*')
        {
            if (i + 1 == strlen(chaine))
                return resultat;

            resultat = (char *)realloc(resultat, strlen(chaine) - i);
            for (int j = i + 1; j < strlen(chaine); j++)
            {
                resultat[j - (i + 1)] = chaine[j];
            }
            resultat[strlen(chaine)] = '\0';
        }
    }
    return resultat;
}

// verifie si pre est prefix de chaine (1 si oui; 0 sinon)
int is_prefix(char *chaine, char *pre)
{
    if (strcmp(pre, "") == 0)
        return 1;

    if (strlen(pre) > strlen(chaine))
        return 0;

    for (int i = 0; i < strlen(pre); i++)
        if (chaine[i] != pre[i])
            return 0;

    return 1;
}

// // verifie si pre est postfix de chaine (1 si oui; 0 sinon)
int is_postfix(char *chaine, char *pre)
{
    if (strcmp(pre, "") == 0)
        return 1;

    if (strlen(pre) > strlen(chaine) || strlen(chaine) == 1)
        return 0;

    for (int i = 1; i < strlen(chaine); i++)
        if (chaine[i] == '.')
        {
            if (strlen(pre) != strlen(chaine) - i)
                return 0;

            for (int j = 0; j < strlen(pre); j++)
                if (chaine[i + j] != pre[j])
                    return 0;

            return 1;
        }

    return 0;
}


int joker_1(char* commande, char* argument_commande, char* nom_dossiers_etoile)
{
    // commande = "a*.c";
    // argument_commande = "ls";
    // nom_dossiers_etoile = "";

    if (!is_valid(argv[1]))
    {
        printf("not valide\n");
        exit(0);
    }

    DIR *repertoir = opendir(".");
    struct dirent *parcours;
    while ((parcours = readdir(repertoir)))
    {
        if (is_prefix(parcours->d_name, get_prefix(nom_dossiers_etoile)) == 1 && is_postfix(parcours->d_name, get_postfix(nom_dossiers_etoile)) == 1)
        {
            if (strcmp(argument_commande,"") != 0)
            {
                char *tmp[4] = {commande, argument_commande, parcours->d_name, NULL};
                execCMD(commande, tmp);
            }
            else
            {
                char *tmp[3] = {commande, parcours->d_name, NULL};
                execCMD(commande, tmp);
            }
        }
    }
    return 0;
}
