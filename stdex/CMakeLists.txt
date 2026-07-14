# CMakeList.txt : CMake project for Visual Modeling Code, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

project ("stdex" VERSION 0.2.12 LANGUAGES C CXX)

# Set project variables

set(LIB_SOURCES 
    ${PROJECT_SOURCE_DIR}/src/chrono.cpp
    ${PROJECT_SOURCE_DIR}/src/mutex.cpp
    ${PROJECT_SOURCE_DIR}/src/thread.cpp
)

# Set CMake variables

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/lib")
foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${PROJECT_SOURCE_DIR}/lib")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${PROJECT_SOURCE_DIR}/lib")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} "${PROJECT_SOURCE_DIR}/lib")
endforeach()

# Add includes

include_directories(
    ${PROJECT_SOURCE_DIR} 
)

if (CMAKE_SYSTEM MATCHES Windows*)
    include_directories(
        ${PROJECT_SOURCE_DIR} 
        ${PROJECT_SOURCE_DIR}/../pthread/
    )
    set(LIB_SOURCES ${LIB_SOURCES} ${PROJECT_SOURCE_DIR}/../pthread-win32/pthread-EH.cpp)
    add_compile_definitions(HAVE_CONFIG_H PTW32_STATIC_LIB __PTHREAD_JUMBO_BUILD__=1)
endif()

# Create lib

add_library(${PROJECT_NAME} STATIC
    ${LOGICS_SOURCES}
    ${LIB_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES WORKING_DIRECTORY "${TESTS_DIRECTORY}/lib")
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${TESTS_DIRECTORY}/lib")
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "-d")


# TODO: Add tests and install targets if needed.
