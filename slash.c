
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "slash.h"

/* COMMANDES INTERNES*/

DIR *cd (char *pathname, char *option, char *ref){
    return;
}

int exit(char *val){
    return 0;
}

char *pwd(char *arg){
    return NULL;
}

/*FONCTIONS AUXILIAIRES*/

char ** explode(char *ligne,char *delimeter){
    char *tab = malloc(sizeof(char)*strlen(ligne));

    char *mot = strtok(ligne,delimeter);
    int cmp = 0;
    while(mot != NULL){
        tab[cmp] = *mot;
        printf(" Voici un mot %s \n",mot);
        mot = strtok ( NULL, delimeter);
        cmp ++;
    }
    return &tab;

}

int main(void){
    DIR *dir;
    if((dir = opendir(".")) < 0) exit("1");

    char *ligne;
    ligne = malloc(MAX_ARGS_STRLEN);
    if(ligne == NULL) perror("malloc");

   

    while(1){
        char *pre = "$ ";
        // TODO: On affiche le prompt (invite de commande)
        if(write(1, pre, strlen(pre)) < 0) exit("1");

        // TODO: On lit la ligne (entré standart) et on la stocke dans ligne
        if(read(0, ligne, MAX_ARGS_STRLEN) < 0) exit("1");

        // TODO: On utilise readline pour simplifier la lecture         
        ligne = readline(ligne);

        // TODO: On ajoute la dernière commande à l'historique

        add_history(ligne);
        
        // TODO: On transforme la ligne en tableau
        char *delimiter = " ";

        //Ici on recupere un tableau via la fonction explode qui découpe la ligne en mots 
        char **tab = explode(ligne,delimiter);
       

        //On traite notre tableau 
        
        if(strcmp("exit",&tab[0]) == 0){
            break;
        }else if(strcmp("cd",&tab[0])==0){
            break;
        }else if(strcmp("pwd",&tab[0]) == 0){
            break;
        }else{
            //Faire la commande externe .
        }
    }

    free(ligne);
    closedir(dir);

    return 0;
}