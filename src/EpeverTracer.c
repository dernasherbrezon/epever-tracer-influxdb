#include "EpeverTracer.h"
#include <modbus/modbus-rtu.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

typedef struct {
	char *category;
	char *parameter;
	int addr;
	int size;
} epever_register_t;

const epever_register_t registers[] = {
		{"solar", "voltage", 0x3100, 1},
		{"solar", "current", 0x3101, 1},
		{"solar", "power", 0x3102, 2},
		{"battery", "voltage", 0x3104, 1},
		{"battery", "current", 0x3105, 1},
		{"battery", "power", 0x3106, 2},
		{"battery", "temperature", 0x3110, 1},
		{"battery", "remoteTemperature", 0x311B, 1},
		{"battery", "realRatedVoltage", 0x311D, 1},
		{"battery", "soc", 0x311A, 1},
		{"load", "voltage", 0x310C, 1},
		{"load", "current", 0x310D, 1},
		{"load", "power", 0x310E, 2},
		{"device", "temperature", 0x3112, 1}
};

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

	epever_register_t *ptr = registers;
	const epever_register_t *endPtr = registers + sizeof(registers) / sizeof(registers[0]);
	while (ptr < endPtr) {
		memset(output, 0, 2 * sizeof(uint16_t));
		for (int i = 0; i < ptr->size; i++) {
			uint16_t dest;
			rc = modbus_read_input_registers(modbus, ptr->addr + i, 0x1, &dest);
			if (rc == -1) {
				fprintf(stderr, "Failed to modbus_read_input_registers: %s\n", modbus_strerror(errno));
			}
			output[i] = dest;
		}
		uint32_t fullRegister = (output[1] << 8) | (output[0]);
		fprintf(stdout, "%s %s=%f\n", ptr->category, ptr->parameter, (float) (fullRegister / 100.0));
		ptr++;
	}
	free(output);
	modbus_close(modbus);
	modbus_free(modbus);
	return EXIT_SUCCESS;
}
