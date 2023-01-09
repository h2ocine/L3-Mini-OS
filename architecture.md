Nous avons divisé le projet en plusieurs fichiers :

Chaque grande fonctionnalité est concernée par deux fichiers un .c et un header

Les fichiers .c sont dans le dossier Main et les header dans le dossier Header, compte a slash.c il est en dehors de ces deux derniers.


Voici ce que contiennent les fichiers :

-cdaux.c : comporte toutes les méthodes auxiliaires à la commande cd;

-cdext.c : comporte toute les méthodes concernant les commandes externes.

-cdint.c : comporte toutes les méthodes en lien à l'exécution de commandes internes.

-joker.c : rassemble toutes les méthodes qui concernent le joker

-pwdaux.c : contient toutes les méthodes auxiliaires à pwd

-prompt.c : rassemble toutes les méthodes qui traite ou manipule le prompt

-redirections.c : contient toutes les méthodes auxiliaires à redirections

-signal.h : rassemble les méthodes qui traitent les signaux dans ce projet

-util.h : rassemble toutes les méthodes dites 'utile' pour notre projet