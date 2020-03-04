/*
 * Utils.h
 *
 *  Created on: 01 lug 2018
 *      Author: rodolfo
 *
 *  File contenente le funzioni di utilità, ad esempio per la stampa di un array,
 *  per l' inizializzazione di un' array, etc...
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include "Settings.h"


/*
 * Stampa l'array (utile in fase di debug)
 *
 * @param array da stampare
 * @param size dell' array
 */
void printArray(double *array, long int size);

/*
 * Inizializza l'array con valori random che vanno
 * appartenenti all' intervallo [MIN_VALUE, MAX_VALUE]
 * con MIN_VALUE e MAX_VALUE definiti in "Settings.h"
 *
 * @param array da inizializzare
 * @param size dell' array
 */
void initArray(double *array, long int size);


#endif /* UTILS_H_ */
