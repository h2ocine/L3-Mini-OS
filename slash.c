
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "slash.h"


DIR *cd (char *pathname, char *option, char *ref){
    return;
}

int exit(char *val){
    return 0;
}

char *pwd(char *arg){
    return;
}

int main(void){
    DIR *dir;
    if((dir = opendir(".")) < 0) exit(1);

    char *ligne;
    ligne = malloc(MAX_ARGS_STRLEN);
    if(ligne == NULL) perror("malloc");

   

    while(1){
        char *pre = "$ ";
        // TODO: On affiche le prompt (invite de commande)
        if(write(1, pre, strlen(pre)) < 0) exit(1);

        // TODO: On lit la ligne (entré standart) et on la stocke dans ligne
        if(read(0, ligne, MAX_ARGS_STRLEN) < 0) exit(1);

        // TODO: On utilise readline pour simplifier la lecture         
        ligne = readline(ligne);

        // TODO: On ajoute la dernière commande à l'historique
        

        // TODO: On transforme la ligne en tableau
        char *delimiter = " ";
        char tab[] = strtok(ligne, delimiter);
        
        switch (tab[0]){
            case "exit": break;
        }
    }

    free(ligne);

    return 0;
}