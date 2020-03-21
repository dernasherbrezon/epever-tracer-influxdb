#include "EpeverTracer.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

extern int errno;
extern char *optarg;
extern int opterr;

int main(int argc, char *argv[]) {
	int opt;
	opterr = 0;
	char *device = NULL;
	int verbose = 0;
	uint32_t timeoutSecond = 5;
	while ((opt = getopt(argc, argv, "d:t:vh")) != EOF) {
		switch (opt) {
		case 'd':
			device = optarg;
			break;
		case 'v':
			verbose++;
			break;
		case 't':
			timeoutSecond = (uint32_t) strtoul(optarg, NULL, 10);
			break;
		case 'h':
		default:
			fprintf(stderr, "%s [-d device] [-v] [-t timeoutSeconds(optional)] [-h]\n", argv[0]);
			return EXIT_FAILURE;
		}
	}
	if (device == NULL) {
		fprintf(stderr, "device is not specified\n");
		errno = EINVAL;
		return EXIT_FAILURE;
	}
	return epever_tracer_process(device, timeoutSecond, verbose);
}
