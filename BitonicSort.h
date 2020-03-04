/*
 * BitonicSort.h
 *
 *  Created on: 03 lug 2018
 *      Author: rodolfo
 *
 *  Bitonic Sort
 *  @see { per il corretto funzionamento bisogna usare più di 2 processi!
 *  in generale un numero di processi p > 2 tale che p è una potenza di due.
 *  L' algoritmo altrimenti non funziona! }
 */

#ifndef BITONICSORT_H_
#define BITONICSORT_H_

#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include "Utils.h"
#include "Settings.h"
#include "SerialSort.h"

/*
 * Algoritmo del bitonic sort.
 *
 * @id passagio per riferimento dell'id del processo temporaneo di mpi
 * @nums passagio per riferimento del numero di processi del programma
 */
void bitonicSort(int *id, int *nums);

/*
 * Fase crescente del bitonic sort,
 * basa il merge su pid < partner
 *
 * @param local_array array locale
 * @param received buffer buffer di ricezione
 * @param merge_buffer buffer di merge
 * @param pSize dimensione temporanea (passata nel ciclo principale)
 * @id passagio per riferimento dell'id del processo temporaneo di mpi
 * @nums passagio per riferimento del numero di processi del programma
 *
 */
void bitonicPhaseIncr(double* local_array, double* received_buffer, double* merge_buffer, int pSize, int *id, int *nums);

/*
 * Fase decrescente del bitonic sort,
 * basa il merge su pid > partner
 *
 * @param local_array array locale
 * @param received buffer buffer di ricezione
 * @param merge_buffer buffer di merge
 * @param pSize dimensione temporanea (passata nel ciclo principale)
 * @id passagio per riferimento dell'id del processo temporaneo di mpi
 * @nums passagio per riferimento del numero di processi del programma
 *
 */
void bitonicPhaseDecr (double* local_array, double* received_buffer, double* merge_buffer, int pSize, int *id, int *nums);

/*
 * Fase di merge del bitonic.
 * Trasferisce local_array su received_array ed in base a mergeType
 * determinerà il tipo di merge che andrà in merge_buffer
 * (see merge low o merge high)
 *
 * @param local_array array locale
 * @param received buffer buffer di ricezione
 * @param merge_buffer buffer finale di merge (viene modificato esattamente nella fase low e high)
 * @param mergeType determina il tipo di merge (una booleana tipo)
 * @param partner processo partner della fase di merge (processo in cui sbambiare local_array e received_buffer)
 * @id passagio per riferimento dell'id del processo temporaneo di mpi
 * @nums passagio per riferimento del numero di processi del programma
 *
 */
void mergePhase(double *local_array,  double* received_buffer, double* merge_buffer, int mergeType, int partner, int *id, int *nums);

/*
 * Fase di mergeLow in cui modifico merge_buffer
 * facendolo sulla base di local_array[index1] <= received_buffer[index2]
 *
 * @param local_array array locale
 * @param received buffer buffer di ricezione
 * @param merge_buffer buffer finale di merge
 * @id passagio per riferimento dell'id del processo temporaneo di mpi
 * @nums passagio per riferimento del numero di processi del programma
 */
void mergeLow(double* local_array, double* received_buffer, double* merge_buffer, int *id, int *nums);

/*
 * Fase di mergeLow in cui modifico merge_buffer
 * facendolo sulla base di local_array[index1] >= received_buffer[index2]
 *
 * @param local_array array locale
 * @param received buffer buffer di ricezione
 * @param merge_buffer buffer finale di merge
 * @id passagio per riferimento dell'id del processo temporaneo di mpi
 * @nums passagio per riferimento del numero di processi del programma
 */
void mergeHigh(double* local_array, double* received_buffer, double* merge_buffer, int *id, int *nums);


#endif /* BITONICSORT_H_ */
