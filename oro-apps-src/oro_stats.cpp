#include <string>
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

const string port = "6969";

map<string, clock_t, ltstr> timetable;

int main(int argc, char* argv[]) {

	string hostname;
	string query;
	map<string, string> result;
	Ontology* onto;

	//We catch ctrl+c to cleanly close the application
	signal( SIGINT,sigproc);
	
	cout << "********* ORO - Quality of Service *********" << endl;
	if (argc > 1 && (argv[2] == "-h")){
		cout << "Syntax:\n> oro-stats [hostname of the oro server]" << endl << "By default, \"localhost\" is used."<<endl;
		return(0);
	}
	cout << "Press ctrl+c to exit." << endl;
	
	
	if (argc > 1)
		hostname = argv[1];
	else
		hostname = "localhost";
	
	//Instanciate the ontology with the YARP connector.
	SocketConnector connector(hostname, port);
	onto = Ontology::createWithConnector(connector);
	
	onto->stats(result);
	
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
