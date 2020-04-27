#define _CRT_SECURE_NO_WARNINGS
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define M 3
#define N 10

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
	MPI_Status status;

	if (size < 2)
	{
		printf("Error: size < 2\n");
		MPI_Finalize();
		return;
	}

	if (myid == 0)
	{
		startwtime = MPI_Wtime();
		long int mas[N][M], buf[M];
		srand(time(NULL));
		for (int i = 0; i < N; ++i) {
			for (int j = 0; j < M; ++j) {
				mas[i][j] = rand() % 99 + 1;
				printf(" %d", mas[i][j]);
			}
			printf("\n");
		}
		printf("\n");

		int dest = 0, reverse = 0;
		for (int i = 0; i < N; ++i) {
			MPI_Send(mas[i], M, MPI_LONG, (dest % (size - 1)) + 1, dest, MPI_COMM_WORLD);
			dest++;
			if (size == 2)
			{
				MPI_Recv(buf, M, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
				for (int k = 0; k < M; ++k)
					mas[status.MPI_TAG][k] = buf[k];
			}
			else if ((!(dest % (size - 2)) ^ (dest == N - 1)))
			{
				for (int j = reverse; j < dest; ++j) {
					MPI_Recv(buf, M, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
					for (int k = 0; k < M; ++k)
						mas[status.MPI_TAG][k] = buf[k];
					reverse = dest;
				}
			}
		}
		printf("Sorted:\n");

		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++)
				printf("%d ", mas[i][j]);
			printf("\n");
		}
		endwtime = MPI_Wtime();
		printf("Wall clock time = %f\n", endwtime - startwtime);
	}
	else {
		long int mas[M];
		int l = 0;
		if ((myid - 1) < (N % (size - 1)))
			l++;
		for (int k = 0; k < (N / (size - 1)) + l; ++k) {
			MPI_Recv(mas, M, MPI_LONG, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			for (int i = 0; i < M - 1; ++i)
				for (int j = 0; j < M - i - 1; ++j)
					if (mas[j] < mas[j + 1]) {
						int tmp = mas[j];
						mas[j] = mas[j + 1];
						mas[j + 1] = tmp;
					}
			MPI_Send(mas, M, MPI_LONG, 0, status.MPI_TAG, MPI_COMM_WORLD);
		}
	}
	MPI_Finalize();
}
