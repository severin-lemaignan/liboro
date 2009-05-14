
include_directories (${LIBORO_SOURCE_DIR}/liboro-src)

link_directories (${LINK_DIRECTORIES})

add_executable (oro-benchmark oro_benchmark.cpp)

target_link_libraries (oro-benchmark oro ${LIBS}) 

SET_TARGET_PROPERTIES(oro-benchmark PROPERTIES
	INSTALL_RPATH_USE_LINK_PATH TRUE
	INSTALL_RPATH ${INSTALL_PREFIX}/lib
	BUILD_WITH_INSTALL_RPATH TRUE )

INSTALL (TARGETS oro-benchmark
	RUNTIME DESTINATION bin
)
	
