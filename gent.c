
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gent.h"
#include "analizer.h"
#include "manager.h"
//#include "gens.h"
//#include "reader.h"
//#include "merger.h"

static int n;

struct ngram *rand_prefix(struct data *data){
	return data->ngrams[ (int)((*data).number * 1.0 * rand()/RAND_MAX) ];
}

struct sufix *rand_sufix(struct ngram *ngram){
	return ngram->sufixes[ (int)((*ngram).number * 1.0 * rand()/RAND_MAX) ];
}

struct ngram *next_ngram(char **prefix, struct data *data){
	struct ngram *tmp = find_ngram( prefix, data, 1 );
	if( tmp != NULL)
		return tmp;
	else
		return NULL;
}

void generate(struct data *data){
	int i;
	int n = get_number( "mark" );
	char **string;
	struct ngram *prefix;
	struct sufix *sufix;
	int number_of_words = get_number( "words" );
	char *out = strdup( get_name( "out" ) );
	int wrote = 0;
	FILE *output;
	output = fopen( out, "w" );
	srand(time(NULL));
	string = (char **)malloc(n * sizeof (char *) );
	
		prefix = rand_prefix(data);
		for( i = 0; i < n - 1; i++){
			if( prefix->prefix != NULL ){
				string[i] = strdup( prefix->prefix[i] );
			}
		}
		sufix = rand_sufix( prefix );
		string[n - 1] = strdup( sufix->sufix );
		for( i = 0; i < n; i++ )
				fprintf(output, "%s ", string[i] );
		prefix = next_ngram( string, data );
		number_of_words -= n;
		wrote += n;
	
		while(number_of_words--){
			if( wrote % 8 == 0 )
				fprintf( output, "\n" );
			for( i = 1; i < n; i++ ){
				string[i - 1] = strdup( string[ i ] );
			}
			sufix = rand_sufix( prefix );
			string[n - 1] = strdup( sufix->sufix );
			fprintf(output, "%s ", string[n - 1] );
			prefix = next_ngram( string, data );
			if( prefix == NULL ){
				prefix = rand_prefix(data);
				for( i = 0; i < n - 1; i++ )
					string[i] = strdup( prefix->prefix[i] );
			}
			wrote += 1;
		}
	fclose(output);
}