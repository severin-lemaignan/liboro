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
 * This header defines the SocketConnector class, which is the socket-based 
 * implementation of the IConnector interface. It implements a connector to the
 * ontology server based on the TCP sockets.
 */

#ifndef SOCKET_CONNECTOR_H_
#define SOCKET_CONNECTOR_H_

//the maximum amount of ms the processus should wait for an answer from the 
//ontology server.
#define ORO_MAX_DELAY 1500

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <stdlib.h>
#include <time.h>

#include "oro_connector.h"
#include "oro.h"


#define MSG_FINALIZER "#end#\n"
#define MSG_SEPARATOR "\n"

namespace oro
{


/** A class responsible for communication with the ontology server via sockets.
*
* TODO: Update the doc
* The structure of these messages is based on YARP bottles. *
* This lib send that kind of message to the server:
* \verbatim
* ([YARP port for answering] [name of the method] ([param1] [param2] [...]))
* \endverbatim
* Parameters are enclosed in a nested bottle (a list), and these parameters can 
* themselves be lists.
* Currently, these list are casted to vectors of string.
*
* The server answer a result bottle of this kind:
* \verbatim
* ([ok|error] [result|error msg])
* \endverbatim
* Result is a list (a nested bottle) of objects.
*/
class SocketConnector : public IConnector {

public:


	/** Creates a new connector to the ontology, using YARP as underlying communication framework.\n
	 * You are responsible for calling the YARP Network::init() function before creating a new YARP port.\n
	 * \param[in] port_name the name of the local YARP port (for instance, "my_application"), without any slashes.
 	 * \param[in] oro_in_port_name the name of the input YARP port of the ontology server, without any slashes. It will be suffixed with "/in". For instance, if you pass "oro", the actual port the connector will send request to will be "/oro/in"
	 */
	
	SocketConnector(const std::string hostname, const std::string port);

	virtual ~SocketConnector();
	
	/* IConnector interface implementation */
	ServerResponse execute(const std::string& query, const std::vector<server_param_types>& args);
	ServerResponse execute(const std::string& query, const server_param_types& arg);
	ServerResponse execute(const std::string& query);
	
	void setEventCallback(
				void (*evtCallback)(const std::string& event_id, 
									const server_return_types& raw_event_content)
				);
				
	/** Executes a query but, unlike execute(), don't wait for an answer.
	 * 
	 */
	void executeDry(const std::string query);
	
	static void serializeSet(const std::set<std::string>& data, std::string& msg);
	static void serializeVector(const std::vector<std::string>& data, std::string& msg);
	static void serializeMap(const std::map<std::string, std::string>& data, std::string& msg);
	
private:

	static std::string& protectValue(std::string& value);
	static std::string& cleanValue(std::string& value);
	
	void deserialize(const std::string& msg, server_return_types& result);
	server_return_types makeCollec(const std::string& msg);
	void read(ServerResponse& response);
	int msleep(unsigned long milisec);

	bool isConnected;

	// Socket related fields
	int sockfd;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	
	// The event callback
	void (*_evtCallback)(const std::string& event_id, 
						const server_return_types& raw_event_content);
};

/**
 * Visitor for serialization of the method parameters before querying
 * the ontology server.
 */
class ParametersSerializationHolder : public boost::static_visitor<>
{
	std::string args;
	
public:

	ParametersSerializationHolder() : args("") {}

    void operator()(const int i)
    {
        args += i;
		args += MSG_SEPARATOR;
    }
	
	void operator()(const double i)
    {
        args += i;
		args += MSG_SEPARATOR;
    }

    void operator()(const std::string & str)
    {
        args += str + MSG_SEPARATOR;
    }
	
	void operator()(const bool b)
    {
        b ? args += "true" : args += "false";
		args += MSG_SEPARATOR;
    }
	
	void operator()(const std::set<std::string> & strs)
    {
		SocketConnector::serializeSet(strs, args);
		args += MSG_SEPARATOR;
    }
	
	void operator()(const std::vector<std::string> & strs)
    {
		SocketConnector::serializeVector(strs, args);
		args += MSG_SEPARATOR;
    }
	
	void operator()(const std::map<std::string, std::string> & strs)
    {
		SocketConnector::serializeMap(strs, args);
		args += MSG_SEPARATOR;
    }
	
	std::string& getArgs() {return args;}
	

};

}

#endif /* SOCKET_CONNECTOR_H_ */
