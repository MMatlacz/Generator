#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "analizer.h"
#include "manager.h"

static int n;
static int number_of_words;
static char *intermediate_filename;

void initialize_sufix( struct ngram *pointer, int i ){
	pointer->sufixes[i] = (struct sufix *)malloc( sizeof (struct sufix) );
	if( pointer->sufixes[i] == NULL )
		exit(EXIT_FAILURE);
	(*pointer->sufixes[i]).occurance = 0;
}

void initialize_ngram( struct data *data, int i ){
	int j;
	data->ngrams[i] = (struct ngram *)malloc( sizeof (struct ngram) );
	if( data->ngrams == NULL )
		exit(EXIT_FAILURE);
	data->ngrams[i]->prefix = (char **)malloc( (n-1) * sizeof (char *) );
	if( data->ngrams[i]->prefix == NULL )
		exit(EXIT_FAILURE);
	data->ngrams[i]->sufixes = (struct sufix **)malloc( 5 * sizeof (struct sufix *) );
	if( data->ngrams[i]->sufixes == NULL )
		exit(EXIT_FAILURE);
	(*data->ngrams[i]).occurance = 0;
	(*data->ngrams[i]).number = 0;
	(*data->ngrams[i]).capacity = 5; 
	for( j = 0; j < 5; j++ ){
		initialize_sufix( data->ngrams[i], j );
	}
}

void initialize_data( struct data *data ){
	int i = 0;
	data->ngrams = (struct ngram **)malloc( 20 * sizeof (struct ngram *) );
	if( data->ngrams == NULL )
		exit(EXIT_FAILURE);
	(*data).capacity = 20;
	(*data).number = 0;
	for( i = 0; i < (*data).capacity; i++ ){
		initialize_ngram( data, i );

	}
}

void realloc_data( struct data *data ){
	struct ngram **temp = NULL;
	if( (*data).number == (*data).capacity ){
		temp = (struct ngram **)realloc( data->ngrams, 2 * (*data).capacity * sizeof (struct ngram *) );
		if( temp == NULL )
			exit(EXIT_FAILURE);
		data->ngrams = temp;
		(*data).capacity *= 2;
	}
}

void realloc_sufixes( struct ngram *ngram ){
	struct sufix **temp = NULL;
	if( (*ngram).capacity == (*ngram).number ){
		temp = (struct sufix **)realloc( ngram->sufixes, 2 * (*ngram).capacity * sizeof (struct sufix *) );
		if( temp == NULL )
			exit(EXIT_FAILURE);
		ngram->sufixes = temp;
		(*ngram).capacity *= 2;
	}
}

char **rewrite_text_to_array( char *basefilename ){
	FILE *basefile;
	char string[50];
	register int i = 0;
	char **text;
	char **temp;
	int size = 20;
	basefile = fopen( basefilename, "r" );
	if( basefile == NULL )
		exit(EXIT_FAILURE);
	text = (char **)malloc( size * sizeof( char *) );
	i = 0;
	while( fscanf(basefile, "%s", string ) != EOF ){
		if( i == size ){
				if( (temp = (char **)realloc( text, 2 * size * sizeof (char *) ) ) == NULL ){
					free(text);
					printf( "Błąd alokacji pamięci\n" );
					exit(EXIT_FAILURE);
				}
		text = temp;
		size *= 2;
		}
		text[i] = (char *)malloc( (strlen(string) + 1) * sizeof (char) );
		strcpy( text[i], string );
		i++;
	}
	if( i == size ){
			if( (temp = (char **)realloc( text, (size + 1) * sizeof (char *) ) ) == NULL ){
				printf( "Błąd alokacji pamięci\n" );
				exit(EXIT_FAILURE);
			}
	text = temp;
	size += 1;
	}
	text[i] = (char *)malloc( (strlen("\0")) * sizeof (char) );
	strcpy( text[i], "\0" );
	number_of_words = i;
	return text;
}

struct ngram *find_ngram( char **text, struct data *data, int k ){
	int i, j;
	int flag;
	for( i = 0; i< (*data).number; i++ ){
		flag = 0;
		for( j = 0; j < n - 1; j++ ){
			if( strcmp( data->ngrams[i]->prefix[j], text[k+j] ) == 0){
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

void add_sufix( struct ngram *pointer, char *text ){
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
	pointer->sufixes[(*pointer).number]->sufix = (char *)malloc( ( strlen(text) + 1 ) * sizeof (char) );
	if( pointer->sufixes[(*pointer).number]->sufix == NULL )
		exit(EXIT_FAILURE);
	strcpy( pointer->sufixes[(*pointer).number]->sufix, text);
	(*pointer->sufixes[(*pointer).number]).occurance++;
	(*pointer).number++;
}


void add_ngram( struct data *data, char **text, int i ){
	int j = 0;
	realloc_data( data );
	if( data->ngrams[(*data).number] == NULL ){
		initialize_ngram( data, (*data).number );
	}
	
	for( j = 0; j < n - 1; j++ ){
		data->ngrams[(*data).number]->prefix[j] = (char *)malloc( ( strlen(text[i+j]) + 1 ) * sizeof (char) );
		if( data->ngrams[(*data).number]->prefix[j] == NULL )
			exit(EXIT_FAILURE);
		strcpy( data->ngrams[(*data).number]->prefix[j], text[i+j] );
	}
	
	if ( text[i+j] == NULL ){
		(*data->ngrams[(*data).number]).occurance = 1;
		(*data->ngrams[(*data).number]).number = 0;
	} else {
		add_sufix( data->ngrams[(*data).number], text[i+j] );
	}
	(*data).number += 1;
}



//ANALIZATOR
void process_ngrams( struct data *data, char **text ){
	//data - kontener z ngramami, text - wektor zawierający tekst
	struct ngram *pointer = NULL;
	register int i = 0;
	while( number_of_words - i >= n ){
		//sprawdzam czy ngram juz wystepuje
		pointer = find_ngram( text, data, i );//zwraca wskaźnik do występującego ngramu 
		//jeżeli ngram już jest lub NULL jeżeli go jeszcze nie ma
		if( pointer != NULL ){
			add_sufix( pointer, text[i+n-1] ); //dodaje sufix do istniejącego ngramu
		} else {
			add_ngram( data, text, i ); //dodaje nowy ngram
		}
		++i;
	}
}

void save_intermediate_file( struct data *data, char *filename ){
	register int i, j;
	FILE *intermediate;
	intermediate = fopen( filename, "w" );
	if ( intermediate == NULL )
		exit(EXIT_FAILURE);
	fprintf(intermediate, "Prefixes: \t%d \tWords: \t%d ", (*data).number, number_of_words );
	fprintf(intermediate, "N: %d\n", n );
	for( i = 0; i < (*data).number; i++ ){
		for( j = 0; j < n-1; j++ ){
			fprintf(intermediate, "%s ", data->ngrams[i]->prefix[j] );
		}
		fprintf(intermediate, " \t%lu \t%d:", (*data->ngrams[i]).occurance, (*data->ngrams[i]).number );
		for( j = 0; j < (*data->ngrams[i]).number; j++ ){
			fprintf(intermediate, " \t%s \t%lu, ", data->ngrams[i]->sufixes[j]->sufix, (*data->ngrams[i]->sufixes[j]).occurance );
		}
		fprintf(intermediate, "\n" );	
	}

	fclose(intermediate);
}

int process_data( char * basefile, struct data *data ){
	char **text;
	int i = 0;
	intermediate_filename = strdup( "intermediate_file" );
	n = get_number( "mark" );
	initialize_data( data );
	text = rewrite_text_to_array( basefile );
	process_ngrams( data, text );
	save_intermediate_file( data, intermediate_filename );

	return 0;

}
