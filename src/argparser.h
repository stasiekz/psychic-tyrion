#ifndef _ARGPARSER_H_
#define _ARGPARSER_H_

#include "stdio.h"

#define DEFAULT_NUMBER_OF_WORDS 100
#define DEFAULT_NUMBER_OF_PARAGRAPHS 6
#define DEFAULT_N_GRAM 2

typedef struct {
	
	int n_words; // liczba slow do wygenerowania
	int n_parag; // liczba paragrafow do wygenerowania
	int n_gram;
	int show_help; // zmienna logiczna do wyswietlania helpa
	int gen_stat; // zmienna logiczna do wyswietlania statystyk
	int read_base;
	int make_base;

	FILE **input; //  bedzie tablica uchwytow do plikow wejsciowych
	int inputs; // liczba plikow wejsciowych
	FILE *base_file; // ew. plik przechodni
	FILE *output; // plik do wygenerowania tekstu wyjsciowego
	FILE *stat_file; // ew. plik do wygenerowania statystyki

} param_t;

int parse_args(int argc, char **argv, param_t *s);

void close_files( FILE **f, int n);

#endif
