#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int main(int argc, char *argv[])
{
   int   pet;                   // mensaje de peticion
   char  resp;                  // mensaje de respuesta
   int   sd;                    // descriptor del socket
   struct sockaddr_un   sin; 	// direcciones socket

   if (argc != 2) { //Verifica parametros de entrada
      fprintf(stderr,"Error, uso: %s <numero> \n",argv[0]);
      exit(1);
   }

   strcpy(sin.sun_path, "./socket"); //Asigna valores a la dirección
   sin.sun_family = AF_UNIX;

   if ( (sd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) { //Obtiene socket
      perror("socket");
      exit(1);
   }

   if ( connect(sd, (struct sockaddr *)&sin, sizeof(sin)) == -1) { //Conecta al archivo
      perror("connect");
      exit(1);
   }

   pet = atoi(argv[1]); //Envia mensaje al servidor
   if ( write(sd, &pet, sizeof(pet)) == -1) {
      perror("write");
      exit(1);
   }

   if ( read(sd, &resp, sizeof(resp)) == -1) { //Espera por la respuesta
      perror("read");
      exit(1);
   }

   printf("El numero %d es ",pet ); //Imprime los resultados
   if (resp == 'i') printf("impar \n");
    else printf("par \n");

   close(sd); //Cierra socket

   return 0; 
}

