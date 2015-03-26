#ifndef  _STORAGE_H_
#define  _STORAGE_H_

/* glowna struktura danych */
typedef struct {
	char **prefix;
	char **suffix;

	int n_suff;
	int cap_suff;

} data_t;


/* wezel drzewa */
typedef struct node {

	data_t *d;
	struct node *left, *right;

} node_t, *tree_t;

/*
   dynamiczna tablica 
   wskaznikow na wezly drzewa
 */
typedef struct {

	node_t **n;
	int n_nodes;
	int cap_nodes;
} node_vec;


typedef struct {

	tree_t tree;
	node_vec *v;

} storage_t;


/* dodaje zawartosc bufora do kontenera danych */
void add_to_storage(storage_t *storage, char **buf, int ngram);


/* zwalnia pamiec po kontenerze danych */
void free_storage(storage_t *storage, int ngram);

/* dodaje wskaznik na nowoutworzony wezel do tablicy */
node_vec *add_to_node_vec( node_vec *v, node_t *node);

/* zwalnia pamiec po tablicy wskaznikow na wezly drzewa */
void free_node_vec(node_vec *v);

/* umieszcza zawartosc bufora w drzewie i
   zarazem dodaje nowy wskaznik do tablicy wskaznikow na wezly
 */
tree_t insert( tree_t t, node_vec **v, char **buf, int ngram);

/* wyszukuje prefix w drzewie */
node_t *lookup_tree(tree_t t, char **buf, int ngram);

/* pomocniczna */
void print_tree(tree_t t, int ngram);

/* zwalnia pamiec po drzewie */
void free_tree(tree_t t, int ngram);

/* umiesza w istniejacym wezle drzewa suffix z bufora */
void insert_suffix(data_t *data, char **buf, int ngram);

/* porownuje prefix z bufora z tym zawartym w strukturze data */
int cmp_data( data_t *data, char **buf, int ngram);

/* tworzy nowa strukture data_t i kopiuje do niej zawartosc bufora */
data_t * create_data(char **buf, int ngram);

/* zwlania pamiec po danych z wezla drzewa */
void free_data(data_t *data, int ngram);



#endif
