/** \file
 * This header defines the IConnector abstract base class (an inteface) that must be implemented by all the transport modules which aim to be used as connector to the ontology server.
 */

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
		
		/**
	 	* This method is intended to perform a query execution with a flat list of parameters on the remote server, to wait for an answer and to return this answer.
		 */
		virtual ServerResponse execute(const std::string query, const std::vector<std::string>& args) {}
		
		/**
		 * This method is intended to perform a query execution with a list of list of data as parameter on the remote server, to wait for an answer and to return this answer.
		 * 
		 * Examples of RPC call possibly relying on this prototype of \p execute include the \link Ontology::find(const std::string&, const std::vector<std::string>&, std::vector<Concept>&) find \endlink and \link Ontology::find(const std::string&, const std::vector<std::string>&, const std::vector<std::string>&, std::vector<Concept>&) filtredFind \endlink methods.
		 */
		virtual ServerResponse execute(const std::string query, const std::vector<std::vector<std::string> >& args) {}
		
		/**
		 * This method is intended to perform a query execution without parameters on the remote server, to wait for an answer and to return this answer.
		 */
		virtual ServerResponse execute(const std::string query) {}
	
};

}

#endif /* ORO_CONNECTOR_H_ */
