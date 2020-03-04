/*
 * OddEvenSort.c
 *
 *  Created on: 01 lug 2018
 *      Author: rodolfo
 *
 *  Implementazione di OddEvenSort.h
 */
#include "OddEvenSort.h"

void compareSplitPhase(double *local_array, double *read_array, double *write_array, long int size, int pid, MPI_Status status) {
	int i, j, k;

	for (i = 0; i < size; i++) // inizializzo l' array locale di scrittura
		write_array[i] = local_array[i];

	if (pid < status.MPI_SOURCE) { // piglio il più piccolo tra gli elemeni locali
		for (i = j = k = 0; k < size; k++) {
			if (j == size || (i < size && write_array[i] < read_array[j]))
				local_array[k] = write_array[i++];
			else
				local_array[k] = read_array[j++];
		}
	} else { // piglio il più grande tra gli elemeni locali
		for (i = k = size - 1, j = size - 1; k >= 0; k--) {
			if (j == 0 || (i >= 0 && write_array[i] >= read_array[j]))
				local_array[k] = write_array[i--];
			else
				local_array[k] = read_array[j--];
		}
	}
}

void oddEvenIndexPhase(int pid, int pnums, int *rangoDispari, int *rangoPari){
	if (pid % 2 == 0) { // inizializzo rangoDispari e rangoPari se il processo corrente è pari
		*rangoDispari = pid - 1;
		*rangoPari = pid + 1;
	} else { // inizializzo rangoDispari e rangoPari se il processo corrente è dispari
		*rangoDispari = pid + 1;
		*rangoPari = pid - 1;
	}
	/* per i processi che vanno "out of range" al controllo precedente, rispeetto al normale numero di processi di esecuzione */
	if (*rangoDispari == -1 || *rangoDispari == pnums)
		*rangoDispari = MPI_PROC_NULL;
	if (*rangoPari == -1 || *rangoPari == pnums)
		*rangoPari = MPI_PROC_NULL;

}

void oddEvenSort(int *id, int *nums){
	int pid = *id, pnums = *nums; // per non lavorare ogni volta con *id e *num

	MPI_Status status; // status per la recv

	double *read_array, *write_array, *local_array; // creo gli array locali
	int rangoPari, rangoDispari; // servono poi per la fase pari e la fase dispari

	// alloco gli array locali
	local_array = (double*) malloc(SIZE_ARRAY/pnums * sizeof(double)); // array locale (ogni processo possiede SIZE_ARRAY/pnums elementi dell' array totale di SIZE_ARRAAY elementi)
	read_array = (double*) malloc(SIZE_ARRAY/pnums * sizeof(double)); // array locale di lettura (ogni processo possiede SIZE_ARRAY/pnums elementi dell' array totale di SIZE_ARRAAY elementi)
	write_array = (double*) malloc(SIZE_ARRAY/pnums * sizeof(double)); // array locale di scrittura (ogni processo possiede SIZE_ARRAY/pnums elementi dell' array totale di SIZE_ARRAAY elementi)

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

	// inizializzo l' array locale principale (ed ogni volta modifico il seed del rand)
	srand(time(0) + pid);
	initArray(local_array, SIZE_ARRAY/pnums); // inizializzo l' array locale
	serialIterativeQuickSort(local_array, SIZE_ARRAY/pnums); // ordino gli array locali

#ifdef DEBUG
	printf("Processo %i , Array locale: (PRIMA DELL' ODD EVEN)", pid); printArray(local_array, SIZE_ARRAY/pnums);
#endif

	// fase in cui assego gli indici pari e dispari (rangoPari e rangoDispari)
	oddEvenIndexPhase(pid, pnums, &rangoDispari, &rangoPari);

    /*
     * Main loop dell' odd-even (è la parte principale)
     * Quì verifico la fase pari o la fase dispari (in base in quale processo sono)
     * e scambio in base al processo attuale con i processi di rangoPari e di rangoDispari
     * trovati precedentemente
     * (lavoro sul read_array)
     */
	for (int i = 0; i < pnums - 1; i++) {
		if (i % 2 == 1) // fase dispari (scambio gli elementi di indice pari di local_array in read_array per i processi rangoDispari)
			MPI_Sendrecv(local_array, SIZE_ARRAY/pnums, MPI_DOUBLE, rangoDispari, 1,
						 read_array, SIZE_ARRAY/pnums, MPI_DOUBLE, rangoDispari, 1, MPI_COMM_WORLD, &status);
		else // fase pari (scambio gli elementi di indice dispari di local_array in read_array per i processi rangoPari)
			MPI_Sendrecv(local_array, SIZE_ARRAY/pnums, MPI_DOUBLE, rangoPari, 1,
						 read_array, SIZE_ARRAY/pnums, MPI_DOUBLE, rangoPari, 1, MPI_COMM_WORLD, &status);

		// (lavoro sul write_array)
		compareSplitPhase(local_array, read_array, write_array, SIZE_ARRAY/pnums, pid, status);
	}

#ifdef DEBUG
	MPI_Barrier(MPI_COMM_WORLD);
	printf("Processo %i , Array locale (DOPO L' ODD EVEN): ", pid); printArray(local_array, SIZE_ARRAY/pnums);
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
	if(pid==0) printf("\n ***** Odd Even Sort ***** \n\tTempo di esecuzione totale: %.5g\n\tDimensione array: %i\n\tNumero Processi: %i\n" , elapsed, SIZE_ARRAY, pnums);
	printf("\n\tTempo locale per il processo %i: %.5g" , pid, localElapsed);
#endif


	free(local_array);
	free(read_array);
	free(write_array);
}


