#include "Heads.h"

void source_udp(unsigned int long_message, int nb_message, const char * dest, int port);

void puit_udp(unsigned int long_message, int nb_message, int port);



/*

--Socket de type SOCK_DGRAM--

         SOURCE
- Create Socket()
- Create Adress()  
- Use sendto()
   *Addr dest
   *Addr message
   *Message Length
-Close(socket)

         PUIT
- Create Socket()
- Create Adress() 
- Addr Bind()
- Use recvfrom()
   *Addr mess recep
   *Max length
   *Addr socket emett
   *Taille octets
   
*/   
