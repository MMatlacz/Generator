//
// Created by marcin on 4/14/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stats.h"

void generateStats(struct data *d, char *filename, char **basefiles, char **intermediatefiles){
    FILE *statsIn = fopen(filename,"w");
    char files[500];
    struct ngram *pointers[10];
    int i, j;
    i = 0;
    if( basefiles[i] != NULL )
        strcpy(files, basefiles[i]);
    i++;
    while( basefiles[i] != NULL ){
        strcat(files, " ");
        strcat(files, basefiles[i]);
        i++;
    }
    fprintf(statsIn, "Statystyki plik%s %s: \n", i > 1 ? "ów" : "u", files);
    fprintf(statsIn, "Liczba ngramów: %d\n", d->number);
    for( j = 0; j < 10; j++ ) {
        for (i = 0; i < d->number; i++) {
            if (pointers[j] == NULL) {
                pointers[j] = d->ngrams[i];
                continue;
            } else if( pointers[j]->capacity < d->ngrams[i]->capacity ){
                pointers[j] = d->ngrams[i];
            }
        }
    }
    for( i = 0; i < 10; i++ ){
        fprintf(stderr, "%s %d ", pointers[i]->prefix[0], pointers[i]->occurance);
    }
}