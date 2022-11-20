#include "fonctions_auxiliaires.c"

/*----------------------------------
    EXIT
*---------------------------------*/
int exits(char *val,int last_exit){
    if (val != NULL)
    {
        if (atoi(val)!= 0)
        {
            exit(atoi(val));
            //return (atoi(val));
        }
    }
    //cas exit sans paramètre
    exit(last_exit);
    //return(last_exit);
}


/*----------------------------------
    CD
*---------------------------------*/
int pop(int *tab, int taille){
    for(int i=taille-1; i>= 0 ; i--){
        if(tab[i] != 0){
            return tab[i];
        }
    }
    return 0;
}

void deleteLast(int *tab, int taille){
    for(int i=taille-1; i>= 0 ; i--){
        if(tab[i] != 0){
            tab[i] = 0;
            return;
        }
    }
}

void push(int *tab, int taille, int n){
    for(int i=0; i<taille; i++){
        if(tab[i] == 0){
            tab[i] = n;
            return;
        }
    }
}

void truncateString(char *s, int n){
    int len = strlen(s);
    for(int i= len-1; i>= len - n; i--){
        s[i] = '\0';
    }
}

char *realLogiquePath(char *path){
    int taille;
    char **tab = explode(path, "/", &taille);
    free(path);

    char *res = malloc(4000);
    sprintf(res, "%s", "/");
    int *size = calloc(taille, sizeof(int));
    
    
    for(int i=0; i<taille; i++){
        int len = strlen(res);
        int last_size = pop(size, taille);

        // on ignore
        if(strcmp(tab[i], ".") == 0){
            if(i == taille-1) truncateString(res, 1);
            continue;

        // On revient au dossier parent
        }else if(strcmp(tab[i], "..") == 0){
            int nb_slash; // Nombre de '/' a supprimer
            if(i == taille -1){
                nb_slash = 2;
            }else{
                nb_slash = 1;
            }
            truncateString(res, last_size + nb_slash);
            deleteLast(size, taille);
            
        }else{
            sprintf(&res[len], "%s", tab[i]);
            if(i != taille-1) strcat(res, "/");
            push(size, taille, strlen(tab[i]));
        }
    }
    free(size);
    free_StingArrayArray(tab, taille);
    return res;
}

int cd (char *pathname, char *option, char *ref){
    char *l = "-L";
    char *p = "-P";
    char dest[MAX_ARGS_NUMBER];

    if(ref != NULL && ref[0] == '/'){
        chdir(ref);
        setenv("PWD", ref, 1);
        return 0;
    }

    if(ref != NULL)
    {
        if(strcmp(ref, "-") == 0)
        {
            sprintf(dest, "%s", oldPath);
        }
        else{
            sprintf(dest, "%s", pathname);
            sprintf(&dest[strlen(dest)], "/");
            strcat(dest, ref);
        }
    }else{
        sprintf(dest, "%s", getenv("HOME"));
    }

    if(option == NULL || strcmp(option, l) == 0)
    {
        char *c = malloc(strlen(dest));
        sprintf(c, "%s", dest);
        char *realpath = realLogiquePath(c);

        if(chdir(realpath) < 0)
        {
            return cd(pathname, p, ref);
        }
        setenv("PWD", realpath, 1);
        memset(oldPath, 0, MAX_ARGS_NUMBER);
        strcpy(oldPath, dossier_courant);

        memset(dossier_courant, 0, MAX_ARGS_NUMBER);
        strcpy(dossier_courant, realpath);
    }
    else if(strcmp(option, p) == 0) 
    {
        char real[MAX_ARGS_NUMBER];
        realpath(dest, real);
        if(chdir(real) < 0) 
        {
            printf("\033[36mbash: cd: %s: No such file or directory\n", ref);
            return 1;
        }
        setenv("PWD", real, 1);
        strcpy(oldPath, dossier_courant);
        strcpy(dossier_courant, real);
    }
    return 0;
}


/*----------------------------------
    PWD
*---------------------------------*/
//write
int pwd_affichage(char* ref)
{
    if(write(1,ref,strlen(ref)) == -1)
    {
        perror("Erreur ");
        free(ref);
        return 1;
    }
    return 0;
}

//Recuperation de la référence absolu logique
int pwd_logique(char* ref)
{
    if(!strcpy(ref,getenv("PWD")))   
    {
        perror("getenv - Erreur ");
        free(ref);
        return 1;
    }
    strcat(ref,"\n");
    if(pwd_affichage(ref) == 1)
        return 1;
    free(ref);
    return 0;
}

//Recuperation de la référence absolu phyique
int pwd_physique(char* ref)
{
    if(!getcwd(ref,MAX_ARGS_STRLEN))   
    {
        perror("getcwd - Erreur ");
        free(ref);
        return 1;
    }
    strcat(ref,"\n");
    if(pwd_affichage(ref) == 1)
        return 1;
    free(ref);
    return 0;
}

int pwd(int argc, char **argv){
    char *ref = malloc(MAX_ARGS_STRLEN);

    //Cas par défault : (Référence absolu logique)
    if(argc == 1)
    {
        return pwd_logique(ref);
    }
    else
    {
        //Cas -L : (Référence absolu logique)
        if(strcmp(argv[1],"-L") == 0 || strcmp(argv[1],"-l") == 0)
            return pwd_logique(ref);
        else
        {
            //Cas -P : (Référence absolu phyique)
            if(strcmp(argv[1],"-P") == 0 || strcmp(argv[1],"-p") == 0)
                return pwd_physique(ref);
            //Cas paramètre faux
            else
                return 1; 
        }

    }   
}

