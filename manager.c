#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gent.h"
#include "analizer.h"
#include "manager.h"
//#include "gens.h"
#include "reader.h"


static char 	*name_text_out = NULL; 
static char 	*name_stats_in = NULL; 
static char 	*name_stats_out = NULL;
static int 		number_of_words 		= 100;
static int 		number_of_paragraphs 	= 1;
static int 		mark 					= 2;
static char 	*base_files[5];
static char 	*intermediate_files[5];

char *get_name( char *name ){
	if( !strcmp( name, "out" ) )
		return name_text_out;
	else if( !strcmp( name, "stats") )
		return name_stats_in;
	else
		return "/0";
}

int get_number( char *name ){
	if( !strcmp( name, "words" ) )
		return number_of_words;
	else if( !strcmp( name, "paragraphs" ) )
		return number_of_paragraphs;
	else if( !strcmp( name, "mark" ) ) 
		return mark;
}
				

void print_instruction(){
	printf(		"Sposób uruchomienia\n"
				"./generator [-b nazwa pliku] [-p nazwa plik] <opcje>\n"
				"-b\t użytkownik podaje tekst bazowy (lub kilka)\n"
				"-p\t uzytkownik podje plik pośredni (lub kilka)\n"
				"-g\t <nazwa_generowanego_tekstu>\n"
				"-s\t <liczba_słów>\n"
				"-a\t <liczba akapitów>\n"
				"-stat\t <nazwa_generowanych_statystyk>\n"
				"-n\t rząd ngramów > 1\n"
				"[]\t - argumenty obowiązkowe\n"
				"<>\t - argumenty opcjonalne\n");
 	exit(EXIT_FAILURE);
}
int main(int argc, char const **argv){
	register int i;
	int it = 1;
	struct data data;
	data.ngrams = NULL;
	data.number = 0;
	data.capacity = 0;
	if( argc < 2){
		print_instruction();
		return 1;
	}
	while( it < argc ){
		if( !strcmp( argv[it], "-b" ) ){
			i = 0;
			while( it + 1 < argc && argv[it + 1][0] != '-' && i < 5){
				base_files[i] = strdup( argv[it + 1] );
				i++;
				it += 1;
			}
			if( !strcmp( argv[it], "-b" ) ){
				exit(EXIT_FAILURE);
			}
			it += 1;
		} else if( !strcmp( argv[it], "-p" ) ){
			i = 0;
			while( it + 1 < argc && argv[it + 1][0] != '-' && i < 5){
				intermediate_files[i] = strdup( argv[it + 1] );
				i++;
				it += 1;
			}
			if( !strcmp( argv[it], "-b" ) ){
				exit(EXIT_FAILURE);
			}
			it += 1;
		} else if( !strcmp( argv[it], "-g" ) ){
			if( it + 1 < argc && argv[it + 1][0] != '-' ){
				name_text_out =  strdup( argv[it + 1] );
				it += 2;
			} else {
				name_text_out = (char *)malloc( (strlen("book") + 1) * sizeof (char) );
				if( name_text_out == NULL )
					exit(EXIT_FAILURE);
				strcpy( name_text_out, "book" ); 
				it += 1;
			}
		} else if(  !strcmp( argv[it], "-s" ) ){
			if( it + 1 < argc && argv[it + 1][0] != '-' ){
				number_of_words =  atoi( argv[it + 1] );
				it += 2;
			} 
		} else if( !strcmp( argv[it], "-a" ) ){
			if( it + 1 < argc && argv[it + 1][0] != '-' ){
				number_of_paragraphs =  atoi( argv[it + 1] );
				it += 2;
			} 
		} else if( !strcmp( argv[it], "-stat" ) ){
			if( it + 1 < argc && argv[it + 1][0] != '-' ){
				name_stats_in =  strdup( argv[it + 1] );
				it += 2;
			} else {
				name_stats_in = (char *)malloc( (strlen("book") + strlen("_stat") + 1) * sizeof (char) );
				if( name_stats_in == NULL )
					exit(EXIT_FAILURE);
				strcpy( name_stats_in, name_text_out ); 
				strcat( name_stats_in, "_stat");
				it += 1;
			}
		} else if(  !strcmp( argv[it], "-n" ) ){
			if( it + 1 < argc && argv[it + 1][0] != '-' ){
				mark =  atoi( argv[it + 1] );
				if(mark <= 1){
					printf("Za niski rząd ngramów\n");
					exit(EXIT_FAILURE);
				}
				it += 2;
			} 
		} else {
			break;
		}
	}
	i = 0;
	while( base_files[i] != NULL ){
		process_data( base_files[i], &data );
		i++;
	}
	i = 0;
	while( intermediate_files[i] != NULL ){
		read( intermediate_files[i], &data );
		i++;
	}
	if(name_text_out != NULL)
		generate( &data );
	if(name_stats_in != NULL){
		//generate_stats( data );
	}
	if(name_stats_out != NULL){
		free(data.ngrams);
		process_data( name_text_out, &data );
		//generate_stats( data );
	}
	free(data.ngrams);
	return 0;
}