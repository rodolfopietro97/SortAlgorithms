/*
 * SequentialSort.h
 *
 *  Created on: 29 giu 2018
 *      Author: rodolfo
 *
 *  Algoritmi di ordinamento (SERIALI) serviranno per essere
 *  utilizzati come metriche di paragone e come supporto per
 *  gli algoritmi paralleli
 */

#ifndef SERIALSORT_H_
#define SERIALSORT_H_

#include <stdio.h>
#include <stdlib.h>
#include "Settings.h"

/*
 * Funzione di swap che non utilizza una terza variabile
 *
 * @param a primo dato
 * @param b secondo dato
 */
void swap(double *a, double *b);

/*
 * Algoritmo di ordinamento quadratico seriale
 *
 * @param array da ordinare
 * @param size dimensione dell'array
 */
void serialQuadraticSort(double *array, long int size);

/*
 * Quicksort seriale (ITERATIVO).
 *
 * @see { Lo utilizzeremo come metrica di paaragone principale
 * perchè il quicksort è il migliore algoritmo seriale
 * di ordinamento. }
 *
 * @param array da ordinare
 * @param size dimensione dell'array
 *
 * FONTE :  https://en.wikibooks.org/wiki/Algorithm_Implementation/Sorting/Quicksort
 */
void serialIterativeQuickSort(double *array, long int size);


/*
 * Versione seriale (RICORSIVA) del quicksort
 *
 * @see { per un corretto funzionamento fare la chiamata iniziale con SIZE-1:
 * serialRecursiveQuickSort(array, 0, SIZE-1) }
 *
 * @param array da ordinare
 * @param in parte iniziale dell' array (da dove partire)
 * @param fin parte finale dell' array (da dove finire)
 */
void serialRecursiveQuickSort(double *array, long int in, long int fin);

/*
 * Fase di partizionamento del quicksort ricorsivo
 *
 * @param array da ordinare
 * @param in parte iniziale dell' array (da dove partire)
 * @param fin parte finale dell' array (da dove finire)
 * @param pivot da passare
 *
 * @return pivotNuovo
 */
long int recursiveQuickSortPartition(double *array, long int in, long int fin, long int pivot);



#endif /* SERIALSORT_H_ */
