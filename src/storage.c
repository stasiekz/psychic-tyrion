#include "storage.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


void add_to_storage(storage_t *storage, char **buf, int ngram) {

	storage->tree = insert( storage->tree, &storage->v, buf, ngram);
}



void free_storage(storage_t *storage, int ngram) {

	free_tree(storage->tree, ngram);
	free_node_vec(storage->v);

}


node_vec *add_to_node_vec( node_vec *v, node_t *node) {


	if( v == NULL) {
		v = malloc(sizeof*v);
		v->n_nodes = 0;
		v->n = NULL;
		//v->n = calloc(sizeof*v->n, 50);
		v->n = calloc(50, sizeof*v->n);
		v->cap_nodes = 50;

	} else if( v->n_nodes == v->cap_nodes ) {

		v->n = realloc(v->n , 2*v->cap_nodes*sizeof*v->n);
		v->cap_nodes *= 2;
	}

	v->n[v->n_nodes++] = node;

	return v;

}

void free_node_vec(node_vec *v) {

	if( v == NULL ) return ;

	free(v->n);
	free(v);
}



tree_t insert( tree_t t, node_vec **v, char **buf, int ngram ) {
	if( t == NULL ) {
		node_t *n = malloc( sizeof *n);
		n->d = create_data(buf, ngram);
		n->left = n->right = NULL;
		*v = add_to_node_vec(*v, n);
		return n;
	} else if( cmp_data( t->d, buf, ngram ) > 0  ) {
		t->left = insert( t->left, v, buf, ngram);
		return t;
	} else if( cmp_data( t->d, buf, ngram ) < 0 ) {
		t->right = insert( t->right, v, buf, ngram);
		return t;
	} else { // dodaj suffix jezezli istnieje juz taki prefix
		if( ngram > 1 ) 
			insert_suffix(t->d, buf, ngram);
		return t;
	}

}

node_t *lookup_tree(tree_t t, char **buf, int ngram) {

	if( t == NULL ) return NULL;

	if( cmp_data( t->d, buf, ngram ) > 0  ) {
		return lookup_tree(t->left, buf, ngram);
	} else if( cmp_data( t->d, buf, ngram ) < 0 ) {
		return lookup_tree(t->right, buf, ngram);
	} else {
		return t;
	}

}

void print_tree(tree_t t, int ngram) {

	int i;
	int prefix_size = ngram == 1 ? 1 : ngram-1;

	if(t == NULL) return;

	printf("\n\nPREFIX:       ");
	for(i = 0; i < prefix_size; i++) {
		printf(" %s ", t->d->prefix[i]);
	}

	printf("\n\nSUFFIX [%d]\n", t->d->n_suff);
	for(i = 0; i < t->d->n_suff; i++)
		printf(" %s ", t->d->suffix[i]);
	printf("\n------------------------------------\n\n");		

	print_tree(t->left, ngram);
	print_tree(t->right, ngram);
}




int cmp_data( data_t *data, char **buf, int ngram) { // compare prefix

	int i, j;
	int prefix_size = ngram == 1 ? 1 : ngram-1;

	for(i = 0; i < prefix_size; i++)
		if((j = (strcmp(buf[i], data->prefix[i]))))
			return j;
	return 0;
}



void insert_suffix(data_t *data, char **buf, int ngram) {


	if(data->n_suff == data->cap_suff){ // zwieksz tablice suffixow z powtorzeniami
		data->suffix = realloc(data->suffix, 2*data->cap_suff*sizeof*data->suffix);
		data->cap_suff *= 2;
	}
	data->suffix[data->n_suff++] = strdup(buf[ngram-1]);

}

data_t * create_data(char **buf, int ngram) {

	int i;
	int prefix_size = ngram == 1 ? 1 : ngram-1;

	data_t *newdata = malloc(sizeof*newdata);
	newdata->prefix = malloc(prefix_size*sizeof*newdata->prefix);

	for(i = 0; i < prefix_size; i++) // kopiuj prefix
		newdata->prefix[i] = strdup(buf[i]);

	if ( ngram > 1 ) { // nie unigram

		newdata->suffix = malloc(10*sizeof*newdata->suffix);

		newdata->cap_suff = 10;
		newdata->suffix[0] = strdup(buf[prefix_size]); // kopiuj pierwszu suffix
		newdata->n_suff = 1;

	} else { // unigram
		newdata->suffix = NULL;
		newdata->n_suff = 0;
		newdata->cap_suff = 0;
	}

	return newdata;

}



void free_data(data_t *d, int ngram) {

	int i;
	int prefix_size = ngram == 1 ? 1 : ngram-1;

	if(d == NULL) return;

	for(i = 0; i < prefix_size; i++)
		free(d->prefix[i]);
	free(d->prefix);

	if( ngram > 1 ) { // nie unigram

		for(i = 0; i < d->n_suff; i++) {
			free(d->suffix[i]);
		}

		free(d->suffix);
	}

	free(d);

}


void free_tree(tree_t t, int ngram) {

	if( t == NULL) return;
	free_data(t->d, ngram);

	free_tree(t->left, ngram);
	free_tree(t->right, ngram);
	free(t);
}


