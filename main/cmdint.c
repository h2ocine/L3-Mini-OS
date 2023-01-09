#include "../header/cmdint.h"
#include "../header/joker.h"

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
char **ajoute_debut_tab(char **tab, int size_tab, char *s)
{
    char **res = malloc(size_tab + 1);
    res[0] = s;

    for (int i = 1; i < size_tab + 1; i++)
    {
        res[i] = tab[i - 1];
    }
    return res;
}

int isCommandeInterne(char *cmd){
    if(strcmp(cmd, "exit") == 0){
        return 1;
    }
    if(strcmp(cmd, "cd") == 0){
        return 1;
    }
    if(strcmp(cmd, "pwd") == 0){
        return 1;
    }
}

void casEtoileCmd(char **tab, int *last_exit, int taille){
    
    
    char *courant = "./";
    char *pre = malloc(strlen(courant)+1);
    snprintf(pre, strlen(courant)+1, "%s", tab[0]);
    pre[strlen(courant)] = '\0';
    
    int tailleAllFic;
    char **all_fic;
    if(strcmp(pre, courant) == 0){
        all_fic = all_joker_fic(tab[0], ".", &tailleAllFic);
    }else{
        all_fic = trans(".", tab[0], &tailleAllFic);
    }
    free(pre);

    // affiche_mat(all_fic, tailleAllFic);
    // *o -> ["echo"]
    // affiche_mat(all_fic, tailleAllFic);

    for(int i=0; i<tailleAllFic; i++){
        struct stat st;
        if(stat(all_fic[i], &st) <0){
            free_StingArrayArray(all_fic, tailleAllFic);
            exit(1);
        }
        // Verifie si on a les droits d'execution
        if(S_ISREG(st.st_mode)){
            int tailleNewTab = 1;
            char **newTab = malloc(sizeof(char *));
            newTab[0] = malloc(strlen(all_fic[0])+1);
            snprintf(newTab[0], strlen(all_fic[0])+1, "%s", all_fic[0]);
            newTab[0][strlen(all_fic[0])] = '\0';

            int tailleArgs = 0;
            char **args = NULL;
            for(int h=1; h<taille; h++){
                if(tab[h][0] == '-'){
                    tailleArgs++;
                    args = realloc(args, sizeof(char *) * tailleArgs);
                    args[tailleArgs-1] = malloc(strlen(tab[h])+1);
                    snprintf(args[tailleArgs-1], strlen(tab[h])+1, "%s", tab[h]);
                    args[tailleArgs-1][strlen(tab[h])] = '\0';

                }
            }

            int tailleCpyNTab = tailleNewTab;
            char **cpyNTab = copie_tab(newTab, tailleNewTab);
            free_StingArrayArray(newTab, tailleNewTab);

            newTab = cat_tabs(cpyNTab, tailleCpyNTab, args, tailleArgs, &tailleNewTab);
            free_StingArrayArray(cpyNTab, tailleCpyNTab);
            free_StingArrayArray(args, tailleArgs);
            

                for(int j=tailleArgs+1; j<taille; j++){

                    int taillePos;
                    char **pos = trans(".", tab[j], &taillePos);

                    int tailleCpy = tailleNewTab;
                    char **cpy = copie_tab(newTab, tailleNewTab);
                    free_StingArrayArray(newTab, tailleNewTab);

                    newTab = cat_tabs(cpy, tailleCpy, pos, taillePos, &tailleNewTab);
                    free_StingArrayArray(cpy, tailleCpy);

                    free_StingArrayArray(pos, taillePos);
                }
            
            recherche_commande_interne(newTab, last_exit, tailleNewTab);
            free_StingArrayArray(newTab, tailleNewTab);
            free_StingArrayArray(all_fic, tailleAllFic);
            return;
        }
    
    }
    free_StingArrayArray(all_fic, tailleAllFic);
}

void recherche_commande_interne(char **tab, int *last_exit, int taille)
{     
    
    if(strchr(tab[0], '*') != NULL){
        casEtoileCmd(tab, last_exit, taille);
        return ;
    }

    // On met tout les chemins mentionnées par l'utilisateur dans un tableau; ls * *.c -> ["*", "*.c"]
    char **all_path = NULL;
    int size_path = 0;
    //printf(" ca rentre \n");
    for (int i = 1; i < taille; i++)
    {
        if (tab[i][0] != '-')
        {   
            size_path++;
            all_path = realloc(all_path, sizeof(char *) * size_path);
            all_path[size_path - 1] = malloc(strlen(tab[i]) + 1);
            snprintf(all_path[size_path - 1], strlen(tab[i]) + 1, "%s", tab[i]);
            all_path[size_path - 1][strlen(tab[i])] = '\0';
        }
    }

    int size_all_joker;
    
    
    char **all_joker = all(all_path, size_path, "", &size_all_joker);

    free_StingArrayArray(all_path,size_path);
    

    if (strcmp("exit", tab[0]) == 0)
    {
        if (taille >= 2)
        {
            if (size_all_joker > 1)
            {
                free_StingArrayArray(all_joker, size_all_joker);
                return;
            }
            char t[strlen(tab[1]) + 1];
            snprintf(t, strlen(tab[1]) + 1, "%s", tab[1]);
            t[strlen(tab[1]) + 1] = '\0';
            free_StingArrayArray(tab, taille);
            
            free_StingArrayArray(all_joker, size_all_joker);
            *last_exit = exits(t, *last_exit);
        }
        else
        {   
            free_StingArrayArray(all_joker, size_all_joker);
            *last_exit = exits(NULL, *last_exit);
        }
    }
    else if (strcmp("cd", tab[0]) == 0)
    {
        if (size_all_joker > 1)
        {
            free_StingArrayArray(all_joker, size_all_joker);
            return;
        }

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
        // set commande (commande avec ses arguments)
        // initialiser commande avec la commande de base

        char **commande = malloc(sizeof(char *) * 1);
        int size_cmd = 1;

        // tab[0]
        commande[0] = malloc(strlen(tab[0]) + 1);
        strncpy(commande[0], tab[0], strlen(tab[0])+1);
        commande[0][strlen(tab[0])] = '\0';

        for (int h = 1; h < taille; h++)
        {   
            
            if (tab[h][0] != '-')
            {
                break;
            }
            size_cmd++;

            commande = realloc(commande, sizeof(char *) * size_cmd);
            commande[h] = malloc(strlen(tab[h]) + 1);
            strncpy(commande[h], tab[h], strlen(tab[h])+1);
            commande[h][strlen(tab[h])] = '\0';
        }
        
        int tailleAll;
        char **all = cat_tabs(commande, size_cmd, all_joker, size_all_joker, &tailleAll);


        if(strcmp(tab[0], "echo") == 0){
            char **echo = NULL;
            int size_echo = 0;

            for(int e=0; e<size_all_joker; e++){
                char *path = truncate_str(all_joker[e], '/'); 
                
                char *fich = truncate_start(all_joker[e], strlen(path)+1);

                if(isIn(path, fich) == 1){
                    size_echo ++;
                    echo = realloc(echo, sizeof(char *) * size_echo);
                    echo[size_echo-1] = malloc(strlen(all_joker[e])+1);
                    strncpy(echo[size_echo-1], all_joker[e], strlen(all_joker[e])+1);
                }

                free(path);
                free(fich);
            }

            
            char **tab_echo = NULL;
            int size_tab_echo = 0;

            if(size_echo != 0){ 
                int size_final;
                char **final = cat_tabs(commande, size_cmd, echo, size_echo, &size_final);
                commande_externe(final, size_final,last_exit);
                
                free_StingArrayArray(final, size_final);
            }else{
                

                for(int s=1; s<taille; s++){
                    size_tab_echo++;
                    tab_echo = realloc(tab_echo, sizeof(char *)*size_tab_echo);
                    tab_echo[size_tab_echo-1] = malloc(strlen(tab[s])+1);
                    strncpy(tab_echo[size_tab_echo-1], tab[s], strlen(tab[s])+1);;
                }

                int size_final;
                char **final = cat_tabs(commande, size_cmd, tab_echo, size_tab_echo, &size_final);
                commande_externe(final, size_final,last_exit);
                free_StingArrayArray(final, size_final);
            }
            

            free_StingArrayArray(echo, size_echo);
            free_StingArrayArray(tab_echo, size_tab_echo);
        }else{
            if(!(size_path != 0 && size_all_joker == 0))

            
            commande_externe(all, tailleAll,last_exit);
            
        }
        free_StingArrayArray(all, tailleAll);
        free_StingArrayArray(commande, size_cmd);
    }
    
    free_StingArrayArray(all_joker, size_all_joker);
}
