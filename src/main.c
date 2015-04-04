#include "storage.h"
#include "reader.h"
#include "argparser.h"
#include "textgen.h"
#include "statgen.h"
#include "serializer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



char *usage = 	"\n\n********** GENERATOR TEKSTU **********\n"
		"SPOSOB UZYCIA :                       \n\n"
		"./markowgen -f [FILE...] [OPTIONS]\n\n"
		"[OPTIONS]:\n\n"
		"\t-w, --nwords=N\n\n"
		"\t\tgeneruje tekst wyjściowy składający się z N słów. Wartość domyślna N=100.\n"
		"\t-p, --nparag=P\n\n"
		"\t\tgeneruje tekst wyjściowy o P akapitach.Wartość domyślna P=4.\n"
		"\t-n, --n-gram=N\n\n"
		"\t\tgeneruje tekst na podstawie N-gramów.Wartość domyślna N=2.\n"
		"\t-s, --stat=FILE\n\n"
		"\t\tgeneruje statystykę tekstów we-wy do pliku FILE. Domyślnie FILE=stdout.\n"
		"\t-o, --output=FILE\n\n"
		"\t\tzapisuje wygenerowany tekst do pliku FILE. Domyślnie FILE=stdout.\n"
		"\t-h, --help\n\n"
		"\t\twyświetla sposób użytkowania programu.\n"
		"\t-b, --basefile=BASEFILE\n\n"
		"\t\twczytuje dane z pliku bazowego.\n\n";

int main(int argc, char **argv) {

	param_t p;
	stat_t stat;
	storage_t storage;

	/* zczytaj argumenty do struktury parametrow 
	 */	
	parse_args(argc, argv, &p);

	if(argc == 1  ) {
		printf("%s\n", usage);
		return 2;
	}


	/* wczytaj pliki tekstowe i zapisz 
	   dane do drzewa + dodaj dane statystyczne
	 */
	if( p.read_base ) {
		if ( deserialize_storage(&storage, &p) ) {
			fprintf(stderr, "%s: Bledny plik posredni\n", argv[0]);
			return 1;
		}
	}
	else {
		read(&storage, p, &stat);

		if( p.make_base )
			serialize_storage(storage, p);
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
