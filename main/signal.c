#include "../header/signal.h"

//fonctions de gestion de signal (handler) qui s'executera a l'appelle de SIGINT et SIGTERM

//cas commandes internes : handler qui ignore le signal
void sig_ignor_handler(int signum) {
  // Afin d'ignorer le signal, on ne fait rien
  //write(1,"handlerPere\n",strlen("handlerPere\n"));
}

//cas commandes externes : handler qui ajoute au prompt la chaîne "[SIG]" en lieu et place de la valeur de retour
void sig_val_retour_handler(int signum) {
  //write(1,"HANDLER\n",strlen("HANDLER\n"));
}

//fonction pour definir les handler pour ignorer les signeaux 
void ignorer_signeaux(){
     struct sigaction act;

    act.sa_handler = sig_ignor_handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }
    if (sigaction(SIGTERM, &act, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }
}

//fonction pour definir les handler pour ajouter la valeur de retour SIG
void valeur_de_retour_SIG(){
    struct sigaction act;

    act.sa_handler = sig_val_retour_handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);

    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }
    if (sigaction(SIGTERM, &act, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }
}
