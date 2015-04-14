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
    int i = 0;
    if( basefiles[i] != NULL )
        strcpy(files, basefiles[i]);
    i++;
    while( basefiles[i] != NULL ){
        strcat(files, " ");
        strcat(files, basefiles[i]);
        i++;
    }
    fprintf(statsIn, "Statystyki plik%s %s: \n", i > 1 ? "ów" : "u", files);
}