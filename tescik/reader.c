#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "reader.h"

#include "datadef.h"
#include "utree.h"




static char **buf[2];
static int n_gram;

int main(int argc, char **argv) {


	FILE *in = fopen(argv[1], "r");

	printf("\n\nLICZBA SLOW : %d\n\n", read(in, atoi(argv[2])));

	fclose(in);

	return 0;
}


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


void print_buf(int buf_indx) {

	int i;

	if(buf_indx != 0 && buf_indx != 1)
		return;

	for(i = 0; i < n_gram; i++)
		printf("%s ", buf[buf_indx][i]);
	printf("\n");
}



int read(FILE *in, int n) { ///////////////////////////// USTALIC WARTOSCI ZWRACANE

	int i, j;
	int wordc = -1;

	tree_t tree = NULL;


	if( !in ) 
		return 0; // ????????????

	if( n < 1 )
		return 0; // ?????????????

	if( init_buf_failed( n ) )
		return 0; // ???????

	for(i = 0; i < n_gram; i++) // ZCZYTAJ PIERWSZY N_GRAM
		if ( (fscanf(in, "%s", buf[0][i]) != 1 ) )
			return 0;	///// ????

	do {
		i = ++wordc % 2; // indeks dla bufora na zmiane 0 lub 1;
		///////////////////////////////print_buf( i ); 
		tree = insert(tree, buf[i], n_gram);
		for( j = 1; j < n_gram; j++)
			strcpy(buf[ !i ][j-1], buf[i][j]); // negacja w celu korzystania z buforow na zmiane

		flush_buf( i );

	} while( (fscanf(in, "%s", buf[ !i ][n_gram-1]) ) == 1 );

	free_buf();

	print_tree(tree, n_gram); // ??????????????????
	free_tree(tree, n_gram);

	return wordc + n_gram; // zwroc liczbe wczytanych slow
}



