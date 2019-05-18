/*
 * main.c
 *
 *  Created on: May 17, 2019
 *      Author: bruno-dagostino
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "bool.h"
#include "./handle_tcp_server/handle_tcp_server.h"

/* Type Defs ******************************************************************/

/**
 * This structure contains all the options parsed from the command line.
 */
struct mainopts {
	BOOL listen;
	BOOL command;
	BOOL upload;
	char *execute;
	char *target;
	char *upload_destination;
	unsigned short port;
};

/* Private API ****************************************************************/

static void usage();

/* Main ***********************************************************************/

int main(int argc, char *argv[]) {
	struct mainopts opts;
	int c;
	opterr = 0;

	if (argc == 1) {
		usage();

		exit(1);
	}

	while ((c = getopt(argc, argv, "           hle:t:p:cu:")) != -1)
		switch (c) {
		case 'h':
			usage();
			break;
		case 'l':
			opts.listen = TRUE;
			break;
		case 'e':
			opts.execute = optarg;
			break;
		case 't':
			opts.target = optarg;
			break;
		case 'p':
			opts.port = optarg;
			break;
		case 'c':
			opts.command = TRUE;
			break;
		case 'u':
			opts.upload = TRUE;
			opts.upload_destination = optarg;
			break;
		case '?':
			if (optopt == 'e')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (optopt == 't')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (optopt == 'p')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (optopt == 'u')
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);

			return 1;
		default:
			abort();
		}

	if ((opts.listen == FALSE) && (strlen(opts.target) > 0) && (opts.port > 0))
		client_sender(opts.target, opts.port);

	return 0;
}

/* Private Impl ***************************************************************/

void usage() {
	printf("B Net Tool\n\n");
	printf("Usage: ./bnet -t <target> -p <port>\n");
	printf("-l\t- listen on [host]:[port] for incoming connections\n");
	printf("-e\t- execute the given file upon receiving a connection\n");
	printf("-c\t- initialize a command shell\n");
	printf(
			"-u\t- upon receiving connection upload a file and write to [destination]\n\n");
	printf("Examples: \n");
	printf("./bnet -l -p 5555\n");
	printf("./bnet -l -p 5555 -c\n");
	printf("./bnet -l -p 5555 -u c:\\target.exe\n");
	printf("./bnet -l -p 5555 -e 'cat /etc/passwd'\n\n");
}
