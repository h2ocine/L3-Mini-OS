#include "../header/cdaux.h"

char *logiquePath(char *path)
{
    if (strlen(path) != 1 && path[strlen(path) - 1] == '/')
        perror("mauvais format");
    int taille;
    char **tab = explode(path, "/", &taille);
    free(path);

    char *res = malloc(1);
    res[0] = '\0';
    for (int i = 0; i < taille; i++)
    {
        if (strcmp(tab[i], ".") == 0)
        {
            continue;

            // On revient au dossier parent
        }
        else if (strcmp(tab[i], "..") == 0)
        {
            if (strlen(res) != 0)
            {
                char *s = truncate_str(res, '/');
                videString(res);
                res = realloc(res, strlen(s) + 1);
                snprintf(res, strlen(s) + 1, "%s", s);
                res[strlen(s)] = '\0';
                free(s);
            }
        }
        else
        {
            size_t t = strlen(res) + strlen(tab[i]) + 2;
            res = realloc(res, t * sizeof(char));

            strcat(res, "/");
            strcat(res, tab[i]);
            res[t - 1] = '\0';
        }
    }
    if (strlen(res) == 0)
    {
        res = realloc(res, 2);
        res[0] = '/';
        res[1] = '\0';
    }
    free_StingArrayArray(tab, taille);
    return res;
}

int cd_physique(char *path, char *ref)
{
    char real[MAX_ARGS_NUMBER];
    realpath(path, real);

    // CAS : fichier inexistant
    if (chdir(real) < 0)
    {
        printf("\033[36mbash: cd: %s: Aucun fichier ou dossier de ce type\n", ref);
        return 1;
    }

    // On vide la chaine de caractere oldPath et on lui donne la valeur de dossier_courant
    setenv("OLDPWD", getenv("PWD"), 1);

    // On vide la chaine de caractere dossier_courant et on lui donne la valeur de path
    setenv("PWD", real, 1);

    return 0;
}

int cd_logique(char *path, char *ref)
{
    // On cree une copie de path car pour utiliser la fonction explode on a besoin d'une chaine de caractère alouer dynamiquement
    char *c = malloc(strlen(path) + 1);
    if (snprintf(c, strlen(path) + 1, "%s", path) < 0)
    {
        perror("sprintf erreur ");
        exit(1);
    }
    c[strlen(path)] = '\0';

    char *realpath = logiquePath(c);
    // On vérifie le chemin physique si le chemin realpath n'a pas de sens(càd qu'il existe)
    if (chdir(realpath) < 0)
    {
        free(realpath);
        return cd_physique(path, ref);
    }

    // On vide la chaine de caractere oldPath et on lui donne la valeur de dossier_courant
    setenv("OLDPWD", getenv("PWD"), 1);

    // On vide la chaine de caractere dossier_courant et on lui donne la valeur de path
    setenv("PWD", realpath, 1);

    free(realpath);
    return 0;
}