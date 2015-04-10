#ifndef _INCL_ANALIZER
#define _INCL_ANALIZER


struct data{
        struct ngram **ngrams;
        int number;     //liczba ngramów
	    int capacity;   //pojemność ngrams
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

#endif
