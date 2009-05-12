// To be benchmarked, the ontology server should be started with "oro_bench.conf" configuration file.

#include <string>
#include <iostream>
#include <iterator>
#include <signal.h>
#include <map>
#include <time.h>

#include "oro.h"
#include "oro_library.h"
#include "oro_connector.h"
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
void displayTime(void);


const string oro_port = "oro";
const string local_port = "oro_benchmark";

map<string, clock_t, ltstr> timetable;
YarpConnector connector(local_port, oro_port);

int main(void) {

	Ontology* onto;

	cout << "********* ORO Benchmark *********" << endl;
	cout << "Press ctrl+c to exit." << endl;

	//We catch ctrl+c to cleanly close the application
	signal( SIGINT,sigproc);
	
	//Instanciate the ontology with the YARP connector.
	onto = Ontology::createWithConnector(connector);

	Concept myself = Concept("myself"); //create a new instance in the ontology which represents "myself" ie the robot/process where this code executes.
	
	cout << " * Connected to ontology server at /" << oro_port << "/in" << endl;
	cout << " * Now starting benchmarking." << endl;


	/////////////////////////////
	//  START OF BENCHMARKING  //
	/////////////////////////////

	timetable["0- start"] = clock();

	int nb_stmt = 100;

	//  TEST 1 //
	cout << " * <BENCH1> Insertion of " << nb_stmt << " statements" << endl;

	for (int i = 0 ; i < nb_stmt ; i++)
	{
		Class testClass = Class("test");
		Concept test = Concept::create<Concept>(testClass);
	}

	timetable["1- insertion statements"] = clock();

	//  TEST 2 //
	cout << " * <BENCH2> Insertion of "<< nb_stmt << " statements with buffering" << endl;

	onto->bufferize();
	for (int i = 0 ; i < nb_stmt ; i++)
	{
		Class testClass = Class("test");
		Concept test = Concept::create<Concept>(testClass);
	}
	onto->flush();

	timetable["2- insertion buffered statements"] = clock();

	///////////////////////////
	///////////////////////////

	displayTime();

}

void displayTime()
{
	double tick_ms = ((double) CLOCKS_PER_SEC) / 1000;

	cout << endl << "*************** Timetable *****************" << endl;
	map<string, clock_t>::iterator bench = timetable.begin();
	clock_t tmp = (*bench).second;
	++bench;

	for( ; bench != timetable.end() ; ++bench) {
		cout << (*bench).first << " -> " << ((*bench).second - tmp) / tick_ms << "ms" << endl;
		tmp = (*bench).second;
	}

	cout << "*******************************************" << endl;

}

void sigproc(int sig)
{
	signal(SIGINT, sigproc); /*  */
	 /* NOTE some versions of UNIX will reset signal to default
	 after each call. So for portability reset signal each time */

	displayTime();

	exit(0);
}
