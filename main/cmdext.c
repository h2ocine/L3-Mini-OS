#include "../header/cmdext.h"
#include "../main/signal.c"


void execCMD(char *cmd, char **args,int *last_exit)
{   
    int status;
    int t = 0;  
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
    }
    else if (pid != 0)
    {  
        // Rendre le comportement par défault de SIGINT et SIGTERM 
        waitpid(pid,last_exit, 0);

        // Vérification de la cause de l'arrêt du processus fils -> si c'est le cas retourner la valeur 255 (afin d'afficher SIG dans affiche_prompt)
        if (WIFSIGNALED(*last_exit))
            *last_exit = 255;
    }
    else
    {
        gestion_signeaux(1);

        if(execvp(cmd, args) < 0){
            exit(WEXITSTATUS(t));
        }
    }
}

void commande_externe(char **tab, int taille,int *last_exit)
{
    char pre[3];
    snprintf(pre, 3, "%s", tab[0]);
    pre[2] = '\0';

    if (strcmp(pre, "./") == 0)
    {
        //printf(" ca rentre\n");
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
            arguments_exec[0] = malloc(strlen(cpyPwd)+1);
            strncpy(arguments_exec[0], cpyPwd, strlen(cpyPwd)+1);
            for (int i = 1; i < taille; i++)
            {
                arguments_exec[i] = malloc(strlen(tab[i]) + 1);
                snprintf(arguments_exec[i], strlen(tab[i]) + 1, "%s", tab[i]);
            }
            arguments_exec[taille] = NULL;
            execCMD(cpyPwd, arguments_exec,last_exit);
            free_StingArrayArray(arguments_exec, taille);
        }else{
            *last_exit = 127;
            //printf("bash: ./%s: Aucun fichier ou dossier de ce type \n",sep[0]);
        }
        
        free(cpyPwd);

        free_StingArrayArray(sep, t);
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
            // On verifie si chaque chemin du tabl eau pathDecoup a un fichier de nom la commande ecrite par l'utilisateur (tab[0])
            // Si non on essaye avec un autre chemin
            if (isIn(pathDecoupe[i], tab[0]) == 0){
                continue;
            }
            else
            {   
                size_t taille_elem = strlen(pathDecoupe[i]);

                // On cree un tableau de chaine de caratere pour la fonction exec qui doit ressembler à ça pour l'exemple "ls -l path" -> [/usr/bin/ls, -l, path, NULL]
                // Taille + 1 car le dernier element doit etre NULL
                char **arguments_exec = malloc((taille + 1) * sizeof(char *));

                char *cmd = malloc(taille_elem + 1 + /* taille du '/' */ +strlen(tab[0]) + 1);
                if (cmd == NULL) perror("malloc");
                snprintf(cmd, taille_elem + 1, "%s", pathDecoupe[i]);
                cmd[taille_elem] = '\0';

                // equivalent de strcat(arg0,tab[0]) :
                cmd[taille_elem] = '/';
                snprintf(&cmd[taille_elem + 1], strlen(tab[0]) + 1, "%s", tab[0]);
                int t = taille_elem + 1 + strlen(tab[0]);
                cmd[t] = '\0';

                // On complete le premier argument du tableau avec la commande
                arguments_exec[0] = malloc(strlen(cmd) + 1);
                snprintf(arguments_exec[0], strlen(cmd) + 1, "%s", cmd);
                arguments_exec[0][strlen(cmd)] = '\0';

                // On remplit le tableau pour les options
                for (int i = 1; i < taille; i++)
                {
                    arguments_exec[i] = malloc(strlen(tab[i]) + 1);
                    snprintf(arguments_exec[i], strlen(tab[i]) + 1, "%s", tab[i]);
                    arguments_exec[i][strlen(tab[i])] = '\0';
                }
                arguments_exec[taille] = NULL;

                execCMD(cmd, arguments_exec,last_exit);
                free(cmd);
                free_StingArrayArray(arguments_exec, taille);
                free_StingArrayArray(pathDecoupe, taillarrpath);
                return;
            }
        }
        
        char **arguments_exec = copie_tab(tab, taille);

        arguments_exec = realloc(arguments_exec, sizeof(char *) * taille+1);
        arguments_exec[taille] = NULL;

        // char **arguments_exec = malloc(sizeof(char *)*4);

        // char *cmd = "../commands/normalise";
        // arguments_exec[0] = malloc(strlen(cmd)+1);
        // snprintf(arguments_exec[0], strlen(cmd)+1, "%s", cmd);

        // char *header = "header";
        // arguments_exec[1] = malloc(strlen(header)+1);
        // snprintf(arguments_exec[1], strlen(header)+1, "%s", header);

        // char *main = "main";
        // arguments_exec[2] = malloc(strlen(main)+1);
        // snprintf(arguments_exec[2], strlen(main)+1, "%s", main);

        // arguments_exec[3] = NULL;
        
        execCMD(arguments_exec[0], arguments_exec, last_exit);

        free_StingArrayArray(arguments_exec, taille);
        // *last_exit = 127;
        free_StingArrayArray(pathDecoupe, taillarrpath);
    }
    
}