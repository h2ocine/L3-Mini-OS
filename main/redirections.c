#include "../header/redirections.h"
#include "../header/util.h"
#include "../header/prompt.h"
#include "../header/cmdext.h"
#include "../header/cmdint.h"

#include <stdlib.h>

const char *redirections[] = {">", "<", ">|", ">>", "2>", "2>|", "2>>"};

//[coucou,zebi,|wsh,trkl]

//[[coucou,zebi][salut,ma poule]

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



// [bonjour,ca,va,|,super,et,toi] avec nombre_pipe =1 devrait renvoyer [[bonjour,ca,va],[super,et,toi]]

char ***getTab_cmd(char **tab, int taille, int nombre_pipe)
{
    printf("nombre de pipes = %d\n", nombre_pipe);

    // Alloue de la mémoire pour le tableau de tableaux de char
    char ***newtab = malloc(sizeof(char **) * (nombre_pipe + 1));
    if (newtab == NULL)
    {
        perror("malloc");
        exit(1);
    }

    int i = 0; // Indice pour parcourir les lignes du tableau de tableaux de char
    int j = 0; // Indice pour parcourir les mots du tableau "tab"

    // Pour chaque ligne du tableau de tableaux de char
    while (i < nombre_pipe + 1)
    {
        // Alloue de la mémoire pour le tableau de char
        char **tabaux = malloc(sizeof(char *));
        if (tabaux == NULL)
        {
            perror("malloc");
            exit(1);
        }
        int cmp = 0;

        // Pour chaque mot du tableau "tab"
        while (j < taille)
        {
            // Si le mot courant est un pipe ou si c'est le dernier mot du tableau
            if (strcmp(tab[j], "|") == 0 || j == taille - 1)
            {
                // Ajoute le tableau de char au tableau de tableaux de char
                newtab[i] = tabaux;
                cmp = 0;
                i++; // Passe à la ligne suivante du tableau de tableaux de char
                // Alloue de la mémoire pour le nouveau tableau de char
                tabaux = malloc(sizeof(char *));
                if (tabaux == NULL)
                {
                    perror("malloc");
                    exit(1);
                }
            }
            else
            {
                printf("mot = %s\n", tab[j]);
                // Redimensionne le tableau de char pour ajouter le nouveau mot
                char **tmp = realloc(tabaux, (cmp + 1) * sizeof(char *));
                if (tmp == NULL)
                {
                    perror("realloc");
                    exit(1);
                }
                tabaux = tmp;
                // Ajoute le mot au tableau de char
                tabaux[cmp] = strdup(tab[j]);
                if (tabaux[cmp] == NULL)
                {
                    perror("strdup");
                    exit(1);
                }
                cmp++;
            }
            j++;
        }
    }
    return newtab;
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
    // Pas oublier de free apres
    int *flags = malloc(sizeof(int) * 100);
    int *descriptor = malloc(sizeof(int) * 100);

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

// Va mettre dans le pointeur les tableaux qu'on va pouvoir rajouter à leurs tableaux respectifs
void decoupe_commande_pipe(char **tab, int taille, int *last_exit, char **arguments_exec, char *cmd)
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
            arguments_exec = malloc((taille + 1) * sizeof(char *));

            cmd = malloc(taille_elem + 1 + /* taille du '/' */ +strlen(tab[0]) + 1);
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
            // Verifie quel type d'exec il va falloir cela depend du type de la redirection
            // whichExec(cmd, arguments_exec, last_exit, typeredirection, fic);
            // redirect_cmd(cmd, arguments_exec, last_exit, fic, typeredirection);

            free(cmd);
            free_StingArrayArray(arguments_exec, taille);
            free_StingArrayArray(pathDecoupe, taillarrpath);
        }
    }
    *last_exit = 127;
    free_StingArrayArray(pathDecoupe, taillarrpath);
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
            // Verifie quel type d'exec il va falloir cela depend du type de la redirection
            // whichExec(cmd, arguments_exec, last_exit, typeredirection, fic);
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
    for (int i = 0; i < taille; i++)
    {
        if (strcmp(tab[i], "|") == 0)
        {
            int nb = compte_nombre_pipe(tab, taille);
            printf("nb de pipe = %d et taille de l'ancien explode = %d",nb,taille);
            char ***tab_of_pipe = getTab_cmd(tab, taille, nb);
            /*
            for(int i = 0;i<(taille-nb);i++){
                char *cmd = NULL;
                char **args = NULL;
                //decoupe_commande_pipe(tab_of_pipe[i],taille,last_exit,args,cmd);
                }
             */
            
        


            
            

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
                    decoupe_commande(tab_clean, taille_tabclean, last_exit, tab[taille - 1], tab[taille - 2]);
                    return 1;
                }
            }
        }
    }
    return 0;
}
