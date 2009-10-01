/** \file
 * This header defines the YarpConnector class, which is the YARP-based implementation of the IConnector interface. It implements a connector to the ontology server based on the YARP middleware.
 */

#ifndef SOCKET_CONNECTOR_H_
#define SOCKET_CONNECTOR_H_

//the maximum amount of ms the processus should wait for an answer from the ontology server.
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


/** The SocketConnector class does actually send and recieve messages to the ontology server.
*
* TODO: Update the doc
* The structure of these messages is based on YARP bottles. *
* This lib send that kind of message to the server:
* \verbatim
* ([YARP port for answering] [name of the method] ([param1] [param2] [...]))
* \endverbatim
* Parameters are enclosed in a nested bottle (a list), and these parameters can themselves be lists.
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
	
	ServerResponse execute(const std::string& query, const std::vector<server_param_types>& args);
	ServerResponse execute(const std::string& query, const server_param_types& arg);
	ServerResponse execute(const std::string& query);
	
	/** Executes a query but, unlike execute(), don't wait for an answer.
	 * 
	 */
	void executeDry(const std::string query);
	
	static void serializeSet(const std::set<std::string>& data, std::string& msg);
	static void serializeVector(const std::vector<std::string>& data, std::string& msg);
	static void serializeMap(const std::map<std::string, std::string>& data, std::string& msg);
	
private:
	void deserialize(const std::string& msg, server_return_types& result);
	server_return_types makeCollec(const std::string& msg);
	void read(ServerResponse& response);
	int msleep(unsigned long milisec);

	bool isConnected;
	
	// Socket related fields
	int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
		
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
