#include "../header/prompt.h"

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
