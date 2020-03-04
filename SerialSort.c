/*
 * SerialSort.c
 *
 *  Created on: 01 lug 2018
 *      Author: rodolfo
 *
 *  Implementazione di SerialSort.h
 */
#include "SerialSort.h"


void swap(double *a, double *b){
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}

/*
 * FONTE :  https://en.wikibooks.org/wiki/Algorithm_Implementation/Sorting/Quicksort
 */
void serialIterativeQuickSort(double *array, long int size){
	unsigned seed = rand();
	long int left = 0, stack[MAX], pos = 0;
	for (;;) {
		for (; left + 1 < size; size++) {/* sort left to size-1 */
			if (pos == MAX)
				size = stack[pos = 0]; /* stack overflow, reset */
			double pivot = array[left + seed % (size - left)]; /* pick random pivot */
			seed = seed * 69069 + 1; /* next pseudorandom number */
			stack[pos++] = size;
			for (unsigned right = left - 1;;) { /* inner loop: partitioning */
				while (array[++right] < pivot) /* look for greater element */
					;
				while (pivot < array[--size])/* look for smaller element */
					;
				if (right >= size) /* partition point found? */
					break;
				swap(&array[right], &array[size]); /* the only swap */

			}
		}
		/* partitioned, continue left part */
		if (pos == 0) /* stack empty? */
			break;
		left = size; /* left to right is sorted */
		size = stack[--pos]; /* get next range to sort */
	}
}

void serialQuadraticSort(double *array, long int size) {
	for (long int i = 0; i < size; i++) {
		for (long int j = 0; j < size; j++) {
			if (array[i] < array[j])
				swap(&array[i], &array[j]);
		}
	}
}

void serialRecursiveQuickSort(double *array, long int in, long int fin) {
	if (fin - in < 20)
		serialQuadraticSort(array, fin - in); // servono almeo un numero maggiore di 20 elementi per ottenere buone prestazioni dal qucksort
	if (fin - in <= 0) {
		return;
	} else {
		long int pivot = array[fin];
		long int partitionPoint = recursiveQuickSortPartition(array, in, fin, pivot);
		serialRecursiveQuickSort(array, in, partitionPoint - 1);
		serialRecursiveQuickSort(array, partitionPoint + 1, fin);
	}
}

long int recursiveQuickSortPartition(double *array, long int in, long int fin, long int pivot) {
	int inPointer = in - 1;
	int finPointer = fin;

	while (1) {
		while (array[++inPointer] < pivot) {
			;
		}

		while (finPointer > 0 && array[--finPointer] > pivot) {
			// ciclo vuoto
		}

		if (inPointer >= finPointer) {
			break;
		} else {
			swap(&array[inPointer], &array[finPointer]);
		}
	}
	swap(&array[inPointer], &array[fin]);
	return inPointer;

}





