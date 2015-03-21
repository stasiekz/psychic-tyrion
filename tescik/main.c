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
	tree_t tree = NULL;
	
	parse_args(argc, argv, &p); // zczytaj argumenty do struktury parametrow
	read(tree, &p, &stat); // wczytaj pliki tekstowe i zapisz dane do drzewa + dodaj dane statystyczne
	// gen_text(tree, &p, &stat);  generuj tekst + dodaj dane stat.
	//gen_stat(tree, &p, &stat);  generuj statystyke

	free_tree(tree, p.n_gram); // zwalania pamiec po drzewie
	print_tree(tree, p.n_gram);
	
	printf("LICZBA WYRAZOW W PLIKU WEJSCIOWYM %d \n", stat.n_words_in); 

	
	return 0; // ?????

}
