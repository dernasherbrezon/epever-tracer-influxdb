#include <modbus/modbus-rtu.h>
#include <modbus/modbus.h>

#define modbus_new_rtu modbus_new_rtu_mocked
#define modbus_set_slave modbus_set_slave_mocked
#define modbus_free modbus_free_mocked
#define modbus_set_debug modbus_set_debug_mocked
#define modbus_set_error_recovery modbus_set_error_recovery_mocked
#define modbus_set_response_timeout modbus_set_response_timeout_mocked
#define modbus_connect modbus_connect_mocked
#define modbus_close modbus_close_mocked
#define gethostname gethostname_mocked
#define malloc malloc_mocked
#define modbus_read_input_registers modbus_read_input_registers_mocked

modbus_t* modbus_new_rtu_mocked(const char *device, int baud, char parity,
                                    int data_bit, int stop_bit);
int modbus_set_slave_mocked(modbus_t* ctx, int slave);
void modbus_free_mocked(modbus_t *ctx);
int modbus_set_debug_mocked(modbus_t *ctx, int flag);
int modbus_set_error_recovery_mocked(modbus_t *ctx, modbus_error_recovery_mode error_recovery);
int modbus_set_response_timeout_mocked(modbus_t *ctx, uint32_t to_sec, uint32_t to_usec);
int modbus_connect_mocked(modbus_t *ctx);
void modbus_close_mocked(modbus_t *ctx);
int	gethostname_mocked(char *, size_t);
void *malloc_mocked(size_t size);
int modbus_read_input_registers_mocked(modbus_t *ctx, int addr, int nb, uint16_t *dest);

#include "../src/EpeverTracer.c"

#undef modbus_new_rtu
#undef modbus_set_slave
#undef modbus_free
#undef modbus_set_debug
#undef modbus_set_error_recovery
#undef modbus_set_response_timeout
#undef modbus_connect
#undef modbus_close
#undef gethostname
#undef malloc

struct _modbus {
	int dummy;
};

typedef struct {
	int addr;
	uint16_t value;
} mock_register;

typedef struct {
	int failmodbus_new_rtu;
	int failmodbus_set_slave;
	int failmodbus_set_debug;
	int failmodbus_set_error_recovery;
	int failmodbus_set_response_timeout;
	int failmodbus_connect;
	int failgethostname;
	char *hostname;
	int failmalloc;
	mock_register *registers;
	size_t registers_size;
} mock_status;

mock_status mock;

modbus_t* modbus_new_rtu_mocked(const char *device, int baud, char parity, int data_bit, int stop_bit) {
	if (mock.failmodbus_new_rtu) {
		return NULL;
	}
	return (modbus_t*) malloc(sizeof(modbus_t));
}

int modbus_set_slave_mocked(modbus_t *ctx, int slave) {
	if (mock.failmodbus_set_slave) {
		return -1;
	}
	return 0;
}

int modbus_set_debug_mocked(modbus_t *ctx, int flag) {
	if (mock.failmodbus_set_debug) {
		return -1;
	}
	return 0;
}

int modbus_set_error_recovery_mocked(modbus_t *ctx, modbus_error_recovery_mode error_recovery) {
	if (mock.failmodbus_set_error_recovery) {
		return -1;
	}
	return 0;
}

int modbus_set_response_timeout_mocked(modbus_t *ctx, uint32_t to_sec, uint32_t to_usec) {
	if (mock.failmodbus_set_response_timeout) {
		return -1;
	}
	return 0;
}

int modbus_connect_mocked(modbus_t *ctx) {
	if(mock.failmodbus_connect) {
		return -1;
	}
	return 0;
}

int	gethostname_mocked(char *hostname, size_t maxSize) {
	if( mock.failgethostname ) {
		return -1;
	}
	strcpy(hostname, mock.hostname);
	return 0;
}

void* malloc_mocked(size_t size) {
	if (mock.failmalloc) {
		return NULL;
	}
	return malloc(size);
}

int modbus_read_input_registers_mocked(modbus_t *ctx, int addr, int nb, uint16_t *dest) {
	for (int i = 0; i < mock.registers_size; i++) {
		if (mock.registers[i].addr == addr) {
			*dest = mock.registers[i].value;
			return 0;
		}
	}
	return -1;
}

void modbus_free_mocked(modbus_t *ctx) {
	free(ctx);
}

void modbus_close_mocked(modbus_t *ctx) {
	//do nothing
}

void epever_tracer_mock_reset() {
	mock.failmodbus_new_rtu = 0;
	mock.failmodbus_set_slave = 0;
	mock.failmodbus_set_debug = 0;
	mock.failmodbus_set_error_recovery = 0;
	mock.failmodbus_set_response_timeout = 0;
	mock.failmodbus_connect = 0;
	mock.failgethostname = 0;
	mock.hostname = "mockhost";
	mock.failmalloc = 0;
	if (mock.registers != NULL) {
		free(mock.registers);
	}
	mock.registers_size = 0;
}
