#include "Udp.h"
#include "Tcp.h"







int main (int argc, char **argv)
{
    int c;
    extern char *optarg;
    extern int optind;
    int nb_message = -1; /* Nb de messages Ã  envoyer ou Ã  recevoir, par dÃ©faut : 10 en Ã©mission, infini en rÃ©ception */
    unsigned int long_message = 30;
    int mode = MODE_TCP;

    int source = -1 ; /* 0=puits, 1=source */
    int port = 0;
    char * dest;
    while ((c = getopt(argc, argv, "psul:n:")) != -1) {
        switch (c) {
        case 'p':
            if (source == 1) {
                printf("usage: cmd [-p|-s][-n ##]A\n");
                exit(1);
            }

            source = 0;
            break;

        case 's':
            if (source == 0) {
                printf("usage: cmd [-p|-s][-n ##]B\n");
                exit(1) ;
            }
            source = 1;
            dest = argv[argc-2];
            break;

        case 'n':
            nb_message = atoi(optarg);
            break;

        case 'l':
            long_message= atoi(optarg);
            if(long_message<6){
                printf("Longueur des message insuffisante."
                " (Veuillez s'il vous plait recommencer avec une valeur supérieure à 5."
                "Respectueusement.");
                exit(1);
            }
            break;
        case 'u':
            mode = MODE_UDP;
            break;
        default:

            printf("usage: cmd [-p|-s][-n ##], %c\n",c);
            break;
        }
    }

    //Recuperation du port
    port = atoi(argv[argc-1]);


    if (source == -1) {
        printf("usage: cmd [-p|-s][-n ##]\n");
        exit(1) ;
    }

    if (source == 1)
    {
        printf("SOURCE : lg_mesg_emis=%d,   port=%d,  nb_envois=%d,  TP=%s dest=%s\n",long_message,port,nb_message,mode==MODE_TCP?"tcp":"udp",dest);
        if(mode==MODE_UDP)
            source_udp(long_message, nb_message, dest, port);
        if(mode==MODE_TCP)
            source_tcp(long_message, nb_message, dest, port);
    }else{

        char numberofmessage[5];
        sprintf(numberofmessage,"%d",nb_message);
        printf("PUIT : lg_mesg_lu=%d,   port=%d,  nb_reception=%s,  TP=%s\n", long_message, port, nb_message==-1 ?"infini": numberofmessage ,mode==MODE_TCP?"tcp":"udp");
        if(mode==MODE_UDP)
            puit_udp(long_message, nb_message, port);
        if(mode==MODE_TCP)
            puit_tcp(long_message, nb_message, port);

    }
    
    
    return 0;
}


