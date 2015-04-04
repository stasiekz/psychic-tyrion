#ifndef _SERIALIZER_H_
#define _SERIALIZER_H_

#include "storage.h"
#include "argparser.h"
#include <stdio.h>


void fwrite_data(FILE *out, data_t *d, param_t p);

void serialize_data(FILE *out, tree_t t, param_t p);

void serialize_connections(FILE *out, tree_t t, param_t p);

void serialize_storage(FILE *out, storage_t s, param_t p);

void deserialize_storage(FILE *in, storage_t *s, param_t *p);

data_t * fread_data(FILE *in, data_t *d, param_t *p);

void deserialize_data(FILE *in, node_t **n, param_t *p, storage_t *s);

void deserialize_connections(FILE *in, storage_t *s, param_t *p);


#endif
