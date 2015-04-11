#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include "analizer.h"
#include "manager.h"


static int 	number_of_words;
static char *intermediate_filename;
static int  n;

void setN( int value ){
	n = value;
}

void save_intermediate_file( struct data *data, char *filename ){
	register int i, j;
	FILE *intermediate;
	intermediate = fopen( filename, "w" );
	if ( intermediate == NULL )
		exit(EXIT_FAILURE);
	fprintf(intermediate, "%d\n", n);
	for( i = 0; i < (*data).number; i++ ){
		j = 0;
		for( j = 0; j < n-1; j++ ){
			fprintf(intermediate, "%s ", data->ngrams[i]->prefix[j] );
			j++;
		}
		fprintf(intermediate, "%d %d ", data->ngrams[i]->occurance, data->ngrams[i]->number );
		for( j = 0; j < (*data->ngrams[i]).number; j++ ){
			fprintf(intermediate, "%s ", data->ngrams[i]->sufixes[j] );
		}
		fprintf(intermediate, "\n" );
	}

	fclose(intermediate);
}

/*
char* readFile(char* filename)
{
    FILE* file = fopen(filename,"r");
    if(file == NULL)
    {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long int size = ftell(file);
    rewind(file);

    char* content = calloc(size + 1, 1);

    fread(content,1,size,file);

    return content;
}
*/
char *trimwhitespace(char *str)
{
	char *end;

	// Trim leading space
	while(isspace(*str)) str++;

	if(*str == 0)  // All spaces?
		return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)) end--;

	// Write new null terminator
	*(end+1) = 0;

	return str;
}

char **rewrite_text_to_array( char *basefilename ){
	int i = 0;
	long int size = 0;
	char *content = NULL;
	char *string = NULL;
	char **text = NULL;
	FILE* file = fopen(basefilename,"r");
	assert(file != NULL);
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	rewind(file);
	if( size == 0) {
		fprintf(stderr, "Plik %s nie zawiera tekstu", basefilename);
		text = NULL;
		return text;
	}
	content = calloc(size + 1, 1);

	fread(content,1,size,file);
	fclose(file);
	text = (char **)malloc( (size + 1) * sizeof( char *) );
	i = 0;
	string = strtok (content, " \n\r\t");
	while (string != NULL)
	{
		trimwhitespace( string );
		text[i] = strdup( string );
		string = strtok (NULL, " \n\r\t");
		i++;
	}
	number_of_words = i;
	return text;
}


void initialize_ngram( struct data *ng ){
	struct ngram *tmp = NULL;
	if( ng->ngrams[ng->number] != NULL )
		return;
	tmp = malloc( sizeof *tmp );
	assert( tmp != NULL );
	tmp->prefix = malloc( (n-1) * sizeof *tmp->prefix );
	assert( tmp->prefix != NULL );
	tmp->sufixes = malloc( 10 * sizeof *tmp->sufixes);
	assert( tmp->sufixes != NULL );
	tmp->capacity = 10;
	tmp->number = 0;
	tmp->occurance = 0;
	ng->ngrams[ng->number] = tmp;
}

void initialize_data( struct data *data ){
	int i = 0;
	struct ngram **tmp;
	tmp = malloc( 20 * sizeof *tmp);
	assert( tmp != NULL );
	data->ngrams = tmp;
	data->number = 0;
	data->capacity = 20;
}

void realloc_data( struct data *data ){
	struct ngram **temp = NULL;
	int i = 0;
	if( data->number == data->capacity ){
		data->capacity *= 2;
		temp = realloc( data->ngrams, data->capacity * sizeof *temp );
		assert( temp != NULL );
		data->ngrams = temp;
		for( i = data->number; i < data->capacity; i++ )
			data->ngrams[i] = NULL;
	}
}

void realloc_sufixes( struct ngram *ngram ){
	char **temp = NULL;
	if( (*ngram).capacity == (*ngram).number ){
		ngram->capacity *= 2;
		temp = realloc( ngram->sufixes, ngram->capacity * sizeof *temp );
		if( temp == NULL )
			exit(EXIT_FAILURE);
		ngram->sufixes = temp;
	}
}

struct ngram *find_ngram( char **text, struct data *data, int k ){
	register int i, j;
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
	realloc_sufixes( pointer );
	pointer->sufixes[(*pointer).number] = strdup( text );
	(*pointer).number++;
}

void add_prefix( struct ngram *pointer, char **text, int position ){
	register int i = 0;
	for( i = 0; i < n - 1; i++ ){
		pointer->prefix[i] = strdup( text[i+position] );
	}
	pointer->occurance = 1;
}

void add_ngram( struct data *data, char **text, int position ){
	register int i = 0;
	int sufix_pos = n - 1;
	initialize_ngram( data );
	add_prefix( data->ngrams[data->number], text, position );
	if ( text[position + sufix_pos] != NULL ){
		add_sufix( data->ngrams[data->number], text[position+sufix_pos] );
	}
	data->number += 1;
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
			realloc_data( data );
			add_ngram( data, text, i ); //dodaje nowy ngram
		}
		++i;
	}
}

int process_data( char * basefile, struct data *data ){
	char **text;
	n = get_number( "mark" );
	if( data->ngrams == NULL)
		initialize_data( data );
	text = rewrite_text_to_array( basefile );
	if( text == NULL )
		return 1;
	process_ngrams( data, text );

	return 0;

}
