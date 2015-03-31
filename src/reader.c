#include "reader.h"
#include "storage.h"
#include "statgen.h"
#include "argparser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>



static char **buf[2];
static int n_gram;


int init_buf_failed(int n) {

	int i, j;

	for(i = 0; i < 2; i++)
		if( ! (buf[i] = malloc( n * sizeof(char**)) ) )
			return 1;

	for(i = 0; i < 2; i++)
		for(j = 0; j < n; j++)
			if( ! (buf[i][j] = malloc( MAX_WORD_LENGTH * sizeof(char*)) ) )
				return 1;
	n_gram = n;

	return 0;
}

void free_buf() {

	int i, j;

	for(i = 0; i < 2; i++) {

		for(j = 0; j < n_gram; j++)
			free( buf[i][j] );
		free( buf[i] );
	}
}


void flush_buf(int buf_indx) {

	int i;

	if(buf_indx != 0 && buf_indx != 1)
		return;

	for(i = 0; i < n_gram; i++)
		buf[buf_indx][i][0] = '\0';

}


/*
   void print_buf(int buf_indx) {

   int i;

   if(buf_indx != 0 && buf_indx != 1)
   return;

   for(i = 0; i < n_gram; i++)
   printf("%s ", buf[buf_indx][i]);
   printf("\n");
   }
 */



int read(storage_t *storage, param_t p, stat_t *s) {

	int i, j, k;
	int wordc = -1;

	storage->tree = NULL;
	storage->v = NULL;


	if( init_buf_failed( p.n_gram ) )
		return 1; 

	for(k = 0; k < p.inputs; k++) {
		for(i = 0; i < n_gram; i++) // ZCZYTAJ PIERWSZY N_GRAM
			if ( (fscanf(p.input[k], "%s", buf[0][i]) != 1 ) ) {
				free_buf();
				close_files(p.input, p.inputs);
				exit(2);
			}

		do {
			i = ++wordc % 2; // indeks dla bufora na zmiane 0 lub 1;

			add_to_storage(storage, buf[i], n_gram); // TODO zrobic zeby zwracal cos jak zabraknie pamieci

			for( j = 1; j < n_gram; j++)
				strcpy(buf[ !i ][j-1], buf[i][j]); // negacja w celu korzystania z buforow na zmiane

			flush_buf( i );

		} while( (fscanf(p.input[k], "%s", buf[ !i ][n_gram-1]) ) == 1 );
	}

	close_files(p.input, p.inputs);
	free_buf();


	s->n_words_in = wordc + n_gram; // zapisz liczbe slow wczytanych

	return 0;
}



