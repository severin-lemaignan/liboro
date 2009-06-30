// To be benchmarked, the ontology server should be started with "oro_bench.conf" configuration file.

#include <string>
#include <iostream>
#include <iterator>
#include <signal.h>
#include <map>
#include <time.h>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>


#ifdef assert
#undef assert
#endif

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
class eventHandler;
void sigproc(int);
void displayVector(const vector<string>& result);
void displayTime(void);

//boost::condition cond;
//boost::mutex mut;

bool waitingForEvent = true;
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

	onto->add(Statement("gorilla rdf:type Monkey"));
	onto->add(Statement("gorilla age 12^^xsd:int"));
	onto->add(Statement("gorilla weight 75.2"));

	timetable["<BENCH1> simple assertions"] = clock();
	
	/*
	//  TEST 2 //
	int nb_stmt = 100;
	cout << " * <BENCH2> Insertion of " << nb_stmt << " statements" << endl;

	for (int i = 0 ; i < nb_stmt ; i++)
	{
		Class testClass = Class("test");
		Concept test = Concept::create<Concept>(testClass);
	}

	timetable["<BENCH2> insertion statements"] = clock();
	*/
	
	/*
	//  TEST 3 //
	
	cout << " * <BENCH3> Insertion of "<< nb_stmt << " statements with buffering" << endl;

	onto->bufferize();
	for (int i = 0 ; i < nb_stmt ; i++)
	{
		Class testClass = Class("test");
		Concept test = Concept::create<Concept>(testClass);
	}
	onto->flush();

	timetable["<BENCH3> insertion buffered statements"] = clock();
	*/
	
	//  TEST 4 //
	vector<string> result;
	
	name = "<BENCH4> Simple getInfos query";
	
	cout << " * " << name << endl;

	cout << "\t* Looking for infos on Gorillas:" << endl;
			
	onto->getInfos("gorilla", result);
	
	displayVector(result);
	
	timetable["<BENCH4> Simple getInfos query (existing resource)"] = clock();
	
	cout << "\t* Looking for infos on Schtroumphs" << endl;
	
	try {
		onto->getInfos("schtroumph", result);
	} catch (ResourceNotFoundOntologyException e)
	{
		cout << "\t Good: nothing found, as expected."<<endl;
	}
	
	timetable["<BENCH4> Simple getInfos query (inexistant resource)"] = clock();
	
	
	//  TEST 5 //
	string event_port = local_port + "_events";
	
	name = "<BENCH5> Event registration";
	
	cout << " * " << name << endl;
	
	boost::thread eventHandleThread(eventHandler(event_port));

	onto->subscribe("?object rdf:type Donkey", Ontology::ON_TRUE_ONE_SHOT, event_port);

	Concept dudule = Concept::create(Class("Donkey"));

	timetable[name] = clock();
	
	/*boost::mutex::scoped_lock lock(mut);
	while(waitingForEvent)
	{
		cond.wait(lock);
	}*/

	//  TEST 6 //
	
	name = "<BENCH6> SPARQL test";
	
	cout << " * " << name << endl;
	result.clear();
	
	string query = "SELECT ?object WHERE { ?object rdf:type oro:Monkey }";
	
	onto->query("object", query, result);

	displayVector(result);

	timetable[name] = clock();
	
	//  TEST 7 //
	
	name = "<BENCH7> Find test";
	
	cout << " * " << name << endl;
	
	vector<Concept> resultConcepts;
	
	onto->find("object", "?object rdf:type Monkey", resultConcepts);

	copy(resultConcepts.begin(), resultConcepts.end(), ostream_iterator<Concept>(cout, "\n"));
	

	timetable[name] = clock();
	
	
	//  TEST 8 //
	
	name = "<BENCH8> Filtred find test";
	
	cout << " * " << name << endl;
	
	resultConcepts.clear();
	
	vector<string> partial_stmts;
	vector<string> filters;
	
	partial_stmts.push_back("?mysterious rdf:type oro:Monkey");
	partial_stmts.push_back("?mysterious oro:weight ?value");
 
	filters.push_back("?value >= 50");
 
	onto->find("mysterious", partial_stmts, filters, resultConcepts);


	copy(resultConcepts.begin(), resultConcepts.end(), ostream_iterator<Concept>(cout, "\n"));

	timetable[name] = clock();
	
	
	///////////////////////////
	///////////////////////////

	displayTime();

}

class eventHandler
{
  string portName;
public:
  eventHandler(string portName) : portName(portName) { }

  void operator()()
  {
    BufferedPort<Bottle> eventPort;
    eventPort.open(("/" + portName).c_str());

    Bottle *incomingEventBottle = eventPort.read();

    if (incomingEventBottle != NULL) {
	cout << "BREAKING NEWS: A donkey appeared in the ontology!" << endl;
    } else {
	cout << "bouhouh...no events at all!" << endl;
    }

    {
	   /* boost::mutex::scoped_lock lock(mut);
	    waitingForEvent = false;
	    cond.notify_all();
	    */
    }

  }

};




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
