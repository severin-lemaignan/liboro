#include <string>
#include <iostream>
#include <signal.h>

#ifdef assert
#undef assert
#endif

#include "oro.h"
#include "socket_connector.h"

using namespace std;


using namespace oro;

//Forward declarations
void sigproc(int);

const string hostname = "localhost";
const string port = "6969";

int main(int argc, char* argv[]) {

	string path;
	vector<string> result;
	Ontology* onto;

	//We catch ctrl+c to cleanly close the application
	signal( SIGINT,sigproc);
	
	if (argc != 2){
		cout << "Default path used. If you wish, you can provide a path + file name."<<endl;
		path = "openrobots.owl";
	}
	else path = argv[1];

	
	//Instanciate the ontology with the YARP connector.
	SocketConnector connector(hostname, port);
	onto = Ontology::createWithConnector(connector);

	cout << "Exporting the current ontology to " << path << "... ";

	
	onto->save(path);

	cout << "done." << endl;


}



void sigproc(int sig)
{
	signal(SIGINT, sigproc); /*  */
	 /* NOTE some versions of UNIX will reset signal to default
	 after each call. So for portability reset signal each time */

	exit(0);
}
