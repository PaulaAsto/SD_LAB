#include "circulo.h"

int
main (int argc, char *argv[])
{

  CLIENT *clnt;
  char *host;
  int a;
  double *cir;
  double *are;

  if (argc !=3 ) {
     fprintf(stderr,"Error: %s host a \n",argv[0]);
     exit(1);
  }

  a = atoi(argv[1]);

  clnt = clnt_create (host, CIRCULO_PROG, CIRCULO_VERS, "udp");
  if (clnt == NULL) {
     clnt_pcreateerror (host);
     exit (1);
  }

  cir = circunferencia_1(&a, clnt);
  if (cir == (int *) NULL) {
    clnt_perror (clnt, "call failed");
  }

  are = area_1(&a, clnt);
  if (are == (double *) NULL) {
    clnt_perror (clnt, "call failed");
  }

  printf("La circunferencia del círculo es %d \n",*cir);      
  printf("El area del circulo es %d \n",*are);

#ifndef	DEBUG
  clnt_destroy(clnt);
#endif	 /* DEBUG */

}

