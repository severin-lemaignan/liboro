#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <signal.h>
#include <map>
#include <time.h>

#ifdef assert
#undef assert
#endif

#include "oro.h"
#include "yarp_connector.h"

using namespace std;
using namespace yarp::os;

using namespace oro;
using namespace boost;

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

const string oro_port = "oro";
const string local_port = "oro_query";

map<string, clock_t, ltstr> timetable;

int main(int argc, char* argv[]) {

	string query;
	vector<string> result;
	Ontology* onto;

	//We catch ctrl+c to cleanly close the application
	signal( SIGINT,sigproc);
	
	cout << "********* ORO - SPARQL query *********" << endl;
	if (argc != 3){
		cout << "Syntax:\n> oro-query variable_to_select my_query.sparql" << endl << "Please provide a file containing exactly one valid SPARQL query as parameter and the name of the variable you want to select."<<endl;
		return(0);
	}
	cout << "Press ctrl+c to exit." << endl;
	
	string variable(argv[1]);
	string sparql_file(argv[2]);
	
	//Instanciate the ontology with the YARP connector.
	YarpConnector connector(local_port, oro_port);
	
	try {
		onto = Ontology::createWithConnector(connector);
	} catch (OntologyServerException ose) {
		cout << "Server error: " << ose.what() << endl;
		return 0;
	}

	cout << "Now opening " + sparql_file + " (variable: "+ variable + ") and executing the query on " + oro_port << endl;

	timetable["0- start"] = clock();
			
	string line;
	ifstream inFile( sparql_file.c_str() );
	if( !inFile ) {
		cerr << "Couldn´t open input file" << endl;
		return false;
	}

	while(getline(inFile,line))
		query += line + "\n";
	
	
	cout << endl << "**********************\n" << "*       Query        *\n" << "**********************" << endl;
	
	cout << query << endl;
	
	try {
		onto->query(variable, query, result);
	} catch (InvalidQueryException iqe) {
		cout << "Invalid query: " << iqe.what() << endl;
		return 0;
	} catch (OntologyServerException ose) {
		cout << "Server error: " << ose.what() << endl;
		return 0;
	}
	

	cout << endl << "**********************\n" << "*       Result       *\n" << "**********************" << endl;
	displayVector(result);

	timetable["Execution duration"] = clock();
	
	
	///////////////////////////
	///////////////////////////

	displayTime();
	
	return 0;

}


void displayTime()
{
	double tick_ms = ((double) CLOCKS_PER_SEC) / 1000;

	cout << endl << "********************************" << endl;
	map<string, clock_t>::iterator bench = timetable.begin();
	clock_t tmp = (*bench).second;
	++bench;

	for( ; bench != timetable.end() ; ++bench) {
		cout << (*bench).first << " -> " << ((*bench).second - tmp) / tick_ms << "ms" << endl;
		tmp = (*bench).second;
	}

	cout << "********************************" << endl;
}

void displayVector(const vector<string>& result)
{
	copy(result.begin(), result.end(), ostream_iterator<string>(cout, "\n")); //ce n'est pas moi qui ait écrit ça

}

void sigproc(int sig)
{
	signal(SIGINT, sigproc); /*  */
	 /* NOTE some versions of UNIX will reset signal to default
	 after each call. So for portability reset signal each time */

	displayTime();

	exit(0);
}
