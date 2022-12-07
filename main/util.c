#include "../header/util.h"



char *truncate_str(char *s, char spr)
{

    int stop = strlen(s);

    for (int i = stop - 1; i >= 0; i--)
    {
        stop--;
        if (s[i] == spr)
            break;
    }

    char *res = malloc(stop + 1);
    if(res == NULL) perror("malloc");

    if(snprintf(res, stop + 1, "%s", s) < 0) perror("snprintf");
    res[stop] = '\0';
    return res;
}

void videString(char *s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        s[i] = '\0';
    }
}

int isIn(char *path, char *fic)
{
    DIR *dir = opendir(path);
    struct dirent *entry;

    while ((entry = readdir(dir)))
    {
        if (strcmp(entry->d_name, fic) == 0)
        {
            closedir(dir);
            return 1;
        }
    }
    closedir(dir);
    return 0;
}

char **explode(char *str, const char *separators, int *taille)
{
    int i = 0;
    int size = 0;
    char *s = NULL;
    char **res = NULL;

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

void free_StingArrayArray(char **s, int taille)
{
    for (int i = 0; i < taille; i++)
        free(s[i]);

    free(s);
}