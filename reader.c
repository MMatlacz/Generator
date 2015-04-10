//
// Created by marcin on 4/10/15.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analizer.h"
#include "reader.h"

static int n = 0;
static FILE *file = NULL;

void read( char *ifile, struct data *d ){
    int i = 0;
    long int size = 0;
    char *content = NULL;
    char *string = NULL;
    char **text = NULL;
    file = fopen(ifile,"r");
    if(file == NULL)
    {
        return;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    content = calloc(size + 1, 1);

    fread(content,1,size,file);
    text = (char **)malloc( (size + 1) * sizeof( char *) );
    i = 0;
    string = strtok (content, " \n");
    while (string != NULL)
    {
        text[i] = strdup( string );
        i++;
        string = strtok (NULL, " \n");
    }
    FILE *out = fopen( "test", "w" );
    while( i-- )
        fprintf( out, "%s", text[i-1] );
    fclose( out );
    return;

}

