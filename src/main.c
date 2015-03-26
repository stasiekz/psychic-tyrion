#include "storage.h"
#include "reader.h"
#include "argparser.h"
#include "textgen.h"
#include "statgen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *usage = "";

int main(int argc, char **argv) {

	param_t p;
	stat_t stat;
	storage_t storage;

	/* zczytaj argumenty do struktury parametrow 
	 */	
	parse_args(argc, argv, &p);


	/* wczytaj pliki tekstowe i zapisz 
	   dane do drzewa + dodaj dane statystyczne
	 */
	if ( read(&storage, p, &stat) ) {
		;	
	}
	/* generuj tekst + dodaj dane stat.
	 */
	gen_text(storage, p, &stat);
	/* generuj statystyke
	 */
	gen_stat(storage, p, stat);

	free_storage(&storage, p.n_gram);

	return EXIT_SUCCESS;

}
