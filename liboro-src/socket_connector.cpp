
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

#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <time.h>

#include <cstring>

#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>
#include <boost/thread/locks.hpp>

#include "oro_exceptions.h"
#include "socket_connector.h"

using namespace std;
using namespace boost;

namespace oro {

	//Defines three constants needed to decode msg status.
	const char* OK = "ok";
	const char* ERROR = "error";
	const char* EVENT = "event";


class ParametersSerializationHolder; //forward declaration. Defined in socket_connector.h
	
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
		close(sockfd);
        throw ConnectorException("Error while connecting to \"" + hostname + "\". Wrong port ? Abandon.");
	}

	isConnected = true;
	
	_evtCallback = NULL;

        _goOn = true;

        thread _eventListnerThrd(bind(&SocketConnector::run, this));
}

SocketConnector::~SocketConnector(){
        cout << "Stopping the event listener...";
        _goOn = false;
        _eventListnerThrd.join();

        cout << "done.\nClosing socket connection...";
	
	if (isConnected) {
		executeDry("close");
		close(sockfd);
		isConnected = false;
	}
		
	cout << "done." << endl;
}

ServerResponse SocketConnector::execute(const string& query, const vector<server_param_types>& vect_args){
		
    {
        lock_guard<mutex> lock(inbound_lock);

        inbound_requests.push(query_type(query, vect_args));
    }

    while (true) {
        {
            lock_guard<mutex> lock(outbound_lock);

            if (!outbound_results.empty()) {
                ServerResponse res = outbound_results.front();
                outbound_results.pop();
                //cout << "[II] Poping a result for query " << query << endl;
                return res;
            }
        }

        msleep(50);
    }
}

ServerResponse SocketConnector::execute(const string& query, const server_param_types& arg){
        vector<server_param_types> p(1, arg);
        return execute(query, p);
}

ServerResponse SocketConnector::execute(const string& query){
        vector<server_param_types> p;
        return execute(query, p);
}

void SocketConnector::setEventCallback(
				void (*evtCallback)(const std::string& event_id, 
									const server_return_types& raw_event_content)
				) {
	_evtCallback = evtCallback;
}

void SocketConnector::executeDry(const string query){
	
	string completeQuery = query + MSG_SEPARATOR + MSG_FINALIZER;
	
	// Now send it on its way
	write(sockfd,completeQuery.c_str(),completeQuery.length());
	
}


void SocketConnector::read(ServerResponse& res, bool only_events){
	
		//cout << "Waiting for answer...";
		
		vector<string> rawResult;
		
		size_t MAX_LINE_LENGTH = 2047;
		
		char *buffer;
		
		buffer = (char *) malloc (MAX_LINE_LENGTH + 1);

		FILE *socket_stream_in = fdopen(sockfd, "r");
		
		while (true) {
			
			ssize_t bytes_read = getline(&buffer, &MAX_LINE_LENGTH, socket_stream_in);
			
			if (bytes_read < 0) {
				res.status = ServerResponse::failed;
				res.exception_msg = "ConnectorException";
				res.error_msg = "Error reading from the server! Connection closed by the server?";
				return;
			}
				
			if (bytes_read == 0) {
				res.status = ServerResponse::failed;
				res.exception_msg = "OntologyServerException";
				res.error_msg = "Error reading from the server! Empty string";
				return;
			}
			
			string field = buffer;

			//cout << "[II RAW] bytes read: " << bytes_read << endl;
			//cout << "[II RAW] " << field;
			if (field == MSG_FINALIZER)
				break;

			field = field.substr(0, field.length() - 1); //remove the trailing "\n"	
			rawResult.push_back(cleanValue(field));
		}
		
		free(buffer);
		
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
		
		if (rawResult.size() < 2 || rawResult.size() > 3) {
			res.status = ServerResponse::failed;
			res.exception_msg = "OntologyServerException";
			res.error_msg = "Internal server error! Wrong number of result element returned by the server.";
			return;
		}
		
		if (rawResult[0] == ERROR){
			
			if (only_events) { //we were waiting for events but got another message!!
				cout << "Got an ERROR message from the server that was unexpected!" << endl;
				read(res, true);
			}
			
			res.status = ServerResponse::failed;
			res.exception_msg = rawResult[1];
			res.error_msg = rawResult[2];
			return;
		}

		if (rawResult[0] == OK){
			
			if (only_events) { //we were waiting for events but got another message!!
				cout << "Got an OK message from the server that was unexpected!" << endl;
				read(res, true);
			}

			res.status = ServerResponse::ok;

			res.raw_result = rawResult[1];
			//cout << "[II] Raw result: " << rawResult[1] << endl;

			try {
				deserialize(rawResult[1], res.result);
			} catch (OntologyServerException ose) {
					res.status = ServerResponse::failed;
					res.exception_msg = "OntologyServerException";
					res.error_msg = ose.what();
					return;
				}

			//cout << "Query to ontology server succeeded." << endl;
			return;
		}

		if (rawResult[0] == EVENT){ //We got an event, instead of the expected answer!
			
			if (_evtCallback != NULL) {
			cout << "Got an event! " << rawResult[1] << " (content: " << rawResult[2] << ")" << endl;
				server_return_types raw_event_content;
				
				try {
					deserialize(rawResult[2], raw_event_content);
				} catch (OntologyServerException ose) {
					res.status = ServerResponse::failed;
					res.exception_msg = "OntologyServerException";
					res.error_msg = ose.what();
					return;
				}
				
				_evtCallback(rawResult[1], raw_event_content);
			}
			

			if (!only_events) {
			//Fetch the next message, hoping it's the right one.
			read(res, only_events);
			}
			
			return;
		}
		
		res.status = ServerResponse::failed;
		res.exception_msg = "OntologyServerException";
		res.error_msg = "Internal server error! The server answer should start with \"ok\", \"event\" or \"error\"";


}

void SocketConnector::run(){

	ServerResponse res;

	bool gotAQuery = false;
	query_type q;
	ParametersSerializationHolder paramsHolder;

	while (_goOn) {
		{
			lock_guard<mutex> lock(inbound_lock);

			if (!inbound_requests.empty()) {
				gotAQuery = true;

				q = inbound_requests.front();
				inbound_requests.pop();
			}
		}

		if (gotAQuery) {

			gotAQuery = false;

			string completeQuery = q.first + MSG_SEPARATOR ;

			if (!q.second.empty()) {
				//serialization of arguments
				std::for_each(
						q.second.begin(),
						q.second.end(),
						boost::apply_visitor(paramsHolder)
				);

				 completeQuery +=  paramsHolder.getArgs();
				 paramsHolder.reset();
			}

			completeQuery += MSG_FINALIZER;

			// Now send it on its way
			write(sockfd,completeQuery.c_str(),completeQuery.length());

			read(res, false); //will block until an answer is read.

			{
				lock_guard<mutex> lock(outbound_lock);

				outbound_results.push(res);
			}

		}
		else {

			//TODO: replace this 'timeout' approach by a blocking select 
			//that would be unblocked when a write is needed (signaled by a write
			//on a pipe, for instance)
			tv.tv_sec = 0;
			tv.tv_usec = 10000; //10ms

			FD_ZERO(&sockets_to_read);
			FD_SET(sockfd, &sockets_to_read);

			int retval = select(sockfd + 1, &sockets_to_read, NULL, NULL, &tv);

			if (retval != -1)
				read(res, true); //got something to read from the server!

		}
	}
}

/** Remove leading and trailing quotes and whitespace if needed. 
 * 
 * @param value
 * @return
 */
string& SocketConnector::cleanValue(string& value) {
	
	//First, trim the string
    size_t startpos = value.find_first_not_of(" \t");
    size_t endpos = value.find_last_not_of(" \t");

    if(( string::npos == startpos ) || ( string::npos == endpos))
    {
        value = "";
    }
    else
        value = value.substr( startpos, endpos-startpos+1 );

	//Then remove quotes
	if ((value[0] == '"' && value[value.length()-1] == '"') || (value[0] == '\'' && value[value.length()-1] == '\''))
		value = value.substr(1, value.length() - 2);
	
	return value;
}

/** Protect a string by escaping the quotes and surrounding the string with quotes.
 * 
 * @param value
 * @return
 */
string SocketConnector::protectValue(const string& value) {

	//Escape double quotes
	size_t start_pos = 0;
	
	string res(value);
	
	while(true) {
		size_t pos = res.find("\"", start_pos);
		if( string::npos != pos ) {
			start_pos = pos + 2;
			res.replace(pos, 1, "\\\"");
		}
		else break;
	}	
	
	//Quote the whole string
	res = "\"" + res + "\"";
	return res;
}
		
void SocketConnector::serializeVector(const vector<string>& data, string& msg)
{
	if (data.size() == 0) {msg += "[]"; return;}
	
	vector<string>::const_iterator itData = data.begin();

	msg += "[";
	
	for( ; itData != data.end() ; ++itData)
		msg += protectValue(*itData) + ",";
	
	msg = msg.substr(0, msg.length() - 1) + "]";

}

void SocketConnector::serializeSet(const set<string>& data, string& msg)
{
	if (data.size() == 0) {msg += "[]"; return;}
		
	set<string>::const_iterator itData = data.begin();
	
	msg += "[";
	
	for( ; itData != data.end() ; ++itData)
		msg += protectValue(*itData) + ",";

	msg = msg.substr(0, msg.length() - 1) + "]";

}

void SocketConnector::serializeMap(const map<string, string>& data, string& msg)
{
	if (data.size() == 0) {msg += "{}"; return;}
	
	map<string, string>::const_iterator itData = data.begin();

	msg += "{";
	
	for( ; itData != data.end() ; ++itData)
		msg += protectValue((*itData).first) + ":" + protectValue((*itData).second) + ",";
		
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
        {
		result = makeCollec(msg);
                //cout << "[II] Collec!" << endl;
        }
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
                //cout << "[II] Str!" << msg << endl;
	
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
	
	if (!isValidMap && !isValidSet) throw OntologyServerException("INTERNAL ERROR! The server answered an invalid collection!");

	string collection = msg.substr(1, msg.length() - 2); //remove the [] or {}
		
	if (isValidMap) {
		map<string, string> result;
		
		tokenizer<char_separator<char> > tokens(collection, sep);
		BOOST_FOREACH(string t, tokens)
		{
		  size_t found = t.find(':');
		  if (found == string::npos) throw OntologyServerException("INTERNAL ERROR! The server answered an invalid map (missing semicolon)!");
		  
		  string key = t.substr(0, found);
		  string value = t.substr(found + 1, string::npos);
		  result[cleanValue(key)] = cleanValue(value);
		}
		
		return result;
	}

	else{
		assert(isValidSet);
		set<string> result;
		
		tokenizer<char_separator<char> > tokens(collection, sep);
		BOOST_FOREACH(string t, tokens)
		{  
		  result.insert(cleanValue(t));
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
