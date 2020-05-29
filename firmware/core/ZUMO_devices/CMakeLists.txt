set(NAME ZUMO_devices)

file(GLOB_RECURSE SOURCES
		"*.h"
		"*.c"
		"*.cpp"
		"*.hpp"
)

add_library(${NAME} ${SOURCES} BHI160.cpp BHI160.h)


#target_include_directories(${NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../ZUMO_hal" )
target_include_directories(${NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}" )