#ifndef _INCL_ANALIZER
#define _INCL_ANALIZER


struct data{
        struct ngram **ngrams;
        int number;     //liczba ngramów
	int capacity;   //pojemność ngrams
};

struct ngram{
        char **prefix;
        long int occurance; //liczba wystąpień ngramu
        struct sufix **sufixes;
        int number;       //liczba sufiksów
	int capacity;     //pojemność sufixes
};

struct sufix{
        char *sufix;    //wyraz (sufix)
        long int occurance;  //liczba wystąpień
};

struct ngram *find_ngram( char **text, struct data *data, int k );
void add_sufix( struct ngram *pointer, char *text );
void add_ngram( struct data *data, char **text, int i );
void save_intermediate_file( struct data *data, char *filename );
int process_data( char * basefile, struct data *data );



#endif
