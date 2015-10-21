#include "Heads.h"


void source_tcp(unsigned int long_message, int nb_message, const char * dest, int port);


void puit_tcp(unsigned int long_message, int nb_message, int port);


/*
Obligation d'etablir une connexion
Garantie d'ordre et fiab
Pas de preserv limite messages

--Socket type SOCK_STREAM--

         SOURCE
-Create socket()
-Adresse du socket auquel il souhaite s'adresser
-Bind(son adresse) --Optionnel
-Demande de connexion connect()
-echec = retour arriere / succes = Dialogue write/read send/recv

         PUITS
-Creer un socket
-Adresse de ce socket
-Bind()
-Listen() --Dimensionnement de la file d'attente
-Se mettre en Accept()
-Then 
      *Traiter soit même
      OR
      *Creer un processus fils puis retourner en etat d'attente
-write/read     send/recv
-close

*/
