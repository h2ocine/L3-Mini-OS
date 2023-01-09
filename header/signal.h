/* 
    Ce header rassemble les methodes qui traitent les signeaux dans ce projet
*/
#include <signal.h>
#include <stdlib.h>

//fonction de gestion des signeaux
//boolean = 0 -> ignorer SIGINT et SIGTERM
//boolean = 1 -> action par défault
void gestion_signeaux(int boolean);

void gestion_signeaux_fonctions_externes();