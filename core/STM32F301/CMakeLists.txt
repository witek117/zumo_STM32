set(NAME ${MCU_DRIVERS_DIRECTORY})

file(GLOB_RECURSE SOURCES
	"*.h"
	"*.c"
	"*.cpp"
	"*.hpp"
)

add_library(${NAME} ${SOURCES})

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../ZUMO_hal)
#target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../targets/arm/Drivers/CMSIS/Device/ST/STM32F3xx/Include)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../targets/arm/Drivers/CMSIS/Include)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../targets/arm/Drivers/STM32F3xx_HAL_Driver/Inc)
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../targets/arm/Inc)
