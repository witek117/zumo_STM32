# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.2.3\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.2.3\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug

# Include any dependencies generated for this target.
include core/STM32F301/CMakeFiles/STM32F301.dir/depend.make

# Include the progress variables for this target.
include core/STM32F301/CMakeFiles/STM32F301.dir/progress.make

# Include the compile flags for this target's objects.
include core/STM32F301/CMakeFiles/STM32F301.dir/flags.make

core/STM32F301/CMakeFiles/STM32F301.dir/GPIO.cpp.obj: core/STM32F301/CMakeFiles/STM32F301.dir/flags.make
core/STM32F301/CMakeFiles/STM32F301.dir/GPIO.cpp.obj: ../core/STM32F301/GPIO.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object core/STM32F301/CMakeFiles/STM32F301.dir/GPIO.cpp.obj"
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301 && C:\PROGRA~2\GNUTOO~1\92019-~1\bin\AR10B2~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\STM32F301.dir\GPIO.cpp.obj -c C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\core\STM32F301\GPIO.cpp

core/STM32F301/CMakeFiles/STM32F301.dir/GPIO.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STM32F301.dir/GPIO.cpp.i"
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301 && C:\PROGRA~2\GNUTOO~1\92019-~1\bin\AR10B2~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\core\STM32F301\GPIO.cpp > CMakeFiles\STM32F301.dir\GPIO.cpp.i

core/STM32F301/CMakeFiles/STM32F301.dir/GPIO.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STM32F301.dir/GPIO.cpp.s"
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301 && C:\PROGRA~2\GNUTOO~1\92019-~1\bin\AR10B2~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\core\STM32F301\GPIO.cpp -o CMakeFiles\STM32F301.dir\GPIO.cpp.s

core/STM32F301/CMakeFiles/STM32F301.dir/UART.cpp.obj: core/STM32F301/CMakeFiles/STM32F301.dir/flags.make
core/STM32F301/CMakeFiles/STM32F301.dir/UART.cpp.obj: ../core/STM32F301/UART.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object core/STM32F301/CMakeFiles/STM32F301.dir/UART.cpp.obj"
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301 && C:\PROGRA~2\GNUTOO~1\92019-~1\bin\AR10B2~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\STM32F301.dir\UART.cpp.obj -c C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\core\STM32F301\UART.cpp

core/STM32F301/CMakeFiles/STM32F301.dir/UART.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STM32F301.dir/UART.cpp.i"
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301 && C:\PROGRA~2\GNUTOO~1\92019-~1\bin\AR10B2~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\core\STM32F301\UART.cpp > CMakeFiles\STM32F301.dir\UART.cpp.i

core/STM32F301/CMakeFiles/STM32F301.dir/UART.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STM32F301.dir/UART.cpp.s"
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301 && C:\PROGRA~2\GNUTOO~1\92019-~1\bin\AR10B2~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\core\STM32F301\UART.cpp -o CMakeFiles\STM32F301.dir\UART.cpp.s

core/STM32F301/CMakeFiles/STM32F301.dir/ring_buffer.cpp.obj: core/STM32F301/CMakeFiles/STM32F301.dir/flags.make
core/STM32F301/CMakeFiles/STM32F301.dir/ring_buffer.cpp.obj: ../core/STM32F301/ring_buffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object core/STM32F301/CMakeFiles/STM32F301.dir/ring_buffer.cpp.obj"
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301 && C:\PROGRA~2\GNUTOO~1\92019-~1\bin\AR10B2~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\STM32F301.dir\ring_buffer.cpp.obj -c C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\core\STM32F301\ring_buffer.cpp

core/STM32F301/CMakeFiles/STM32F301.dir/ring_buffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/STM32F301.dir/ring_buffer.cpp.i"
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301 && C:\PROGRA~2\GNUTOO~1\92019-~1\bin\AR10B2~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\core\STM32F301\ring_buffer.cpp > CMakeFiles\STM32F301.dir\ring_buffer.cpp.i

core/STM32F301/CMakeFiles/STM32F301.dir/ring_buffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/STM32F301.dir/ring_buffer.cpp.s"
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301 && C:\PROGRA~2\GNUTOO~1\92019-~1\bin\AR10B2~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\core\STM32F301\ring_buffer.cpp -o CMakeFiles\STM32F301.dir\ring_buffer.cpp.s

# Object files for target STM32F301
STM32F301_OBJECTS = \
"CMakeFiles/STM32F301.dir/GPIO.cpp.obj" \
"CMakeFiles/STM32F301.dir/UART.cpp.obj" \
"CMakeFiles/STM32F301.dir/ring_buffer.cpp.obj"

# External object files for target STM32F301
STM32F301_EXTERNAL_OBJECTS =

build/arm/lib/libSTM32F301.a: core/STM32F301/CMakeFiles/STM32F301.dir/GPIO.cpp.obj
build/arm/lib/libSTM32F301.a: core/STM32F301/CMakeFiles/STM32F301.dir/UART.cpp.obj
build/arm/lib/libSTM32F301.a: core/STM32F301/CMakeFiles/STM32F301.dir/ring_buffer.cpp.obj
build/arm/lib/libSTM32F301.a: core/STM32F301/CMakeFiles/STM32F301.dir/build.make
build/arm/lib/libSTM32F301.a: core/STM32F301/CMakeFiles/STM32F301.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX static library ..\..\build\arm\lib\libSTM32F301.a"
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301 && $(CMAKE_COMMAND) -P CMakeFiles\STM32F301.dir\cmake_clean_target.cmake
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\STM32F301.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
core/STM32F301/CMakeFiles/STM32F301.dir/build: build/arm/lib/libSTM32F301.a

.PHONY : core/STM32F301/CMakeFiles/STM32F301.dir/build

core/STM32F301/CMakeFiles/STM32F301.dir/clean:
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301 && $(CMAKE_COMMAND) -P CMakeFiles\STM32F301.dir\cmake_clean.cmake
.PHONY : core/STM32F301/CMakeFiles/STM32F301.dir/clean

core/STM32F301/CMakeFiles/STM32F301.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\core\STM32F301 C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301 C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-debug\core\STM32F301\CMakeFiles\STM32F301.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : core/STM32F301/CMakeFiles/STM32F301.dir/depend

