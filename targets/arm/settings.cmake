set (CWARN "-Wall -Wstrict-prototypes -Wextra -Werror")
set (CXXWARN "-Wall -Wextra -Werror")
set(CTUNING "-fomit-frame-pointer -ffunction-sections -fdata-sections")
set (ARMFLOAT "-mfloat-abi=soft")

ReadVariables(${CMAKE_CURRENT_LIST_DIR}/Makefile "CPU")        # get flag -DCPU = -mcpu=cortex-mXX
set(CMCU "-mcpu=${CPU} -mthumb ${ARMFLOAT}")

ReadVariables(${CMAKE_CURRENT_LIST_DIR}/Makefile "C_DEFS")        # get flag -DSTM32F****

string(REPLACE ";" " " C_DEFS ${C_DEFS})
message("${C_DEFS}")
set(RANDOM_DEFS "-DUSE_HAL_DRIVER -DSTM32F103xB")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 ${CWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17 -Dregister=\"/**/\" -fno-exceptions -fno-rtti ${CXXWARN} ${CTUNING} ${CMCU} ${RANDOM_DEFS}")

ReadVariables(${CMAKE_CURRENT_LIST_DIR}/Makefile "LDSCRIPT")      # get LD file
set(PLATFORM_LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/${LDSCRIPT})
