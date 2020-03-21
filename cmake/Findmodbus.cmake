
find_package(PkgConfig)
pkg_check_modules(PC_MODBUS QUIET libmodbus)

find_path(MODBUS_INCLUDE_DIR NAMES modbus/modbus-rtu.h)

find_library(MODBUS_LIBRARY NAMES modbus
	HINTS ${PC_MODBUS_LIBDIR} ${PC_MODBUS_LIBRARY_DIRS})

set(MODBUS_LIBRARIES ${MODBUS_LIBRARY})
set(MODBUS_INCLUDE_DIRS ${MODBUS_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)

find_package_handle_standard_args(MODBUS DEFAULT_MSG MODBUS_LIBRARY MODBUS_INCLUDE_DIR)

mark_as_advanced(MODBUS_INCLUDE_DIR MODBUS_LIBRARY)
