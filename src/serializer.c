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

	int tmp = nnum;

	if( t == NULL) return;

	fwrite(&tmp, sizeof(tmp), 1, out); // wypisz numer wezla
	fwrite_data(out, t->d, p);
	nnum++;

	serialize_data(out, t->left, p);
	serialize_data(out, t->right, p);
}

void serialize_connections(FILE *out, tree_t t, param_t p) {

	int tmp = nnum;
	int dir; // 0 NULL , 1 left, 2 right

	if( t == NULL) {
		dir = 0;
		fwrite(&dir, sizeof(dir), 1, out); // wypisz 0 zamiast NULLa
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


	//	fwrite(&s.v->cap_nodes, sizeof s.v->cap_nodes, 1, out);
	fwrite(&s.v->n_nodes, sizeof s.v->n_nodes, 1, p.base_file);
	fwrite(&p.n_gram, sizeof p.n_gram, 1, p.base_file);

	serialize_data(p.base_file, s.tree, p);
	nnum = 0;
	serialize_connections(p.base_file, s.tree, p);

	fclose(p.base_file);
}


void deserialize_storage(storage_t *s, param_t *p) {

	s->v = malloc(sizeof*s->v); // przydziel pamiec na strukture ze wskaznikami na wezly drzewa

	fread(&s->v->n_nodes, sizeof(s->v->n_nodes), 1, p->base_file);
	fread(&p->n_gram, sizeof(p->n_gram), 1, p->base_file);

	s->v->n = NULL; // ??
	s->v->n = calloc(s->v->n_nodes, sizeof*s->v->n); // przydziel pamiec na tablice wskaznikow na wezly drzewa

	deserialize_data(p->base_file, s->v->n, p, s);
	nnum = 0;
	deserialize_connections(p->base_file, s, p);
	s->tree = s->v->n[0]; // przypisz pierwszy element tablicy wskaznikow na wezly do korzenia drzewa

	fclose(p->base_file);

}

data_t * fread_data(FILE *in, data_t *d, param_t *p) { // TODO

	int i, size;
	int prefix_size = p->n_gram == 1 ? 1 : p->n_gram-1;

	d = malloc(sizeof*d);

	d->prefix = malloc(prefix_size*sizeof*d->prefix);


	for(i = 0; i < prefix_size; i++) { // zczytaj suffixy
		fread(&size, sizeof(size), 1, in);
		d->prefix[i] = malloc(size);
		fread(d->prefix[i], sizeof*d->prefix[i], size, in);
	}

	fread(&d->n_suff, sizeof(d->n_suff), 1, in);

	d->suffix = malloc(d->n_suff*sizeof*d->suffix);


	for(i = 0; i < d->n_suff; i++) { // zczytaj suffixy
		fread(&size, sizeof(size), 1, in);
		d->suffix[i] = malloc(size);
		fread(d->suffix[i], sizeof*d->suffix[i], size, in);
	}

	return d;
}

void deserialize_data(FILE *in, node_t **n, param_t *p, storage_t *s) {

	int i, tmp;

	for(i = 0; i < s->v->n_nodes; i++) {
		fread(&tmp, sizeof tmp, 1, in); // ?? opusc zbedny numer TODO
		s->v->n[i] = malloc(sizeof*s->v->n[i]); // wezel drzewa
		s->v->n[i]->d = fread_data(in, s->v->n[i]->d, p);
	}

}

void deserialize_connections(FILE *in, storage_t *s, param_t *p) {

	int i, dir, pos_1, pos_2;
	fread(&i, sizeof(i), 1, in); // pomin pierwsza liczbe TODO

	for(i = 0; i < 2*s->v->n_nodes; i++) {

		fread(&dir, sizeof(dir), 1, in);
		fread(&pos_1, sizeof(pos_1), 1, in);
		fread(&pos_2, sizeof(pos_2), 1, in);

		if(dir == 1) {
			s->v->n[pos_1]->left = (pos_2 != 0) ? s->v->n[pos_2] : NULL;

		} else {
			s->v->n[pos_1]->right = (pos_2 != 0) ? s->v->n[pos_2] : NULL;
		}

	}
}


