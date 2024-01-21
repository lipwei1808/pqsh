/* options.c: PQSH Options */

#include "pqsh/macros.h"
#include "pqsh/scheduler.h"

/**
 * Display usage message.
 * @param       program     String containing name of program.
 **/
void usage(const char *program) {
    fprintf(stderr, "Usage: %s [options]\n\n", program);
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "    -n CORES           Number of CPU cores to utilize\n");
    fprintf(stderr, "    -p POLICY          Scheduling policy (fifo, rdrn)\n");
    fprintf(stderr, "    -t MICROSECONDS    Timer interrupt interval\n");
    fprintf(stderr, "    -h                 Print this help message\n");
}

bool my_parse(int argc, char* argv[], Scheduler* s) {
	int i = 1;
	// strlen(argv[i]) > 1 to check for [-] arg
	while (i < argc && strlen(argv[i]) > 1 && argv[i][0] == '-') {
		char* arg = argv[i++];
		switch (arg[1]) {
			case 'n': {
				s->cores = atoi(argv[i++]);
				break;
			}
			case 'p': {
				char* policy = argv[i++];
				if (streq(policy, "rdrn")) {
					s->policy = RDRN_POLICY;
				} else if (streq(policy, "fifo")) {
					s->policy = FIFO_POLICY;
				} else {
					fprintf(stderr, "Unknown policy: %s\n", policy);
					usage(argv[0]);
					return false;
				}
				break;
			}
			case 't': {
				int timer = atoi(argv[i++]);
				if (timer < 0 || timer > 1000000) {
					fprintf(stderr, "Invalid timer value, using default of 250000\n");
				}
				break;
			}
			case 'h': {
				usage(argv[0]);
				return false;
			}
			default: {
				usage(argv[0]);
				return false;
			}
		}
	}
	return true;
}

/**
 * Parse command line options.
 * @param   argc        Number of command line options.
 * @param   argv        Array of command line options.
 * @param   s           Pointer to Scheduler structure.
 * @return  Whether or not the command line options were parsed successfully.
 **/
bool parse_command_line_options(int argc, char *argv[], Scheduler *s) {
    int argind = 1;

    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
			char *arg = argv[argind++];
			char *opt = NULL;
			switch (arg[1]) {
					case 'n':
						s->cores = atoi(argv[argind++]);
						break;
					case 'p':
							opt = argv[argind++];
							if (streq(opt, "fifo")) {
									s->policy = FIFO_POLICY;
										} else if (streq(opt, "rdrn")) {
									s->policy = RDRN_POLICY;
										} else {
												fprintf(stderr, "Unknown policy: %s\n", opt);
												return false;
										}
						break;
					case 't':
						s->timeout = atoi(argv[argind++]);
						break;
					case 'h':
						usage(argv[0]);
						return false;
					default:
						usage(argv[0]);
						return false;
			}
    }

    return true;
}

/* vim: set expandtab sts=4 sw=4 ts=8 ft=c: */
