#include "statgen.h"
#include "storage.h"
#include "reader.h"
#include "argparser.h"
#include "textgen.h"

#include <stdio.h>


void gen_stat(storage_t storage, param_t p, stat_t s) {

	if(p.gen_stat) {

		fprintf(p.stat_file, "\n\n\nLICZBA WYRAZOW W PLIKU WEJSCIOWYM %d \n", s.n_words_in);
		fprintf(p.stat_file, "\n\n\nLICZBA WEZLOW W DRZEWIE %d \n\n\n", storage.v->n_nodes );

		fclose(p.stat_file);
	}


}
