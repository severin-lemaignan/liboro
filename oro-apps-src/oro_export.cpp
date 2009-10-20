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

string hostname;
string port;

int main(int argc, char* argv[]) {

	string path;
	vector<string> result;
	Ontology* onto;
	
	cout << "********* oro-export - Export the current ontology to a file *********" << endl;
	if (argc != 3 && argc != 4){
		cout << "Syntax:\n> oro-server host port [path]" << endl << "If no path is provided, the default one ('$(server_working_directory)/openrobots.owl') is used." << endl;
		return(0);
	}

	//We catch ctrl+c to cleanly close the application
	signal( SIGINT,sigproc);
	
	hostname = argv[1];
	port = argv[2];
	
	if (argc == 3){
		cout << "Default path used. If you wish, you can provide a path + file name."<<endl;
		path = "openrobots.owl";
	}
	else path = argv[3];

	
	//Instanciate the ontology with the socket connector.
	SocketConnector connector(hostname, port);
	onto = Ontology::createWithConnector(connector);

	cout << "Exporting the current ontology to " << path << "... ";

	try {
		onto->save(path);
	} catch (OntologyServerException &e)
	{
		cerr << endl << "Error! Check the filename you provided is valid and points to a writable location for the server." << endl;
		exit(1);
	}

	cout << "done." << endl;


}



void sigproc(int sig)
{
	signal(SIGINT, sigproc); /*  */
	 /* NOTE some versions of UNIX will reset signal to default
	 after each call. So for portability reset signal each time */

	exit(0);
}
