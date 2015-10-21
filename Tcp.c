#include "Tcp.h"
//#define FORK

void source_tcp(unsigned int long_message, int nb_message, const char * dest, int port)
{
    //        SOURCE

    //creation du socket
    int sock=socket(AF_INET, SOCK_STREAM,0);
    if(sock==-1){
        printf("Erreur à la création du socket.\n");
        exit(1);
    }

    //-Adresse du socket auquel il souhaite s'adresser
    struct sockaddr_in adr_distant;
    memset((char*)&adr_distant, 0, sizeof(adr_distant));
    adr_distant.sin_family=AF_INET;
    adr_distant.sin_port=htons(port);

    //Recuperation de l'adresse destinataire
    struct hostent * hp = gethostbyname(dest);
    if(hp==NULL){
        printf("Erreur à la résolution du destinataire");
        exit(1);
    }
    memcpy((char*)&adr_distant.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);
    //-Bind(son adresse) --Optionnel


    //-Demande de connexion connect()
    while(connect(sock,(struct sockaddr*)&adr_distant,sizeof(adr_distant))==-1)
    {
        printf("Erreur du connect.\n");
        sleep(1);
    }

    //DIALOGUE -echec = retour arriere / succes = Dialogue write/read send/recv

        //initialisation du buffer
        int send_len;
        char buff[long_message+1];//+1 pour le '\0' (pour l'affichage)
        memcpy((char*)&adr_distant.sin_addr.s_addr, hp->h_addr_list[0], hp->h_length);

        for(int i = 0; i<nb_message || nb_message==-1;i++){
                //generation de la chaine a envoyer
                sprintf(buff,"%5d",i+1);
                memset((char*)(buff+5),(char)(97+(i%26)),long_message-5);
                buff[long_message]='\0';//ajout pour l'affichage
                //envoi
                send_len = write(sock,buff,long_message);
                printf("SOURCE : Envoi n°%d (%d) [%s]\n",i+1,send_len,buff);
        }
        printf("SOURCE : fin\n");



    //fermeture du socket
    if(close(sock)==-1){
        printf("Erreur à la destruction du socket.\n");
        exit(1);
    }




}


void puit_tcp(unsigned int long_message, int nb_message, int port)
{



    //         PUITS
    //-Creer un socket
    int sock=socket(AF_INET, SOCK_STREAM,0);
    if(sock==-1){
        printf("Erreur à la création du socket.\n");
        exit(1);
    }

    //-Adresse de ce socket
    struct sockaddr_in adr_local;
    memset((char*)&adr_local, 0, sizeof(adr_local));
    adr_local.sin_family=AF_INET;
    adr_local.sin_port=htons(port);
    adr_local.sin_addr.s_addr=INADDR_ANY;
    
    //-Bind()
    //liaison socket <-> adresse locale
    if(bind(sock, (struct sockaddr *)&adr_local, sizeof(adr_local))==-1){
        printf("Erreur au bind.\n");
        exit(1);
    }
    
    //-Listen() --Dimensionnement de la file d'attente
    listen(sock,5);
    
    //-Se mettre en Accept()

    struct sockaddr_in adr_client;
    socklen_t adr_len = sizeof(adr_client) ;
    int clientSock;
    int lg_rec;
    
    #ifndef FORK
  
    if((clientSock = accept(sock,(struct sockaddr*)&adr_client,&adr_len) )==-1){
        printf("Echec du accept.\n");
    }
   
    //-Then   --PAGE 41--
    //      *Traiter soit même
  
      char buffer[long_message];    
      for(int i = 0; i<nb_message || nb_message==-1;i++){
        //on met le buffer a 0
        memset(buffer,0,long_message);
        //recuperation des messages
        lg_rec = read(clientSock,buffer,long_message);
        if(lg_rec<0){
            printf("Echec à la lecture.");
            exit(1);
        }
        else if(lg_rec==0)
           break;
        printf("PUITS : Réception n°%d (%d) [%s]\n",i+1,(int)lg_rec,buffer);

    }
    #else
    
    
    
    
    //Creer un processus fils puis retourner en etat d'attente
    char buffer[long_message];    
    while(1){///
        if((clientSock = accept(sock,(struct sockaddr*)&adr_client,&adr_len) )==-1){
            printf("Echec du accept.\n");
        }
        
        ///FORK///
        switch(fork())
        {
            case -1:
                printf("Erreur fork");
                exit(1);
            case 0: //processus fils
                close(sock); // fermeture du socket proc. père
      			for(int i = 0; i<nb_message || nb_message==-1;i++){
					//on met le buffer a 0
					memset(buffer,0,long_message);
					//recuperation des messages
					lg_rec = read(clientSock,buffer,long_message);
					if(lg_rec<0){
						printf("Echec à la lecture.");
						exit(1);
					}
					else if(lg_rec==0)
					   break;
					printf("PUITS : Réception n°%d (%d) [%s]\n",i+1,(int)lg_rec,buffer);
    			}
                exit(0);
            default:
                close(clientSock); // fermeture du sock client.
        }
        ///FORK///
    
    
    }

    #endif
    //-close
    if(close(clientSock)==-1)
    {
      printf("Erreur à la destruction du socket.\n");
      exit(1);
    }
    
    if(close(sock)==-1){
        printf("Erreur à la destruction du socket.\n");
        exit(1);
    }

}
