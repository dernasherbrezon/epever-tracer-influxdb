#include <stdlib.h>
#include <check.h>
#include "MockEpeverTracer.c"

START_TEST (test_unableToCreateModBus) {
	mock.failmodbus_new_rtu = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failToSetSlave) {
	mock.failmodbus_set_slave = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failToSetDebug) {
	mock.failmodbus_set_debug = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 1), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failToSetErrorRecovery) {
	mock.failmodbus_set_error_recovery = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failToSetTimeout) {
	mock.failmodbus_set_response_timeout = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failToConnect) {
	mock.failmodbus_connect = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failGetHostname) {
	mock.failgethostname = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0), EXIT_FAILURE);
}
END_TEST

START_TEST (test_failMalloc) {
	mock.failmalloc = 1;
	ck_assert_int_eq(epever_tracer_process("/dev/ttyXRUSB0", 5, 0), EXIT_FAILURE);
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


