#include "argparser.h"

#include <stdlib.h>

int parse_args(int argc, char **argv, param_t *p) {

	p->input = fopen(argv[1], "r");
	p->n_gram = atoi(argv[2]);

	if ( 0 ){
		return -1; // problem z plikami wejsciowymi
	}

	if( 0 ) {
		return -2; // niewlasciwe argumenty
	}

	return 0;

}
