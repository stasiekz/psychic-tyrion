#ifndef _DATADEF_H_
#define _DATADEF_H_

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


void insert_suffix(data_t *data, char **buf, int ngram);

int cmp_data( data_t *data, char **buf, int ngram);

data_t * create_data(char **buf, int ngram);

void free_data(data_t *data, int ngram);
		


	

#endif
