#include "analizer.h"

#ifndef _INCL_GENERATOR
#define _INCL_GENERATOR


struct ngram *randPrefix(struct data *data);

char *randSufix(struct ngram *ngram);

struct ngram *nextNgram(char **prefix, struct data *data);

void generate(struct data *data);





#endif