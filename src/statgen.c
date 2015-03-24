#include "statgen.h"
#include "storage.h"
#include "reader.h"
#include "argparser.h"
#include "textgen.h"

#include <stdio.h>


void gen_stat(storage_t storage, param_t p, stat_t s) {

	printf("LICZBA WYRAZOW W PLIKU WEJSCIOWYM %d \n", s.n_words_in);
	printf("LICZBA WEZLOW W DRZEWIE %d \n\n\n", storage.v->n_nodes );

}
