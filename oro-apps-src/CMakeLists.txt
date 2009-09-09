
#The first one doesn't work, but should? looks better at least...
#include_directories (${LIBORO_SOURCE_DIR}/liboro-src)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../liboro-src)

link_directories (${LINK_DIRECTORIES})

##################################################
#                ORO-BENCHMARK                   #
##################################################

add_executable (oro-benchmark oro_benchmark.cpp)

target_link_libraries (oro-benchmark oro ${LIBS}) 

SET_TARGET_PROPERTIES(oro-benchmark PROPERTIES
	INSTALL_RPATH_USE_LINK_PATH TRUE
	INSTALL_RPATH ${INSTALL_PREFIX}/lib
	BUILD_WITH_INSTALL_RPATH TRUE )

INSTALL (TARGETS oro-benchmark
	RUNTIME DESTINATION bin
)

##################################################
#                ORO-QUERY                       #
##################################################

add_executable (oro-query oro_query.cpp)

target_link_libraries (oro-query oro ${LIBS}) 

SET_TARGET_PROPERTIES(oro-query PROPERTIES
	INSTALL_RPATH_USE_LINK_PATH TRUE
	INSTALL_RPATH ${INSTALL_PREFIX}/lib
	BUILD_WITH_INSTALL_RPATH TRUE )

INSTALL (TARGETS oro-query
	RUNTIME DESTINATION bin
)

##################################################
#                ORO-EXPORT                      #
##################################################

add_executable (oro-export oro_export.cpp)

target_link_libraries (oro-export oro ${LIBS}) 

SET_TARGET_PROPERTIES(oro-export PROPERTIES
	INSTALL_RPATH_USE_LINK_PATH TRUE
	INSTALL_RPATH ${INSTALL_PREFIX}/lib
	BUILD_WITH_INSTALL_RPATH TRUE )

INSTALL (TARGETS oro-export
	RUNTIME DESTINATION bin
)

##################################################
#                ORO-STATS                       #
##################################################

add_executable (oro-qos oro_stats.cpp)

target_link_libraries (oro-qos oro ${LIBS}) 

SET_TARGET_PROPERTIES(oro-qos PROPERTIES
	INSTALL_RPATH_USE_LINK_PATH TRUE
	INSTALL_RPATH ${INSTALL_PREFIX}/lib
	BUILD_WITH_INSTALL_RPATH TRUE )

INSTALL (TARGETS oro-qos
	RUNTIME DESTINATION bin
)

##################################################
#                ORO-TEST                        #
##################################################

#add_executable (oro-test oro_test_suite.cpp)
#target_link_libraries (oro-test oro ${LIBS}) 
#
#SET_TARGET_PROPERTIES(oro-test PROPERTIES
#	INSTALL_RPATH_USE_LINK_PATH TRUE
#	INSTALL_RPATH ${INSTALL_PREFIX}/lib
#	BUILD_WITH_INSTALL_RPATH TRUE )
#
#INSTALL (TARGETS oro-test
#	RUNTIME DESTINATION bin
#)	

add_executable (oro-test test.cpp)

target_link_libraries (oro-test oro ${LIBS}) 

SET_TARGET_PROPERTIES(oro-test PROPERTIES
	INSTALL_RPATH_USE_LINK_PATH TRUE
	INSTALL_RPATH ${INSTALL_PREFIX}/lib
	BUILD_WITH_INSTALL_RPATH TRUE )
