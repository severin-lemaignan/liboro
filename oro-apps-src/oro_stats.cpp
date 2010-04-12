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
