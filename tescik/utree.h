#ifndef _UTREE_H
#define _UTREE_H

#include "datadef.h"

#include <stdio.h>

typedef struct node {

	data_t *d;
	struct node *left, *right;

} node_t, *tree_t;

tree_t insert( tree_t t, char **buf, int ngram);


void print_tree(tree_t t, int ngram);

void free_tree(tree_t t, int ngram);

#endif
