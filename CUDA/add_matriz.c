#include <stdio.h>
#define N 16

__global__ void matrixMultGPU(int *a, int *b, int*c)
{
	int k, sum = 0;
	int col = threadIdx.x + blockDim.x * blockIdx.x;
	int fil = threadIdx.y + blockDim.y * blockIdx.y;
	
	if(col < N && fil < N)
	{
		for(k = 0; k < N; k++)
			sum += a[fil * N + k] * b[k * N + col];
		c[fil * N + col] = sum;
	}
}

int main()
{
	int a[N][N], b[N][N], c[N][N];
	int *dev_a, *dev_b, *dev_c;
	int cont,i,j;
	
	for(i = 0; i < N; i++)	//Rellenando matriz
	{
		cont = 0;
		for(j = 0; j < N; j++)
		{
			a[i][j] = cont;
			a[i][j] = cont;
			cont++;
		}
	}

	int size = N * N * sizeof(int);

	cudaMalloc((void **) &dev_a, size);
	cudaMalloc((void **) &dev_b, size);
	cudaMalloc((void **) &dev_c, size);

	cudaMemcpy(dev_a, a, size, cudaMencpyHostToDevice);
	cudaMemcpy(dev_b, b, size, cudaMencpyHostToDevice);

	dim3 dimGrid(1,1);
	dim3 dimBlock(N,N);

	matrixMultGPU<<dimGrid, dimBlock>> (dev_a, dev_b, dev_c);
	
	cudaMemcpy(c, dev_c, size, cudaMemcpyDeviceToHost);

	cudaFree(dev_a);
	cudaFree(dev_b);
	cudaFree(dev_c);
	
	int x,y;
	for(x = 0; x < N; x++)
	{
		for(y = 0; y < N; y++)
			printf("[%d][%d]=%d ", x, y, c[x][y]);
		printf("\n");
	}
	return 0;
}
