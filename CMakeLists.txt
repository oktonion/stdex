
cmake_minimum_required (VERSION 3.15)

# Import modules
include(CheckCompilerFlag)
include(CheckLinkerFlag)

# Set the project name
project ("stdex_tests" VERSION 1.0.0 LANGUAGES C CXX)

# Set project variables
set(TESTS_DIRECTORY ${PROJECT_SOURCE_DIR}/tests)
set(STDEX_DIRECTORY ${PROJECT_SOURCE_DIR}/stdex)

# Set CMake variables
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${TESTS_DIRECTORY}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${TESTS_DIRECTORY}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${TESTS_DIRECTORY}/bin")
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${TESTS_DIRECTORY}/bin")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${TESTS_DIRECTORY}/bin")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${TESTS_DIRECTORY}/bin")
endforeach()


# Set compiler and linker flags
check_compiler_flag(CXX -fpermissive SUPPORTS_PERMISSIVE)
check_compiler_flag(CXX -ftemplate-backtrace-limit=0 SUPPORTS_TEMPLATE_BACKTRACE_LIMIT)
check_compiler_flag(CXX -fmax-errors=0 SUPPORTS_MAX_ERRORS)
check_compiler_flag(CXX -Wno-invalid-offsetof SUPPORTS_NO_INVALID_OFFSETOF)
check_compiler_flag(CXX -std=gnu++98 SUPPORTS_GNU_CXX_STANDARD_98)
if (SUPPORTS_PERMISSIVE)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fpermissive>)
endif()
if (SUPPORTS_TEMPLATE_BACKTRACE_LIMIT)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-ftemplate-backtrace-limit=0>)
endif()
if (SUPPORTS_MAX_ERRORS)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-fmax-errors=0>)
endif()
if (SUPPORTS_NO_INVALID_OFFSETOF)
    add_compile_options($<$<COMPILE_LANGUAGE:CXX>:-Wno-invalid-offsetof>)
endif()

# Platform specific: add compile definitions, libraries

add_compile_definitions(DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN)

if (NOT ${CMAKE_CXX_STANDARD})
    if (${CMAKE_COMPILER_IS_GNUCC} AND ${SUPPORTS_GNU_CXX_STANDARD_98})
        set(CMAKE_CXX_STANDARD 98)
    elseif(NOT ${CMAKE_COMPILER_IS_GNUCC})
        set(CMAKE_CXX_STANDARD 98)
    endif()
endif()
message(STATUS "Target compiler CXX standard is C++" ${CMAKE_CXX_STANDARD})

if (CMAKE_SYSTEM MATCHES Windows*)
    set(TARGET_SYSTEM_CAT "Windows")
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS _SCL_SECURE_NO_WARNINGS _WINSOCK_DEPRECATED_NO_WARNINGS)
    message(STATUS "Target system '${CMAKE_SYSTEM}' is ${TARGET_SYSTEM_CAT}")

    if (BORLAND)
        set(ADDITIONAL_LIBS cw32mt.lib)
    endif()

    
    set(PTHREAD_DIRECTORY ${PROJECT_SOURCE_DIR}/pthread)

else()
    if (CMAKE_SYSTEM MATCHES QNX6 OR CMAKE_SYSTEM MATCHES QNX*6*)
        set(TARGET_SYSTEM_CAT "QNX 6")
        message(STATUS "Target system '${CMAKE_SYSTEM}' is ${TARGET_SYSTEM_CAT}")
        set(ADDITIONAL_LIBS m)

    elseif (CMAKE_SYSTEM MATCHES QNX4 OR CMAKE_SYSTEM MATCHES QNX*4*)
        set(TARGET_SYSTEM_CAT "QNX 4")
        message(STATUS "Target system '${CMAKE_SYSTEM}' is ${TARGET_SYSTEM_CAT}")
        set(ADDITIONAL_LIBS m)

    else ()
        set(TARGET_SYSTEM_CAT "Unix")
        message(STATUS "Target system '${CMAKE_SYSTEM}' is unknown: defaulting to ${TARGET_SYSTEM_CAT}")

    endif()
endif()


# Add includes
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${STDEX_DIRECTORY}
    ${PTHREAD_DIRECTORY}
)


# Create executable Tests

aux_source_directory("${TESTS_DIRECTORY}" TESTS_SOURCES)
list(FILTER TESTS_SOURCES EXCLUDE REGEX .*fail.*)

set(FAIL_TESTS "")

foreach(test_source_file ${TESTS_SOURCES})
    get_filename_component(test_name ${test_source_file} NAME_WE)
    add_executable(${test_name} 
        ${test_source_file}
    )
    set_property(TARGET ${test_name} PROPERTY WORKING_DIRECTORY "${TESTS_DIRECTORY}/bin")
    set_property(TARGET ${test_name} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${TESTS_DIRECTORY}/bin")
    target_link_libraries(${test_name}
        ${ADDITIONAL_LIBS}
        stdex
    )
endforeach(test_source_file ${TEST_SOURCES})

# file(GENERATE OUTPUT "OUTPUT_DIRECTORY_TEST.txt" CONTENT RUNTIME_OUTPUT_DIRECTORY_$<UPPER_CASE:$<CONFIG>>)

string(REPLACE ";" "," SUCCESS_TESTS "${TESTS_SOURCES}")

# main library
add_subdirectory(stdex)