cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set source and include directories
set(SYSTEM_TIMING_SRC_DIR "src")
set(SYSTEM_TIMING_INCLUDE_DIR "include")

# Set source files 
set(SYSTEM_TIMING_SRC 
  "system_timing.cpp"
  "processing_flags.cpp"
  "iop_file_interface.cpp"
)

# Prepend the source directory path to all the source files
PREPEND(SYSTEM_TIMING_SRC ${SYSTEM_TIMING_SRC_DIR} ${SYSTEM_TIMING_SRC})

# Set the include files
set(SYSTEM_TIMING_INCLUDE 
  "system_timing.hpp"
  "processing_flags.hpp"
  "iop_file_interface.hpp"
  "to_string.hpp"
)

# Prepend the include directory path to all the include files
PREPEND(SYSTEM_TIMING_INCLUDE ${SYSTEM_TIMING_INCLUDE_DIR} ${SYSTEM_TIMING_INCLUDE})

# Create the library from the source and include files
add_library(SystemTiming SHARED ${SYSTEM_TIMING_SRC} ${SYSTEM_TIMING_INCLUDE})
add_library(${PROJECT_NAME}::SystemTiming ALIAS SystemTiming)

# Specify the include directory to be exported for other moduels to use. The 
# PUBLIC keyword here allows other libraries or exectuables to link to this
# library and use its functionality.
target_include_directories(SystemTiming PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:${SYSTEM_TIMING_PUBLIC_NAME}/include>
 ) 
