#ifndef _READER_H_
#define _READER_H_

#define MAX_WORD_LENGTH 1024

#include "storage.h"
#include "argparser.h"
#include "statgen.h"

/*
   przydziela pamiec dla bufora do
   zczytywania slow.
   MAX dlugość słowa MAX_WORD_LENGTH
   Zwraca 1 w przypadku niepowodzenia
   Zwraca 0 w przypadku powodzenia
 */
int init_buf_failed(int n);

/*
   Zwalnia pamiec bufora
 */
void free_buf();

/*
   Czysci czesc bufora buf_indx =  0 lub 1 
 */
void flush_buf(int buf_indx);

/*
pomocnicza
*/
void print_buf(int buf_indx);

int read(storage_t *storage, param_t *p, stat_t *s);

#endif
