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

	
/**
 * This class defines which methods are open to <a href="http://eris.liralab.it/yarp/">YARP</a> requests.<br/>
 */
class YarpConnector : public IConnector {

public:

	YarpConnector(const std::string port_name, const std::string oro_in_port_name);
	
	virtual ~YarpConnector();
	
	ServerResponse execute(const std::string query, const std::vector<std::string>& args);
	ServerResponse execute(const std::string query);
	
private:
	void pourBottle(yarp::os::Bottle&, std::vector<std::string>&);
	void pourBottle(yarp::os::Bottle& bottle, std::vector<Concept>& result);
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