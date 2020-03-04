/*
 * Utils.c
 *
 *  Created on: 01 lug 2018
 *      Author: rodolfo
 *
 *  Implementazione di Utils.h
 */
#include "Utils.h"


void printArray(double *array, long int size){
	printf("[");
	for (long int i = 0; i < size; i++) {
		if(i!=size-1) printf("%.5g , " , array[i]);
		else printf("%.5g]", array[i]);
	}
	printf("\n");
}

void initArray(double *array, long int size){
	for (long int i = 0; i < size; i++) {
		array[i] = (double) rand() / (double) MAX_VALUE + MIN_VALUE;
	}
}
