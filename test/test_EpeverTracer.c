#include <stdlib.h>
#include <check.h>
#include "MockEpeverTracer.c"

START_TEST (test_unableToCreateModBus) {
	mock.failmodbus_new_rtu = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0, stdout), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failToSetSlave) {
	mock.failmodbus_set_slave = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0, stdout), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failToSetDebug) {
	mock.failmodbus_set_debug = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 1, stdout), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failToSetErrorRecovery) {
	mock.failmodbus_set_error_recovery = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0, stdout), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failToSetTimeout) {
	mock.failmodbus_set_response_timeout = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0, stdout), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failToConnect) {
	mock.failmodbus_connect = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0, stdout), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failGetHostname) {
	mock.failgethostname = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0, stdout), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failMalloc) {
	mock.failmalloc = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0, stdout), EXIT_FAILURE);
}
END_TEST

void assertFiles(char *expected, char *actual) {
	FILE *expectedPtr = fopen(expected, "r");
	ck_assert_ptr_ne(expectedPtr, NULL);
	FILE *actualPtr = fopen(actual, "r");
	ck_assert_ptr_ne(actualPtr, NULL);
	int expectedChar;
	int actualChar;
	do {
		expectedChar = fgetc(expectedPtr);
		actualChar = fgetc(actualPtr);
		ck_assert_uint_eq(expectedChar, actualChar);
	} while (expectedChar != EOF && actualChar != EOF);
	fclose(expectedPtr);
	fclose(actualPtr);
}

START_TEST (test_success) {
	char *actual = "test_success.txt";
	FILE *output = fopen(actual, "w");
	mock.registers_size = 5;
	mock.registers = (mock_register*) malloc(mock.registers_size * sizeof(mock_register));
	mock.registers[0] = (mock_register ) { 0x3104, 1240 };
	mock.registers[1] = (mock_register ) { 0x3101, 110 };
	mock.registers[2] = (mock_register ) { 0x3002, 0x93E0 };
	mock.registers[3] = (mock_register ) { 0x3003, 0x0004 };
	mock.registers[4] = (mock_register ) { 0x3200, 0x0001 };
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0, output), EXIT_SUCCESS);
	fflush(output);
	fclose(output);
	assertFiles("expected.txt", actual);
	remove(actual);
}
END_TEST

int getTotalRegisters() {
	int result = 0;
	int size = sizeof(registers) / sizeof(registers[0]);
	for (int i = 0; i < size; i++) {
		result += registers[i].size;
	}
	return result;
}

START_TEST (test_allMetrics) {
	char *actual = "test_allMetrics.txt";
	FILE *output = fopen(actual, "w");
	mock.registers_size = getTotalRegisters();
	mock.registers = (mock_register*) malloc(mock.registers_size * sizeof(mock_register));
	for (int sourceIndex = 0, destIndex = 0; destIndex < mock.registers_size; sourceIndex++) {
		for (int j = 0; j < registers[sourceIndex].size; j++) {
			mock.registers[destIndex] = (mock_register ) { registers[sourceIndex].addr + j, 0 };
			destIndex++;
		}
	}
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0, output), EXIT_SUCCESS);
	fflush(output);
	fclose(output);
	assertFiles("expectedallMetrics.txt", actual);
	remove(actual);
}
END_TEST

void teardown() {
	epever_tracer_mock_reset();
}

void setup() {
	//do nothing
}

Suite * common_suite(void) {
	Suite *s;
	TCase *tc_core;

	s = suite_create("common");

	/* Core test case */
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_unableToCreateModBus);
	tcase_add_test(tc_core, test_failToSetSlave);
	tcase_add_test(tc_core, test_failToSetDebug);
	tcase_add_test(tc_core, test_failToSetErrorRecovery);
	tcase_add_test(tc_core, test_failToSetTimeout);
	tcase_add_test(tc_core, test_failToConnect);
	tcase_add_test(tc_core, test_failGetHostname);
	tcase_add_test(tc_core, test_failMalloc);
	tcase_add_test(tc_core, test_success);
	tcase_add_test(tc_core, test_allMetrics);

	tcase_add_checked_fixture(tc_core, setup, teardown);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(void) {
	int number_failed;
	Suite *s;
	SRunner *sr;

	s = common_suite();
	sr = srunner_create(s);

	srunner_set_fork_status(sr, CK_NOFORK);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}


