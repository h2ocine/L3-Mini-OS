/*
    Ce header contrient tout les méthodes en lien avec l'affichage du projet
*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ARGS_NUMBER 4096

// Affiche le prompt avec la valeur de retour last_exit et le retourne dans le second paramètre
void affichage_prompt(int last_exit, char* prompt);

void formatage_couleur(int last_exit, char *prompt, char *prompt_exit);

char *truncate_prompt(char *prompt, int max_size);	