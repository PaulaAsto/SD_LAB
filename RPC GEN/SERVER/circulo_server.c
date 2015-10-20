#include "circulo.h"

double *
circunferencia_1_svc(int *argp, struct svc_req *rqstp)
{
	static double  result;

	result = 2 * 3.14 * (*argp); 

	return &result;
}

double *
area_1_svc(int *argp, struct svc_req *rqstp)
{
	static double  result;

        result = 3.14 * ((*argp) * (*argp));
	
	return &result;
}
