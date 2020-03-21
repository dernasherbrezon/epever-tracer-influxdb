#include "EpeverTracer.h"
#include <modbus/modbus-rtu.h>
#include <stdio.h>
#include <errno.h>

typedef struct {
	modbus_t *modbus;
} epever_tracer_t;

int epever_tracer_process(char *device) {
	// values are defined by the protocol
	modbus_t *modbus = modbus_new_rtu(device, 115200, 'N', 8, 1);
	if (modbus == NULL) {
		fprintf(stderr, "Could not connect to MODBUS: %s\n", modbus_strerror(errno));
		return -1;
	}
	epever_tracer_t ctx;
	ctx.modbus = modbus;
	return 0;
}
