
#include "header.h"
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>

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

void free_StingArrayArray(char **s, int taille)
{
    for (int i = 0; i < taille; i++)
        free(s[i]);

    free(s);
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

void execCMD(char *cmd, char **args)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
    }
    else if (pid != 0)
    {
        wait(NULL);
    }
    else
    {
        execvp(cmd, args);
    }
}

void commande_externe(char **tab, int taille)
{

    char pre[3];
    snprintf(pre, 3, "%s", tab[0]);
    pre[2] = '\0';

    if (strcmp(pre, "./") == 0)
    {
        int t;
        char **sep = explode(tab[0], "/", &t);

        char *pwdEnv = getenv("PWD");
        size_t size = strlen(pwdEnv) + 1;
        char *cpyPwd = malloc(size + 1);
        snprintf(cpyPwd, size + 1, "%s", pwdEnv);
        cpyPwd[size - 1] = '/';
        cpyPwd[size] = '\0';

        for (int i = 0; i < t - 1; i++)
        {
            size += strlen(sep[i]) + 1;
            cpyPwd = realloc(cpyPwd, size + 1);
            strncat(cpyPwd, sep[i], strlen(sep[i]));
            cpyPwd[size - 1] = '/';
            cpyPwd[size] = '\0';
        }

        if (isIn(cpyPwd, sep[t - 1]) == 1)
        {
            size += strlen(sep[t - 1]);
            cpyPwd = realloc(cpyPwd, size + 1);
            strncat(cpyPwd, sep[t - 1], strlen(sep[t - 1]));

            char **arguments_exec = malloc((taille + 1) * sizeof(char *));
            arguments_exec[0] = cpyPwd;
            for (int i = 1; i < taille; i++)
            {
                arguments_exec[i] = malloc(strlen(tab[i]) + 1);
                snprintf(arguments_exec[i], strlen(tab[i]) + 1, "%s", tab[i]);
            }
            arguments_exec[taille] = NULL;
            execCMD(cpyPwd, arguments_exec);
            free_StingArrayArray(arguments_exec, taille);
        }
    }
    else
    {

        // On recupere la variable d'environnement path et on lui cree une copie (car si on la modifie la var d'env se modifie)
        char *envPath = getenv("PATH");
        size_t taille_envPath = strlen(envPath);

        // Creation de la copie
        char *path = malloc(taille_envPath + 1);
        snprintf(path, taille_envPath + 1, "%s", envPath);
        path[taille_envPath] = '\0';

        // On decoupe la variable path avec le separateur ":"
        int taillarrpath;
        char **pathDecoupe = explode(path, ":", &taillarrpath);
        free(path);

        // on parcout chaque élément de la variable PATH
        for (int i = 0; i < taillarrpath; i++)
        {
            // On verifie si chaque chemin du tableau pathDecoup a un fichier de nom la commande ecrite par l'utilisateur (tab[0])
            // Si non on essaye avec un autre chemin
            if (isIn(pathDecoupe[i], tab[0]) == 0)
                continue;
            else
            {
                size_t taille_elem = strlen(pathDecoupe[i]);

                // On cree un tableau de chaine de caratere pour la fonction exec qui doit ressembler à ça pour l'exemple "ls -l path" -> [/usr/bin/ls, -l, path, NULL]
                // Taille + 1 car le dernier element doit etre NULL
                char **arguments_exec = malloc((taille + 1) * sizeof(char *));

                char *cmd = malloc(taille_elem + 1 + /* taille du '/' */ +strlen(tab[0]) + 1);
                if (cmd == NULL)
                    perror("malloc");
                snprintf(cmd, taille_elem + 1, "%s", pathDecoupe[i]);
                cmd[taille_elem] = '\0';

                // equivalent de strcat(arg0,tab[0]) :
                cmd[taille_elem] = '/';
                snprintf(&cmd[taille_elem + 1], strlen(tab[0]) + 1, "%s", tab[0]);
                int t = taille_elem + 1 + strlen(tab[0]);
                cmd[t] = '\0';

                // On complete le premier argument du tableau avec la commande
                arguments_exec[0] = malloc(strlen(cmd) + 1); //---------------------->fuite mem
                snprintf(arguments_exec[0], strlen(cmd) + 1, "%s", cmd);
                arguments_exec[0][strlen(cmd)] = '\0';

                // On remplit le tableau pour les options
                for (int i = 1; i < taille; i++)
                {
                    arguments_exec[i] = malloc(strlen(tab[i]) + 1);
                    snprintf(arguments_exec[i], strlen(tab[i]) + 1, "%s", tab[i]);
                }
                arguments_exec[taille] = NULL;

                execCMD(cmd, arguments_exec);
                free(cmd);
                free_StingArrayArray(arguments_exec, taille);
                return;
            }
        }
    }
}

void cherche_true_false(int *last_exit, char **tabvaleurprompt,int newtaille){
    for (int i = 0; i < newtaille; i++)
    {
        if (strcmp(tabvaleurprompt[i], "true") == 0)
        {
            *last_exit = 0;
        }
        else if (strcmp(tabvaleurprompt[i], "false") == 0)
        {
            *last_exit = 1;
        }
    }
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
        commande_externe(tab, taille);
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
