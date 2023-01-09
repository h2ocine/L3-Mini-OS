#include "../header/signal.h"

void sig_handler(int signum) {
    printf("SIG\n");
    exit(0);
}

//fonction de gestion des signeaux
//boolean = 0 -> ignorer SIGINT et SIGTERM
//boolean = 1 -> action par défault
void gestion_signeaux(int boolean){
    
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    
    if(boolean) 
        action.sa_handler = SIG_DFL;
    else 
        action.sa_handler = SIG_IGN;

    if (sigaction(SIGINT, &action, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }
    if (sigaction(SIGTERM, &action, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }
}

void gestion_signeaux_fonctions_externes(){
    
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    
    action.sa_handler = sig_handler;

    if (sigaction(SIGINT, &action, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }
    if (sigaction(SIGTERM, &action, NULL) < 0) {
        perror("sigaction");
        exit(1);
    }
}


