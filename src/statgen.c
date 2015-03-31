#include "statgen.h"
#include "storage.h"
#include "reader.h"
#include "argparser.h"
#include "textgen.h"

#include <stdio.h>


void gen_stat(storage_t storage, param_t p, stat_t s) {

	if(p.gen_stat) {

		fprintf(p.stat_file, "\n\n\nLICZBA WYRAZOW W PLIKU WEJSCIOWYM %d \n", s.n_words_in);
		fprintf(p.stat_file, "\n\n\nLICZBA PREFIXOW W PLIKU WEJSCIOWYM %d \n\n\n", storage.v->n_nodes );
		fprintf(p.stat_file, "\n\n\nLICZBA %d-gramow W PLIKU WEJSCIOWYM %d \n\n\n", p.n_gram, s.n_words_in-(p.n_gram-1) );

		fclose(p.stat_file);
	}


}
