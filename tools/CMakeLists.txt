
include_directories(../src)

##################################################
#                ORO-BENCHMARK                   #
##################################################

add_executable (oro-benchmark oro_benchmark.cpp)

target_link_libraries (oro-benchmark oro ${LIBS}) 

install (TARGETS oro-benchmark RUNTIME DESTINATION bin)

##################################################
#                ORO-QUERY                       #
##################################################

add_executable (oro-query oro_query.cpp)

target_link_libraries (oro-query oro ${LIBS}) 

install (TARGETS oro-query RUNTIME DESTINATION bin)

##################################################
#                ORO-EXPORT                      #
##################################################

add_executable (oro-export oro_export.cpp)

target_link_libraries (oro-export oro ${LIBS}) 

install (TARGETS oro-export RUNTIME DESTINATION bin)

##################################################
#                ORO-STATS                       #
##################################################

add_executable (oro-stats oro_stats.cpp)

target_link_libraries (oro-stats oro ${LIBS}) 

install (TARGETS oro-stats RUNTIME DESTINATION bin)

##################################################
#                ORO-TEST                        #
##################################################

add_executable (oro-test oro_test.cpp)

target_link_libraries (oro-test oro ${LIBS}) 

install (TARGETS oro-test RUNTIME DESTINATION bin)
