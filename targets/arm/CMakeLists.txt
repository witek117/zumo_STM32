set(NAME arm)

add_subdirectory(Drivers)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-strict-prototypes")

include(Drivers/allowed_warnings.cmake)

file(GLOB_RECURSE SOURCES_AUTO
		Inc/*.h
		Src/*.c
		Src/*.cpp
		*.s
)

include_directories(
	Inc
	Drivers/CMSIS/Device/ST/STM32F1xx/Include
	Drivers/CMSIS/Include
	Drivers/STM32F1xx_HAL_Driver/Drivers/STM32F1xx_HAL_Driver/Inc
)

set(SOURCES
	${SOURCES_AUTO}
)



add_executable(${NAME} ${SOURCES})

set_target_properties(${NAME} PROPERTIES LINK_FLAGS "-T ${PLATFORM_LINKER_SCRIPT} -Wl,--gc-sections --specs=nano.specs -lc")

target_link_libraries(${NAME}
	core
	Drivers
)

target_generate_hex(${NAME})

target_memory_report(${NAME})
target_asm_listing(${NAME})

target_format_sources(${NAME} "${SOURCES_PROJECT}")