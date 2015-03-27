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
		{ "file", required_argument, NULL, 'f' },
		{ "nwords", required_argument, NULL, 'w' },
		{ "nparag", required_argument, NULL, 'p' },
		{ "ngram",  required_argument, NULL, 'n' },
		{ "stat",   optional_argument, NULL, 's' },
		{ "output", required_argument, NULL, 'o' },
		{ "basefile", required_argument, NULL, 'b' },
		{ "help", no_argument, NULL, 'h' },
		{ NULL,       no_argument, NULL, 0   }
	};

	p->n_words = DEFAULT_NUMBER_OF_WORDS;
	p->n_parag = DEFAULT_NUMBER_OF_PARAGRAPHS;
	p->n_gram = DEFAULT_N_GRAM;
	p->show_help = 1;
	p->gen_stat = 0;

	p->input = NULL;
	p->inputs = 0;
	p->output = stdout;
	p->stat_file = stdout;

	do {
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);

		switch (next_option)
		{
			case 'f':

				optind--;
				for(p->inputs = 0; optind < argc && *argv[optind] != '-'; optind++) {

					p->input = realloc(p->input, (p->inputs+1)*sizeof*p->input); // poszerz tablice uchytow do plikow o 1
					p->input[p->inputs] = fopen(argv[optind], "r");

					if(p->input[p->inputs] == NULL) { // problem z plikiem
						fprintf(stderr, "%s: Nie moge otowrzyc pliku: \"%s\".\n", argv[0], argv[optind]);
						for(i = 0; i < p->inputs; i++)
							fclose(p->input[i]);
						free(p->input);
						exit(1);
					}
					p->inputs++;

				} 
				p->show_help = 0;
				break;

			case 'w': 
				if( (p->n_words = atoi(optarg) ) <= 0 )
					exit(2); // TODO zwolnic pliki
				break;

			case 'p':  
				if( (p->n_parag = atoi(optarg) ) <= 0 )
					exit(2); // TODO zwolnic pliki
				break;

			case 'n':   
				if( (p->n_gram = atoi(optarg) ) <= 0 )
					exit(2); // TODO zwolnic pliki
				break;

			case 's':   
				p->gen_stat = 1;
				p->stat_file = fopen(optarg, "w");
				break;

			case 'o':   
				if( !( p->output = fopen(optarg, "w")) ) {
					fprintf(stderr, "%s: nie moge utworzyc pliku wynikowego: \"%s\"", argv[0], optarg);
					exit(3); // TODO zwolnic pliki
				}
				break;

			case 'b':   
				p->base_file = fopen(optarg, "r"); // TODO ?
				break;

			case 'h':   
				p->show_help = 1;
				break;

			case -1:
				break;

			default:   
				abort ();

		}

	} while (next_option != -1);

	return 0;

}
