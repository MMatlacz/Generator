
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "gent.h"
#include "analizer.h"
#include "manager.h"




static int n = 0;

struct ngram *rand_prefix(struct data *data){
	return data->ngrams[ (int)((*data).number * 1.0 * rand()/RAND_MAX) ];
}

char *rand_sufix(struct ngram *ngram){
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
	register int i;
	n = get_number( "mark" );
	char **string;
	struct ngram *prefix;
	char *sufix;
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
		string[n - 1] = strdup( sufix );
		string[0][0] = (char) toupper( string[0][0] );
		fprintf(output, "%s ", string[0] );
		for( i = 1; i < n; i++ )
				fprintf(output, "%s ", string[i] );
		prefix = next_ngram( string, data );
		if( prefix == NULL ){
				prefix = rand_prefix(data);
				for( i = 0; i < n - 1; i++ )
					string[i] = strdup( prefix->prefix[i] );
		}
		number_of_words -= n;
		wrote += n;
	
		while(number_of_words--){
			if( wrote % 8 == 0 )
				fprintf( output, "\n" );
			for( i = 1; i < n; i++ ){
				string[i - 1] = strdup( string[ i ] );
			}
			sufix = rand_sufix( prefix );
			string[n - 1] = strdup( sufix );
			fprintf(output, "%s ", string[n - 1] );
			prefix = next_ngram( string, data );
			if( prefix == NULL ){
				prefix = rand_prefix(data);
				for( i = 0; i < n - 1; i++ )
					string[i] = strdup( prefix->prefix[i] );
			}
			wrote += 1;
		}
		if( string[n-1][strlen(string[n-1]) - 1] != '.' )
			fprintf(output, "%c ", '.' );
	fclose(output);
}