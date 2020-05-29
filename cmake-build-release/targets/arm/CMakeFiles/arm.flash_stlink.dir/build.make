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
CMAKE_BINARY_DIR = C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-release

# Utility rule file for arm.flash_stlink.

# Include the progress variables for this target.
include targets/arm/CMakeFiles/arm.flash_stlink.dir/progress.make

targets/arm/CMakeFiles/arm.flash_stlink: build/arm/bin/arm
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Flashing target hardware"
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-release\build\arm\bin && openocd -f interface/stlink-v2-1.cfg -c "transport select hla_swd" -f target/stm32f3x.cfg -c init -c "reset halt" -c "flash write_image erase arm" -c "reset run" -c shutdown

arm.flash_stlink: targets/arm/CMakeFiles/arm.flash_stlink
arm.flash_stlink: targets/arm/CMakeFiles/arm.flash_stlink.dir/build.make

.PHONY : arm.flash_stlink

# Rule to build all files generated by this target.
targets/arm/CMakeFiles/arm.flash_stlink.dir/build: arm.flash_stlink

.PHONY : targets/arm/CMakeFiles/arm.flash_stlink.dir/build

targets/arm/CMakeFiles/arm.flash_stlink.dir/clean:
	cd /d C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-release\targets\arm && $(CMAKE_COMMAND) -P CMakeFiles\arm.flash_stlink.dir\cmake_clean.cmake
.PHONY : targets/arm/CMakeFiles/arm.flash_stlink.dir/clean

targets/arm/CMakeFiles/arm.flash_stlink.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\targets\arm C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-release C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-release\targets\arm C:\Users\eitowiec\Desktop\02_zumo\03_zumo_NEW_IMPL\cmake-build-release\targets\arm\CMakeFiles\arm.flash_stlink.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : targets/arm/CMakeFiles/arm.flash_stlink.dir/depend

