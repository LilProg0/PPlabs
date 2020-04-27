#define _CRT_SECURE_NO_WARNINGS
#include "mpi.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define M 10

void main(int argc, char* argv[])
{
	int* mainVECT;
	int* allVECT;

	int size, myid;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int namelen;
	int status;
	double startwtime, endwtime;

	MPI_Init(&argc, &argv);
	/* Получаем общее количество запущенных процессов */
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	/* Получаем порядковый номер текущего процесса */
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Get_processor_name(processor_name, &namelen);


	int m = M;
	if (M < size)
		m = size;
	else if ((M % size))
		m += size - (M % size);

	if (myid == 0)
		startwtime = MPI_Wtime();
	srand(myid);

	mainVECT = (int*)malloc(m * sizeof(int));
	for (int i = 0; i < m; ++i)
		mainVECT[i] = m * myid + i;

	allVECT = (int*)malloc(m * sizeof(int));
	printf("Process %d: ", myid);

	for (int j = 0; j < m; ++j)
		printf(" %d", mainVECT[j]);
	printf("\n");
	MPI_Alltoall(
		mainVECT, m / size, MPI_INT,
		allVECT, m / size, MPI_INT,
		MPI_COMM_WORLD);
	printf("Process %d:\t\t\t", myid);

	for (int j = 0; j < m; ++j)
		printf(" %d", allVECT[j]);
	printf("\n");

	MPI_Barrier(MPI_COMM_WORLD);

	if (myid == 0) {
		endwtime = MPI_Wtime();
		printf("Wall clock time = %f\n", endwtime - startwtime);
	}
	free(mainVECT);
	free(allVECT);
	MPI_Finalize();
}
