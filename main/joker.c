#include "../header/joker.h"

char is_valid(char *chaine)
{
    if (chaine == NULL)
        return 0;

    if (chaine[0] == '*')
        return 1;

    return 0;
}

char *get_postfix(char *chaine)
{
    char *resultat = malloc(1);
    resultat[0] = '\0';
    for (int i = 0; i < strlen(chaine); i++)
    {
        if (chaine[i] == '*')
        {
            if (i + 1 == strlen(chaine))
                return resultat;

            resultat = (char *)realloc(resultat, strlen(chaine) - i);
            for (int j = i + 1; j < strlen(chaine); j++)
            {
                resultat[j - (i + 1)] = chaine[j];
            }
            resultat[strlen(chaine)] = '\0';
        }
    }
    return resultat;
}

// // verifie si pre est postfix de chaine (1 si oui; 0 sinon)
int is_postfix(char *chaine, char *pre)
{
    if (strcmp(pre, "") == 0)
        return 1;

    if (strlen(pre) > strlen(chaine))
        return 0;

    if (chaine[0] == '.')
        return 0;

    int index_pre = strlen(pre) - 1;
    int index_chaine = strlen(chaine) - 1;
    while (index_pre >= 0)
    {
        if (pre[index_pre] != chaine[index_chaine])
            return 0;

        index_pre--;
        index_chaine--;
    }
    return 1;
}

int nombre_mot_char(char *chaine, char separator)
{
    if (strlen(chaine) == 0)
        return 0;

    int resultat = 1;
    for (int i = 0; i < strlen(chaine); i++)
        if (chaine[i] == 0)
            resultat++;

    return resultat;
}

//ne pas oublier de free qu'on on utlise get_balise
char *get_balise(char *path)
{
    int index = -1;
    for (int i = 0; i < strlen(path); i++)
        if (path[i] == '/')
            index = i;

    char *balise = malloc(strlen(path) - index + 1);

    for (int j = index + 1; j < strlen(path); j++)
        balise[j - (index + 1)] = path[j];

    balise[strlen(path) - (index + 1)] = '\0';
    return balise;
    
}

//ne pas oublier de free qu'on on utlise get_pathtobalise
char *get_pathtobalise(char *path)
{
    int index;
    for (int i = 0; i < strlen(path); i++)
        if (path[i] == '/')
            index = i;

    char *pathtobalise = malloc(index + 1 + 1);

    for (int i = 0; i < index + 1; i++)
        pathtobalise[i] = path[i];

    pathtobalise[index + 1] = '\0';
    return pathtobalise;
}

// retourne 0 en cas d'echec (path non valide); 1 sinon
// char path[] = "a/*.c";
// char argument_commande[] = "ls -a -l"; 
int joker_1(char* path, char* argument_commande)
{
    char *balise = get_balise(path);
    char *pathtobalise = get_pathtobalise(path);

    // initialiser arguments avec commande + argument_commande
    char *arguments = malloc(4000);
    strncpy(arguments, argument_commande, strlen(argument_commande));
    arguments[strlen(argument_commande) + 1] = '\0';

    if (!is_valid(balise))
        return 0;

    DIR *repertoir;
    if(strcmp(pathtobalise,"") == 0)
        repertoir = opendir(".");
    else    
        repertoir = opendir(pathtobalise);
        
    struct dirent *parcours;
    int boolean = 0;
    while ((parcours = readdir(repertoir)))
    {
        if (is_postfix(parcours->d_name, get_postfix(balise)) == 1)
        {
            boolean = 1;
            // ajouter le nom du fichier dans arguments
            int strlen_arguments = strlen(arguments);
            arguments[strlen_arguments] = ' ';
            arguments[strlen_arguments + 1] = '\0';

            strncat(arguments, pathtobalise, strlen(pathtobalise));
            arguments[strlen_arguments + 1 + strlen(pathtobalise)] = '\0';
            
            strncat(arguments, parcours->d_name, strlen(parcours->d_name));
            arguments[strlen_arguments + 1 + strlen(pathtobalise) + strlen(parcours->d_name)] = '\0';
        }
    }
    closedir(repertoir);
    free(balise);
    free(pathtobalise);

    if (boolean)
    {
        int nbmot = nombre_mot_char(arguments, ' ');
        char **tmp = explode(arguments, " ", &nbmot);
        tmp[nbmot] = NULL;
        
        free(arguments);
        execCMD(tmp[0], tmp);
        free_StingArrayArray(tmp, nbmot - 1);
    }
    else
        free(arguments);

    return 1;
}