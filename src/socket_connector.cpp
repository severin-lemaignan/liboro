
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

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/ioctl.h>

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

#ifdef DEBUG

char logtime[80];
/** Compute and format a millisecond-accurate timestamp **/
void timestamp(char* logtime)
{
    struct timeval  tv;
    struct timezone tz;
    struct tm      *tm;

    gettimeofday(&tv, &tz);
    tm = localtime(&tv.tv_sec);

    sprintf(logtime, "%04d%02d%02d %d:%02d:%02d.%03d", 
            tm->tm_year + 1900,
            tm->tm_mon,
            tm->tm_mday,
            tm->tm_hour,
            tm->tm_min, 
            tm->tm_sec, 
            ((int)tv.tv_usec)/1000);
}

#define TRACE(arg) {timestamp(logtime); std::cerr << "[LIBORO DEBUG] " << logtime << ": " << arg << std::endl;}
#else
#define TRACE(arg) sizeof(std::cerr << arg << std::endl)
#endif

using namespace std;
using namespace boost;

namespace oro {

//Defines three constants needed to decode msg status.
const char* OK = "ok";
const char* ERROR = "error";
const char* EVENT = "event";


class ParametersSerializationHolder; //forward declaration. Defined in socket_connector.h

SocketConnector::SocketConnector(const string& hostname, const string& port) :
    host(hostname),
    port(port) {

    _isConnected = false;
    responseToSkip = 0;

    waitingAnAnswer = false;

    oro_connect(hostname, port);

    _evtCallback = NULL;

    _goOn = true;

    thread _eventListnerThrd(bind(&SocketConnector::run, this));
}

SocketConnector::~SocketConnector(){
    cerr << "Waiting for all pending request to finish...";
    if (_isConnected) {
        execute("stats", true); //permit to wait for all previous call to be completed
    }

    cerr << "done.\nStopping the event listener...";
     _goOn = false;
    _eventListnerThrd.join();

    cerr << "done.\nClosing socket connection...";

    if (_isConnected) {
        execute("close", false); //don't wait for ack, it won't come!
        close(sockfd);
        _isConnected = false;
    }

    cerr << "done." << endl;
}

bool SocketConnector::isConnected() {return _isConnected;}

void SocketConnector::oro_connect(const string& hostname, const string& port){

    if(_isConnected) return;

    struct sockaddr_in serv_addr;
    struct addrinfo *haddr, *a;
    int reuse;

    int err;

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
    if ((err = getaddrinfo(hostname.c_str(), port.c_str(), NULL, &haddr)) != 0) {
        close(sockfd);
        cerr << "Error: " << gai_strerror(err) << endl;
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

    _isConnected = true;

}

void SocketConnector::reconnect() {
    oro_connect(host, port);
}

void SocketConnector::reconnect(const string& host, const string& port) {
    oro_connect(host, port);
}

ServerResponse SocketConnector::execute(const string& query,
                                        const vector<server_param_types>& vect_args,
                                        bool waitForAck){

        if (!_isConnected) {
            throw ConnectorException("Not connected to oro-server!");
        }

        ParametersSerializationHolder paramsHolder;

        if (!waitForAck) responseToSkip++;

        string completeQuery = query + MSG_SEPARATOR ;

        if (!vect_args.empty()) {
            //serialization of arguments
            std::for_each(
                        vect_args.begin(),
                        vect_args.end(),
                        boost::apply_visitor(paramsHolder)
                        );

            completeQuery +=  paramsHolder.getArgs();
            paramsHolder.reset();
        }

        TRACE("Sending " << completeQuery << " to oro-server");

        completeQuery += MSG_FINALIZER;

        // Now send it on its way
        write(sockfd,completeQuery.c_str(),completeQuery.length());

        waitingAnAnswer = true;



        //inbound_requests.push(query_type(query, vect_args));
    //}

    ServerResponse res;

    {
        unique_lock<mutex> lock(outbound_lock);

        if(waitForAck) {
            while (outbound_results.empty()) {
                gotResult.wait(lock);

            }

            res = outbound_results.front();
            outbound_results.pop();

            TRACE("Popping a result for query " << query);

            if (res.status == ServerResponse::failed && res.exception_msg == CONNECTOR_EXCEPTION)
            {
                throw ConnectorException(res.error_msg);
            }
        }
        else
        {
            // we don't wait for acknowledgement!
            res.status = ServerResponse::ok;
        }
    }

    return res;

}

ServerResponse SocketConnector::execute(const string& query,
                                        const server_param_types& arg,
                                        bool waitForAck){
    vector<server_param_types> p(1, arg);
    return execute(query, p, waitForAck);
}

ServerResponse SocketConnector::execute(const string& query,
                                        bool waitForAck){
    vector<server_param_types> p;
    return execute(query, p, waitForAck);
}

void SocketConnector::setEventCallback(
    void (*evtCallback)(const std::string& event_id,
                        const server_return_types& raw_event_content)
    ) {
    _evtCallback = evtCallback;
}


// based on libtclserv_client socket implementation
ssize_t SocketConnector::readline(int fd, char *bufp, size_t maxlen)
{
    int nb_to_read = 0;
    char* p;
    size_t size_request;
    ssize_t err;

    // get the len of the buffer
    if (ioctl(fd, FIONREAD, &nb_to_read) == -1)
    {
        cerr << "failed to call ioctl FIONREAD" << endl;
        return -1;
    }

    if (nb_to_read >= maxlen) {
        cerr << "Buffer too small!" << endl;
        return -1;
    }

    err = recv(fd, bufp, nb_to_read, MSG_PEEK);
    if (err == -1) {
        cerr << "Failed to recv" << endl;
        return -1;
    }

    if (err == 0) {
        close(fd);
        fd = -1;
        cerr << "Peer deconnection" << endl;
        return -1;
    }

    // look if we can find a rqst
    p = strstr(bufp, "\n");
    if (p == NULL) { assert(0); } // XXX WTF here
    size_request = p - bufp + 1;

    err = recv(fd, bufp, size_request, 0);
    bufp[err] = 0;
    return err;

}

void SocketConnector::read(ServerResponse& res, bool only_events){

    vector<string> rawResult;

    size_t MAX_LINE_LENGTH = 2048;


    char buffer[MAX_LINE_LENGTH];

    while (true) {

        ssize_t bytes_read = readline(sockfd, buffer, MAX_LINE_LENGTH);

        if (bytes_read < 0) {
            res.status = ServerResponse::failed;
            res.exception_msg = CONNECTOR_EXCEPTION;
            res.error_msg = "Error reading from the server! Connection closed by the server?";
            _isConnected = false;
            close(sockfd);
            return;
        }

        if (bytes_read == 0) {
            res.status = ServerResponse::failed;
            res.exception_msg = CONNECTOR_EXCEPTION;
            res.error_msg = "Error reading from the server! Empty string";
            return;
        }

        string field(buffer);

        if (field == MSG_FINALIZER)
            break;

        field = field.substr(0, field.length() - 1); //remove the trailing "\n"
        rawResult.push_back(cleanValue(field));
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

    if (rawResult.size() < 1 || rawResult.size() > 3) {
        res.status = ServerResponse::failed;
        res.exception_msg = "OntologyServerException";
        res.error_msg = "Internal server error! Wrong number of result element returned by the server.";
        return;
    }

    if (rawResult[0] == EVENT){

        if (_evtCallback != NULL) {
            TRACE("Got an event! " << rawResult[1] << " (content: " << rawResult[2] << ")");
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
            //We were waiting for a response, and we got an event interleaved. Fine.
            //Fetch the next message, hoping it's the right one.
            // -> it means we never go back to run() until we read an answer to our inital request.
            read(res, only_events);
        }

        return;
    }

    //  here => rawResult[0] == ERROR | OK

    //we were waiting for events but got another message!!
    //Maybe we are in waitForAck = false
    if (only_events) {
        if (responseToSkip == 0) {
            cerr << "Got an OK or ERROR message from the server that was unexpected!" << endl;
            cerr << "Content was: " << rawResult[1] << endl;
            cerr << "Discarding it." << endl;
        }
        else {
            responseToSkip--;
        }

        res.status = ServerResponse::discarded;
        return;
    }

    // We are still waiting for response of previous request whose response were
    // skipped (waitForAck = false)
    if (responseToSkip != 0) {
        responseToSkip--;
        res.status = ServerResponse::discarded;
        return;
    }

    if (rawResult[0] == ERROR){
        res.status = ServerResponse::failed;
        res.exception_msg = rawResult[1];
        res.error_msg = rawResult[2];
        return;
    }

    if (rawResult[0] == OK){

        res.status = ServerResponse::ok;
        res.raw_result = rawResult[1];

        if (rawResult.size() == 1) {
            res.result = true;
            return;
        }

        try {
            deserialize(rawResult[1], res.result);
        } catch (OntologyServerException ose) {
            res.status = ServerResponse::failed;
            res.exception_msg = "OntologyServerException";
            res.error_msg = ose.what();
            return;
        }

        return;
    }

    // here => received malformed content from the server!
    res.status = ServerResponse::failed;
    res.exception_msg = "OntologyServerException";
    res.error_msg = "Internal server error! The server answer should start with \"ok\", \"event\" or \"error\"";


}

void SocketConnector::run(){

    ServerResponse res;

    if (!_isConnected) {
        cerr << "Can not start liboro socket connector thread if not connected." << endl;
    }

    while (_goOn) {

        if (!_isConnected) {
            msleep(50);
            continue;
        }

        FD_ZERO(&sockets_to_read);
        FD_SET(sockfd, &sockets_to_read);

        int retval = select(sockfd + 1, &sockets_to_read, NULL, NULL, NULL);

	if (retval == -1) {
	    TRACE("During 'select': " << strerror(errno) << ". Continuing.");
	    // The error is likely EINTR (signal caught). We can safely continue.
	    continue;
	}
	else if (retval) { //got something to read from the server!

	    if (waitingAnAnswer) {

		read(res, false); //will block until an answer is read.

		if (res.status != ServerResponse::discarded)
		{
		    lock_guard<mutex> lock(outbound_lock);

		    outbound_results.push(res);
		    gotResult.notify_all();

		    waitingAnAnswer = false;
		}

	    }
	    else {
		read(res, true); //Read AND process events

		//If an error occurs while dealing with events (like server
		//deconnection), export it in the server responses queue.
		//At the next request, the client while be informed of the
		//disconnection.
		if(res.status == ServerResponse::failed)
		{
		    lock_guard<mutex> lock(outbound_lock);

		    outbound_results.push(res);
		    gotResult.notify_all();
		}
	    }
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
    }

}

server_return_types SocketConnector::makeCollec(const string& msg) {

    bool isValidMap = true;
    bool isValidSet = true;
    escaped_list_separator<char> sep;

    //First, inspect the string to determine the type.
    //If it starts and ends with {}, it's a map
    //If it starts and ends with [], it's a set
    if ( !(msg[0] == '{' && msg[msg.length()-1] == '}'))
        isValidMap = false;

    if ( !(msg[0] == '[' && msg[msg.length()-1] == ']'))
        isValidSet = false;

    if (!isValidMap && !isValidSet) throw OntologyServerException("INTERNAL ERROR! The server answered an invalid collection!");

    string collection = msg.substr(1, msg.length() - 2); //remove the [] or {}

    // Special case: list of pairs (eg '[[a,b], [c,d]]')are treated
    // like maps (eg '{a:b, c:d}')
    if ( (collection[0] == '[' && collection[collection.length()-1] == ']')) {
        map<string, string> result;

        tokenizer<escaped_list_separator<char> > tokens(collection, sep);

        bool key = true;
        string last_key = "";

        BOOST_FOREACH(string t, tokens)
        {
            string c = cleanValue(t);
            // Check if we look like a list of pair: each token starts with '[' or ends with ']'
            if (key) {
                 if (!(c[0] == '[')) throw OntologyServerException("INTERNAL ERROR! The server answered an invalid collection!");

                 c = c.substr(1, c.length() - 1);
                 c = cleanValue(c);
            }
            else { // !key
                if (!(c[c.length()-1] == ']')) throw OntologyServerException("INTERNAL ERROR! The server answered an invalid collection!");

                c = c.substr(0, c.length() - 1);
                c = cleanValue(c);
            }

            if (key) {
                result[c] = "";
                last_key = c;
                key = false;
            }
            else {
                result[last_key] = c;
                key = true;
            }

        }

        return result;

    }

    if (isValidMap) {
        map<string, string> result;

        tokenizer<escaped_list_separator<char> > tokens(collection, sep);
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

        tokenizer<escaped_list_separator<char> > tokens(collection, sep);
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
