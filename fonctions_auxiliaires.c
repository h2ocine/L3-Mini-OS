
#include "header.h"

void free_StingArrayArray(char **s, int taille)
{

    for (int i = 0; i < taille; i++)
        free(s[i]);

    free(s);
}

void recherche_commande_interne(char **tab, int *last_exit, int taille)
{
    if (strcmp("exit", tab[0]) == 0)
    {
        if (taille >= 2)
        {
            char t[strlen(tab[1]) + 1];
            snprintf(t, strlen(tab[1]) + 1, "%s", tab[1]);
            t[strlen(tab[1]) + 1] = '\0';
            free_StingArrayArray(tab, taille);

            *last_exit = exits(t, *last_exit);
        }
        else
        {
            *last_exit = exits(NULL, *last_exit);
        }
    }
    else if (strcmp("cd", tab[0]) == 0)
    {
        char *arg;
        char *ref;
        if (taille == 1)
        {
            arg = NULL;
            ref = NULL; //*//
        }
        else if (taille == 2)
        {
            arg = NULL;
            ref = tab[1];
        }
        else
        {
            arg = tab[1];
            ref = tab[2];
        }
        *last_exit = cd(arg, ref);
    }
    else if (strcmp("pwd", tab[0]) == 0)
    {
        *last_exit = pwd(taille, tab);
    }
    else
    {
        // Fonction commande externe

        // On initialise un tableau pour les options
        char *arr[MAX_ARGS_NUMBER];

        // On complete argument 0
        char *arg0 = malloc(sizeof(char) * MAX_ARGS_STRLEN);
        if (arg0 == NULL)
            perror("malloc");

        strcpy(arg0, "/bin/");
        strcat(arg0, tab[0]);

        // On complete le premier argument du tableau avec la commande
        arr[0] = arg0;

        // On remplit le tableau pour les options
        for (int i = 1; i < taille; i++)
        {
            arr[i] = tab[i];
            // printf(" Element[%d] = %s \n",i,arr[i]);
        }

        // Puis on execute la commande
        switch (fork())
        {
        case -1:
            exit(1);
        case 0:
            if (execvp(arg0, arr) < 0)
            {
                exit(EXIT_FAILURE);
            }
            free(arg0);
            free_StingArrayArray(arr, taille);
            break;
        default:
            free(arg0);
            break;
        }

        // if(execvp(arg0, arr) < 0){
        //     exit(EXIT_FAILURE);
        // }
    }
}

void formatage_couleur(int last_exit, char *prompt, char *prompt_exit)
{
    char *rouge = "\033[0;31m";
    char *vert = "\033[0;32m";
    char *cyan = "\033[36m";

    if (last_exit == 0)
    {
        strcpy(prompt, vert);
        strcat(prompt, "[");
        strcat(prompt, prompt_exit);
        strcat(prompt, "]");
        strcat(prompt, cyan);
        // Pas d'erreur à la derniere commande
    }
    else if (last_exit == 1)
    {
        strcpy(prompt, rouge);
        strcat(prompt, "[");
        strcat(prompt, prompt_exit);
        strcat(prompt, "]");
        strcat(prompt, cyan);
    }
}

// Cette fonction raccourcis si il le faut la chaine prompt à 30 caractères
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

char **explode(char *str, const char *separators, int *taille)
{
    int i = 0;
    int size = 0;
    char *s = NULL;
    char **res = NULL;

    // res[0]  = malloc(1*sizeof(char));
    // res[0][0] = '\0';

    // Cas chaine vide
    if (strlen(str) == 0)
    {
        *taille = 0;
        return NULL;
    }

    // Séparer la chaine en plusieurs sous chaines :
    char *strToken = strtok(str, separators);
    while (strToken != NULL)
    {
        // On copie strToken dans une chaine de caractère s (pour avoir utiliser la taille exact)
        if (!(s = malloc(strlen(strToken) + 1)))
            perror("malloc");
        if (snprintf(s, strlen(strToken) + 1, "%s", strToken) < 0)
        {
            perror("explode snprintf error ");
            exit(1);
        }
        s[strlen(strToken)] = '\0';
        // On ajoute la chaine de caractere s au tableau res
        size += 1;
        res = realloc(res, size * sizeof(char *)); // +1

        if (res == NULL)
            perror("fonction explode : realloc erreur ");

        res[i] = s;
        // res[i+1][] = '\0';
        i++;

        // On demande le token suivant.
        strToken = strtok(NULL, separators);
    }

    if (!s)
        free(s);
    free(strToken);

    *taille = i; // ici on retourne la taille de res
    return res;
}