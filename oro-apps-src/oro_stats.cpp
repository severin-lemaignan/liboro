#include <string>
#include <iostream>
#include <iterator>
#include <signal.h>
#include <map>

#include "oro.h"
#include "yarp_connector.h"

using namespace std;
using namespace yarp::os;

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
void displayVector(const vector<string>& result);
void displayTime(void);

const string local_port = "oro_stats";

map<string, clock_t, ltstr> timetable;

int main(int argc, char* argv[]) {

	string oro_port;
	string query;
	vector<string> result;
	Ontology* onto;

	//We catch ctrl+c to cleanly close the application
	signal( SIGINT,sigproc);
	
	cout << "********* ORO - Quality of Service *********" << endl;
	if (argc > 1 && (argv[2] == "-h")){
		cout << "Syntax:\n> oro-stats [yarp port of the oro server]" << endl << "By default, the oro-server yarp port is \"oro\"."<<endl;
		return(0);
	}
	cout << "Press ctrl+c to exit." << endl;
	
	
	if (argc > 1)
		oro_port = argv[1];
	else
		oro_port = "oro";
	
	//Instanciate the ontology with the YARP connector.
	YarpConnector connector(local_port, oro_port);
	onto = Ontology::createWithConnector(connector);

	cout << "* YARP Server found. Waiting for the ontology server to respond..." << endl;
	
	onto->stats(result);

	cout << "* oro-server found!" << endl;
	cout << "* Hostname: " << result[0] << endl;
	cout << "* Uptime: " << result[1] << endl;
	cout << "* Nb of classes in the ontology: " << result[2] << endl;
	cout << "* Nb of instances in the ontology: " << result[3] << endl;
	cout << "* Nb of currently connected clients: " << result[4] << endl;

}

void sigproc(int sig)
{
	signal(SIGINT, sigproc); /*  */
	 /* NOTE some versions of UNIX will reset signal to default
	 after each call. So for portability reset signal each time */

	exit(0);
}
