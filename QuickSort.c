/*
 * QuickSort.c
 *
 *  Created on: 02 lug 2018
 *      Author: rodolfo
 *
 *  Implementazione di QuickSort.h
 */
#include "QuickSort.h"


double* merge(double *array1, double *array2, long int size1, int size2){
	int i=0, j=0, k=0;
	// alloco l'array di merge (che può avere al più nel caso peggiore size1+size2 elementi)
	double *result = malloc((size1+size2)*sizeof(double));

	// finchè non scorro i due indici (semplice merge, implementato similmente anche nel bitonic)
	while (i < size1 && j < size2)
		if (array1[i] < array2[j]) {
			result[k] = array1[i];
			i++;
			k++;
		} else {
			result[k] = array2[j];
			j++;
			k++;
		}
	if (i == size1)
		while (j < size2) {
			result[k] = array2[j];
			j++;
			k++;
		}
	else
		while (i < size1) {
			result[k] = array1[i];
			i++;
			k++;
		}
	return result;
}

void parallelQuickSort(int *id, int *nums){
	int pid = *id, pnums = *nums; // per non lavorare ogni volta con *id e *num

	/*
	 * creo ed ordino gli array locali per ogni processo
	 */
	double *local_array = (double*) malloc(SIZE_ARRAY/pnums * sizeof(double));
	srand(time(0)+pid);
	initArray(local_array, SIZE_ARRAY/pnums);
	serialIterativeQuickSort(local_array, SIZE_ARRAY/pnums);
#ifdef DEBUG
	printf("Processo %i , Array locale: (GIA ORDINATI)" , pid);
	printArray(local_array, SIZE_ARRAY/pnums);
#endif

#ifdef BENCHMARK
	// inizializzo le variabili che mi servono per il benchmark
	double t0, // istante iniziale
			t1, // istante finale
			localElapsed, // tempo trascorso per un singolo processo (locale)
			elapsed; // tempo trascorso totale

	// registro l' istante iniziale
	MPI_Barrier(MPI_COMM_WORLD);
	t0 = MPI_Wtime();
#endif

	/*
	 * Eseguo il grafo dei merge:
	 * p0             p1             p2             p3     ...     pn-1
	 *    merge(p0,p1)                  merge(p2,p3)
	 * 			merge(merge(p0,p1),merge(p2,p3)
	 * 			             .
	 * 			             .
	 * 			             .
	 * 			             		 p0 (che avrà l' array ordinato)
	 */
	MPI_Status status;

	int step = 1;
	// vecchia dimensione dell' array e nuova dimensione dell' array di merge
	int oldSize = SIZE_ARRAY/pnums, newSize;
	double *other;

	while (step < pnums) {
		if (pid % (2 * step) == 0) { // processi che riceve il syze dell' array temporaneo da mergiare
			if (pid + step < pnums) {
				MPI_Recv(&newSize, 1, MPI_INT, pid + step, 0, MPI_COMM_WORLD, &status);
				other = (double *) malloc(newSize * sizeof(double));
				MPI_Recv(other, newSize, MPI_DOUBLE, pid + step, 0, MPI_COMM_WORLD, &status);
				double *temp = malloc(oldSize * sizeof(double));
				for(long int i=0; i<oldSize; i++) temp[i] = local_array[i];
				free(local_array);
				local_array = merge(temp, other, newSize , oldSize);
				oldSize = oldSize + newSize;
				free(temp);
			}
		} else { // processi che inviano l' array temporaneo da mergiare
			int near = pid - step;
			MPI_Send(&oldSize, 1, MPI_INT, near, 0, MPI_COMM_WORLD);
			MPI_Send(local_array, oldSize, MPI_DOUBLE, near, 0, MPI_COMM_WORLD);
			break;
		}
		step = step * 2;
	}

#ifdef DEBUG
	if(pid == 0) {
		printf("Array Globale Ordinato: ");
		printArray(local_array, SIZE_ARRAY);
	}
#endif



#ifdef BENCHMARK
	// registro l'istante finale ed il tempo trascorso localmente
	t1 = MPI_Wtime();
	localElapsed = t1 - t0;

	/*
	 * Tramite una reduction trovo il tempo trascorso totale (elapsed)
	 * che sarà il massimo tra i tempi locali
	 */
	MPI_Reduce(&localElapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0,
			MPI_COMM_WORLD);

	// Stampo il risultato ottenuto
	if(pid==0) printf("\n ***** Quick Sort ***** \n\tTempo di esecuzione totale: %.5g\n\tDimensione array: %i\n\tNumero Processi: %i\n" , elapsed, SIZE_ARRAY, pnums);
	printf("\n\tTempo locale per il processo %i: %.5g" , pid, localElapsed);
#endif

	free(local_array);
}
