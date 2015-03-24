#include "storage.h"
#include "argparser.h"
#include "statgen.h"
#include "textgen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void gen_text(storage_t storage, param_t p, stat_t *s) {

//	print_tree(storage->tree, p->n_gram);


	srand( time(NULL) );
	int i = rand() % storage.v->n_nodes;
//	int n = p.n_gram;

/*
	do {

	} while ( n < p.n_words );
*/

	print_tree(storage.v->n[i], p.n_gram);


}
