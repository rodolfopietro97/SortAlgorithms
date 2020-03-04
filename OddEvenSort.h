/*
 * OddEvenSort.h
 *
 *  Created on: 01 lug 2018
 *      Author: rodolfo
 *
 *  Odd Even Sort Parallelo
 */

#ifndef ODDEVENSORT_H_
#define ODDEVENSORT_H_

#include <mpi.h>
#include <time.h>
#include "SerialSort.h"
#include "Utils.h"
#include "Settings.h"

/*
 * Algoritmo odd-even sort completo.
 *
 * @see { per il corretto funzionameto di tale algoritmo assumiamo
 * che il numero di processi divide la dimensione dell' array! }
 *
 * @id passagio per riferimento dell'id del processo temporaneo di mpi
 * @nums passagio per riferimento del numero di processi del programma
 */
void oddEvenSort(int *id, int *nums);

/*
 * Fase di assegnamento degli indici dell'algoritmo odd-even.
 * Questa è la prima fase principale dell' algoritmo.
 * In questa fase assegno gli indici "rangoPari" e "rangoDispari"
 * in modo da poter eseguire correttamente la seconda fase.
 *
 * @see{ può darsi che rango pari e rango dispari assuino valori
 * "OutOfRange" rispetto al numero di processi MPI. Utilizziamo perciò
 * la macro di MPI: "MPI_PROC_NULL" per individuare quei processi che
 * per la natura intrinseca dell' algoritmo saranno nulli, e porteranno
 * purtroppo all' idling }
 *
 * @param pid id del processo mpi corrente
 * @param pnums numero di processi mpi utilizzati
 * @param rangoDispari passaggio per riferimento del processo di rango dispari che dovrà essere utilizzato nella seconda fase
 * @param rangoPari passaggio per riferimento del processo di rango pari che dovrà essere utilizzato nella seconda fase
 */
void oddEvenIndexPhase(int pid, int pnums, int *rangoDispari, int *rangoPari);

/*
 * Fase di compare e split dell' algoritmo odd-even.
 * Questa è la seconda fase principale dell' algoritmo.
 * In questa fase utilizzo i tre array (local,write e read)
 * e tramite essi "aggiusto" i dati, in modo da avere alla fine
 * sui p local_array i dati ordinati, formando così p partizioni di
 * SIZE_ARRAY/p elementi dell' array totale ordinato.
 * (Ricordiamo che p è il numero di processi)
 *
 * @param local_array array locale di ogni singolo processo
 * @param read_array array locale di lettura di ogni singolo processo
 * @param write_array array locale di scrittura di igno singolo processo
 * @param size dimensioni degli array locali
 * @param pid id del processo corrente di mpi
 * @param status stato della MPI_Sendrecv antecedente alla chiamata di tale funzione (serve per mpi).
 */
void compareSplitPhase(double *local_array, double *read_array, double *write_array, long int size, int pid, MPI_Status status);


#endif /* ODDEVENSORT_H_ */
