
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
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>

#include <stdlib.h>
#include <time.h>

#include <queue>

#include <boost/thread.hpp>

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

    SocketConnector(const std::string& hostname, const std::string& port);

    virtual ~SocketConnector();

    /** Tries to reconnect on the same host and port used for construction.
     * If already connected, does nothing.
     * Throws oro::ConnectorException if the reconnection fails.
     */
    void reconnect();

    /** Tries to reconnect on a given host and port.
     * If already connected, does nothing.
     * Throws oro::ConnectorException if the reconnection fails.
     */
    void reconnect(const std::string& host, const std::string& port);

    bool isConnected();

    /* IConnector interface implementation */
    ServerResponse execute(const std::string& query,
                const std::vector<server_param_types>& args,
                bool waitForAck);
    ServerResponse execute(const std::string& query,
                const server_param_types& arg,
                bool waitForAck);
    ServerResponse execute(const std::string& query,
                bool waitForAck);

    void setEventCallback(
                void (*evtCallback)(const std::string& event_id,
                                    const server_return_types& raw_event_content)
                );

    static void serializeSet(const std::set<std::string>& data, std::string& msg);
    static void serializeVector(const std::vector<std::string>& data, std::string& msg);
    static void serializeMap(const std::map<std::string, std::string>& data, std::string& msg);

private:

    void oro_connect(const std::string& hostname, const std::string& port);

    static std::string protectValue(const std::string& value);
    static std::string& cleanValue(std::string& value);

    void deserialize(const std::string& msg, server_return_types& result);
    server_return_types makeCollec(const std::string& msg);
    void read(ServerResponse& response, bool only_events);
    int msleep(unsigned long milisec);

    bool _isConnected;

    // Socket related fields
    std::string host;
    std::string port;
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    fd_set sockets_to_read;

    // main() of the 'select' thread.
    void run();
    volatile bool _goOn;
    boost::thread _eventListnerThrd;

    boost::condition_variable gotResult;
    std::queue<ServerResponse> outbound_results;
    boost::mutex    outbound_lock;

    ssize_t readline(int fd, char *bufp, size_t maxlen);

    // The event callback
    void (*_evtCallback)(const std::string& event_id,
                        const server_return_types& raw_event_content);

    /* When not waiting for acknoledgements (waitForAck = false), we need to
     * track how many responses must be skipped to avoid returning a answer to
     * discard (for instance, a 'ok' after a 'add') as the response to keep
     * (for instance, a 'ok' after a 'checkConsistency').
     */
    int responseToSkip;

    bool waitingAnAnswer;
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
        args += "\"" + str + "\"" + MSG_SEPARATOR;
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

    void reset() {args = "";}


};

}

#endif /* SOCKET_CONNECTOR_H_ */
