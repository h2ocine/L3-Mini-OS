#include "fonctions.c"



void formatage_couleur(int last_exit,char *prompt,char *prompt_exit){
    char *rouge = "\033[0;31m";
    char *vert = "\033[0;32m";
    char *cyan = "\033[36m";

    if(last_exit == 0)
    {
        strcpy(prompt,vert);
        strcat(prompt, "[");
        strcat(prompt, prompt_exit);
        strcat(prompt, "]");
        strcat(prompt,cyan);
        //Pas d'erreur à la derniere commande 
    }else if(last_exit == 1)
    {
        strcpy(prompt,rouge);
        strcat(prompt, "[");
        strcat(prompt, prompt_exit);
        strcat(prompt, "]");
        strcat(prompt,cyan);
    }
}
/*
    Cette fonction raccourcis si il le faut la chaine prompt à 30 caractères 
*/
char *truncate_prompt(char *prompt, int max_size){
    int size = strlen(prompt);
    char *res;
    if(size > max_size)
    {
        res = malloc(max_size+1);
        if(res == NULL) 
            perror("malloc");
        strcpy(res, "...");
        for(int i = 3; i < max_size; i++)
        {
            int ind = size - max_size + i;
            res[i] = prompt[ind];
        }
        res[max_size] = '\0';
        return &res[0];
    }
    else
    {
        res = malloc(size + 1);
        if(res == NULL) 
            perror("malloc");
        sprintf(res, "%s", prompt);
        res[size] = '\0';
    }
    return res;

}


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
char**  explode(char *str, const char *separators, int* taille)
{ 
    int i = 0;
    size_t size = 0;
    char* s = NULL;
    char** res  = malloc(0);
    if(res == NULL) 
        perror("malloc");

    //Cas chaine vide
    if(strlen(str) == 0)
    {
        *taille = 0;
        return NULL;
    } 

    
    

    //Séparer la chaine en plusieurs sous chaines :
    char * strToken = strtok (str, separators);

    while ( strToken != NULL ) 
    {
        
        // On copie strToken dans une chaine de caractère s (pour avoir utiliser la taille exact)
        if(!(s = malloc(strlen(strToken) + 1))) 
            perror("malloc");

        if(snprintf(s, strlen(strToken) + 1, "%s", strToken) < 0)
        {
            perror("explode snprintf error ");
            exit(1);
        }
        s[strlen(strToken)] = '\0';

        //On ajoute la chaine de caractere s au tableau res
        size += sizeof(char *);
        res = realloc(res, size);
        if(res == NULL) 
            perror("realloc");
        res[i] = s;
        i++;

        // On demande le token suivant.
        strToken = strtok ( NULL, separators );
    }
    
    
    if(!s)  
        free(s);
    free(strToken);

    *taille = i;//ici on retourne la taille de res
    return res;
}

/*
    main
*/
int main(void){

    char **tab;
    int last_exit = 0;
    rl_outstream = stderr;

    char *blanc = "\033[0m";
    #define TAILLE_PROMPT 47 //7 (rouge ou vert) + 5 (bleu) + 4 (blanc) + 1 ('\0')

    //Initialisation variable d'environnement 
    //setenv("var_env","255",1);
    while(1){

        /*****************************************************************/
        /*****************************************************************/
        //affichage du prompt :
        //---------------------
        //recupération du dossier courant 
        char dossier_courant[MAX_ARGS_NUMBER]; 
        if(!strcpy(dossier_courant,getenv("PWD")))   
        {
            perror("(main) - getenv - Erreur ");
            exit(1);
        }

        //recupération de la valeur de retour dans prompt_exit
        //[TODO] : Faire le cas ou la valeur de retour > 9; 
        char prompt_exit[2];
        prompt_exit[0] = last_exit + '0'; //recuperer le premier caractère
        prompt_exit[1] = '\0'; //fin de la chaine de la valeur de retoure

        //creation du prompt -> il a 30 caractère au maximum + (.)caractère de couleurs
        char prompt[TAILLE_PROMPT];

        //On ajoute la valeur de retour ([0] ou [1]) et s'occupe de la couleur du prompt
        formatage_couleur(last_exit,prompt,prompt_exit);

        //on supprime les carctère en trop du dossier courant et on les remplae par ... avec truncate_prompt
        int max_size = 30 - (strlen(prompt_exit) + 2 ) - 2; //on set le nombre de carctère max du chemin //-3 ([valeur retour]) -2 $ espace
        char *prompt_dir = truncate_prompt(dossier_courant, max_size);
        
        //on ajoute le dossier courant dans le prompt
        strcat(prompt, prompt_dir);
        free(prompt_dir);

        //On met la saisie d'utilisateur en blanc en on ajoute le dollar espace
        strcat(prompt,blanc);
        strcat(prompt, "$ \0");
        //prompt[strlrn] = '\0';

        /*****************************************************************/
        //Lecture de la commande (On la récupére dans tab)
        /*****************************************************************/

        //lecture de la commande et affichage de p
        
        char *ligne = readline(prompt);

        //Cas du CTRL - D 
        if (ligne == NULL) {
            //On appelle exit sans paramètres 
            exits(NULL,last_exit);
            
        } 

        // On ajoute la dernière commande à l'historique
        add_history(ligne);

        //On transforme la ligne en tableau (ici on recupere un tableau via la fonction explode qui découpe la ligne en mots)
        const char* delimiter = " ";
        int taille;

        tab = explode(ligne,delimiter, &taille); /***************--------------------------*********/

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
                //Pas besoin de la variable d'environnement ( voir avec adam)
                last_exit = exits(tab[1],last_exit);
            }
            else if(strcmp("cd",tab[0])==0)
            {
                //printf("ca rentre \n");
                //break;
            }
            else if(strcmp("pwd",tab[0]) == 0)
            {
                last_exit = pwd(taille,tab);
            }
            else
            {
                //last exit = 277
                //chercher les commandes externes
            }
        
        }
        else
        {
            free(tab);
        }
        free_StingArrayArray(tab,taille);


        /*---------------------------*/

    }

    return 0;
}