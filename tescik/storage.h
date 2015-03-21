#ifndef  _STORAGE_H_
#define  _STORAGE_H_

typedef struct {
        char *suffix;
        int occurr;
} suff_t;

typedef struct {
        char **prefix;
        suff_t **suffix;
        int n_suff;
        int cap_suff;
} data_t;


typedef struct node {

	data_t *d;
	struct node *left, *right;

} node_t, *tree_t;




tree_t insert( tree_t t, char **buf, int ngram);

void print_tree(tree_t t, int ngram);

void free_tree(tree_t t, int ngram);

void insert_suffix(data_t *data, char **buf, int ngram);

int cmp_data( data_t *data, char **buf, int ngram);

data_t * create_data(char **buf, int ngram);

void free_data(data_t *data, int ngram);


#endif
