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

void truncateString(char *s, int n){
    int len = strlen(s);
    for(int i= len-1; i>= len - n; i--){
        s[i] = '\0';
    }
}

char * truncate_str(char *s, char spr){

    int stop = strlen(s);

    for(int i=stop-1; i>=0; i--){
        stop--;
        if(s[i] == spr) break;
    }
    printf("stop: %ld\n", strlen(s) - stop);

    // printf("resultat: %s\n", res);
    char *res = malloc(stop + 1);
    snprintf(res, stop + 1 , "%s", s);
    res[stop] = '\0';
    return res;
}

void videString(char *s){
    for(int i=0; i<strlen(s); i++){
        s[i] = '\0';
    }
}

char *logiquePath(char *path){
    if(strlen(path) != 1 && path[strlen(path)-1] == '/') perror("mauvais format");
    int taille;
    char **tab = explode(path, "/", &taille);
    free(path);

    char *res = malloc(1);
    res[0] = '\0';
    for(int i=0; i<taille; i++){
        if(strcmp(tab[i], ".") == 0){
            continue;

        // On revient au dossier parent
        }
        else if(strcmp(tab[i], "..") == 0)
        {   
            if(strlen(res) != 0){
                char *s = truncate_str(res, '/');
                videString(res);
                res = realloc(res, strlen(s) + 1);
                snprintf(res, strlen(s)+1, "%s", s);
                res[strlen(s)] = '\0';
                free(s);
            }

        }
        else
        {   
            size_t t = strlen(res) + strlen(tab[i]) + 2;
            res = realloc(res, t);

            strcat(res, "/");
            strcat(res, tab[i]);
            res[strlen(res)] = '\0';
        }
    }
    if(strlen(res) == 0){
        res = realloc(res, 2);
        res[0] = '/';
        res[1] = '\0';
    }
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

    // On vide la chaine de caractere oldPath et on lui donne la valeur de dossier_courant
    setenv("OLDPWD", getenv("PWD"), 1);
    // memset(oldPath, 0, MAX_ARGS_NUMBER);
    // strcpy(oldPath, dossier_courant);
    // oldPath[strlen(dossier_courant)] = '\0';

    // On vide la chaine de caractere dossier_courant et on lui donne la valeur de path
    setenv("PWD", real, 1);
    // memset(dossier_courant, 0, MAX_ARGS_NUMBER);
    // strcpy(dossier_courant, real);
    // dossier_courant[strlen(real)] = '\0';

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

    
    char *realpath = logiquePath(c);
    // On vérifie le chemin physique si le chemin realpath n'a pas de sens(càd qu'il existe)
    if(chdir(realpath) < 0) return cd_physique(path, ref);
    
    // On vide la chaine de caractere oldPath et on lui donne la valeur de dossier_courant
    setenv("OLDPWD", getenv("PWD"), 1);
    // memset(oldPath, 0, MAX_ARGS_NUMBER);
    // strcpy(oldPath, dossier_courant);
    // oldPath[strlen(dossier_courant)] = '\0';

    // On vide la chaine de caractere dossier_courant et on lui donne la valeur de path
    setenv("PWD", realpath, 1);
    // memset(dossier_courant, 0, MAX_ARGS_NUMBER);
    // strcpy(dossier_courant, realpath);
    // dossier_courant[strlen(realpath)] = '\0';

    free(realpath);
    return 0;
}


int cd (char *option, char *ref){
    char *l = "-L"; // Represente l'option -L
    char *p = "-P"; // Represente l'option -P
    char dest[MAX_ARGS_NUMBER] = "";
    char *pathname = getenv("PWD");


    if(ref != NULL)
    {   
        // Si la ref est un chemin relatif
        if(ref[0] == '/'){
            if(snprintf(dest, strlen(ref)+1, "%s", ref) < 0) {
                perror("sprintf erreur ");
                exit(1);
            }
            dest[strlen(ref)] = '\0';
        }

        // Si la ref est "-" dest vaut oldpath 
        else if(strcmp(ref, "-") == 0)
        {   
            char *oldPath = getenv("OLDPWD");
            if(snprintf(dest, strlen(oldPath)+1, "%s", oldPath) < 0) {
                perror("sprintf erreur");
                exit(1);
            }
            dest[strlen(oldPath)] = '\0';
            
        }
        else
        {
            if(snprintf(dest, strlen(pathname)+1,  "%s", pathname) < 0) {
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
        if(snprintf(dest, strlen(home)+1, "%s", home) < 0) {perror("sprintf erreur ");exit(1);} 
        dest[strlen(home)] = '\0';
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

