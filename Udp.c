#include "Udp.h"


void source_udp(unsigned int long_message, int nb_message, const char * dest, int port){
    //creation du socket
    int sock=socket(AF_INET, SOCK_DGRAM,0);
    if(sock==-1){
        printf("Erreur à la création du socket.\n");
        exit(1);
    }

    //initialisation de l'adresse du socket
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
    //initialisation du buffer
    char buff[long_message+1];
    

    for(int i = 0; i<nb_message || nb_message==-1;i++){
        //generation de la chaine a envoyer
        sprintf(buff,"%5d",i+1);
        memset((char*)(buff+5),(char)(97+(i%26)),long_message-5);
        buff[long_message]='\0';
        //envoi
        sendto(sock,buff,long_message,0,(struct sockaddr *) &adr_distant, sizeof(adr_distant));
        printf("SOURCE : Envoi n°%d (%d) [%s]\n",i+1,long_message,buff);
    }
    printf("SOURCE : fin\n");

    //fermeture du socket
    if(close(sock)==-1){
        printf("Erreur à la destruction du socket\n");
        exit(1);
    }

}






void puit_udp(unsigned int long_message, int nb_message, int port){
    //creation du socket
    int sock=socket(AF_INET, SOCK_DGRAM,0);
    if(sock==-1){
        printf("Erreur à la création du socket.\n");
        exit(1);
    }

    //initialisation de l'adresse
    struct sockaddr_in adr_local;
    memset((char*)&adr_local, 0, sizeof(adr_local));
    adr_local.sin_family=AF_INET;
    adr_local.sin_port=htons(port);
    adr_local.sin_addr.s_addr=INADDR_ANY;

    //liaison socket <-> adresse locale
    if(bind(sock, (struct sockaddr *)&adr_local, sizeof(adr_local))==-1){
        printf("Erreur au bind.\n");
        exit(1);
    }


    char buff[long_message];//buffer de destination
    for(int i = 0; i<nb_message || nb_message==-1;i++){
        //on met le buffer a 0
        memset(buff,0,long_message);
        //recuperation des messages
        ssize_t read = recvfrom(sock,buff,long_message,0, NULL, NULL);
        printf("PUITS : Réception n°%d (%d) [%s]\n",i+1,(int)read,buff);
    }

    //fermeture du socket
    if(close(sock)==-1){
        printf("Erreur à la destruction du socket\n");
        exit(1);
    }

}
