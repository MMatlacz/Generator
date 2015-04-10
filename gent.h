#include "analizer.h"

#ifndef _INCL_GENERATOR
#define _INCL_GENERATOR


struct ngram *rand_prefix(struct data *data);

char *rand_sufix(struct ngram *ngram);

struct ngram *next_ngram(char **prefix, struct data *data);

void generate(struct data *data);





#endif