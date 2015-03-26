#include "storage.h"
#include "argparser.h"
#include "statgen.h"
#include "textgen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void gen_text(storage_t storage, param_t p, stat_t *s) {

	//		print_tree(storage.tree, p.n_gram);



	srand( time(NULL) );

	int n = p.n_gram; // zmienna do liczenia liczby slow wydrukowanych
	int i, j, r;
	int prefix_size = p.n_gram == 1 ? 1 : p.n_gram-1;
	node_t *node = NULL;
	char **buf = malloc (p.n_gram*sizeof*buf);


	fprintf(p.output, "\t");
	// unigram 
	if( p.n_gram == 1 ) { // losuj prefixy z drzewa i drukuj do konca
		while ( n < p.n_words ) {
			r = rand() % storage.v->n_nodes; // losuj indeks dla wezla
			fprintf(p.output, "%s ", storage.v->n[r]->d->prefix[0]); // drukuj pierwszy prefix
			n++;
		}
	} else {

		r = rand() % storage.v->n_nodes; // losuj indeks dla wezla 
		for(i = 0; i < prefix_size; i++)
			fprintf(p.output, "%s ", storage.v->n[r]->d->prefix[i]); // drukuj pierwszy prefix

		j = rand() % storage.v->n[r]->d->n_suff; // losuj pierwszy suffix
		fprintf(p.output, "%s ", storage.v->n[r]->d->suffix[j]);



		for(i = 1; i < prefix_size; i++) // kopiuj prefix od poczatku bufora
			buf[i-1] = storage.v->n[r]->d->prefix[i];
		buf[prefix_size] = storage.v->n[r]->d->suffix[j]; // kopiuj suffix na przed ostatnia

		while( n < p.n_words ) {

			node = lookup_tree(storage.tree, buf, p.n_gram);

			if( node == NULL ) {
				r = rand() % storage.v->n_nodes; // losuj indeks dla wezla
				j = rand() % storage.v->n[r]->d->n_suff;
				fprintf(p.output, "%s ", storage.v->n[r]->d->suffix[j]);


				for(i = 1; i < prefix_size; i++) // kopiuj prefix od poczatku bufora
					buf[i-1] = storage.v->n[r]->d->prefix[i];
				buf[prefix_size] = storage.v->n[r]->d->suffix[j]; // kopiuj suffix na przed ostatnia
			} else {
				j = rand() % node->d->n_suff;
				fprintf(p.output, "%s ", node->d->suffix[j]);


				for(i = 1; i < prefix_size; i++) // kopiuj prefix od poczatku bufora
					buf[i-1] = storage.v->n[r]->d->prefix[i];
				buf[prefix_size] = storage.v->n[r]->d->suffix[j]; // kopiuj suffix na przed ostatnia
			}

			n++;
		}
	}

	free(buf);
}

