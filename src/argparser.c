#include "argparser.h"

#include <stdlib.h>
#include <getopt.h>
#include <stdio.h>


void close_files(FILE **f, int n) {

	int i;
	if( !f )
		return;

	for(i = 0; i < n; i ++)
		fclose(f[i]);
	free(f);
}


int parse_args(int argc, char **argv, param_t *p) {

	int next_option;
	int are_files = 0;

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
	//////	
	p->read_base = 0;
	p->make_base = 0;

	p->input = NULL;
	p->output = stdout;
	p->stat_file = stdout;

	do {
		next_option = getopt_long (argc, argv, short_options, long_options, NULL);

		switch (next_option)
		{
			case 'f':

				are_files = 1;
				optind--;
				for(p->inputs = 0; optind < argc && *argv[optind] != '-'; optind++) {

					p->input = realloc(p->input, (p->inputs+1)*sizeof*p->input); // poszerz tablice uchytow do plikow o 1

					if( (p->input[p->inputs] = fopen(argv[optind], "r")) == NULL ) {
						fprintf(stderr, "%s: Nie moge otworzyc pliku: \"%s\".\n", argv[0], argv[optind]);
						close_files(p->input, p->inputs);
						exit(1);
					}
					p->inputs++;

				} 
				p->show_help = 0;
				break;

			case 'w': 
				if( (p->n_words = atoi(optarg) ) <= 0 ) {
					fprintf(stderr, "%s: Nieprawidlowy paramter opcji -w \"%s\"\n", argv[0], optarg);
					close_files(p->input, p->inputs);
					exit(2);
				}
				break;

			case 'p':  
				if( (p->n_parag = atoi(optarg) ) <= 0 ) {
					fprintf(stderr, "%s: Nieprawidlowy paramter opcji -p \"%s\"\n", argv[0], optarg);
					close_files(p->input, p->inputs);
					exit(2);
				}
				break;

			case 'n':   

				if( (p->n_gram = atoi(optarg) ) <= 0 ){
					fprintf(stderr, "%s: Nieprawidlowy paramter opcji -n \"%s\"\n", argv[0], optarg);
					close_files(p->input, p->inputs);
					exit(2);
				}
				break;

			case 's':   
				p->gen_stat = 1;

				if( !( p->stat_file = fopen(optarg, "w")) ) {
					fprintf(stderr, "%s: nie moge utworzyc pliku wynikowego: \"%s\"", argv[0], optarg);
					close_files(p->input, p->inputs);
					exit(3);
				}
				break;

			case 'o':   
				if( !( p->output = fopen(optarg, "w")) ) {
					fprintf(stderr, "%s: nie moge utworzyc pliku wynikowego: \"%s\"", argv[0], optarg);
					close_files(p->input, p->inputs);
					exit(3);
				}
				break;

			case 'b':   
				if( are_files ) { // utworz baze jezeli sa pliki wejsciowe
					if( !( p->base_file = fopen(optarg, "wb")) ) {
						fprintf(stderr, "%s: nie moge utworzyc pliku bazowego: \"%s\"", argv[0], optarg);
						close_files(p->input, p->inputs);
						exit(3);
					}

					p->make_base = 1;
				}
				else {
					if( !( p->base_file = fopen(optarg, "rb")) ) {
						fprintf(stderr, "%s: nie moge otworzyc pliku bazowego: \"%s\"", argv[0], optarg);
						fclose(p->stat_file);
						fclose(p->output);
						exit(3);
					}	
					p->read_base = 1;
				}
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

	if( optind < argc ) {
		fprintf( stderr, "\n%s: Zly parametr:\n", argv[0] );
		for( ; optind < argc; optind++ )
			fprintf( stderr, "\t\"%s\"\n", argv[optind] );
		close_files(p->input, p->inputs);
		fclose(p->output);
		fclose(p->stat_file);
		p->show_help = 1;
		return 2;

	}

	if(p->n_parag > p->n_words) {
		fprintf(stderr, "%s: Blad ! liczba paragrafow[%d] > liczba slow[%d]\n", argv[0], p->n_parag, p->n_words);
		close_files(p->input, p->inputs);
		fclose(p->output);
		fclose(p->stat_file);
		exit(2);
	}

	return 0;

}
