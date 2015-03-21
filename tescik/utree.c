#include "utree.h"
#include <stdlib.h>
#include <string.h>


tree_t insert( tree_t t, char **buf, int ngram ) {
	if( t == NULL ) {
		node_t *n = malloc( sizeof *n);
		n->d = create_data(buf, ngram);
		n->left = n->right = NULL;
		return n;
	} else if( cmp_data( t->d, buf, ngram ) > 0  ) {
		t->left = insert( t->left, buf, ngram);
		return t;
	} else if( cmp_data( t->d, buf, ngram ) < 0 ) {
		t->right = insert( t->right, buf, ngram);
		return t;
	} else { // add suffix
		insert_suffix(t->d, buf, ngram);
		return t;
	}

}

void print_tree(tree_t t, int ngram) {

	int i;

	if(t == NULL) return;

	printf("\n\nPREFIX:       ");
	for(i = 0; i < ngram-1; i++) {
		printf(" %s ", t->d->prefix[i]);
	}

	printf("\n\nSUFFIX [%d]\n", t->d->n_suff);
	for(i = 0; i < t->d->n_suff; i++)
		printf(" %s[%d] ", t->d->suffix[i]->suffix, t->d->suffix[i]->occurr);
	printf("\n------------------------------------\n\n");		

	print_tree(t->left, ngram);
	print_tree(t->right, ngram);
}

	


int cmp_data( data_t *data, char **buf, int ngram) { // compare suffix

	int i, j;
	for(i = 0; i < ngram-1; i++)
		if((j = (strcmp(buf[i], data->prefix[i]))))
			return j;
	return 0;
}



void insert_suffix(data_t *data, char **buf, int ngram) {

	int i;
	int cap;

	for(i = 0; i < data->n_suff; i++) {
		if( ! (strcmp(buf[ngram-1], data->suffix[i]->suffix)) ) {
			data->suffix[i]->occurr++;
			return;
		}
	}

	
	if(data->n_suff == data->cap_suff){ // extend table of suffixes
		cap = data->cap_suff;
		data->suffix = realloc(data->suffix, 2*cap*sizeof*data->suffix); // 
		for(i = cap; i < 2*cap; i++)
			data->suffix[i] = calloc(sizeof*data->suffix[i], 1); // NIEWIADOMO DLACZEGO ????????????????????
		//	data->suffix[i] = malloc(sizeof(suff_t));
		data->cap_suff *= 2;
	}
		data->suffix[data->n_suff]->suffix = strdup(buf[ngram-1]);
		data->suffix[data->n_suff]->occurr = 1;
		data->n_suff++;
}

data_t * create_data(char **buf, int ngram) {

	int i;

	data_t *newdata = malloc(sizeof*newdata); // 
	newdata->prefix = malloc((ngram-1)*sizeof*newdata->prefix);

	for(i = 0; i < ngram-1; i++) // copy prefix
		newdata->prefix[i] = strdup(buf[i]);

	newdata->suffix = malloc(8*sizeof*newdata->suffix);
	for(i = 0; i < 8; i++)
		//newdata->suffix[i] = malloc(sizeof*newdata->suffix[i]);
		newdata->suffix[i] = calloc(sizeof*newdata->suffix[i], 1);

	newdata->suffix[0]->suffix = strdup(buf[ngram-1]);
	newdata->suffix[0]->occurr = 1;
	newdata->n_suff = 1;
	newdata->cap_suff = 8;

	return newdata;

}



void free_data(data_t *d, int ngram) {

        int i;
        if(d == NULL) return;

        for(i = 0; i < ngram-1; i++)
                free(d->prefix[i]);
        free(d->prefix);
	
        for(i = 0; i < d->cap_suff; i++) {
                free(d->suffix[i]->suffix);
                free(d->suffix[i]);
        }
	
        free(d->suffix);
	free(d);
}


void free_tree(tree_t t, int ngram) {

        if( t == NULL) return;
	free_data(t->d, ngram);

        free_tree(t->left, ngram);
        free_tree(t->right, ngram);
        free(t);
}


