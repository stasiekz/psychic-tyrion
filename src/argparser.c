#include "argparser.h"

#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>

int parse_args(int argc, char **argv, param_t *p) {

	int next_option;
	int i;

	/* A string listing valid short options letters.  */
	const char* const short_options = "f:w:p:n:s:o:b:h";
	/* An array describing valid long options.  */
	const struct option long_options[] = {
		{ "file", 1, NULL, 'f' },
		{ "nwords", 1, NULL, 'w' },
		{ "nparag", 1, NULL, 'p' },
		{ "ngram",  1, NULL, 'n' },
		{ "stat",   1, NULL, 's' },
		{ "output", 1, NULL, 'o' },
		{ "basefile", 1, NULL, 'b' },
		{ "help", 0, NULL, 'h' },
		{ NULL,       0, NULL, 0   }
	};

	p->n_words = 100;
	p->n_parag = 4;
	p->n_gram = 2;
	p->help = 0;

	p->input = calloc(1, sizeof*p->input);
	p->input[0] = stdin;
	p->inputs = 1;
	p->output = stdout;
	p->stat_file = stdout;

	do {
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);

		switch (next_option)
		{
			case 'f':

				optind--;
				for(p->inputs = 0 ;optind < argc && *argv[optind] != '-'; optind++) {

					p->input = realloc(p->input, (p->inputs+1)*sizeof*p->input); // poszerz tablice uchytow do plikow o 1
					p->input[p->inputs] = fopen(argv[optind], "r");

					if(p->input[p->inputs] == NULL) { // problem z plikiem
						fprintf(stderr, "%s: Nie moge otowrzyc pliku: \"%s\".\n", argv[0], argv[optind]);
						for(i = 0; i < p->inputs; i++)
							fclose(p->input[i]);
						free(p->input);
						return -1;
					}
					p->inputs++;

				} 
				break;

			case 'w': 
				p->n_words = atoi(optarg);
				break;

			case 'p':  
				p->n_parag = atoi(optarg);
				break;

			case 'n':   
				p->n_gram = atoi(optarg);
				break;

			case 's':   
				p->stat_file = fopen(optarg, "w");
				break;

			case 'o':   
				if( !( p->output = fopen(optarg, "w")) ) {
					fprintf(stderr, "%s: nie moge utworzyc pliku wynikowego: \"%s\"", argv[0], optarg);
					return -3; // TODO
				}
				break;

			case 'b':   
				p->base_file = fopen(optarg, "r"); // TODO ?
				break;

			case 'h':   
				p->help = 1;
				break;

			case -1:
				break;


			default:   
				abort ();

		}

	} while (next_option != -1);


	if( 0 ) {
		return -2; // niewlasciwe argumenty
	}

	return 0;

}
