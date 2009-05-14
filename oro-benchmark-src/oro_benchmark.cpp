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
void displayVector(const vector<string>& result);
void displayTime(void);


const string oro_port = "oro";
const string local_port = "oro_benchmark";

map<string, clock_t, ltstr> timetable;
YarpConnector connector(local_port, oro_port);

int main(void) {

	char* name;
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

	//  TEST 1 //
	cout << " * <BENCH1> Assertion of some initial facts" << endl;

	onto->add("gorilla rdf:type Monkey");
	onto->add("gorilla age 12^^xsd:int");
	onto->add("gorilla weight 75.2");

	timetable["1- simple assertions"] = clock();
	
	//  TEST 2 //
	int nb_stmt = 100;
	cout << " * <BENCH2> Insertion of " << nb_stmt << " statements" << endl;

	for (int i = 0 ; i < nb_stmt ; i++)
	{
		Class testClass = Class("test");
		Concept test = Concept::create<Concept>(testClass);
	}

	timetable["2- insertion statements"] = clock();

	//  TEST 3 //
	cout << " * <BENCH3> Insertion of "<< nb_stmt << " statements with buffering" << endl;

	onto->bufferize();
	for (int i = 0 ; i < nb_stmt ; i++)
	{
		Class testClass = Class("test");
		Concept test = Concept::create<Concept>(testClass);
	}
	onto->flush();

	timetable["3- insertion buffered statements"] = clock();

	//  TEST 4 //
	vector<string> result;
	
	name = "<BENCH4> Simple getInfos query";
	
	cout << " * " << name << endl;

	onto->getInfos("gorilla", result);
	
	timetable[name] = clock();
	
	displayVector(result);
	
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

void displayVector(const vector<string>& result)
{

	cout << "Results:" << endl;

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
