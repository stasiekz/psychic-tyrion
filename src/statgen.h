#ifndef _STATGEN_H_
#define _STATGEN_H_

#include "storage.h"
#include "argparser.h"
#include "statgen.h"


typedef struct {
	
	int n_words_in; // liczba slow w pliku wejsciowym
	int n_words_out; // liczba slow w pliku wyjsciowym
	int PMI;

} stat_t;

void gen_stat(storage_t *storage, param_t *p, stat_t *s);

#endif
