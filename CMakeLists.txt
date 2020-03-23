cmake_minimum_required(VERSION 2.8)
project(epever-tracer-influxdb)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_C_FLAGS "-std=c99")
if(CMAKE_BUILD_TYPE MATCHES Debug)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
endif()
file(GLOB SOURCES src/*.c)
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")

add_executable(epeverTracer src/main.c)

install(TARGETS epeverTracer DESTINATION /usr/bin/)

add_library(epeverTracerLib ${SOURCES})
target_link_libraries(epeverTracer epeverTracerLib)

find_package(modbus REQUIRED)
include_directories(${MODBUS_INCLUDE_DIRS})
target_link_libraries(epeverTracer ${MODBUS_LIBRARIES})

enable_testing()

file(GLOB TEST_SOURCES test/test_*.c)
file(GLOB TEST_RESOURCES test/resources/*)
file(COPY ${TEST_RESOURCES} DESTINATION "${CMAKE_BINARY_DIR}")

find_package(Check REQUIRED)
include_directories(${CHECK_INCLUDE_DIRS})

find_library(LIBRT rt)
find_package(Threads REQUIRED)

foreach(curTest ${TEST_SOURCES})
	get_filename_component(curTestName ${curTest} NAME_WE)
	add_test(NAME ${curTestName} COMMAND ${curTestName} ${curTest})
	add_executable(${curTestName} ${curTest})
	set_property(TARGET ${curTestName} PROPERTY C_STANDARD 99)
	if(LIBRT)
		target_link_libraries(${curTestName} rt)
	endif()
	target_link_libraries(${curTestName} m)
	target_link_libraries(${curTestName} epeverTracerLib ${CMAKE_THREAD_LIBS_INIT} ${MODBUS_LIBRARIES} ${PC_CHECK_LDFLAGS})
endforeach()

if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_custom_target("coverage")
	add_custom_command(TARGET "coverage" COMMAND gcov ${CMAKE_BINARY_DIR}/CMakeFiles/epeverTracerLib.dir/src/*.c.o)
endif()