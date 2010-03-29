/*
 * ©LAAS-CNRS (2008-2009)
 *
 * contributor(s) : Séverin Lemaignan <severin.lemaignan@laas.fr>
 *
 * This software is a computer program whose purpose is to interface
 * with an ontology server in a robotics context.
 *
 * This software is governed by the CeCILL  license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
*/

/** \file
 * This header defines the IConnector abstract base class (an inteface) that 
 * must be implemented by all the transport modules which aim to be used as 
 * connector to the ontology server.
 */

#ifndef ORO_CONNECTOR_H_
#define ORO_CONNECTOR_H_

#include <set>
#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <boost/variant.hpp>

namespace oro {

typedef boost::variant<	bool, 
						int, 
						double, 
						std::string, 
						std::set<std::string>, 
						std::map<std::string, std::string>
						 > server_param_types;

typedef boost::variant< bool, 
						int, 
						double, 
						std::string, 
						std::set<std::string>, 
						std::map<std::string, std::string>
						 > server_return_types;

struct ServerResponse {


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
	* In case of failure server-side, contains an error message describing the
	* error.
	*/
	std::string error_msg;

	/**
	* A container for the answers from the server. The actual type and content 
	* depend on the query.
	*/
	server_return_types result;
	
	/**
	* Holds the raw (ie JSON-encoded) value returned by the server.
	*/
	std::string raw_result;
		
	/**
	 * Default constructor
	 */
	ServerResponse():	status(ServerResponse::indeterminate), 
						exception_msg(""), 
						error_msg(""),
						raw_result("") {}
	
};

	
/** This is an interface defining what is expected from a network connector to 
 * the ontology server. 
 */
class IConnector {
	public:
		
		/**
	 	* This method is intended to perform a query execution with a flat list 
		* of parameters on the remote server, to wait for an answer and to 
		* return this answer.
		*/
		virtual ServerResponse execute(
							const std::string& query, 
							const std::vector<server_param_types>& args) = 0;
		
		virtual ServerResponse execute(
							const std::string& query, 
							const server_param_types& arg) = 0;
		
		/**
		 * This method is intended to perform a query execution without 
		 * parameters on the remote server, to wait for an answer and to return 
		 * this answer.
		 */
		virtual ServerResponse execute(const std::string& query) = 0;
		
		/**
		 * Sets the callback the connector will call when it receive an event 
		 * from the server. If the connector doesn't handle event, the implementation
		 * of this method may be omitted.
		 */
		virtual void setEventCallback(
				void (*evtCallback)(const std::string& event_id, 
									const server_return_types& raw_event_content)
				) {};
};

}

#endif /* ORO_CONNECTOR_H_ */
