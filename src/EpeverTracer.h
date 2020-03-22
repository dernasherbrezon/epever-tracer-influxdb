#ifndef EPEVERTRACER_H_
#define EPEVERTRACER_H_

#include <stdint.h>
#include <stdio.h>

int epever_tracer_process(char *device, uint32_t timeoutSecond, int verbose, FILE *output);

#endif /* EPEVERTRACER_H_ */
