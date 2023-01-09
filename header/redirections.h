/*
    Ce header contient toute les méthodes des redirections
*/

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <string.h>

int compte_nombre_pipe(char **tab_clean, int taille_tabclean);

int is_redirection(char *tab);

int verif_redirection(char **tab, int taille);

int verif_executable(char **tab, int taille);

int check_cmd_intern(char **tab, int taille);

//Fonction qui "explode" le tableau de la ligne mais retire ce qui suit par exemple [ls,-l,>,fic.txt] -> [ls,-l]
char **explode_redirection(char **tab, int taille);

void exec_interne(char **tab, int *last_exit, int taille);

void redirect_cmd_interne(char **tab, int taille, int *last_exit, char *fic, char *typeredirection);

//fonction qui trouve les bons flags et le bon descripteur pour le type de redirection
void good_flags_descriptor(char *typeredirection, int *flags, int *descriptor);

//fonction qui applique la rediction 
void redirect_cmd_externe(char *cmd, char **args, int *last_exit, char *fic, char *typeredirection);

//fonction qui decoupe la commande et ses arguments, semblable à quelque chose pret à celle de cmdexterne.
void exec_externe(char **tab, int taille, int *last_exit, char *fic, char *typeredirection);

int dedans(const char **s, int taille, char *elem);

char **getTab_Cmd(char **tab, int taille, const char **s, int tailleS, int *t);

char **getTab_args(char **tab, int taille, const char **s, int tailleS, int *t);

char **get_tabredirection(char **tab, int taille, int *tailleredirec);

//fonction qui check quel type de redirection
int new_check_redirection(char **tab, int taille, int *last_exit);




