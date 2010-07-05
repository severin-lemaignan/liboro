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


#include <string>
#include <cstring>
#include <iostream>
#include <iterator>
#include <signal.h>
#include <map>

#include "oro.h"
#include "socket_connector.h"

using namespace std;


using namespace oro;

struct ltstr
{
  bool operator()(const string s1, const string s2) const
  {
    return s1.compare(s2) < 0;
  }
};

//Forward declarations
void sigproc(int);

string hostname;
string port;

map<string, clock_t, ltstr> timetable;

int main(int argc, char* argv[]) {

	string query;
	map<string, string> result;
	Ontology* onto;

	//We catch ctrl+c to cleanly close the application
	signal( SIGINT,sigproc);
	
	cout << "********* ORO - Statistics *********" << endl;
	//if (argc > 1 && strcmp(argv[2], "-h") == 0){
	if (argc != 3){
		cout << "Returns some statistics on a oro-server" << endl << endl;
		cout << "Syntax:\n> oro-stats hostname port" <<endl;
		return(0);
	}
	cout << "Press ctrl+c to exit." << endl;
	
	hostname = argv[1];
	port = argv[2];
	
	
	//Instanciate the ontology with the YARP connector.
	SocketConnector connector(hostname, port);
	onto = Ontology::createWithConnector(connector);
	
	result = onto->stats();
	
	map<string, string>::const_iterator itData = result.begin();
	for( ; itData != result.end() ; ++itData) {
		cout << "* " << (*itData).first << "->" << (*itData).second << endl;
	}
}

void sigproc(int sig)
{
	signal(SIGINT, sigproc); /*  */
	 /* NOTE some versions of UNIX will reset signal to default
	 after each call. So for portability reset signal each time */

	exit(0);
}
