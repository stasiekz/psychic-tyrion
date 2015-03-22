#ifndef  _STORAGE_H_
#define  _STORAGE_H_

typedef struct {
	char *suffix;
	int occurr;
} suff_t;

typedef struct {
	char **prefix;
	suff_t **suffix;

	/*
	   int *index;
	   int total_suff;
	   int cap_index;
	 */

	int n_suff;
	int cap_suff;

} data_t;


typedef struct node {

	data_t *d;
	struct node *left, *right;

} node_t, *tree_t;

typedef struct { // dynamiczna tablica wskaznikow na wezly drzewa

	node_t **n;
	int n_nodes;
	int cap_nodes;
} node_vec;


typedef struct {

	tree_t tree;
	node_vec *v;

} storage_t;

void add_to_storage(storage_t *storage, char **buf, int ngram);

void free_storage(storage_t *storage, int ngram);


node_vec *add_to_node_vec( node_vec *v, node_t *node);

void free_node_vec(node_vec *v);


tree_t insert( tree_t t, node_vec **v, char **buf, int ngram);

node_t *lookup_tree(tree_t t, char **buf, int ngram);

/* pomocniczna
 */
void print_tree(tree_t t, int ngram);

void free_tree(tree_t t, int ngram);

void insert_suffix(data_t *data, char **buf, int ngram);

int cmp_data( data_t *data, char **buf, int ngram);

data_t * create_data(char **buf, int ngram);

void free_data(data_t *data, int ngram);



#endif
