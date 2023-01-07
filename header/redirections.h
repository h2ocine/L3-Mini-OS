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
int check_redirection(char **tab,int taille,int *last_exit);

//Fonction qui "explode" le tableau de la ligne mais retire ce qui suit par exemple [ls,-l,>,fic.txt] -> [ls,-l]
char **explode_redirection(char **tab, int taille);

//fonction qui decoupe la commande et ses arguments, semblable à quelque chose pret à celle de cmdexterne.
void decoupe_commande(char **tab, int taille, int *last_exit, char *fic, char *typeredirection);

char **explode_redirection(char **tab, int taille);
//fonction qui permet de savoir quelle fonction faire 
void whichExec(char *cmd, char **args, int *last_exit, char *typeredirection,char *fic);

//fonction pour la redirection sortie standard vers un fichier , ex ls -l > test.txt
void redirect_cmd_output(char *cmd, char **args, int *last_exit, char *fic);

