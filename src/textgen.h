#ifndef _TEXTGEN_H_
#define _TEXTGEN_H_

#include "storage.h"
#include "argparser.h"
#include "statgen.h"

/* generuj tekst zgodnie z parametrami w strukturze p
   na podstawie danych w storage +
   zapisz dane statystyczne do struktury s
 */
void gen_text(storage_t storage, param_t p, stat_t *s);


#endif
