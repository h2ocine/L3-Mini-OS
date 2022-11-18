#include "fonctions.c"

/*
    Cette fonction raccourcis si il le faut la chaine prompt à 30 caractères 
*/
char *truncate_prompt(char *prompt, int max_size){
    int size = strlen(prompt);
    if(size > max_size){
        char *res = malloc(max_size);
        if(res == NULL) perror("malloc");
        strcpy(res, "...");
        for(int i=3; i<max_size; i++){
            int ind = size - max_size + i;
            res[i] = prompt[ind];
        }
        return &res[0];
    }
    return prompt;

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
        if(!(s = malloc(strlen(strToken)))) 
            perror("malloc");
        if(snprintf(s, strlen(strToken) + 1, "%s", strToken) < 0)
        {
            perror("explode snprintf error ");
            exit(1);
        }

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
    char dossier_courant[MAX_ARGS_NUMBER]; 
    getcwd(dossier_courant, MAX_ARGS_NUMBER);

    char **tab;
    int last_exit = 0;
    rl_outstream = stderr;
    
    //Initialisation variable d'environnement 
    setenv("var_env","255",1);

    while(1){
        
        //Recupération du dosier prompt
        /*****************************************************************/
        /*****************************************************************/
        char prompt_exit[3];
        sprintf(prompt_exit, "%d", last_exit);

        char prompt[30];
        strcpy(prompt, "[");
        strcat(prompt, prompt_exit);
        strcat(prompt, "]");

        int max_size = 30 - (strlen(prompt_exit) + 2) - 2; // 30 - taille de l'affichage du exit ([0] = 3) - 2 (taille du dollar et espace)
        char *prompt_dir = truncate_prompt(dossier_courant, max_size);
        
        strcat(prompt, prompt_dir);
        free(prompt_dir);

        strcat(prompt, "$ ");

        /*****************************************************************/
        /*****************************************************************/

        //Lecture de la commande (On la récupére dans tab)
        /*****************************************************************/
        /*****************************************************************/

        
        char *ligne = readline(prompt);
        

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
                exits(tab[1]);
            }
            else if(strcmp("cd",tab[0])==0)
            {
                //break;
            }
            else if(strcmp("pwd",tab[0]) == 0)
            {
                pwd(taille,tab);
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

    return 0;
}