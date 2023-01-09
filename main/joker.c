#include "../header/joker.h"
#include "../header/cmdext.h"

char **trans(char *path, char *input, int *taille){
    char **res = NULL;
    int size_res = 0;

    // On est dans le car où l'input est "normal" (càd sans joker). Dans ce cas on renvoie un tableau avec pour seul element l'input
    if (strchr(input, '*') == NULL)
    {
        res = malloc(sizeof(char *));

        res[0] = malloc(strlen(input) + 1);
        strncpy(res[0], input, strlen(input)+1);

        *taille = 1;
        return res;
    }

    // On separe l'input par rapport au charactere '*' pour avoir si ils existent le pre (la chaine avant l'étoile; ex: test* -> test) et le post (la chaine après l'étoile; ex: *.c -> .c)
    int size_tab;
    char **tab_pre_post = explode(input, "*", &size_tab);


    // cas où input = "*" on renvoie tout les fichiers dans path
    if (size_tab == 0)
    {   
        res =  all_fic(path, &size_res);
    }

    // cas où l'on a soit un pre et pas un post ou un post et pas un pre
    else if (size_tab != 2)
    {   
        int size_tab0 = strlen(tab_pre_post[0]);

        // On ne sait pas encore si c'est un pre ou un post
        char *pre_ou_post = malloc(size_tab0 + 1);
        strncpy(pre_ou_post, tab_pre_post[0], size_tab0+1);

        // Si l'input commence par une étoile on sait qu'on a un post et donc on renvoie tout les fichers finissant par post dans path
        if (input[0] == '*') res =  end_with(path, pre_ou_post, &size_res);
        
        // Sinon on a un pre et donc on renvoie tout les fichiers commancant par pre dans path
        else  res = begin_with(path, pre_ou_post, &size_res);

        free(pre_ou_post);
    }

    // Cas où on a un pre un post
    else
    {   
        int size_tab0 = strlen(tab_pre_post[0]);
        int size_tab1 = strlen(tab_pre_post[1]);

        // On copie le pre
        char *pre = malloc(size_tab0 + 1);
        strncpy(pre, tab_pre_post[0], size_tab0+1);

        // On copie le post
        char *post = malloc(size_tab1 + 1); 
        strncpy(post, tab_pre_post[1], size_tab1+1);

        res =  begin_end_with(path, pre, post, &size_res);
        
        free(pre);
        free(post);
    }

    *taille = size_res;
    free_StingArrayArray(tab_pre_post,size_tab);
    return res;
}

char **all_joker_fic(char *input, char *dos, int *t){

    // On commance par separer les jokers pour les traiter un à un; "*/*.c" -> [*, *.c]
    int size_tab;
    char *cpyInput = malloc(strlen(input)+1);
    strncpy(cpyInput, input, strlen(input)+1);

    char **tab = explode(cpyInput, "/", &size_tab);

    char *new_tab0;
    if(input[0] == '/' && size_tab > 0){
        size_t len_tab0 = strlen(tab[0]);
        new_tab0 = malloc(1 /* taille du '/' */ + len_tab0 + 1);
        new_tab0[0]= '/';
        new_tab0[1]= '\0';
        strncat(new_tab0, tab[0], len_tab0);
        

    }else{
        new_tab0 = malloc(strlen(tab[0])+1);
        strncpy(new_tab0, tab[0], strlen(tab[0])+1);
    }
    free(cpyInput);

    
    // On cree une copie du dossier et on lui ajoute un slash à la fin si il n'y en a pas
    char *cpyDos = with_slash(dos);

    if(strlen(dos) == 0) dos = "./";

    // On chercher toute les possibilites avec l'input dans dos
    int size_pos = 0;
    char **possibilite = trans(dos, new_tab0, &size_pos);

    char **res = NULL;
    int size_res = 0;

    // Cas où l'on a plus un chemin de joker (ex: */*) mais juste un joker; ex: *, *.c, a* ... 
    if(size_tab == 1){
        size_res = size_pos;
        res = add_start(possibilite, size_res, cpyDos);
        *t = size_pos;

        free_StingArrayArray(tab, size_tab);
        free_StingArrayArray(possibilite, size_pos);
        free(cpyDos);
        free(new_tab0);
        
        return res;
    }
    for (int i = 0; i < size_pos; i++)
    {   
        char *posI = possibilite[i];
        size_t size_posI = strlen(posI);
        
       
        // On cree un nouveau chemin qui est egale à cpyDos + posI
        char *dos_pos = malloc(strlen(cpyDos) + size_posI + 1);
        
        strncpy(dos_pos, cpyDos, strlen(cpyDos)+1);
        strncat(dos_pos, posI, size_posI);
        
        // On enleve la premier joker de l'input; */*a/*.c -> *a/*.c
        char *newInput = truncate_start(input, strlen(new_tab0)+1 );

        int size_jok; // nombre de possibilite
        char **jok = all_joker_fic(newInput, dos_pos, &size_jok);

        free(newInput);
        free(dos_pos);

        if(size_jok > 0){
            int size_cpyRes = size_res;
            char **cpyRes = copie_tab(res, size_res);
            free_StingArrayArray(res, size_res);
            res = cat_tabs(cpyRes, size_res, jok, size_jok, &size_res);

            free_StingArrayArray(cpyRes, size_cpyRes);
        }
        free_StingArrayArray(jok, size_jok);
    }

    free(cpyDos);
    free(new_tab0);
    free_StingArrayArray(tab, size_tab);
    free_StingArrayArray(possibilite, size_pos);
    *t = size_res;
    return res;
}



// char **doubleEtoile(char *dossierCourant, int *taille){
    
//     DIR *dir = opendir(dossierCourant);
//     if(dir == NULL) perror("opendir");
//     struct dirent *entry;

//     int tailleRes = 1;
//     char **res = malloc(sizeof(char *));
//     if(res == NULL) perror("malloc");

//     res[0] = malloc(strlen(dossierCourant)+1);
//     if(res[0] == NULL) perror("malloc");
    

//     snprintf(res[0], strlen(dossierCourant)+1, "%s", dossierCourant);
//     res[0][strlen(dossierCourant)] = '\0';

//     while((entry = readdir(dir))){
//         if(strcmp(entry -> d_name, ".") == 0 || strcmp(entry -> d_name, "..") == 0 || entry->d_name[0] == '.') continue;

//         char *path = transformeEnChemin(dossierCourant, entry->d_name);
//         struct stat st;
//         if(stat(path, &st) <0) perror("stat");

//         // On vérifie que l'entrée est un dossier et n'est pas un lien symbolique
//         if(S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode)){

//             char **cpyRes = copie_tab(res, tailleRes);
//             int tailleCpyRes = tailleRes;

//             int tailleRecherche;
//             char **recherche = doubleEtoile(path, &tailleRecherche);

//             free_StingArrayArray(res, tailleRes);

//             res = cat_tabs(cpyRes, tailleRes, recherche, tailleRecherche, &tailleRes);

//             free_StingArrayArray(cpyRes, tailleCpyRes);
//             free_StingArrayArray(recherche, tailleRecherche);

//         }
//     }
//     closedir(dir);

//     *taille = tailleRes;
//     return res;
// }

char **doubleEtoile(char *dossierCourant, int *taille){
    
    DIR *dir = opendir(dossierCourant);
    if(dir == NULL) perror("opendir");
    struct dirent *entry;

    int tailleRes = 0;
    char **res = NULL;

    while((entry = readdir(dir))){
        if(strcmp(entry -> d_name, ".") == 0 || strcmp(entry -> d_name, "..") == 0 || entry->d_name[0] == '.') continue;

        char *path = transformeEnChemin(dossierCourant, entry->d_name);

        struct stat st;
        if(stat(path, &st) <0) perror("stat");

        // On vérifie que l'entrée est un dossier et n'est pas un lien symbolique
        if(S_ISDIR(st.st_mode) && !S_ISLNK(st.st_mode)){
            char **cpyRes1 = copie_tab(res, tailleRes);
            int tailleCpyRes1 = tailleRes;

            free_StingArrayArray(res, tailleRes);
            res = push(cpyRes1, tailleCpyRes1, path);
            free_StingArrayArray(cpyRes1, tailleCpyRes1);
            tailleRes++;


            char **cpyRes = copie_tab(res, tailleRes);
            int tailleCpyRes = tailleRes;

            int tailleRecherche;
            char **recherche = doubleEtoile(path, &tailleRecherche);

            free_StingArrayArray(res, tailleRes);

            res = cat_tabs(cpyRes, tailleRes, recherche, tailleRecherche, &tailleRes);

            free_StingArrayArray(cpyRes, tailleCpyRes);
            free_StingArrayArray(recherche, tailleRecherche);

        }
        free(path);
    }
    closedir(dir);

    *taille = tailleRes;
    return res;
}

char **doubleEtoileInit(int *taille){
    char **res = doubleEtoile("./", taille);

    // ON supprimer les "./" de toute les chaines de caractere
    char **cpyRes = copie_tab(res, *taille);
    free_StingArrayArray(res, *taille);

    res = delete_pre(cpyRes, *taille, "./");
    free_StingArrayArray(cpyRes, *taille);

    // // On ajoute un slash a la fin de toute les chaines de charactere
    // char **cpyRes2 = copie_tab(res, *taille);
    // free_StingArrayArray(res, *taille);

    // res = add_last(cpyRes2, *taille, "/");
    // free_StingArrayArray(cpyRes2, *taille);

    // affiche_mat(res, *taille);

    return res;
}

int isDoubleEtoile(char *input){
    char *doubleEtoile = "**/";
    if(strlen(input) < strlen(doubleEtoile)) return 0;
   
    char *troisPremierChar = malloc(4);
    snprintf(troisPremierChar, 4, "%s", input);
    troisPremierChar[3] = '\0';

    if(strcmp(troisPremierChar, doubleEtoile) != 0){
        free(troisPremierChar);
        return 0;
    }

    free(troisPremierChar);
    return 1;
}

char ** casDoubleEtoile(char *joker, char **input, int *size_input){
    char *separator = "/";

    int tailleSepar;
    char **separ = explode(joker, separator, &tailleSepar);
    
    int tailleDossiers;
    char **dossiers = doubleEtoileInit(&tailleDossiers);

    if(tailleSepar > 1){
        char *post = separ[1];
        char *postAvecSepar = malloc(1/* taille du '/' */ + strlen(post) + 1);
        postAvecSepar[0] = '/';
        postAvecSepar[1] = '\0';
        strncat(postAvecSepar, post, strlen(post));

        char **cpyDossiers = copie_tab(dossiers, tailleDossiers);

        free_StingArrayArray(dossiers, tailleDossiers);
        dossiers = add_last(cpyDossiers, tailleDossiers, postAvecSepar);
        free_StingArrayArray(cpyDossiers, tailleDossiers);
        free(postAvecSepar);
    }
    char **cpyInput = copie_tab(input, *size_input);
    free_StingArrayArray(input, *size_input);

    char **res = cat_tabs(cpyInput, *size_input, dossiers, tailleDossiers, size_input);

    free_StingArrayArray(separ, tailleSepar);
    free_StingArrayArray(dossiers, tailleDossiers);

    return res;
}

char **all(char **input, int size_input, char *dos, int *taille)
{   
    char **res = NULL;
    int size_res = 0;

    for (int i = 0; i < size_input; i++)
    {   
        if(isDoubleEtoile(input[i]) == 1){
            input = casDoubleEtoile(input[i], input, &size_input);
            continue;
        }
        
        char *input_propre = good_path(input[i]); 
    
        int size_jok;
        char **jok = all_joker_fic(input_propre, dos, &size_jok);
        
        char **cpy = malloc(size_res * sizeof(char *));
        int taille_cpy = 0;

        
        for (int j = 0; j < size_res; j++)
        {
            size_t taille_resj = strlen(res[j]);
            cpy[j] = malloc(taille_resj + 1);
            strncpy(cpy[j], res[j], taille_resj+1);
            taille_cpy++;
        }
        
        res = cat_tabs(cpy, size_res, jok, size_jok, &size_res);

        free(input_propre);
        free_StingArrayArray(cpy, taille_cpy);
        free_StingArrayArray(jok, size_jok);
        
    }
    *taille = size_res;
    
    return res;
}