#include "storage.h"
#include "argparser.h"
#include "statgen.h"
#include "textgen.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

void gen_text(storage_t *storage, param_t *p, stat_t *s) {

//	print_tree(storage->tree, p->n_gram);


	srand( time(NULL) );
	int i = rand() % storage->v->n_nodes;

	//printf("%s \n\n\n", storage->v->n[i]->d->prefix[0]);
	printf("LICZBA WEZLOW W DRZEWIE %d \n\n\n", storage->v->n_nodes );
	print_tree(storage->v->n[i], p->n_gram);


	

	

}
