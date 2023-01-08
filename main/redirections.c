#include "../header/redirections.h"
#include "../header/util.h"
#include "../header/prompt.h"
#include "../header/cmdext.h"
#include "../header/cmdint.h"

#include <stdlib.h>

const char *redirections[] = {">", "<", ">|", ">>", "2>", "2>|", "2>>"};

int compte_nombre_pipe(char **tab_clean, int taille_tabclean)
{
    int count = 0;
    for (int i = 0; i < taille_tabclean; i++)
    {
        if (strcmp(tab_clean[i], "|") == 0)
        {
            count += 1;
        }
    }
    return count;
}

void good_flags_descriptor(char *typeredirection, int *flags, int *descriptor)
{
    if (strcmp(typeredirection, ">") == 0)
    {
        *flags = O_WRONLY | O_CREAT | O_EXCL;
        *descriptor = STDOUT_FILENO;
    }
    else if (strcmp(typeredirection, "<") == 0)
    {
        *flags = O_RDONLY;
        *descriptor = STDIN_FILENO;
    }
    else if (strcmp(typeredirection, ">|") == 0)
    {
        *flags = O_WRONLY | O_CREAT | O_TRUNC | O_NOFOLLOW;
        *descriptor = STDOUT_FILENO;
    }
    else if (strcmp(typeredirection, ">>") == 0)
    {
        *flags = O_APPEND | O_CREAT | O_WRONLY | O_NOFOLLOW;
        *descriptor = STDOUT_FILENO;
    }
    else if (strcmp(typeredirection, "2>") == 0)
    {
        *flags = O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW;
        *descriptor = STDERR_FILENO;
    }
    else if (strcmp(typeredirection, "2>|") == 0)
    {
        *flags = O_WRONLY | O_CREAT | O_TRUNC | O_NOFOLLOW;
        *descriptor = STDERR_FILENO;
    }
    else if (strcmp(typeredirection, "2>>") == 0)
    {
        *flags = O_APPEND | O_CREAT | O_WRONLY | O_NOFOLLOW;
        *descriptor = STDERR_FILENO;
    }
}

void redirect_cmd(char *cmd, char **args, int *last_exit, char *fic, char *typeredirection)
{

    int *flags = malloc(sizeof(int) * 10);
    int *descriptor = malloc(sizeof(int) * 10);

    good_flags_descriptor(typeredirection, flags, descriptor);

    int status;
    int t = 0;

    // Crée un processus fils
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("Erreur lors de la création du processus fils");
        exit(1);
    }
    else if (pid != 0)
    {
        waitpid(pid, last_exit, 0);
    }
    else
    {
        // Dans le processus fils :
        // Redirige l'entrée standard de la commande vers le fichier ou le tube nommé
        int fd = open(fic, *flags, 0644);

        if (fd == -1)
        {
            perror("Erreur lors de l'ouverture du fichier ou du tube nommé");
            exit(1);
        }
        if (dup2(fd, *descriptor) == -1)
        {
            perror("Erreur lors de la redirection de l'entrée standard de la commande");
            exit(1);
        }
        // Ferme le descripteur de fichier
        close(fd);
        free(flags);
        free(descriptor);
        // Exécute la commande

        if (execvp(cmd, args) < 0)
        {
            exit(WEXITSTATUS(t));
        }
    }
}

void cut_cmd_redirec(char **tab, int taille, int *last_exit, char *fic, char *typeredirection)
{
    char pre[3];
    snprintf(pre, 3, "%s", tab[0]);
    pre[2] = '\0';

    if (strcmp(pre, "./") == 0)
    {
        // printf(" ca rentre\n");
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
            // arguments_exec[0] = cpyPwd;
            arguments_exec[0] = malloc(strlen(cpyPwd) + 1);
            strncpy(arguments_exec[0], cpyPwd, strlen(cpyPwd) + 1);
            for (int i = 1; i < taille; i++)
            {
                arguments_exec[i] = malloc(strlen(tab[i]) + 1);
                snprintf(arguments_exec[i], strlen(tab[i]) + 1, "%s", tab[i]);
            }
            arguments_exec[taille] = NULL;
            // execCMD(cpyPwd, arguments_exec, last_exit);
            redirect_cmd(cpyPwd, arguments_exec, last_exit, fic, typeredirection);
            free_StingArrayArray(arguments_exec, taille);
        }
        else
        {
            *last_exit = 127;
            // printf("bash: ./%s: Aucun fichier ou dossier de ce type \n",sep[0]);
        }

        free(cpyPwd);

        free_StingArrayArray(sep, t);
    }
    else
    {

        //  On recupere la variable d'environnement path et on lui cree une copie (car si on la modifie la var d'env se modifie)
        for (int i = 0; i < taille; i++)
        {
            printf("tab[%d] = %s \n", i, tab[i]);
        }
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
            // On verifie si chaque chemin du tabl eau pathDecoup a un fichier de nom la commande ecrite par l'utilisateur (tab[0])
            // Si non on essaye avec un autre chemin
            if (isIn(pathDecoupe[i], tab[0]) == 0)
            {
                // printf("Encore \n");
                continue;
            }
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

                printf("cmd  = %s \n", cmd);

                // On complete le premier argument du tableau avec la commande
                arguments_exec[0] = malloc(strlen(cmd) + 1); //---------------------->fuite mem
                snprintf(arguments_exec[0], strlen(cmd) + 1, "%s", cmd);

                // On remplit le tableau pour les options
                for (int i = 1; i < taille; i++)
                {

                    arguments_exec[i] = malloc(strlen(tab[i]) + 1);
                    snprintf(arguments_exec[i], strlen(tab[i]) + 1, "%s", tab[i]);
                    // printf("args[%d] = %s \n", i, arguments_exec[i]);
                }

                arguments_exec[taille] = NULL;

                // Fait la redirection qui correspond au "typeredirection"
                redirect_cmd(cmd, arguments_exec, last_exit, fic, typeredirection);

                free(cmd);
                free_StingArrayArray(arguments_exec, taille);
                free_StingArrayArray(pathDecoupe, taillarrpath);

                return;
            }
        }
        *last_exit = 127;
        free_StingArrayArray(pathDecoupe, taillarrpath);
    }
}

char **explode_redirection(char **tab, int taille)
{
    char **newtab = NULL;
    for (int i = 0; i < taille - 2; i++)
    {
        newtab = realloc(newtab, strlen(tab[i]));
        newtab[i] = tab[i];
    }

    return newtab;
}



int is_pipe_redirection(char **tab, int taille,int *last_exit)
{
    for (int i = 0; i < taille; i++)
    {
        if (strcmp(tab[i], "|") == 0)
        {
            printf("pipe \n");
            return 1;
        }
        else
        {
            for (int j = 0; j < 7; j++)
            {
                if (strcmp(tab[i], redirections[j]) == 0)
                {
                    char **tab_clean = explode_redirection(tab, taille);
                    int taille_tabclean = taille - 2;
                    cut_cmd_redirec(tab_clean, taille_tabclean, last_exit, tab[taille - 1], tab[taille - 2]);
                    return 1;
                }
            }
        }
    }
    return 0;
}


