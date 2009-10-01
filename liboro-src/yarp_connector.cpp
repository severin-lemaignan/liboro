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

#include <iostream>
#include <iterator>
#include <algorithm>
#include <time.h>

#include "oro_exceptions.h"
#include "yarp_connector.h"

using namespace yarp::os;
using namespace std;

namespace oro {

	//Defines two constants needed to decode YARP status.
	const char* OK = "ok";
	const char* ERROR = "error";


class ParametersSerializationHolder; //forward declaration
	
YarpConnector::YarpConnector(const string port_name, const string oro_in_port_name){

	Network::init();
	
	// Work locally - don't rely on name server (just for this example).
	// If you have a YARP name server running, you can remove this line.
	//Network::setLocalMode(true);

	//Checks that a YARP nameserver is reachable.
	if (! Network::checkNetwork()) {
		cerr << "Looks like there is no YARP server started! Exiting.";
		throw ConnectorException("No YARP server can be found. Abandon.");
		exit(0);
	}

	// we will want to read every message, with no skipping of "old" messages
	// when new ones come in
	in.setStrict();


	// Name the ports
	oro_server = oro_in_port_name;

	in_port = "/" + port_name + "/in";
	out_port = "/" + port_name + "/out";
	
	in.open( in_port.c_str() );
	out.open( out_port.c_str() );

	// Connect the local output port to the ontology server incoming port.
	// No connection to the server results port since this connection is handled by the ontology server itself.
	Network::connect(out_port.c_str() ,("/" + oro_server + "/in").c_str() );
	
	//cout << "Connection to Yarp network ok" << endl;

}

YarpConnector::~YarpConnector(){
	cout << "Closing YARP connection...";
	
	if (Network::isConnected(out_port.c_str() ,("/" + oro_server + "/in").c_str() )) {
		executeDry("close");
		Network::disconnect(out_port.c_str() ,("/" + oro_server + "/in").c_str() );
	}
	in.close();
	out.close();
	
	Network::fini();
	
	cout << "done." << endl;
}

ServerResponse YarpConnector::execute(const string query, const Bottle& args){
	
	ServerResponse res;
	
	// Send one "Bottle" object.  The port is responsible for creating
	// and reusing/destroying that object, since it needs to be sure
	// it exists until communication to all recipients (just one in
	// this case) is complete.
		
	Bottle& outBot = out.prepare();   // Get the object
	outBot.clear();

	outBot.fromString((in_port + " " + query).c_str()); //Prepend the query with the port name where the server should send back the result.

	Bottle& argsBot = outBot.addList();
	
	argsBot.append(args);
	
	//printf("Writing bottle (%s)\n", outBot.toString().c_str());
	
	out.write();                       // Now send it on its way
	
	read(res);
	
	return res;
}

ServerResponse YarpConnector::execute(const string query, const vector<server_param_types>& vect_args){
		
	ParametersSerializationHolder paramsHolder;
	
	//serialization of arguments
	std::for_each(
		vect_args.begin(), 
		vect_args.end(), 
		boost::apply_visitor(paramsHolder)
	);
		
	return execute(query, paramsHolder.getBottle());
}

ServerResponse YarpConnector::execute(const string query, const server_param_types& arg){
	
	ParametersSerializationHolder paramsHolder;
	
	//serialization of argument
	boost::apply_visitor(paramsHolder, arg);
		
	return execute(query, paramsHolder.getBottle());

}


ServerResponse YarpConnector::execute(const string query){
	
	ServerResponse res;

	// Send one "Bottle" object.  The port is responsible for creating
	// and reusing/destroying that object, since it needs to be sure
	// it exists until communication to all recipients (just one in
	// this case) is complete.

	Bottle& outBot = out.prepare();   // Get the object
	outBot.clear();
	outBot.fromString((in_port + " " + query).c_str()); //Prepend the query with the port name where the server should send back the result.

	//printf("Writing bottle (%s)\n", outBot.toString().c_str());

	out.write();                       // Now send it on its way
	
	//cout << "Waiting for an answer from oro-server" << endl;
	read(res);
	//cout << "Got the answer!" << endl;
	
	return res;
}

void YarpConnector::executeDry(const string query){
	
	Bottle& outBot = out.prepare();   // Get the object
	outBot.clear();
	outBot.fromString((in_port + " " + query).c_str()); //Prepend the query with the port name where the server should send back the result.

	out.write();                       // Now send it on its way
}


void YarpConnector::read(ServerResponse& res){
	
		//cout << "Waiting for answer...";

		Bottle *rawResult = NULL;
		int delay = 0;
		
		rawResult = in.read(false);
		
		while (rawResult == NULL && delay < ORO_MAX_DELAY){
			msleep(50);
			delay += 50;
			rawResult = in.read(false);
		}
		
		//The server doesn't answer quickly enough.
		if (rawResult == NULL){
			res.status = ServerResponse::failed;
			res.error_msg = "oro-server read timeout.";
			return;
		}		
		
		//cout << "got it! "  << endl;
			
		//cout << "Taille: " << rawResult->size() << endl;
		
		if (rawResult->size() < 2 || rawResult->size() > 3) throw OntologyServerException("Internal error! Wrong number of result element returned by the server.");
		
		if (!rawResult->get(0).isString()) throw OntologyServerException("Internal error! The server answer should start with a status string!");


		if (rawResult->get(0).asString() == ERROR){
			//throw OntologyServerException(rawResult->get(1).asString());
			res.status = ServerResponse::failed;
			res.exception_msg = rawResult->get(1).asString();
			res.error_msg = rawResult->get(2).asString();
			//cout << "ERROR Query to ontology server failed." << endl;
			return;
		}

		if (rawResult->get(0).asString() == OK){
			res.status = ServerResponse::ok;

			if(rawResult->get(1).isList()){
				Value tmp(rawResult->get(1));
				//cout << "Received bottle: " << tmp.asList()->toString() << endl;
				pourBottle(*(tmp.asList()), res.result);
			}
			else
			{
				throw OntologyServerException("INTERNAL ERROR! The second element of the server answer should be a list (bottle) of results!");
			}
		
			//copy(res.result.begin(), res.result.end(), ostream_iterator<string>(cout, "\n"));
			
			//cout << "Query to ontology server succeeded." << endl;
			return;
		}

		throw OntologyServerException("Internal error! The server answer should start with \"ok\" or \"error\"");
		
}

void YarpConnector::vectorToBottle(const vector<string>& data, Bottle& bottle)
{
	vector<string>::const_iterator itData = data.begin();

	for( ; itData != data.end() ; ++itData)
		bottle.addString((*itData).c_str());

}

void YarpConnector::setToBottle(const set<string>& data, Bottle& bottle)
{
	set<string>::const_iterator itData = data.begin();

	for( ; itData != data.end() ; ++itData)
		bottle.addString((*itData).c_str());

}

void YarpConnector::mapToBottle(const map<string, string>& data, Bottle& bottle)
{
	map<string, string>::const_iterator itData = data.begin();

	for( ; itData != data.end() ; ++itData) {
		Bottle& tmp = bottle.addList();
		tmp.addString((*itData).first.c_str());
		tmp.addString((*itData).second.c_str());
	}

}


void YarpConnector::pourBottle(const Bottle& bottle, server_return_types& result)
{
	//result.clear();
	//cout << bottle.toString() << endl;

	int size = bottle.size();
	
	//Only empty bottles or bottles with one value are handled
	if (size == 0) {
		//do nothing
	}
	else if (size == 1) {
		if (bottle.get(0).isString()) {
			if (bottle.get(0).asString() == "true")
				result = true;
			else if (bottle.get(0).asString() == "false")
				result = false;
			else
				result = bottle.get(0).asString().c_str();
		}
		else if (bottle.get(0).isInt())
			result = bottle.get(0).asInt();
		else if (bottle.get(0).isDouble())
			result = bottle.get(0).asDouble();
		else if (bottle.get(0).isList())
			result = makeCollec(*(bottle.get(0).asList()));
	}
	else {
		assert(false); //the server shouldn't answer more than one value...
	}
	
}

server_return_types YarpConnector::makeCollec(const Bottle& bottle) {

	bool isValidMap = true;
	bool isValidSet = true;
	
	//First, inspect the bottle to determine the type.
	for (int i = 0; i < bottle.size(); i++)
	{
		//cout << "Bottle[i]:" << bottle.get(i).toString().c_str() << endl;
		if (!isValidMap || !bottle.get(i).isList() || bottle.get(i).asList()->size() != 2 || !bottle.get(i).asList()->get(0).isString() || !bottle.get(i).asList()->get(1).isString())
			isValidMap = false;
			
		if (!isValidSet || !bottle.get(i).isString())
			isValidSet = false;
	}
	
	assert(!(isValidMap && isValidSet));
	
	if (!isValidMap && !isValidSet) throw OntologyServerException("INTERNAL ERROR! The server answered an invalid collection!");
	
	//TODO: avoid the unnecessary copy
	if (isValidMap) {
		map<string, string> result;
		for (int i = 0; i < bottle.size(); i++)
		{
			result[bottle.get(i).asList()->get(0).asString().c_str()] = bottle.get(i).asList()->get(1).asString().c_str();
		}
		
		return result;
	} else {
		assert(isValidSet);
		set<string> result;
	
		for (int i = 0; i < bottle.size(); i++)
			{
				result.insert(bottle.get(i).asString().c_str());
			}
		return result;
	}
}

void YarpConnector::pourBottle(const Bottle& bottle, vector<Concept>& result)
{
	result.clear();

	int size = bottle.size();
	
	for (int i = 0;i < size; i++)
	{
		Concept tmp = Concept(bottle.get(i).asString().c_str());
		result.push_back(tmp);
	}
}

int YarpConnector::msleep(unsigned long milisec)
{
    struct timespec req={0};
     time_t sec=(int)(milisec/1000);
     milisec=milisec-(sec*1000);
     req.tv_sec=sec;
     req.tv_nsec=milisec*1000000L;
     while(nanosleep(&req,&req)==-1)
          continue;
     return 1;
}

}
