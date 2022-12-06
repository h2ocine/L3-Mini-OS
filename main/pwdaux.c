#include "../header/pwdaux.h"

int pwd_affichage(char *ref)
{
    if (write(1, ref, strlen(ref)) == -1)
    {
        perror("Erreur ");
        free(ref);
        return 1;
    }
    return 0;
}

int pwd_logique(char *ref)
{
    if (!strcpy(ref, getenv("PWD")))
    {
        perror("getenv - Erreur ");
        free(ref);
        return 1;
    }
    strcat(ref, "\n");
    if (pwd_affichage(ref) == 1)
        return 1;
    free(ref);
    return 0;
}

int pwd_physique(char *ref)
{
    if (!getcwd(ref, MAX_ARGS_STRLEN))
    {
        perror("getcwd - Erreur ");
        free(ref);
        return 1;
    }
    strcat(ref, "\n");
    if (pwd_affichage(ref) == 1)
        return 1;
    free(ref);
    return 0;
}