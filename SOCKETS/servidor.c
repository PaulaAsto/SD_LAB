#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

int     pet;                  //parametro de entrada
char    resp;                    //parametro de salida
int     sd,   sd_actual;	 //descriptores de sockets
struct  sockaddr_un   sind; 	 //direcciones socket

void aborta()
{
   printf("....Abortando el proceso servidor \n");
   close(sd);
   close(sd_actual);
   unlink("./socket");
   exit(1);
}

int main()
{
  signal(SIGINT, aborta); //Captura señal

  strcpy(sind.sun_path, "./socket"); //Asigna valores a la direccion
  sind.sun_family = AF_UNIX;

  if ( (sd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) { //Obtiene socket
	perror("socket");
	exit(1);
  }

  if ( bind(sd, (struct sockaddr *)&sind, sizeof(sind)) == -1 ) { //Asocia el socket al archivo ./socket
	perror("bind");
	exit(1);
  }

  if ( listen(sd, 5) == -1 ) {//Escucha a traves del socket
	perror("listen");
	exit(1);
  }

  while (1) { //Espera al cliente
    if ( (sd_actual = accept(sd, 0, 0)) == -1) {
	perror("accept");
	exit(1);
    }

    if ( read(sd_actual, &pet,  sizeof(pet)) == -1) { //Toma valores del cliente
       perror("read");
       exit(1);
    }

    if ((pet % 2) == 0) resp='p'; //Realiza el servicio
      else resp='i';

    if ( write(sd_actual, &resp, sizeof(resp)) == -1) { //Envia respuesta
       perror("write");
       exit(1);
    }
 
    close(sd_actual);//Se cierra la comunicación
  }
  
  close(sd); //Se cierra canal de conexión
  unlink("./socket");

  return 0;
}
