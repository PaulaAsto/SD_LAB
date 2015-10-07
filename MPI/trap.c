/* trap.c – Integración mediante trapecios.
Algoritmo:
1. Cada proceso se autoasigna su propio intervalo de integración.
2. Cada proceso integra f(x) en su propio intervalo con
el método de los trapecios.
3a. Cada proceso con rango != 0 envía su resultado al proceso 0.
3b. El proceso 0 suma los resultados de los cálculos realizados por los demás e imprime el resultado.
*/
#include <stdio.h>
#include "mpi.h"
/* Prototipo de la fu
nción que integra */
float Trap(float local_a, float local_b, int local_n, float h);
main(int argc, char *argv[]) {
int mi_rango; /* El rango de mi proceso */
int p; /* Número total de procesos */
float a = 0.0; /* Extremo izquierdo */
float b = 1.0; /* Extremo derecho */
int n = 1024; /* Número de trapecios */
float h; /* Base de cada trapecio */
float local_a; /* Extremo izdo. de mi proceso */
float local_b; /* Extermo dcho. de mi proceso */
int local_n; /* Número de trapecios para mi cálculo */
float integral; /* Resultado de la integral en mi intervalo */
float total; /* Integral total */
int fuente; /* Proceso que remite el resultado */
int dest = 0; /* Todos los resultados van al proceso 0 */
int tag = 0;
MPI_Status status;
MPI_Init(&argc, &argv);
/* Arrancamos MPI */
MPI_Comm_rank(MPI_COMM_WORLD, &mi_rango); /* Obtengo mi propio rango */
MPI_Comm_size(MPI_COMM_WORLD, &p); /* Obtengo nº total de procesos */
h = (b-a)/n; /* h es el m ismo para todos los procesos */
local_n = n/p; /* igual que el número de trapecios */
/* La longitud del inte rvalo de integración de cada proceso es igual alocal_n*h. Así pues, mi intervalo empieza y acaba en: */
local_a = a + mi_rango*local_n*h;
local_b = local_a + local_n*h;
/* Calculo la integral en mi intervalo */
integral = Trap(local_a,local_b, local_n, h);
if (mi_rango == 0) {
/* Sumo los cálculos de cada proceso, que me envían los demás */
total = integral;
for (fuente = 1; fuente < p; fuente++) {
MPI_Recv(&integral, 1,MPI_FLOAT, fuente, tag,MPI_COMM_WORLD, &status);
total = total + integral;
}
} else { /* Envío mi resultado al proceso 0 */
MPI_Send(&integral,
1, MPI_FLOAT, dest,
tag, MPI_COMM_WORLD);
}
if (mi_rango == 0) {
/* Imprimo resultados */
printf("Con n = %d trapecios, la integral de f(x) ", n);
printf("desde %f a %f= %f\n", a, b, total);
}
/* Cerrar MPI */
MPI_Finalize();
}
float Trap(float local_a, /*Í*/
float local_b, /*Í*/
int local_n, /*Í*/
float h) { /*Í*/
float integral; /* Para almacenar el resultado */
float x;
int i;
float f(float x); /* Función a integrar */
integral = (f(local_a) + f(local_b))/2.0;
x = local_a;
for (i = 1; i <= local_n-1; i++) {
x = x + h;
integral = integral + f(x);
}
integral = integral*h;
return integral;
}
/* Función que vamos a integrar. Metemos el código que corresponda a la función que deseamos integrar */
float f(float x) {
float return_val;
/* Calculamos f(x) y lo almacenamos en return_val */
return return_val;
}
