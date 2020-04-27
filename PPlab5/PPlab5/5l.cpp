#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#define N 20
#define M 70

int main() {
	long int* mas;
	int i, chunk = 1;
	mas = (long int*)malloc(N * M * sizeof(long int));
	srand(time(NULL));
	for (i = 0; i < M * N; ++i)
		mas[i] = rand() % 99 + 1;

	printf("\tLine #1:\n");
	for (i = 0; i < M * N; ++i)
	{
		printf("%ld ", mas[i]);
		if (!((i + 1) % M)) {
			printf("\n");
			if (i + 1 != (M * N))
				printf("\tLine #%d:\n", (i / M) + 2);
		}
	}
	printf("\n");

#pragma omp parallel for shared(mas) private(i) schedule(static,chunk)
	for (i = 0; i < N; ++i)
		for (int k = 0; k < M - 1; ++k)
			for (int j = 0; j < M - k - 1; ++j)
				if (mas[M * i + j] > mas[M * i + j + 1]) {
					int tmp = mas[M * i + j];
					mas[M * i + j] = mas[M * i + j + 1];
					mas[M * i + j + 1] = tmp;
				}

	printf("After \n\tLine #1:\n");
	for (i = 0; i < M * N; ++i)
	{
		printf("%ld ", mas[i]);
		if (!((i + 1) % M)) {
			printf("\n");
			if (i + 1 != (M * N))
				printf("\tLine #%d:\n", (i / M) + 2);
		}
	}
	free(mas);
	return 0;
}