/*
 * ©LAAS-CNRS (2008-2009)
 *
 * contributor(s) : Séverin Lemaignan <severin.lemaignan@laas.fr>
 *
 * This software is a computer program whose purpose is to interface
 * with an ontology in a robotics context.
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
#include <fstream>
#include <iterator>
#include <algorithm>
#include <time.h>

#include <cstring>

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>



#include "oro_exceptions.h"
#include "socket_connector.h"

using namespace std;
using namespace boost;

namespace oro {

	//Defines two constants needed to decode msg status.
	const char* OK = "ok";
	const char* ERROR = "error";


class ParametersSerializationHolder; //forward declaration
	
SocketConnector::SocketConnector(const string hostname, const string port){
	
	isConnected = false;
	
	struct sockaddr_in serv_addr;
    struct addrinfo *haddr, *a;
    int reuse;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sockfd < 0) {
		throw ConnectorException("Error while opening the socket! Exiting.");
	}
	
	/* set reuse addr option */
    reuse = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse))) {
      close(sockfd);
      throw ConnectorException("Cannot set reuseaddr on server socket");
    }
	
	/* resolve host address */
    if (getaddrinfo(hostname.c_str(), port.c_str(), NULL, &haddr)) {
      close(sockfd);
      throw ConnectorException("Cannot get remote host addresses");
    }
	
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    for(a = haddr; a; a = a->ai_next) switch (a->ai_family) {
      case AF_INET:
		serv_addr = *(struct sockaddr_in *)a->ai_addr;
		break;
      default: break;
    }
	
    if (serv_addr.sin_addr.s_addr == htonl(INADDR_ANY)) {
      close(sockfd);
      throw ConnectorException("Cannot resolve remote host address");
    }
	
    if (connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0) {
        throw ConnectorException("Error while connecting to \"" + hostname + "\". Wrong port ? Abandon.");
	}

	isConnected = true;
}

SocketConnector::~SocketConnector(){
	cout << "Closing socket connection...";
	
	if (isConnected) {
		executeDry("close");
		close(sockfd);
		isConnected = false;
	}
		
	cout << "done." << endl;
}

ServerResponse SocketConnector::execute(const string& query, const server_param_types& arg){
	
	ServerResponse res;
	
	ParametersSerializationHolder paramsHolder;
	
	//serialization of argument
	boost::apply_visitor(paramsHolder, arg);
	
	
	string completeQuery = query + MSG_SEPARATOR + paramsHolder.getArgs() + MSG_FINALIZER;
	
	//cout << "Query: " << completeQuery << endl;
	
	// Now send it on its way
	write(sockfd,completeQuery.c_str(),completeQuery.length());
	
	read(res);
	
	return res;
}

ServerResponse SocketConnector::execute(const string& query, const vector<server_param_types>& vect_args){
		
	ServerResponse res;
	
	ParametersSerializationHolder paramsHolder;
	
	//serialization of arguments
	std::for_each(
		vect_args.begin(), 
		vect_args.end(), 
		boost::apply_visitor(paramsHolder)
	);
	
	string completeQuery = query + MSG_SEPARATOR + paramsHolder.getArgs() + MSG_FINALIZER;
	
	// Now send it on its way
	write(sockfd,completeQuery.c_str(),completeQuery.length());
	
	read(res);
	
	return res;
}

ServerResponse SocketConnector::execute(const string& query){
	
	ServerResponse res;
	
	
	string completeQuery = query + MSG_SEPARATOR + MSG_FINALIZER;
	
	//cout << endl << "Send query: " << completeQuery << endl;
	
	// Now send it on its way
	write(sockfd,completeQuery.c_str(),completeQuery.length());

	
	read(res);
	
	return res;
}

void SocketConnector::executeDry(const string query){
	
	string completeQuery = query + MSG_SEPARATOR + MSG_FINALIZER;
	
	// Now send it on its way
	write(sockfd,completeQuery.c_str(),completeQuery.length());
	
}


void SocketConnector::read(ServerResponse& res){
	
		//cout << "Waiting for answer...";
		
		vector<string> rawResult;
		
		unsigned int MAX_LINE_LENGTH = 255;
		
		char *buffer;
		
		buffer = (char *) malloc (MAX_LINE_LENGTH + 1);

		FILE *socket_stream_in = fdopen(sockfd, "r");
		
		while (true) {
				
			
			int bytes_read = getline(&buffer, &MAX_LINE_LENGTH, socket_stream_in);
			
			if (bytes_read < 0){
				close(sockfd);
				throw OntologyServerException("Error reading from the server! Connection closed by the server?");
			 }
			
			string field = buffer;
			
			if (field == "#end#\n")
				break;
				
			field = field.substr(0, field.length() - 1); //remove the trailing "\n"				
			rawResult.push_back(field);
		}
/*
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
*/		
		//cout << "got it! "  << endl;
			
		//cout << "Taille: " << rawResult->size() << endl;
		
		if (rawResult.size() < 2 || rawResult.size() > 3) throw OntologyServerException("Internal error! Wrong number of result element returned by the server.");
		
		if (rawResult[0] == ERROR){
			//throw OntologyServerException(rawResult->get(1).asString());
			res.status = ServerResponse::failed;
			res.exception_msg = rawResult[1];
			res.error_msg = rawResult[2];
			//cout << "ERROR Query to ontology server failed." << endl;
			return;
		}

		if (rawResult[0] == OK){
			res.status = ServerResponse::ok;

			deserialize(rawResult[1], res.result);
			
			//cout << "Query to ontology server succeeded." << endl;
			return;
		}

		throw OntologyServerException("Internal error! The server answer should start with \"ok\" or \"error\"");
	
}

void SocketConnector::serializeVector(const vector<string>& data, string& msg)
{
	vector<string>::const_iterator itData = data.begin();

	msg += "[";
	
	for( ; itData != data.end() ; ++itData)
		msg += *itData + ",";
		
	msg = msg.substr(0, msg.length() - 1) + "]";

}

void SocketConnector::serializeSet(const set<string>& data, string& msg)
{
	set<string>::const_iterator itData = data.begin();

	msg += "[";
	
	for( ; itData != data.end() ; ++itData)
		msg += *itData + ",";
		
	msg = msg.substr(0, msg.length() - 1) + "]";

}

void SocketConnector::serializeMap(const map<string, string>& data, string& msg)
{
	map<string, string>::const_iterator itData = data.begin();

	msg += "{";
	
	for( ; itData != data.end() ; ++itData)
		msg += (*itData).first + ":" + (*itData).second + ",";
		
	msg = msg.substr(0, msg.length() - 1) + "}";
}

void SocketConnector::deserialize(const string& msg, server_return_types& result)
{

	if (msg == "true")
		result = true;
	else if (msg == "false")
		result = false;
	else if (
			 (msg[0] == '[' && msg[msg.length()-1] == ']') || 
			 (msg[0] == '{' && msg[msg.length()-1] == '}')
			)
		
		result = makeCollec(msg);
	else {
		try {
			result = lexical_cast<int>(msg);
			return;
		} catch (const bad_lexical_cast &e) {}
		
		try {
			result = lexical_cast<double>(msg);
			return;
		} catch (const bad_lexical_cast &e) {}
		
		result = msg;
	
	}
	
}

server_return_types SocketConnector::makeCollec(const string& msg) {

	bool isValidMap = true;
	bool isValidSet = true;
	char_separator<char> sep(",");
	
	//First, inspect the string to determine the type.
	//If it starts and ends with {}, it's a map
	//If it starts and ends with [], it's a set
	if ( !(msg[0] == '{' && msg[msg.length()-1] == '}'))
			isValidMap = false;
	
	if ( !(msg[0] == '[' && msg[msg.length()-1] == ']'))
			isValidSet = false;
	
	string collection = msg.substr(1, msg.length() - 2); //remove the [] or {}
		
	assert(!(isValidMap && isValidSet));
	
	if (!isValidMap && !isValidSet) throw OntologyServerException("INTERNAL ERROR! The server answered an invalid collection!");
	
	if (isValidMap) {
		map<string, string> result;
		
		tokenizer<char_separator<char> > tokens(collection, sep);
		BOOST_FOREACH(string t, tokens)
		{
		  size_t found = t.find(':');
		  if (found == string::npos) throw OntologyServerException("INTERNAL ERROR! The server answered an invalid map (missing semicolon)!");
		  result[t.substr(0, found)] = t.substr(found + 1, string::npos);
		}
		
		return result;
	}

	else{
		assert(isValidSet);
		set<string> result;
		
		tokenizer<char_separator<char> > tokens(collection, sep);
		BOOST_FOREACH(string t, tokens)
		{  
		  result.insert(t);
		}
		
		return result;
	}

}

int SocketConnector::msleep(unsigned long milisec)
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
