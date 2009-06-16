/** \file
 * This header defines the YarpConnector class, which is the YARP-based implementation of the IConnector interface. It implements a connector to the ontology server based on the YARP middleware.
 */

#ifndef YARP_CONNECTOR_H_
#define YARP_CONNECTOR_H_

#include <yarp/os/Network.h>
#include <yarp/os/BufferedPort.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Time.h>

#include <stdlib.h>
#include <time.h>

#include "oro_connector.h"
#include "oro.h"


namespace oro
{


/** The YarpConnector class does actually send and recieve YARP messages.
*
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
class YarpConnector : public IConnector {

public:


	/** Creates a new connector to the ontology, using YARP as underlying communication framework.\n
	 * You are responsible for calling the YARP Network::init() function before creating a new YARP port.\n
	 * \param[in] port_name the name of the local YARP port (for instance, "my_application"), without any slashes.
 	 * \param[in] oro_in_port_name the name of the input YARP port of the ontology server, without any slashes. It will be suffixed with "/in". For instance, if you pass "oro", the actual port the connector will send request to will be "/oro/in"
	 */
	
	YarpConnector(const std::string port_name, const std::string oro_in_port_name);

	virtual ~YarpConnector();
	
	ServerResponse execute(const std::string query, const yarp::os::Bottle& args);
	ServerResponse execute(const std::string query, const std::vector<std::vector<std::string> >& args);
	ServerResponse execute(const std::string query, const std::vector<std::string>& args);
	ServerResponse execute(const std::string query);
	
	/** Executes a query but, unlike execute(), don't wait for an answer.
	 * 
	 */
	void executeDry(const std::string query);
	
	
private:
	void pourBottle(const yarp::os::Bottle&, std::vector<std::string>&);
	void pourBottle(const yarp::os::Bottle& bottle, std::vector<Concept>& result);
	void read(ServerResponse& response);
	void vectorToBottle(const std::vector<std::string>& data, yarp::os::Bottle& bottle);

	// Create two ports that we'll be using to transmit "Bottle" objects.
	// The ports are buffered, so that sending and receiving can happen
	// in the background.
	yarp::os::BufferedPort<yarp::os::Bottle> in;
	yarp::os::BufferedPort<yarp::os::Bottle> out;

	std::string in_port;
	std::string out_port;

	std::string oro_server;
	
	// Initialize YARP - some OSes need network and time service initialization
	static yarp::os::Network _yarp_network;
	
};


}

#endif /* YARP_CONNECTOR_H_ */
