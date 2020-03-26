set(NAME unit_tests)

file(GLOB SOURCES
        "*.h"
        "*.c"
		"*.hpp"
		"*.cpp"
)

add_executable(${NAME} ${SOURCES} )
message(STATUS " flags:${CMAKE_CXX_FLAGS} ")
target_link_libraries(${NAME}

		core
		command_terminal
		ZUMO_devices
		ZUMO_hal
	gtest
	gmock
	gtest_main
)

target_format_sources(${NAME} "${SOURCES}")