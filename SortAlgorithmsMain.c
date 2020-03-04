/*
 ============================================================================
 Name        : SortAlgorithms.c
 Author      : Rodolfo Pietro Calabrò
 Version     : 1.0
 Copyright   : Rodolfo Pietro Calabrò
 Description : Progetto finale di APSD.
 ============================================================================
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// #include <GL/glut.h>



#include "OddEvenSort.h"
#include "QuickSort.h"
#include "Settings.h"
#include "BitonicSort.h"
/*
 * Funzione per eseguire i benchmark degli algoritmi seriali
 */
void serialBenchmark();

int main(int argc, char* argv[]){
	int pid, // id dei processi
		pnums; // numero totale dei processi


	// inizializzo le MPI
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &pnums);

    /*
     * Eliminare il commento per eseguire l' algoritmo scelto
     */
	//oddEvenSort(&pid, &pnums);
  	parallelQuickSort(&pid, &pnums);
	//bitonicSort(&pid, &pnums);


	MPI_Finalize();

	return 0;
}




void serialBenchmark(){
	/*
	 * Per il benchmark degli algoritmi seriali.
	 * (Ovviamente tutto va eseguito con un solo processo)
	 */
		double *array = (double*) malloc(SIZE_ARRAY * sizeof(double));
		initArray(array,SIZE_ARRAY);
		//printArray(array, SIZE_ARRAY);

#ifdef BENCHMARK
		// inizializzo le variabili che mi servono per il benchmark
		double t0, // istante iniziale
				t1; // istante finale

		MPI_Barrier(MPI_COMM_WORLD);
		t0 = MPI_Wtime();

		// funzioni misurare... (cacciare il commento per scegliere la funzione seriale da misurare)
		//serialQuadraticSort(array, SIZE_ARRAY);
		//serialIterativeQuickSort(array,SIZE_ARRAY);
		//serialRecursiveQuickSort(array, 0, SIZE_ARRAY-1);
		//printArray(array, SIZE_ARRAY);

		t1 = MPI_Wtime();
		printf("\n ***** Test Seriale ***** \n\tTempo di esecuzione: %.5g", t1 - t0);
#endif

		free(array);
}
