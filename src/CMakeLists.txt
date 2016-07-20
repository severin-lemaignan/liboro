
set(ORO_HEADERS oro.h 
                oro_event.h 
                oro_connector.h 
                oro_exceptions.h 
                socket_connector.h 
                oro_library.h 
                dummy_connector.h)

add_library (oro
             ontology.cpp
             concepts.cpp
             socket_connector.cpp
             class.cpp
             property.cpp
             statement.cpp
             dummy_connector.cpp
             oro_library.cpp) 

target_link_libraries(oro ${LIBS})

install (TARGETS oro
         LIBRARY DESTINATION lib
         ARCHIVE DESTINATION lib)

install(FILES ${ORO_HEADERS} DESTINATION include/liboro)

