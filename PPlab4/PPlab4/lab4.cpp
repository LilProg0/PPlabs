#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>
#define N 9

int main() {
	int num = 0;
	printf("Enter number of vectors\n");
	scanf("%d", &num);
	long int* mas, * min;
	int i, chunk = 1;
	mas = (long int*)malloc(N * num * sizeof(long int));
	min = (long int*)malloc(num * sizeof(long int));
	srand(time(NULL));
	for (i = 0; i < num * N; ++i)
		mas[i] = rand() % 99 + 1;

	printf("\nVector #1: ");
	for (i = 0; i < num * N; ++i)
	{
		printf("\t%ld", mas[i]);
		if (!((i + 1) % N)) {
			printf("\n");
			if (i + 1 != (num * N))
				printf("Vector #%d: ", (i / N) + 2);
		}
	}
	printf("\n");
#pragma omp parallel for shared(mas) private(i) schedule(static,chunk)
	for (i = 0; i < num; ++i) {
		min[i] = mas[N * i];
		for (int j = 1; j < N; ++j)
			if (mas[N * i + j] < min[i]) {
				min[i] = mas[N * i + j];
			}
	}

	for (i = 0; i < num; ++i)
		printf("MIN of vector %d = %d\n", i + 1, min[i]);

	free(mas);
	free(min);
	return 0;
}
