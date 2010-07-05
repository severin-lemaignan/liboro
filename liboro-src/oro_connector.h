
/*
 * Copyright (c) 2008-2010 LAAS-CNRS Séverin Lemaignan slemaign@laas.fr
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/** \file
 * This header defines the IConnector abstract base class (an inteface) that 
 * must be implemented by all the transport modules which aim to be used as 
 * connector to the ontology server.
 */

#ifndef ORO_CONNECTOR_H_
#define ORO_CONNECTOR_H_

#include <set>
#include <utility>
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

typedef std::pair<std::string, std::vector<server_param_types> > query_type;

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
		 * from the server. If the connector doesn't handle events, the 
		 * implementation of this method may be omitted.
		 */
		virtual void setEventCallback(
				void (*evtCallback)(const std::string& event_id, 
									const server_return_types& raw_event_content)
				) {};
};

}

#endif /* ORO_CONNECTOR_H_ */
