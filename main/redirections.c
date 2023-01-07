#include "../header/redirections.h"
#include "../header/util.h"
#include "../header/prompt.h"
#include "../header/cmdext.h"
#include "../header/cmdint.h"



void execSORTIE(char *cmd, char **args, int *last_exit, char *fic)
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
        waitpid(pid, last_exit, 0);
    }
    else
    {
        
        /*
        Pour les tubes nommés il faut revérifier

        if (mknod(fic, S_IFIFO | 0644, 0) == -1) {
            perror("Erreur lors de la création du tube nommé");
            exit(1);
        }
         */

        int fd = open(fic, O_WRONLY | O_CREAT | O_EXCL, 0644);
        if (fd == -1)
        {
            perror("Erreur lors de l'ouverture du fichier ou du tube nommé");
            exit(1);
        }

        // Redirige la sortie standard de la commande "cmd" vers le descripteur de fichier ouvert
        if (dup2(fd, STDOUT_FILENO) == -1)
        {
            perror("Erreur lors de la redirection de la sortie standard de la commande");
            exit(1);
        }

        // Ferme le descripteur de fichier (il reste redirigé vers la sortie standard de la commande)
        if (close(fd) == -1)
        {
            perror("Erreur lors de la fermeture du descripteur de fichier");
            exit(1);
        }

        if (execvp(cmd, args) < 0)
        {
            exit(WEXITSTATUS(t));
        }
    }
}

void whichExec(char *cmd, char **args, int *last_exit, char *typeredirection,char *fic)
{
 
        if (strcmp(typeredirection, "<") == 0)
        {
            printf("a \n");
        }
        else if (strcmp(typeredirection, ">") == 0)
        {
            // redirect_cmd_output(tab, tab[taille-1], last_exit, taille);
            execSORTIE(cmd,args,last_exit,fic);        
        }
        else if (strcmp(typeredirection, ">|") == 0)
        {
            printf("redirection sortie standard avec ecrasement \n");
        }
        else if (strcmp(typeredirection, ">>") == 0)
        {
            printf("redirection sortie standard en en concatenation \n");
        
        }
        else if (strcmp(typeredirection, "2>") == 0)
        {
            printf("redirection de la sortie erreur standard sans ecrasement ");
        }
        else if (strcmp(typeredirection, "2>>") == 0)
        {
            printf("redirection sortie erreur standard en concatenation \n");
          
        }
        else if (strcmp(typeredirection, "|") == 0)
        {
            printf("redirection de la sortie standard de cmd1 et l'entrée standard cm2 tube \n");
        }
    }



void decoupe_commande(char **tab, int taille, int *last_exit, char *fic, char *typeredirection)
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
                printf("args[%d] = %s \n", i, arguments_exec[i]);
            }

            arguments_exec[taille] = NULL;
            //Verifie quel type d'exec il va falloir cela depend du type de la redirection
            whichExec(cmd,arguments_exec,last_exit,typeredirection,fic);

            free(cmd);
            free_StingArrayArray(arguments_exec, taille);
            free_StingArrayArray(pathDecoupe, taillarrpath);

            return;
        }
    }
    *last_exit = 127;
    free_StingArrayArray(pathDecoupe, taillarrpath);
}

// Faire plusieurs execmd en fonction du > peut être une bonne idée

// Faire une fonction avec exec CMD

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

// Fonction qui va dispatcher les différents cas
int check_redirection(char **tab, int taille, int *last_exit)
{
    // Initialisation du nouveau tableau
    char **tab_clean = explode_redirection(tab, taille);
    int taille_tabclean = taille - 2;

    for (int i = 0; i < taille; i++)
    {
        if (strcmp(tab[i], "<") == 0)
        {
            return 1;
        }
        else if (strcmp(tab[i], ">") == 0)
        {
            printf("ca rentre au bon endroit \n");
            // redirect_cmd_output(tab, tab[taille-1], last_exit, taille);
            decoupe_commande(tab_clean, taille_tabclean, last_exit,tab[taille-1],">");
            return 1;
        }
        else if (strcmp(tab[i], ">|") == 0)
        {
            printf("redirection sortie standard avec ecrasement \n");
            return 1;
        }
        else if (strcmp(tab[i], ">>") == 0)
        {
            printf("redirection sortie standard en en concatenation \n");
            return 1;
        }
        else if (strcmp(tab[i], "2>") == 0)
        {
            printf("redirection de la sortie erreur standard sans ecrasement ");
            return 1;
        }
        else if (strcmp(tab[i], "2>>") == 0)
        {
            printf("redirection sortie erreur standard en concatenation \n");
            return 1;
        }
        else if (strcmp(tab[i], "|") == 0)
        {

            printf("redirection de la sortie standard de cmd1 et l'entrée standard cm2 tube \n");
            return 1;
        }
    }
    return 0;
}
