#ifndef _INCL_ANALIZER
#define _INCL_ANALIZER


struct data{
        struct ngram **ngrams;
        int number;     //liczba ngramów
	    int capacity;   //pojemność ngrams
        int numberOfWords; //liczba wyrazów przetworzonych tekstów
        };

struct ngram{
        char **prefix;
        int occurance; //liczba wystąpień ngramu
        char **sufixes;
        int number;       //liczba sufiksów
	    int capacity;     //pojemność sufixes
};

struct ngram *find_ngram( char **text, struct data *data, int k );
void add_sufix( struct ngram *pointer, char *text );
void add_ngram( struct data *data, char **text, int i );
void save_intermediate_file( struct data *data, char *filename );
int process_data( char * basefile, struct data *data );
void initialize_data( struct data *data );
void realloc_data( struct data *data );
void initialize_ngram( struct data *ng );
char **rewrite_text_to_array( char *basefilename );
void setN( int value );
void add_prefix( struct ngram *pointer, char **text, int position );
void realloc_sufixes( struct ngram *ngram );
char *trimwhitespace(char *str);
void removeSpaces(char* source);

#endif
