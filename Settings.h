/*
 * Settings.h
 *
 *  Created on: 29 giu 2018
 *      Author: rodolfo
 *
 *  File contenente i "settagi", ad esempio la dimensione dell' array, abilitazione di opzioni, etc...
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

/*
 * Dimensioni dello stack per un massimo di 2^(64/2) elemeti dell' array (SERVE PER IL QUICKSORT SERIALE)
 */
#define MAX 64

/*
 * Dimensione dell'array
 */
const static long int SIZE_ARRAY = 8*90000000;

/*
 * Debug (commentare per non vedere le operazioni in fase di "debug")
 */
//#define DEBUG

/*
 * Benchmark (commentare per non vedere le operazioni di Benchmark)
 * Il benchmark misura i tempi impiegati da tutti i processi per svolgere
 * l' algoritmo. Il tempo di riferimento sarà poi il tempo maggiore
 * tra tutti quelli impiegati
 *
 * @see { Per avere un benchmark corretto si consiglia di eliminare il debug! }
 */
#define BENCHMARK


/*
 * Intervalllo di generazione degli elementi casuali dell' array da ordinare
 * [MIN_VALUE ; MAX_VALUE]
 */
const static double MIN_VALUE = 0.0000001;
const static double MAX_VALUE = 99999999.999999999;


#endif /* SETTINGS_H_ */
