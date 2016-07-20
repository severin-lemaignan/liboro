cmake_minimum_required (VERSION 2.6)
project(liboro)

# Pick up our scripts - they are all in the conf subdirectory
set(LIBORO_MODULE_PATH ${PROJECT_SOURCE_DIR}/conf)
set(CMAKE_MODULE_PATH ${LIBORO_MODULE_PATH})

find_package(Boost REQUIRED COMPONENTS system thread)
include_directories(${Boost_INCLUDE_DIRS})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

set(BUILD_SHARED_LIBS true)

# Get the current liboro version
include(LiboroVersion)
add_definitions(-pthread -DORO_VERSION="${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

include("${CMAKE_MODULE_PATH}/TargetDoc.cmake" OPTIONAL)

add_subdirectory(src)

##########################################
##       PKGCONFIG file                ###
##########################################

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/liboro.pc.in")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/liboro.pc.in 
                   ${CMAKE_CURRENT_BINARY_DIR}/liboro.pc
                   @ONLY)
    message(STATUS "configured ${CMAKE_CURRENT_SOURCE_DIR}/liboro.pc.in --> ${CMAKE_CURRENT_BINARY_DIR}/liboro.pc")

    install(FILES
            ${CMAKE_CURRENT_BINARY_DIR}/liboro.pc
            DESTINATION lib/pkgconfig)
endif()

##########################################
##           Options                   ###
##########################################


option (COMPILE_TESTS "Compile test/demo applications" OFF)
option (DEBUG "Display debug messages" OFF)

if (DEBUG)
    add_definitions(-DDEBUG)
endif()


if (COMPILE_TESTS)
    add_subdirectory (testing) 
endif()

