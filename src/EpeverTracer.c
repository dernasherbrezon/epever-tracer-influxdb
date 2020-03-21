#include "EpeverTracer.h"
#include <modbus/modbus-rtu.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

typedef struct {
	modbus_t *modbus;
} epever_tracer_t;

int epever_tracer_process(char *device, uint32_t timeoutSecond, int verbose) {
	// values are defined by the protocol
	modbus_t *modbus = modbus_new_rtu(device, 115200, 'N', 8, 1);
	if (modbus == NULL) {
		fprintf(stderr, "Could not connect to MODBUS: %s\n", modbus_strerror(errno));
		return EXIT_FAILURE;
	}
	int rc = modbus_set_slave(modbus, 1);
	if (rc == -1) {
		fprintf(stderr, "unable to setup slave: %s\n", modbus_strerror(errno));
		modbus_free(modbus);
		return EXIT_FAILURE;
	}
	if (verbose > 0) {
		modbus_set_debug(modbus, TRUE);
		if (rc == -1) {
			fprintf(stderr, "unable to setup debug mode: %s\n", modbus_strerror(errno));
			modbus_free(modbus);
			return EXIT_FAILURE;
		}
	}
	rc = modbus_set_error_recovery(modbus, MODBUS_ERROR_RECOVERY_LINK | MODBUS_ERROR_RECOVERY_PROTOCOL);
	if (rc == -1) {
		fprintf(stderr, "unable to setup error recovery: %s\n", modbus_strerror(errno));
		modbus_free(modbus);
		return EXIT_FAILURE;
	}
	rc = modbus_set_response_timeout(modbus, timeoutSecond, 0);
	if (rc == -1) {
		fprintf(stderr, "unable to setup timeout: %s\n", modbus_strerror(errno));
		modbus_free(modbus);
		return EXIT_FAILURE;
	}
	rc = modbus_connect(modbus);
	if (rc == -1) {
		fprintf(stderr, "connection failed: %s\n", modbus_strerror(errno));
		modbus_free(modbus);
		return EXIT_FAILURE;
	}
	uint16_t *output = (uint16_t *) malloc(2 * sizeof(uint16_t));
	memset(output, 0, 2 * sizeof(uint16_t));
	rc = modbus_read_input_registers(modbus, 0x3004, 0x1, output);
	if (rc == -1) {
		fprintf(stderr, "Failed to modbus_read_input_registers: %s\n", modbus_strerror(errno));
	}
	fprintf(stdout, "voltage=%f\n", (float)(output[0] / 100.0));
	free(output);
	modbus_close(modbus);
	modbus_free(modbus);
	return EXIT_SUCCESS;
}
