/*
 * QuickSort.h
 *
 *  Created on: 02 lug 2018
 *      Author: rodolfo
 *
 *  Quick sort parallelo (versione ricorsiva)
 */

#ifndef QUICKSORT_H_
#define QUICKSORT_H_

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "SerialSort.h"
#include "Utils.h"

/*
 * Fase di merge degli array.
 * Dati due array v1 e v2, restituisce l'array di merge ordinato.
 *
 * @param array1 primo array
 * @param array2 secondo array
 * @param size1 dimensioni del primo array
 * @param size2 dimensioni del secondo array
 *
 * @return arrayMergiato
 */
double* merge(double *array1, double *array2, long int size1, int size2);

/*
 * Algoritmo parallel quicksort "uscito fuori dopo tanti tentativi"
 * Si basa sull' ordinamento degli array locali che poi vengo mergiati
 * in un array che raddoppia (tramite la funzione merge)
 * che viene allocato su un processo (una sorta di recursive decomposition basata su un grafo).
 * Tra i tre algoritmi paralleli è il più veloce,
 * ma ha una pecca. Se utilizziamo dimensioni dell' input molto grandi
 * e tanti processi si crea un grosso load-unbalance sul processo 0
 * che è il processo che conterrà l' array finale ordinato
 *
 * @id passagio per riferimento dell'id del processo temporaneo di mpi
 * @nums passagio per riferimento del numero di processi del programma
 *
 */
void parallelQuickSort(int *id, int *nums);



#endif /* QUICKSORT_H_ */
