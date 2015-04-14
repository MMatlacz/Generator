
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "gent.h"
#include "manager.h"




static int n = 0;
static int numberOfWords;
static int numberOfParagraphs;
static int wordsInParagraph;
static int lefOvers;
static int wrote;

struct ngram *randPrefix(struct data *data){
	return data->ngrams[ (int)((*data).number * 1.0 * rand()/RAND_MAX) ];
}

char *randSufix(struct ngram *ngram){
	return ngram->sufixes[ (int)((*ngram).number * 1.0 * rand()/RAND_MAX) ];
}

struct ngram *nextNgram(char **prefix, struct data *d){
	struct ngram *tmp = find_ngram( prefix, d, 1 );
	if( tmp != NULL)
		return tmp;
	else
		return NULL;
}

void copyPrefix( struct ngram *p, char **s ){ //zwraca liczbę skopiowanych wyrazów
	int i;
	for(i = 0; i < n - 1; i++) {
		s[i] = strdup( p->prefix[i] ); //przekopiuj prefix do stringa
	}
}

void generate(struct data *data){
	register int i;
	char **string;
	struct ngram *prefix;
	char *sufix;
	char *out;
	int wordsInParagraphCounter;
	out = strdup( get_name( "out" ) );
	numberOfWords = get_number( "w" );
	numberOfParagraphs = get_number( "p" );
	wordsInParagraph = numberOfWords / numberOfParagraphs;
	lefOvers = numberOfWords - numberOfParagraphs * wordsInParagraph;
	FILE *output;
	output = fopen( out, "w" );
	srand(time(NULL));
	string = malloc(n * sizeof *string );
	n = get_number("m");
	while( numberOfParagraphs-- ) {
		if (numberOfParagraphs == 0) {
			wordsInParagraph += lefOvers;
		}
		wordsInParagraphCounter = wordsInParagraph;
		fprintf(output, "\t");
		while(wordsInParagraphCounter--) {
			//jeżeli nie ma jescze żadnych wypisanych wyrazów to wylosuj pierwszy
			if (wrote == 0) {
				prefix = randPrefix(data);
				copyPrefix(prefix, string);
				string[0][0] = toupper(string[0][0]); //Zmien pierwsza litere na wielka
				for (i = 0; i < n - 1; i++) {
					fprintf(output, "%s ", string[i]); //Wypisz prefix
					wrote++;
					if (wrote % 8 == 0) { //Wypisuj tylko 8 wyrazów w wierszu
						fprintf(output, "\n");
					}
				}
				wordsInParagraphCounter -= n - 1;
			} else {
				//Jeżeli wypisano już jakieś słowa to znajdź kolejny ngram
				prefix = nextNgram(string, data);
				if (prefix == NULL) {
					prefix = randPrefix(data);
					copyPrefix(prefix, string);
					string[0][0] = toupper(string[0][0]); //Zmien pierwsza litere na wielka
					for (i = 0; i < n - 1; i++) {
						fprintf(output, "%s ", string[i]); //Wypisz prefix
						wrote++;
						if (wrote % 8 == 0) { //Wypisuj tylko 8 wyrazów w wierszu
							fprintf(output, "\n");
						}
					}
					wordsInParagraphCounter -= n - 1;
				}
			}

			sufix = randSufix(prefix); //znajdz odpowiadajacy sufix
			string[n - 1] = strdup(sufix);
			fprintf(output, "%s ", string[n - 1]); //wypisz sufix
			wrote++;
			wordsInParagraphCounter--;
			if (wrote % 8 == 0) { //Wypisuj tylko 8 wyrazów w wierszu
				fprintf(output, "\n");
			}
			for (i = 1; i < n; i++) {
				string[i - 1] = strdup(string[i]);
			}
		}
		if( string[n-1][strlen(string[n-1]) - 1] != '.' )
			fprintf(output, "." );
		if( numberOfParagraphs > 0)
			fprintf(output, "\n");
		wrote = 0;
	}


	fclose(output);
}