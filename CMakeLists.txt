# The name of the project is "LIBORO". CMakeLists files in this project can
# refer to the root source directory of the project as ${LIBORO_SOURCE_DIR} and
# to the root binary directory of the project as ${LIBORO_BINARY_DIR}.
cmake_minimum_required (VERSION 2.6)
PROJECT(liboro)

#Uncomment next line to get verbose makefiles.
#SET(CMAKE_VERBOSE_MAKEFILE true)

# Pick up our scripts - they are all in the conf subdirectory
SET(LIBORO_MODULE_PATH ${PROJECT_SOURCE_DIR}/conf)
SET(CMAKE_MODULE_PATH ${LIBORO_MODULE_PATH})

#Include boost libraries
FIND_PACKAGE(Boost 1.34)
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )
LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS} )

#Include YARP
FIND_PACKAGE(YARP REQUIRED)
INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})


SET(LIBS ${LIBS} ${YARP_LIBRARIES} ${Boost_LIBRARIES})

SET(BUILD_SHARED_LIBS true)

# Get the current liboro version
INCLUDE(LiboroVersion)
ADD_DEFINITIONS(-DORO_VERSION="${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

INCLUDE("${CMAKE_MODULE_PATH}/TargetDoc.cmake" OPTIONAL)


ADD_SUBDIRECTORY (liboro-src)
ADD_SUBDIRECTORY (oro-apps-src) 

