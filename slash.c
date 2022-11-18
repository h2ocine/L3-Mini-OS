#include "fonctions.c"


/*
    Libere la mémoire de toute les chaines de caractères presente dans s
*/
void free_StingArrayArray(char **s,int taille){
    for(int i = 0; i < taille ; i++)
        free(s[i]);
    
    free(s);
}

/*
 * Retourne un tableau de chaînes de caractères, chacune d'elle étant une sous-chaîne du paramètre str extraite en utilisant le séparateur separators
*/
char**  explode(char *str, const char *separators, int* taille){ 
    char** res  = malloc(0);
    if(res == NULL) perror("malloc");


    int i = 0;
    size_t size = 0;
    if(strlen(str) == 0)
    {
        *taille = 0;
        return NULL;
    } 
        

    char * strToken = strtok ( str, separators );
    while ( strToken != NULL ) {

        // On alloue dynamiquement un char * avec pour valeur strToken
        char *s = malloc(strlen(strToken));
        if(s == NULL) perror("malloc");
        sprintf(s, "%s", strToken);

        // On ajoute la chaine de caractere au tableau
        size += sizeof(char *);
        res = realloc(res, size);
        if(res == NULL) perror("realloc");
        res[i] = s;
        i++;

        // On demande le token suivant.
        strToken = strtok ( NULL, separators );
    }
    
    *taille = i;
    return res;
}

/*
    main
*/
int main(void){

    DIR *dir;
    if((dir = opendir(".")) < 0) exits("1");
    char **tab;
    rl_outstream = stderr;
   

    while(1){

        //Recupération du dosier prompt
        /*****************************************************************/
        /*****************************************************************/
        char prompt[MAX_ARGS_NUMBER];

        strcpy(prompt, getenv("USER")); //username
        strcat(prompt,"@"); //@
        strcat(prompt,getenv("NAME")); //deskeopname
        strcat(prompt,":"); //:
        strcat(prompt,getenv("PWD")); // /home/hocine/ + repertoire
        strcat(prompt, "$\0"); // $

        int t;
        //Supprimer /home/nom
        char** tabtmp = NULL;
        tabtmp = explode(prompt, "/", &t);
        char strcommande[MAX_ARGS_NUMBER];
        
        strcpy(strcommande,tabtmp[0]); 
        for(int i = 1; i<t;i++)
        {
            if(i!=1 && i!=2)//eviter /home/nom
            {
                strcat(strcommande,tabtmp[i]); 
                if(i != t-1)
                    strcat(strcommande,"/");
            }
            if(i == 1)
                strcat(strcommande,"~/");
        }
        free(tabtmp);
        /*****************************************************************/
        /*****************************************************************/

        //Lecture de la commande (On la récupére dans tab)
        /*****************************************************************/
        /*****************************************************************/
        char *ligne = readline(strcommande);

        //Cas du CTRL - D 
        if (ligne == NULL) {
            //On appelle exit sans paramètres 
            exits("NULL");
            
        } 

        // TODO: On ajoute la dernière commande à l'historique
        add_history(ligne);
        
        //On transforme la ligne en tableau (ici on recupere un tableau via la fonction explode qui découpe la ligne en mots)
        const char* delimiter = " ";
        int taille;
        tab = explode(ligne,delimiter, &taille);
        /*****************************************************************/
        /*****************************************************************/
        
        //Traitement de la commande
        /*****************************************************************/
        /*****************************************************************/
        if(taille == 0) 
            continue;
        
        //On traite notre tableau 
        if (taille > 0){

            if(strcmp("exit",tab[0]) == 0)
            {
                //break;
            }
            else if(strcmp("cd",tab[0])==0)
            {
                //break;
            }
            else if(strcmp("pwd",tab[0]) == 0)
            {
                //pwd(tab);
            }
            else
            {
                //Faire la commande externe .
            }
        
        }
        else
        {
            free(tab);
        }
        free_StingArrayArray(tab,taille);
        /*****************************************************************/
        /*****************************************************************/
    }
    closedir(dir);

    return 0;
}