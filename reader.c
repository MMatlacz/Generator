#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analizer.h"
#include "reader.h"
static int number_of_prefixes;
static int number_of_words;
static int n;

void initialize_sufix_r( struct ngram *pointer, int i ){
	pointer->sufixes[i] = (struct sufix *)malloc( sizeof (struct sufix) );
	if( pointer->sufixes[i] == NULL )
		exit(EXIT_FAILURE);
	(*pointer->sufixes[i]).occurance = 0;
}

void initialize_ngram_r( struct data *data, int i ){
	int j;
	data->ngrams[i] = (struct ngram *)malloc( sizeof (struct ngram) );
	if( data->ngrams == NULL )
		exit(EXIT_FAILURE);
	data->ngrams[i]->prefix = (char **)malloc( (n - 1) * sizeof (char *) );
	if( data->ngrams[i]->prefix == NULL )
		exit(EXIT_FAILURE);
	data->ngrams[i]->sufixes = (struct sufix **)malloc( 5 * sizeof (struct sufix *) );
	if( data->ngrams[i]->sufixes == NULL )
		exit(EXIT_FAILURE);
	(*data->ngrams[i]).occurance = 0;
	(*data->ngrams[i]).number = 0;
	(*data->ngrams[i]).capacity = 5; 
	for( j = 0; j < 5; j++ ){
		initialize_sufix_r( data->ngrams[i], j );
	}
}

void initialize_data_r( struct data *data ){
	int i = 0;
	data->ngrams = (struct ngram **)malloc( number_of_prefixes * sizeof (struct ngram *) );
	if( data->ngrams == NULL )
		exit(EXIT_FAILURE);
	(*data).capacity = number_of_prefixes;
	(*data).number = 0;
	for( i = 0; i < (*data).capacity; i++ ){
		initialize_ngram_r( data, i );

	}
}


void realloc_sufixes_r( struct ngram *ngram ){
	struct sufix **temp = NULL;
	if( (*ngram).capacity == (*ngram).number ){
		temp = (struct sufix **)realloc( ngram->sufixes, 2 * (*ngram).capacity * sizeof (struct sufix *) );
		if( temp == NULL )
			exit(EXIT_FAILURE);
		ngram->sufixes = temp;
		(*ngram).capacity *= 2;
	}
}

struct ngram *find_ngram_r( char **text, struct data *data ){
	int i, j;
	int flag;
	for( i = 0; i< (*data).number; i++ ){
		flag = 0;
		for( j = 0; j < n - 1; j++ ){
			if( strcmp( data->ngrams[i]->prefix[j], text[j] ) == 0){
				flag++;
			} else {
				flag = 0;
				continue;
			}
			if( flag == n - 1 ){
				return data->ngrams[i];
			}
		}
	}
	return NULL;
}

void add_sufix_r( struct ngram *pointer, char *text ){
	int i = 0;
	(*pointer).occurance++;
	for( i = 0; i < (*pointer).number; i++ ){
		if( !strcmp( pointer->sufixes[i]->sufix, text ) ){
			(*pointer->sufixes[i]).occurance++;
			return;
		}
	}

	realloc_sufixes( pointer );
	initialize_sufix( pointer, (*pointer).number );
	pointer->sufixes[(*pointer).number]->sufix = strdup( text );
	(*pointer->sufixes[(*pointer).number]).occurance++;
	(*pointer).number++;
}

void add_ngram_r( struct data *data, char **text, char *sufix ){
	int j = 0;
	realloc_data( data );
	if( data->ngrams[(*data).number] == NULL ){
		initialize_ngram( data, (*data).number );
	}
	
	for( j = 0; j < n - 1; j++ ){
		data->ngrams[(*data).number]->prefix[j] = strdup( text[j] );
	}

	add_sufix_r( data->ngrams[(*data).number], sufix );

	(*data).number += 1;
}



void read_prefix_r( char **prefix, struct data *data, int ngram_occurance, int ngram_number_of_sufixes, char *sufix ){
	struct ngram *pointer = NULL;
	register int i = 0;
		//sprawdzam czy ngram juz wystepuje
		pointer = find_ngram_r( prefix, data );//zwraca wskaźnik do występującego ngramu 
		//jeżeli ngram już jest lub NULL jeżeli go jeszcze nie ma
		if( pointer != NULL ){
			add_sufix_r( pointer, sufix ); //dodaje sufix do istniejącego ngramu
		} else {
			add_ngram_r( data, prefix, sufix ); //dodaje nowy ngram
		}
}



void read( char *intermediate_file_name, struct data *data ){
	FILE *input = fopen(intermediate_file_name, "r");
	char 	**prefix;
	char 	*sufix;
	register int 	i 					= 0;
	register int 	j 					= 0;
	int 	ngram_occurance 			= 0;
	int 	ngram_number_of_sufixes 	= 0;
	int 	sufix_occurance 			= 0;

	
	if( fscanf(input, "%*s %d %*s %d %*s %d", &number_of_prefixes, &number_of_words, &n) != 3){
		exit(EXIT_FAILURE);
	}

	prefix = (char **)malloc( (n - 1) * sizeof (char *) );
	if( prefix == NULL )
		exit(EXIT_FAILURE);
	for( i = 0; i < n; i++ ){
		prefix[i] = (char *)malloc( 35 * sizeof (char) );
		if( prefix[i] == NULL ){
			exit(EXIT_FAILURE);
		}
	}
	sufix = (char *)malloc( 35 * sizeof (char) );
	if( data->ngrams == NULL)
		initialize_data_r( data );

	for( i = 0; i < number_of_prefixes; i++ ){
		for( j = 0; j < n - 1; j++ ){
			fscanf(input, "%s", prefix[j]);
		}
		fscanf(input, "%d %d", &ngram_occurance, &ngram_number_of_sufixes);
		for( j = 0; j < ngram_number_of_sufixes; j++ ){
			fscanf(input, "%s %d", sufix, &sufix_occurance);
			read_prefix_r( prefix, data, ngram_occurance, ngram_number_of_sufixes, sufix );
		}	
	}
}

