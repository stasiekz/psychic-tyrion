#include "storage.h"
#include "argparser.h"
#include "statgen.h"
#include "textgen.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void add_buf( char **buf, char **pref, char *suf, int ngram ){
	int i;
	for (i = 1; i < ngram-1; i++){
		buf[i-1] = pref[i];
	}
	buf[i-1] = suf;
}



void gen_text(storage_t storage, param_t p, stat_t *s) {

	srand( time(NULL) );

	int i;
	int n = 0; // zmienna do liczenia liczby slow wydrukowanych
	int parag = p.n_words / p.n_parag+1;
	int prefix_size = p.n_gram == 1 ? 1 : p.n_gram-1;
	char **buf = malloc (prefix_size*sizeof*buf);
	char *suffix;
	node_t *node = NULL;


	while( n < p.n_words ) {

		if( n % parag == 0 ) fprintf(p.output, n == 0 ? "\t" : "\n\t");

		if( !node ) { // nie ma takiego ngramu(prefixu)
			node = storage.v->n[ rand()%storage.v->n_nodes ]; // losuj wezel drzewa

			if( p.n_gram == 1) { // unigram

				fprintf(p.output, "%s ", node->d->prefix[0]); // wypisz prefix
				node = NULL;
				n++;
				continue;

			} else {
				for( i = 0; i < prefix_size; i++)
					fprintf(p.output, "%s ", node->d->prefix[i]); // wypisz prefix
				fprintf(p.output, "%s ", suffix = node->d->suffix[ rand()%node->d->n_suff ] ); // losuj i wypisz suffix
				n += p.n_gram; //TODO liczba wyrazow
				add_buf(buf, node->d->prefix, suffix, p.n_gram); // dodaj dane do bufora
			}

		} else {

			fprintf(p.output, "%s ", suffix = node->d->suffix[ rand()%node->d->n_suff ] ); // losuj i wypisz suffix
			add_buf(buf, node->d->prefix, suffix, p.n_gram);
			n++;
		}

		node = lookup_tree(storage.tree, buf, p.n_gram);
	}
	fprintf(p.output, "\n");

	free(buf);
	fclose(p.output);
}

