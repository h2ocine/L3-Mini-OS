/*
    Ce header contient toute les méthodes auxiliaire à redirections
*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>


//fonction qui check quel type de redirection
int new_check_redirection(char **tab, int taille,int *last_exit);

int check_redirection(char **tab, int taille,int *last_exit);

//Fonction qui "explode" le tableau de la ligne mais retire ce qui suit par exemple [ls,-l,>,fic.txt] -> [ls,-l]
char **explode_redirection(char **tab, int taille);

//fonction qui decoupe la commande et ses arguments, semblable à quelque chose pret à celle de cmdexterne.
void exec_externe(char **tab, int taille, int *last_exit, char *fic, char *typeredirection);

//fonction qui applique la rediction 
void redirect_cmd_externe(char *cmd, char **args, int *last_exit, char *fic, char *typeredirection);

//fonction qui trouve les bons flags et le bon descripteur pour le type de redirection
void good_flags_descriptor(char *typeredirection, int *flags, int *descriptor);
