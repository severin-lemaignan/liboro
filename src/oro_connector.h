
#ifndef ORO_CONNECTOR_H_
#define ORO_CONNECTOR_H_

#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

namespace oro {

/**
 * TODO : Use templates for the result member.
 */
struct  ServerResponse {

	enum Status {
		ok,
		failed,
  		indeterminate
	} status;

	/**
	* In case of failure server-side, contains the name of the thrown exception.
	*/
	std::string exception_msg;

	/**
	* In case of failure server-side, contains an error message describing the error.
	*/
	std::string error_msg;

	/**
	* a vector of string containing the answers from the server. The actual content depends on the query.
	*/
	std::vector<std::string> result;
	
	/**
	 * Default constructor
	 */
	ServerResponse():status(ServerResponse::indeterminate), exception_msg(""), error_msg(""), result(std::vector<std::string>()) {}
	
};

	
/** This is an interface defining what is expected from a network connector to the ontology server.
 * 
 */
class IConnector {
	public:
		virtual ServerResponse execute(const std::string query, const std::vector<std::string>& args) {}
		virtual ServerResponse execute(const std::string query) {}
	
};

}

#endif /* ORO_CONNECTOR_H_ */
