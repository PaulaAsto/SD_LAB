#include <stdio.h>
#include "circle.h"

main(int argc, char *argv[])
{
   int radio;
   double cir;
   double are;

   if (argc !=2 ) {
     fprintf(stderr,"Error: %s a \n",argv[0]);
     exit(1);
   }

   a = atoi(argv[1]);

   cir = circunferencia(radio);
   are = area(radio);

   printf("La circunferencia del círculo es %d \n",per);      	   
   printf("El area del circulo es %d \n",sup);

}
