#include "argparser.h"

#include <stdlib.h>

int parse_args(int argc, char **argv, param_t *p) {

	p->input = fopen(argv[1], "r");
	p->n_gram = atoi(argv[2]);

	return 1;

}
