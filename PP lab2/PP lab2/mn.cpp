#define _CRT_SECURE_NO_WARNINGS
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#define M 30

void main(int argc, char* argv[])
{
	int size, myid;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int namelen;
	double startwtime, endwtime;

	MPI_Init(&argc, &argv);
	/* Получаем общее количество запущенных процессов */
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	/* Получаем порядковый номер текущего процесса */
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	MPI_Get_processor_name(processor_name, &namelen);

	if (myid == 0)
	{
		startwtime = MPI_Wtime();
	}

	long int sbuf[M];
	//printf("\tProcess %d:\n", myid);
	for (int i = 0; i < M; ++i) {
		sbuf[i] = M * myid + i;
		//printf("%d ", sbuf[i]);
	}
	long int* rbuf;
	rbuf = (long int*)malloc(M * sizeof(long int));
	//MPI_Barrier(MPI_COMM_WORLD);
	/* Выполнение BAND и возвращение результата в буфер главного процесса */
	MPI_Reduce(sbuf, rbuf, M, MPI_LONG, MPI_BAND, 0, MPI_COMM_WORLD);

	if (myid == 0)
	{
		printf("\n\nBAND: ");
		for (int i = 0; i < M; ++i)
			printf("%d  ", rbuf[i]);
		endwtime = MPI_Wtime();
		printf("\nWall clock time = %f\n", endwtime - startwtime);
	}
	//free(sbuf);
	free(rbuf);
	MPI_Finalize();
}
