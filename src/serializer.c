#include "serializer.h"
#include "argparser.h"
#include "storage.h"

#include <string.h>
#include <stdlib.h>

static int nnum = 0;


void fwrite_data(FILE *out, data_t *d, param_t p) {


	int i, size;
	int prefix_size = p.n_gram == 1 ? 1 : p.n_gram-1;


	for(i = 0; i < prefix_size; i++) { // wypisz prefixy
		size = strlen(d->prefix[i]) + 1;
		fwrite(&size, sizeof(size), 1, out);
		fwrite(d->prefix[i], sizeof(*d->prefix[i]), size, out);
	}

	fwrite(&d->n_suff, sizeof(d->n_suff), 1, out); // wypisz liczbe suffixow


	for(i = 0; i < d->n_suff; i++) { // wypisz suffixy
		size = strlen(d->suffix[i]) + 1;
		fwrite(&size, sizeof(size), 1, out);
		fwrite(d->suffix[i], sizeof(*d->suffix[i]), size, out);
	}
}

void serialize_data(FILE *out, tree_t t, param_t p) {


	if( t == NULL) return;

	fwrite_data(out, t->d, p);

	serialize_data(out, t->left, p);
	serialize_data(out, t->right, p);
}

void serialize_connections(FILE *out, tree_t t, param_t p) {

	int tmp = nnum;
	int dir; // 0 NULL , 1 left, 2 right

	if( t == NULL) {
		dir = 0;
		fwrite( &dir, sizeof(dir), 1, out); // wypisz 0 zamiast NULLa
		return;
	}

	fwrite(&tmp, sizeof(tmp), 1, out);
	nnum++;


	dir = 1; // left
	fwrite(&dir, sizeof(dir), 1, out);
	fwrite(&tmp, sizeof(tmp), 1, out);
	serialize_connections(out, t->left, p);
	dir = 2; // right
	fwrite(&dir, sizeof(dir), 1, out);
	fwrite(&tmp, sizeof(tmp), 1, out);
	serialize_connections(out, t->right, p);

}

void serialize_storage(storage_t s, param_t p) {


	fwrite(&s.v->n_nodes, sizeof s.v->n_nodes, 1, p.base_file);
	fwrite(&p.n_gram, sizeof p.n_gram, 1, p.base_file);

	serialize_data(p.base_file, s.tree, p);
	serialize_connections(p.base_file, s.tree, p);

	fclose(p.base_file);
}


int deserialize_storage(storage_t *s, param_t *p) {

	s->v = malloc(sizeof*s->v); // przydziel pamiec na strukture ze wskaznikami na wezly drzewa

	if ( !fread(&s->v->n_nodes, sizeof(s->v->n_nodes), 1, p->base_file) ||
		!fread(&p->n_gram, sizeof(p->n_gram), 1, p->base_file)  || 
		s->v->n_nodes <= 0 || p->n_gram <= 0) {
			free(s->v);
			fclose(p->base_file);
			return 1;
	}
		

	if ( !(s->v->n = calloc(s->v->n_nodes, sizeof*s->v->n) ) ) { // przydziel pamiec na tablice wskaznikow na wezly drzewa
		free(s->v);
		fclose(p->base_file);
		return 1;
	}
		

	if ( deserialize_data(p->base_file, p, s) || 
		deserialize_connections(p->base_file, s, p) ) {
		free(s->v->n);
		free(s->v);
		fclose(p->base_file);
		return 1;
	
	}
	s->tree = s->v->n[0]; // przypisz pierwszy element tablicy wskaznikow na wezly do korzenia drzewa

	fclose(p->base_file);

	return 0;
}

data_t * fread_data(FILE *in, data_t *d, param_t *p) { 

	int i, size;
	int prefix_size = p->n_gram == 1 ? 1 : p->n_gram-1;

	d = malloc(sizeof*d);

	if ( !(d->prefix = malloc(prefix_size*sizeof*d->prefix) ) ) {
		free(d);
		return NULL;
	}


	for(i = 0; i < prefix_size; i++) { // zczytaj suffixy
		if ( !fread(&size, sizeof(size), 1, in) || size <= 0 ) {
			free(d->prefix);
			free(d);
			return NULL;
		}
		d->prefix[i] = malloc(size);
		if ( fread(d->prefix[i], sizeof*d->prefix[i], size, in) != size ){
			free(d->prefix);
			free(d);
			return NULL;
		}
	}

	if ( !fread(&d->n_suff, sizeof(d->n_suff), 1, in) || d->n_suff < 0 ) {
		free(d);
		return NULL;
	}

	if(d->n_suff > 0 )
		d->suffix = malloc(d->n_suff*sizeof*d->suffix);


	for(i = 0; i < d->n_suff; i++) { // zczytaj suffixy
		if ( !fread(&size, sizeof(size), 1, in) || size <= 0) {
			free(d->prefix);
			free(d->suffix);
			free(d);
			return NULL;
		}
		d->suffix[i] = malloc(size);
		if ( fread(d->suffix[i], sizeof*d->suffix[i], size, in) != size ) {
			free(d->prefix);
			free(d->suffix);
			free(d);
			return NULL;
		}
	}

	return d;
}

int deserialize_data(FILE *in, param_t *p, storage_t *s) {

	int i;

	for(i = 0; i < s->v->n_nodes; i++) {
		s->v->n[i] = malloc(sizeof*s->v->n[i]); // wezel drzewa
		if ( !(s->v->n[i]->d = fread_data(in, s->v->n[i]->d, p) ) ) {
			free(s->v->n[i]);
			return 1;
		}
	}
	
	return 0;

}

int deserialize_connections(FILE *in, storage_t *s, param_t *p) {

	int i, dir, pos_1, pos_2;

	if ( fseek(in, sizeof(int), SEEK_CUR) )
		return 1;

	for(i = 0; i < 2*s->v->n_nodes; i++) {

		if ( !fread(&dir, sizeof(dir), 1, in) ||
			!fread(&pos_1, sizeof(pos_1), 1, in) ||
			!fread(&pos_2, sizeof(pos_2), 1, in) ||
			(dir != 1 && dir != 2 && dir != 0) ||
			pos_1 < 0 ||
			pos_2 < 0 ||
			(pos_1 >= s->v->n_nodes) ||
			(pos_2 >= s->v->n_nodes) )
				return 1;

		if(dir == 1) 
			s->v->n[pos_1]->left = (pos_2 != 0) ? s->v->n[pos_2] : NULL;

		else 
			s->v->n[pos_1]->right = (pos_2 != 0) ? s->v->n[pos_2] : NULL;

	}

	return 0;
}


