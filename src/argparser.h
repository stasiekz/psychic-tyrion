#ifndef _ARGPARSER_H_
#define _ARGPARSER_H_

#include "stdio.h"

typedef struct {
	
	int n_words; // liczba slow do wygenerowania
	int n_parag; // liczba paragrafow do wygenerowania
	int n_gram;
	int help; // zmienna logiczna do wyswietlania helpa

	FILE *input; //  bedzie tablica uchwytow do plikow wejsciowych
	int n_inputs; // liczba plikow wejsciowych
	FILE *base_file; // ew. plik przechodni
	FILE *output; // plik do wygenerowania tekstu wyjsciowego
	FILE *stat_file; // ew. plik do wygenerowania statystyki

} param_t;

int parse_args(int argc, char **argv, param_t *s);

#endif
