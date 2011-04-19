# Create a library called "oro"

set(ORO_HEADERS oro.h oro_event.h oro_connector.h oro_exceptions.h socket_connector.h oro_library.h dummy_connector.h)

ADD_LIBRARY (oro ontology.cpp concepts.cpp socket_connector.cpp class.cpp property.cpp statement.cpp dummy_connector.cpp oro_library.cpp) 

#set_target_properties(oro
#                      PROPERTIES PUBLIC_HEADER "${ORO_HEADERS}"
#)

TARGET_LINK_LIBRARIES(oro ${LIBS})

INSTALL (TARGETS oro
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
#	PUBLIC_HEADER DESTINATION include/liboro
)

INSTALL(FILES
	${ORO_HEADERS}
	DESTINATION include/liboro
)

