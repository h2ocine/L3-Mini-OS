#include "../header/cmdint.h"

#define MAX_ARGS_NUMBER 4096
#define MAX_ARGS_STRLEN 4096

int exits(char *val, int last_exit)
{
    if (val != NULL)
    {
        if ((*val) == '0')
            exit(0);
        int n = atoi(val);
        if (n != 0)
        {
            printf("n: %d\n", n);
            exit(n);
        }
    }
    else
    {
        exit(last_exit);
    }
    return 1;
}

int cd(char *option, char *ref)
{
    char *l = "-L"; // Represente l'option -L
    char *p = "-P"; // Represente l'option -P
    char dest[MAX_ARGS_NUMBER] = "";
    char *pathname = getenv("PWD");

    if (ref != NULL)
    {
        // Si la ref est un chemin relatif
        if (ref[0] == '/')
        {
            if (snprintf(dest, strlen(ref) + 1, "%s", ref) < 0)
            {
                perror("sprintf erreur ");
                exit(1);
            }
            dest[strlen(ref)] = '\0';
        }

        // Si la ref est "-" dest vaut oldpath
        else if (strcmp(ref, "-") == 0)
        {
            char *oldPath = getenv("OLDPWD");
            if (snprintf(dest, strlen(oldPath) + 1, "%s", oldPath) < 0)
            {
                perror("sprintf erreur");
                exit(1);
            }
            dest[strlen(oldPath)] = '\0';
        }
        else
        {
            if (snprintf(dest, strlen(pathname) + 1, "%s", pathname) < 0)
            {
                perror("sprintf erreur ");
                exit(1);
            }
            dest[strlen(pathname)] = '/';
            dest[strlen(pathname) + 1] = '\0';
            strcat(dest, ref);
            dest[strlen(pathname) + 1 + strlen(ref)] = '\0';
        }
    }
    else
    {
        char *home = getenv("HOME");
        if (snprintf(dest, strlen(home) + 1, "%s", home) < 0)
        {
            perror("sprintf erreur ");
            exit(1);
        }
        dest[strlen(home)] = '\0';
    }

    // Cas lien logique
    if (option == NULL || strcmp(option, l) == 0)
        return cd_logique(dest, ref);
    else if (strcmp(option, p) == 0)
        return cd_physique(dest, ref);
    else
        return 1; // Cas d'une autre option

    return 0;
}

int pwd(int argc, char **argv)
{
    char *ref = malloc(MAX_ARGS_STRLEN);

    // Cas par défault : (Référence absolu logique)
    if (argc == 1)
    {
        return pwd_logique(ref);
    }
    else
    {
        // Cas -L : (Référence absolu logique)
        if (strcmp(argv[1], "-L") == 0 || strcmp(argv[1], "-l") == 0)
            return pwd_logique(ref);
        else
        {
            // Cas -P : (Référence absolu phyique)
            if (strcmp(argv[1], "-P") == 0 || strcmp(argv[1], "-p") == 0)
                return pwd_physique(ref);
            // Cas paramètre faux
            else
                return 1;
        }
    }
}

void recherche_commande_interne(char **tab, int *last_exit, int taille)
{
    if(taille > 0)
    {
        int taille_malloc_argument_commande = 1;
        for(int i = 0; i < taille - 1; i++)
        {
            taille_malloc_argument_commande = taille_malloc_argument_commande + strlen(tab[i]) + 1;
        }
        
        // char* path = "";
        // char* argument_commande = "";

        char * path = malloc(strlen(tab[taille-1]) + 1);
        char* argument_commande = malloc(taille_malloc_argument_commande);

        path[0] = '\0';
        argument_commande[0] = '\0';
        
        printf("[TEST]path = %s\n",path);
        printf("[TEST]argument_commande = %s\n",argument_commande);

        //set path
        strncpy(path,tab[taille-1],strlen(tab[taille-1]));
        path[strlen(tab[taille-1])] = '\0';

        //set argument_commande
        int strlen_argument_commande = 0;
        for(int i = 0; i < taille - 1; i++)
        {
            strncat(argument_commande,tab[i],strlen(tab[i]));
            strlen_argument_commande = strlen_argument_commande + strlen(tab[i]) + 1;
                
            if(i == taille - 2)
                continue;
            
            argument_commande[strlen_argument_commande - 1] = ' ';
            argument_commande[strlen_argument_commande] = '\0';
        }
        
        printf("path = %s\n",path);
        printf("argument_commande = %s\n",argument_commande);

        if(joker_1(path,argument_commande) == 1)
        {
            printf("joker1\n");
            free(path);
            free(argument_commande);
            return;
        }
        
        printf("ne rentre pas dans joker1\n");
        free(path);
        free(argument_commande);
    }

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