# Install script for directory: C:/Users/eitowiec/Desktop/02_zumo/03_zumo_NEW_IMPL/core

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/ZUMO")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/eitowiec/Desktop/02_zumo/03_zumo_NEW_IMPL/cmake-build-debug/core/STM32F301/cmake_install.cmake")
  include("C:/Users/eitowiec/Desktop/02_zumo/03_zumo_NEW_IMPL/cmake-build-debug/core/ZUMO_hal/cmake_install.cmake")
  include("C:/Users/eitowiec/Desktop/02_zumo/03_zumo_NEW_IMPL/cmake-build-debug/core/ZUMO_devices/cmake_install.cmake")
  include("C:/Users/eitowiec/Desktop/02_zumo/03_zumo_NEW_IMPL/cmake-build-debug/core/external/cmake_install.cmake")
  include("C:/Users/eitowiec/Desktop/02_zumo/03_zumo_NEW_IMPL/cmake-build-debug/core/command_terminal/cmake_install.cmake")
  include("C:/Users/eitowiec/Desktop/02_zumo/03_zumo_NEW_IMPL/cmake-build-debug/core/cores/cmake_install.cmake")

endif()

