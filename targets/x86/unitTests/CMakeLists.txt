set(NAME unit_tests)

file(GLOB SOURCES
        "*.h"
        "*.c"
		"*.hpp"
		"*.cpp"
)

add_executable(${NAME} ${SOURCES} )
target_link_libraries(${NAME}
		ZUMO_hal
		ZUMO_devices
		core
		command_terminal
		gtest
		gmock
		gtest_main
)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../core)