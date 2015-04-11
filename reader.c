#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analizer.h"
#include "reader.h"

static int n = 0;
static FILE *file = NULL;
static int numberOfWords = 0;
static int position = 0;

void add_sufix_r( struct ngram *pointer, char *text, int number){
    if( pointer->capacity == 0){
        pointer->sufixes = malloc( number * sizeof *pointer->sufixes);
        assert(pointer->sufixes != NULL);
        pointer->capacity = number;
    } else {
        realloc_sufixes( pointer );
    }
    pointer->sufixes[(*pointer).number] = strdup( text );
    (*pointer).number++;
}

void add_ngram_r( struct data *data, char **text ){
    register int i = 0;
    int sufix_pos = n - 1;
    int numberOfPrefixesOccurance = 0;
    int numberOfSufixes = 0;
    initialize_ngram( data );
    add_prefix( data->ngrams[data->number], text, position );
    position += n - 1;
    numberOfPrefixesOccurance = atoi( text[position] );
    position++;
    numberOfSufixes = atoi( text[position] );
    position++;
    for( i = 0; i < numberOfSufixes; i++ ) {
        add_sufix_r(data->ngrams[data->number], text[position], numberOfSufixes); //dodaje sufix do istniejącego ngramu
        position++;
    }
    data->ngrams[data->number]->occurance = numberOfPrefixesOccurance;
    data->number += 1;
}

void read( char *ifile, struct data *d ){
    int i = 0;
    long int size = 0;
    int numberOfSufixes = 0;
    int numberOfPrefixesOccurance = 0;
    int numberOfPrefixesOccuranceBefore = 0;
    char *content = NULL;
    char *string = NULL;
    char **text = NULL;
    struct ngram *pointer = NULL;
    numberOfWords = 0;
    position = 0;
    file = fopen(ifile,"r");
    if(file == NULL)
    {
        return;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    if( size == 0) {
        fprintf(stderr, "Plik %s nie zawiera tekstu", ifile);
        return;
    }
    content = calloc(size + 1, 1);

    fread(content,1,size,file);
    fclose(file);
    text = (char **)malloc( (size + 1) * sizeof( char *) );
    i = 0;
    string = strtok (content, " \n");
    while (string != NULL)
    {
        text[i] = strdup( string );
        i++;
        string = strtok (NULL, " \n");
    }
    while( text[numberOfWords] != NULL )
        numberOfWords++;
    if(numberOfWords == 0)
        return;
    n = atoi( text[position] );
    if( n == 0 ){
        fprintf(stderr, "Plik %s jest nieprawidłowy", ifile);
        return;
    }
    setN( n );
    position++;
    if( d->ngrams == NULL)
        initialize_data( d );


    while( text[position] != NULL ) {
        pointer = find_ngram( text, d, position );
        if (pointer != NULL) {
            position += n - 1;
            numberOfPrefixesOccurance = atoi(text[position]);
            numberOfPrefixesOccuranceBefore = pointer->occurance;
            position++;
            numberOfSufixes = atoi(text[position]);
            position++;
            for (i = 0; i < numberOfSufixes; i++) {
                add_sufix_r(pointer, text[position], numberOfSufixes); //dodaje sufix do istniejącego ngramu
                position++;
            }
            pointer->occurance = numberOfPrefixesOccuranceBefore + numberOfPrefixesOccurance;
        } else {
            realloc_data(d);
            add_ngram_r(d, text); //dodaje nowy ngram
        }
    }

    return;

}

