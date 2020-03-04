/*
 * BitonicSort.c
 *
 *  Created on: 03 lug 2018
 *      Author: rodolfo
 *
 *  Implementazione di "BitonicSort.h"
 */
#include "BitonicSort.h"


void bitonicPhaseIncr(double* local_array, double* received_buffer, double* merge_buffer, int pSize, int *id, int *nums){
	int pid = *id, pnums = *nums; // per non lavorare ogni volta con *id e *num

    unsigned  xorBit; // bit di xor
    int       pDim = log2(pSize); // dimensione del processo
    int       partner; // processo parthner a cui inviare il local_array in received_buffer nella fase di merge

    xorBit = 1 << (pDim - 1); // ogni volta sarà una potenza di due

    for (int stage = 0; stage < pDim; stage++) {
        partner = pid ^ xorBit; // il parthner (essendo xorBit una potenza di due) sarà un numero dispari
        if (pid < partner) // merge in cui il processo corrente è MINORE di quello partner
            mergePhase(local_array, received_buffer, merge_buffer, 0, partner, id, nums);
        else
            mergePhase(local_array, received_buffer, merge_buffer, 1, partner, id, nums);
        xorBit = xorBit >> 1;// potenza di due successiva
    }
}



void bitonicPhaseDecr (double* local_array, double* received_buffer, double* merge_buffer, int pSize, int *id, int *nums) {
	int pid = *id, pnums = *nums; // per non lavorare ogni volta con *id e *num

    unsigned  xorBit;  // bit di xor
    int       pDim = log2(pSize);  // dimensione del processo
    int       partner; // processo parthner a cui inviare il local_array in received_buffer nella fase di merge

    xorBit = 1 << (pDim - 1); // ogni volta sarà una potenza di due
    for (int stage = 0; stage < pDim; stage++) {
        partner = pid ^ xorBit; // il parthner (essendo xorBit una potenza di due) sarà un numero dispari
        if (pid > partner) // merge in cui il processo corrente è MAGGIORE di quello partner (opposto della fase Incr)
            mergePhase(local_array, received_buffer, merge_buffer, 0, partner, id, nums);
        else
            mergePhase(local_array, received_buffer, merge_buffer, 1, partner, id, nums);
        xorBit = xorBit >> 1;// potenza di due successiva
    }

}


void mergePhase(double *local_array,  double* received_buffer, double* merge_buffer, int mergeType, int partner, int *id, int *nums){
	int pid = *id, pnums = *nums; // per non lavorare ogni volta con *id e *num

    MPI_Status status;

    // invia e riceve l' array local_array su received_buffer sul processo parthner
    MPI_Sendrecv(local_array, SIZE_ARRAY/pnums, MPI_DOUBLE,
                 partner, 0, received_buffer, SIZE_ARRAY/pnums,
                 MPI_DOUBLE, partner, 0, MPI_COMM_WORLD, &status);
    if (mergeType == 1)
        mergeHigh(local_array, received_buffer, merge_buffer, id, nums);
    else
        mergeLow(local_array, received_buffer, merge_buffer, id, nums);
}


void mergeLow(double* local_array, double* received_buffer, double* merge_buffer, int *id, int *nums) {
	int pid = *id, pnums = *nums; // per non lavorare ogni volta con *id e *num

    long int  i;
    long int  index1 = 0;
    long int  index2 = 0;

    for (i = 0; i < SIZE_ARRAY/pnums; i++)
        if (local_array[index1] <= received_buffer[index2]) {
        	merge_buffer[i] = local_array[index1];
            index1++;
        } else {
        	merge_buffer[i] = received_buffer[index2];
            index2++;
        }
    for (i = 0; i < SIZE_ARRAY/pnums; i++)
    	local_array[i] = merge_buffer[i];

}


void mergeHigh(double* local_array, double* received_buffer, double* merge_buffer, int *id, int *nums) {
	int pid = *id, pnums = *nums; // per non lavorare ogni volta con *id e *num

    int  i;
    int  index1 = SIZE_ARRAY/pnums - 1;
    int  index2 = SIZE_ARRAY/pnums - 1;

    for (i = SIZE_ARRAY/pnums - 1; i >= 0; i--)
        if (local_array[index1] >= received_buffer[index2]) {
        	merge_buffer[i] = local_array[index1];
            index1--;
        } else {
        	merge_buffer[i] = received_buffer[index2];
            index2--;
        }
    for (i = 0; i < SIZE_ARRAY/pnums; i++)
    	local_array[i] = merge_buffer[i];

}

void bitonicSort(int *id, int *nums){
	int pid = *id, pnums = *nums; // per non lavorare ogni volta con *id e *num

	// alloco gli array locali per ogni processo
	double *local_array = malloc(SIZE_ARRAY/pnums * sizeof(double)); // array locale di ogni processo
	double *received_buffer = calloc(SIZE_ARRAY/pnums, sizeof(double)); // buffer degli elementi della fase di ricezione (inizializzato a 0 con calloc)
	double *merge_buffer = calloc(SIZE_ARRAY/pnums, sizeof(double)); // buffer degli elementi della fase di merge (inizializzato a 0 con calloc)

	// inizializzo l' array locale principale (ogni volta modifico il seed del rand per non ottenere gli stessi valori su tutti gli array locali)
	srand(time(0) + pid);
	initArray(local_array,SIZE_ARRAY/pnums);

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

#ifdef DEBUG
	printf("Array locale processo %i (PRIMA DELL' ORDINAMENTO)" , pid);
	printArray(local_array, SIZE_ARRAY/pnums);
#endif
	serialIterativeQuickSort(local_array, SIZE_ARRAY/pnums); // ordino gli array locali
#ifdef DEBUG
	printf("Array locale processo %i (DOPO DELL' ORDINAMENTO LOCALE)" , pid);
	printArray(local_array, SIZE_ARRAY/pnums);
#endif

	/*
	 * Ordinamento delle liste locali (fase principale del bitonic)
	 */
    for (int pSize = 2, // numero di processi di rierimento per la fase (si vede l'utilità in bitonicPhaseIncr e bitonicPhaseDecr)
    	 phaseDeterminator = 2; // determina la fase da eseguire
    	 pSize <= pnums;
    	 pSize *= 2,
    	 phaseDeterminator = phaseDeterminator << 1) // tramite lo shift viene generata la potenza di due successiva
    {
        if ((pid & phaseDeterminator) == 0) // se l'phaseDeterminator sarà uguale al processo corrente pid eseguo la fase di ordinamento crescente
            bitonicPhaseIncr(local_array, received_buffer, merge_buffer,  pSize, id, nums);
        else // altrimenti la fase di ordinamento decrescente
        	bitonicPhaseDecr(local_array, received_buffer, merge_buffer, pSize, id, nums);
    }

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
	if(pid==0) printf("\n ***** Bitonic Sort ***** \n\tTempo di esecuzione totale: %.5g\n\tDimensione array: %i\n\tNumero Processi: %i\n" , elapsed, SIZE_ARRAY, pnums);
	printf("\n\tTempo locale per il processo %i: %.5g" , pid, localElapsed);
#endif

#ifdef DEBUG
	printf("\nArray locale processo %i (DOPO L' ORDINAMENTO GLOBALE)" , pid);
	printArray(local_array, SIZE_ARRAY/pnums);
#endif

	free(local_array);
	free(received_buffer);
	free(merge_buffer);
}



