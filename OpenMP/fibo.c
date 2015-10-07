
/*********************************************************
fibo.c
Ejemplo de uso de la directiva task en un programa
recursivo para calcular el numero de fibonacci
(version paralela con sleep)
**********************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#ifdef _OPENMP
#include <omp.h>
#define TRUE 1
#define FALSE 0
#else
#define omp_get_thread_num() 0
#define omp_get_num_threads() 1
#endif
long fibonacci(int n)
{
	long fn1, fn2, fn;
	usleep(1);
	if ( n == 0 || n == 1 ) return(1);
	// if ( n < 30 ) return(fibonacci(n-1) + fibonacci(n-2));
	#pragma omp task shared(fn1)
	fn1 = fibonacci(n-1);
	#pragma omp task shared(fn2)
	fn2 = fibonacci(n-2);
	#pragma omp taskwait
	fn = fn1 + fn2;
	return(fn);
}
main ()
{
	struct timeval t0, t1;
	double tej;
	int nthr=0;
	int n;
	long resul;
	printf("\n Ingrese numero a calcular_ ");
	scanf("%d", &n);
	gettimeofday(&t0, 0);
	#pragma omp parallel shared (resul)
	{
		#pragma omp single
		{
			resul = fibonacci(n);
		}
	}
	gettimeofday(&t1, 0);
	printf ("\nEl numero Fibonacci de %5d es %ld", n, resul);
	tej = (t1.tv_sec - t0.tv_sec) + (t1.tv_usec - t0.tv_usec) / 1e6;
	printf("\n T. de ejec. = %1.3f ms \n\n", tej*1000);
}

