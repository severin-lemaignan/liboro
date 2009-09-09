# - Try to find YARP
# Once done this will define
#
#  YARP_FOUND - system has YARP
#  YARP_INCLUDE_DIRS - the YARP include directory
#  YARP_LIBRARIES - Link these to use YARP
#  YARP_DEFINITIONS - Compiler switches required for using YARP
#
#  Copyright (c) 2009 slemaignan <slemaign@laas.fr>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (YARP_LIBRARIES AND YARP_INCLUDE_DIRS)
  # in cache already
  set(YARP_FOUND TRUE)
else (YARP_LIBRARIES AND YARP_INCLUDE_DIRS)
  # use pkg-config to get the directories and then use these values
  # in the FIND_PATH() and FIND_LIBRARY() calls
  if (${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4)
    include(UsePkgConfig)
    pkgconfig(yarp-2.2.2 _YARP_INCLUDEDIR _YARP_LIBDIR _YARP_LDFLAGS _YARP_CFLAGS)
  else (${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4)
    find_package(PkgConfig)
    if (PKG_CONFIG_FOUND)
      pkg_check_modules(_YARP yarp-2.2.2)
    endif (PKG_CONFIG_FOUND)
  endif (${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4)

  find_path(YARP_INCLUDE_DIR
    NAMES
      yarp/os/Network.h
    PATHS
      ${_YARP_INCLUDEDIR}
      /usr/include
      /usr/local/include
      /opt/local/include
      /sw/include
      $ENV{OPENROBOTS_BASE}/lib
  )
  mark_as_advanced(YARP_INCLUDE_DIR)

  find_library(YARP_OS_LIBRARY
    NAMES
      YARP_OS
    PATHS
      ${_YARP_LIBDIR}
      /usr/lib
      /usr/local/lib
      /opt/local/lib
      /sw/lib
      $ENV{OPENROBOTS_BASE}/lib
  )
  mark_as_advanced(YARP_OS_LIBRARY)

  if (YARP_OS_LIBRARY)
    set(YARP_OS_FOUND TRUE CACHE INTERNAL "Whether the YARP_OS library has been found" FORCE)
  endif (YARP_OS_LIBRARY)

  set(YARP_INCLUDE_DIRS
    ${YARP_INCLUDE_DIR}
  )

  if (YARP_OS_FOUND)
    set(YARP_LIBRARIES
      ${YARP_LIBRARIES}
      ${YARP_OS_LIBRARY}
    )
  endif (YARP_OS_FOUND)

  if (YARP_INCLUDE_DIRS AND YARP_LIBRARIES)
     set(YARP_FOUND TRUE)
  endif (YARP_INCLUDE_DIRS AND YARP_LIBRARIES)

  if (YARP_FOUND)
    if (NOT YARP_FIND_QUIETLY)
      message(STATUS "Found YARP: ${YARP_LIBRARIES}")
    endif (NOT YARP_FIND_QUIETLY)
  else (YARP_FOUND)
    if (YARP_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find YARP")
    endif (YARP_FIND_REQUIRED)
  endif (YARP_FOUND)

  # show the YARP_INCLUDE_DIRS and YARP_LIBRARIES variables only in the advanced view
  mark_as_advanced(YARP_INCLUDE_DIRS YARP_LIBRARIES)

endif (YARP_LIBRARIES AND YARP_INCLUDE_DIRS)

