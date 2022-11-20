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

    //char *res = malloc(200);
    char *res = malloc(2);
    if(sprintf(res, "%s", "/")<0) {perror("sprintf erreur ");exit(1);}
    res[1] = '\0';

    int *size = calloc(taille, sizeof(int));

    for(int i = 0 ; i < taille ; i++)
    {
        int len = strlen(res);
        int last_size = pop(size, taille);

        // On ignore
        if(strcmp(tab[i], ".") == 0){
            if(i == taille-1) truncateString(res, 1);
            continue;

        // On revient au dossier parent
        }
        else if(strcmp(tab[i], "..") == 0)
        {
            int nb_slash; // Nombre de '/' a supprimer
            if(i == taille -1)
                nb_slash = 2;
            else
                nb_slash = 1;
            truncateString(res, last_size + nb_slash);
            deleteLast(size, taille);
        }
        else
        {
            printf("res = %s ; %ld\n",res,strlen(res));
            printf("tab[i] = %s ; %ld\n",tab[i],strlen(tab[i]));

            if(realloc(res, strlen(res) + strlen(tab[i])) == NULL)
            {
                perror("(ligne 97)fonction realLogiquePath realloc erreur ");
                exit(1);
            }
            if(sprintf(&res[len], "%s", tab[i]) < 0) {perror("sprintf erreur ");exit(1);}
            res[len + strlen(tab[i])] = '\0';
        
            if(i != taille-1)
            {
                if(realloc(res, strlen(res) + 1) == NULL)
                {
                    perror("(ligne 107)fonction realLogiquePath realloc erreur ");
                    exit(1);
                }    
                res[len + strlen(tab[i])] = '/';
                res[len + strlen(tab[i]) + 1] = '\0';
            }
            push(size, taille, strlen(tab[i]));
        }
    }
    free(size);
    free_StingArrayArray(tab, taille);
    return res;
}

int cd_physique(char *path, char *ref){
    char real[MAX_ARGS_NUMBER];
    realpath(path, real);

    //CAS : fichier inexistant 
    if(chdir(real) < 0) 
    {
        printf("\033[36mbash: cd: %s: No such file or directory\n", ref);
        return 1;
    }
    setenv("PWD", real, 1);

    // On vide la chaine de caractere oldPath et on lui donne la valeur de dossier_courant
    memset(oldPath, 0, MAX_ARGS_NUMBER);
    strcpy(oldPath, dossier_courant);
    oldPath[strlen(dossier_courant)] = '\0';

    // On vide la chaine de caractere dossier_courant et on lui donne la valeur de path
    memset(dossier_courant, 0, MAX_ARGS_NUMBER);
    strcpy(dossier_courant, real);
    dossier_courant[strlen(real)] = '\0';

    return 0;
}

int cd_logique(char *path, char *ref){

    // On cree une copie de path car pour utiliser la fonction explode on a besoin d'une chaine de caractère alouer dynamiquement
    char *c = malloc(strlen(path));
    if(sprintf(c, "%s", path) < 0) {
        perror("sprintf erreur ");
        exit(1);
    }
    c[strlen(path)] = '\0';

    
    char *realpath = realLogiquePath(c);
    // On vérifie le chemin physique si le chemin realpath n'a pas de sens(càd qu'il existe)
    if(chdir(realpath) < 0) return cd_physique(path, ref);
    setenv("PWD", realpath, 1); // On change la var d'environnement PWD car elle est utiliser dans la commande 
    
    // On vide la chaine de caractere oldPath et on lui donne la valeur de dossier_courant
    memset(oldPath, 0, MAX_ARGS_NUMBER);
    strcpy(oldPath, dossier_courant);
    oldPath[strlen(dossier_courant)] = '\0';

    // On vide la chaine de caractere dossier_courant et on lui donne la valeur de path
    memset(dossier_courant, 0, MAX_ARGS_NUMBER);
    strcpy(dossier_courant, realpath);
    dossier_courant[strlen(realpath)] = '\0';

    return 0;
}

int cd (char *pathname, char *option, char *ref){
    char *l = "-L"; // Represente l'option -L
    char *p = "-P"; // Represente l'option -P
    char dest[MAX_ARGS_NUMBER] = "";


    if(ref != NULL)
    {   
        // Si la ref est un chemin relatif
        if(ref[0] == '/'){
            if(sprintf(dest, "%s", ref) < 0) {
                perror("sprintf erreur ");
                exit(1);
            }
        }

        // Si la ref est "-" dest vaut oldpath 
        else if(strcmp(ref, "-") == 0)
        {   
            if(sprintf(dest, "%s", oldPath) < 0) {
                perror("sprintf erreur");
                exit(1);
            }
            dest[strlen(oldPath)] = '\0';
            
        }
        else
        {
            if(sprintf(dest, "%s", pathname) < 0) {
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
        if(sprintf(dest, "%s", getenv("HOME")) < 0) {perror("sprintf erreur ");exit(1);} 
        dest[strlen(getenv("HOME"))] = '\0';
    }

    // Cas lien logique
    if(option == NULL || strcmp(option, l) == 0) cd_logique(dest, ref);
    else if(strcmp(option, p) == 0) cd_physique(dest, ref);
    else return 1;  // Cas d'une autre option

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

