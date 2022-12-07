#include "fonctions_auxiliaires.c"
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

    if(chaine[0] == '.') 
        return 0;

    int index_pre = strlen(pre) - 1;
    int index_chaine = strlen(chaine) - 1;
    while(index_pre >= 0)
    {
        if(pre[index_pre] != chaine[index_chaine])
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

//retourne 0 en cas d'echec
int joker_1(char* commande, char* argument_commande)
{
    char commande[] = "*.c";
    char argument_commande[] = "ls -a -l";

    //initialiser arguments avec commande + argument_commande
    char* arguments = malloc(4000);
    strncpy(arguments,commande,strlen(commande));
    arguments[strlen(commande)] = ' ';
    arguments[strlen(commande) + 1] = '\0';
    strncat(arguments,argument_commande,strlen(argument_commande));
    arguments[strlen(commande) + 1 + strlen(argument_commande)] = '\0';

    printf("\narguments : %s\n", arguments);

    if(!is_valid(commande))
        return 0;

    DIR *repertoir = opendir(".");
    struct dirent *parcours;
    int boolean = 0;
    while ((parcours = readdir(repertoir)))
    {
        if (is_postfix(parcours->d_name, get_postfix(commande)) == 1)
        {
            boolean = 1;
            //ajouter le nom du fichier dans arguments
            int strlen_arguments = strlen(arguments);
            arguments[strlen_arguments] = ' ';
            arguments[strlen_arguments + 1] = '\0';
            strncat(arguments,parcours->d_name,strlen(parcours->d_name));
            arguments[strlen_arguments + 1 + strlen(parcours->d_name)] = '\0';
        }
    }
    printf("arguments + fichiers : %s\n", arguments);
    if(boolean)
    {
        int nbmot = nombre_mot_char(arguments,' ');
        char **tmp = explode(arguments," ", &nbmot);
        tmp[nbmot] = NULL;
        printf("%d\n",nbmot);
        
        int i=0;
        while(tmp[i] != NULL)
        {
             printf("%s\n",tmp[i]);i++;
        }

        free(arguments);
        execCMD(argv[2], tmp);
    }
    else
        free(arguments);

    return 1;
}