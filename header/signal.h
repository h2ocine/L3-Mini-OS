/* 
    Ce header rassemble les methodes qui traitent les signeaux dans ce projet
*/
#include <signal.h>
#include <stdlib.h>

//int valeur_de_retour_SIG = 0;

//fonction vide pour ignorer les signeaux
void sig_ignor_handler(int signum);

//handler qui ajoute au prompt la chaîne "[SIG]" en lieu et place de la valeur de retour
void sig_val_retour_handler(int signum);

//fonction pour definir les handler pour ignorer les signeaux 
void ignorer_signeaux();

//fonction pour definir les handler pour ajouter la valeur de retour SIG
void valeur_de_retour_SIG();