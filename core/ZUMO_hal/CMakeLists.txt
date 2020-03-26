set(NAME ZUMO_hal)

file(GLOB_RECURSE SOURCES
	"*.h"
	"*.c"
	"*.cpp"
	"*.hpp"
)
add_library(${NAME} ${SOURCES})

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../${MCU_DRIVERS_DIRECTORY})