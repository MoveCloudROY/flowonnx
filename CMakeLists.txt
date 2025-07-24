cmake_minimum_required(VERSION 3.16)

project(flowonnx VERSION 0.0.1.1 LANGUAGES C CXX)
set(PROJECT_FULL_NAME "Diffsinger Flow OnnxRuntime Inference Engine")

# ----------------------------------
# CMake Policies
# ----------------------------------

# Enable RPATH on MacOS/OSX
if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif()

# Always link with full path
if(POLICY CMP0060)
    cmake_policy(SET CMP0060 NEW)
endif()

# Use <PackageName>_ROOT env. variable as a prefix
if(POLICY CMP0074)
    cmake_policy(SET CMP0074 NEW)
endif()


# ----------------------------------
# Build Options
# ----------------------------------
option(FLOWONNX_BUILD_STATIC "Build static library" ON)
option(FLOWONNX_INSTALL "Install library" ON)
option(FLOWONNX_BUILD_TESTS "Build test cases" ON)

# ----------------------------------
# CMake Settings
# ----------------------------------
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    message(STATUS "CMAKE_BUILD_TYPE is unset, defaulting to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel RelWithAssert RelWithMDd...")
endif()

if(NOT DEFINED CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 99)
endif()

if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
endif()

if(NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

if(NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

if(NOT DEFINED CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
endif()

if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /manifest:no")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /manifest:no")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")

    if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
        set(CMAKE_DEBUG_POSTFIX "d")
    endif()
elseif(MINGW)
    set(CMAKE_STATIC_LIBRARY_PREFIX "")
    set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif()

if(FLOWONNX_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)
endif()

# ----------------------------------
# Project Variables
# ----------------------------------
set(FLOWONNX_VERSION ${PROJECT_VERSION})
set(FLOWONNX_INSTALL_NAME ${PROJECT_NAME})

# ----------------------------------
# Add source modules
# ----------------------------------
find_package(nlohmann_json REQUIRED)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)
add_subdirectory(src)

if(FLOWONNX_BUILD_TESTS)
    add_subdirectory(tests)
endif()