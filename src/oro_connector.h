
#ifndef ORO_CONNECTOR_H_
#define ORO_CONNECTOR_H_

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

namespace oro {

/** This is an interface defining what is expected from a network connector to the ontology server.
 * 
 */
class IConnector {
	public:
		
	virtual void execute(const std::string query, const std::vector<std::string>& args) {}
	virtual void execute(const std::string query) {}

	
};

}

#endif /* ORO_CONNECTOR_H_ */
