
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

// To be benchmarked, the ontology server should be started with
// "oro_bench.conf" configuration file.

#include <string>
#include <iostream>
#include <iterator>
#include <signal.h>
#include <map>
#include <set>
#include <vector>
#include <ctime>

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include "oro.h"
#include "oro_library.h"
#include "oro_connector.h"
#include "socket_connector.h"

using namespace std;


using namespace oro;
using namespace boost;

struct ltstr
{
  bool operator()(const string& s1, const string& s2) const
  {
    return s1.compare(s2) < 0;
  }
};

//Forward declarations
class eventHandler;
void sigproc(int);
void displayCollec(const set<string>& result);
void displayTime(void);

//boost::condition cond;
//boost::mutex mut;

bool waitingForEvent = true;
const string hostname = "localhost";
const string port = "6969";

//map<string, gettimeofday_t, ltstr> timetable;
map<string, timeval, ltstr> timetable;
SocketConnector connector(hostname, port);

int main(void) {

        timeval time;
    string name;

    cout << "********* ORO Benchmark *********" << endl;
    cout << "Press ctrl+c to exit." << endl;

    //We catch ctrl+c to cleanly close the application
    signal( SIGINT,sigproc);


    Ontology* onto;
    //Instanciate the ontology with the YARP connector.
    onto = Ontology::createWithConnector(connector);

    Concept myself = Concept("myself"); //create a new instance in the ontology which represents "myself" ie the robot/process where this code executes.

    cout << " * Connected to ontology server on " << hostname << ":" << port << endl;
    cout << " * Now starting benchmarking." << endl;


    /////////////////////////////
    //  START OF BENCHMARKING  //
    /////////////////////////////


        gettimeofday(&time, NULL);
        timetable["0- start"] = time;

    //  TEST 1 //
    cout << " * <BENCH01> Assertion of some initial facts" << endl;

    onto->add(Statement("gorilla rdf:type Monkey"));
    onto->add(Statement("gorilla age 12^^xsd:int"));
    onto->add(Statement("gorilla weight 75.2"));

        gettimeofday(&time, NULL);
        timetable["<BENCH01> simple assertions"] = time;


    //  TEST 2 //
    int nb_stmt = 100;

    cout << " * <BENCH02> Insertion of " << nb_stmt << " statements" << endl;

    for (int i = 0 ; i < nb_stmt ; i++)
    {
        Class testClass = Class("test");
        Concept test = Concept::create(testClass);
    }

        gettimeofday(&time, NULL);
        timetable["<BENCH02> insertion statements"] = time;



    //  TEST 3 //

    cout << " * <BENCH03> Insertion of "<< nb_stmt << " statements with buffering" << endl;

    onto->bufferize();
    for (int i = 0 ; i < nb_stmt ; i++)
    {
        Class testClass = Class("test");
        Concept test = Concept::create(testClass);
    }
    onto->flush();

        gettimeofday(&time, NULL);
        timetable["<BENCH03> insertion buffered statements"] = time;


    //  TEST 4 //
    set<string> result;

    name = "<BENCH04> Simple getInfos query";

    cout << " * " << name << endl;

    cout << "\t* Looking for infos on Gorillas:" << endl;

    onto->getInfos("gorilla", result);

    displayCollec(result);

        gettimeofday(&time, NULL);
        timetable["<BENCH04> Simple getInfos query (existing resource)"] = time;

    cout << "\t* Looking for infos on Schtroumphs" << endl;

    try {
        onto->getInfos("schtroumph", result);
    } catch (ResourceNotFoundOntologyException e)
    {
        cout << "\t Good: nothing found, as expected."<<endl;
    }

        gettimeofday(&time, NULL);
        timetable["<BENCH04> Simple getInfos query (inexistant resource)"] = time;

    /*
    //  TEST 5 //
    string event_port = local_port + "_events";

    name = "<BENCH5> Event registration";

    cout << " * " << name << endl;

    boost::thread eventHandleThread(eventHandler(event_port));

    onto->subscribe("?object rdf:type Donkey", Ontology::ON_TRUE_ONE_SHOT, event_port);

    Concept dudule = Concept::create(Class("Donkey"));

        timetable[name] = gettimeofday();

    //~ boost::mutex::scoped_lock lock(mut);
    //~ while(waitingForEvent)
    //~ {
        //~ cond.wait(lock);
    //~ }
    */

    //  TEST 6 //

    name = "<BENCH06> SPARQL test";

    cout << " * " << name << endl;
    result.clear();

    string query = "SELECT ?object WHERE { ?object rdf:type oro:Monkey }";

    onto->query("object", query, result);

    displayCollec(result);

        gettimeofday(&time, NULL);
        timetable[name] = time;

    //  TEST 7 //

    name = "<BENCH07> Find test";

    cout << " * " << name << endl;

    set<Concept> resultConcepts;

    onto->find("object", "?object rdf:type Monkey", resultConcepts);

    copy(resultConcepts.begin(), resultConcepts.end(), ostream_iterator<Concept>(cout, "\n"));


        gettimeofday(&time, NULL);
        timetable[name] = time;


    //  TEST 8 //

    name = "<BENCH08> Filtred find test";

    cout << " * " << name << endl;

    resultConcepts.clear();

    set<string> partial_stmts;
    set<string> filters;

    partial_stmts.insert("?mysterious rdf:type oro:Monkey");
    partial_stmts.insert("?mysterious oro:weight ?value");

    filters.insert("?value >= 50");

    onto->find("mysterious", partial_stmts, filters, resultConcepts);


    copy(resultConcepts.begin(), resultConcepts.end(), ostream_iterator<Concept>(cout, "\n"));

        gettimeofday(&time, NULL);
        timetable[name] = time;

    //  TEST 9 //

    name = "<BENCH09> Consistency check test";

    cout << " * " << name << endl;

    resultConcepts.clear();

    if (!onto->checkConsistency()) {cout<<"Error: the ontology should be found to be consistent."<<endl;}

        gettimeofday(&time, NULL);
        timetable[name] = time;

    ///////////////////////////
    ///////////////////////////

    //  TEST 10 //
    onto->alwaysWaitForAcknowlegment(false);

    cout << " * <BENCH10> Insertion of " << nb_stmt << " statements with 'waitForAck=false'" << endl;

    for (int i = 0 ; i < nb_stmt ; i++)
    {
        Class testClass = Class("test");
        Concept test = Concept::create(testClass);
    }

        gettimeofday(&time, NULL);
        timetable["<BENCH10> insertion statements without waiting for ack"] = time;



    //  TEST 11 //

    cout << " * <BENCH11> Insertion of "<< nb_stmt << " statements with buffering and without waiting for ack" << endl;

    onto->bufferize();
    for (int i = 0 ; i < nb_stmt ; i++)
    {
        Class testClass = Class("test");
        Concept test = Concept::create(testClass);
    }
    onto->flush();

        gettimeofday(&time, NULL);
        timetable["<BENCH11> insertion buffered statements without waiting for ack"] = time;


    connector.waitForPendingRequests();
    gettimeofday(&time, NULL);
    timetable["Time to flush"] = time;


    displayTime();


}
/*
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
        boost::mutex::scoped_lock lock(mut);
        waitingForEvent = false;
        cond.notify_all();

    }

  }

};
*/

void displayTime()
{
    double tick_ms = ((double) CLOCKS_PER_SEC) / 1000;

    cout << endl << "*************** Timetable *****************" << endl;
        map<string, timeval>::iterator bench = timetable.begin();
        timeval tmp = (*bench).second;
    ++bench;

    for( ; bench != timetable.end() ; ++bench) {
                int ms_duration = ((*bench).second.tv_sec * 1000 + (*bench).second.tv_usec / 1000) -
                                  (tmp.tv_sec * 1000 + tmp.tv_usec / 1000);
                cout << (*bench).first << " -> " << ms_duration << "ms" << endl;
        tmp = (*bench).second;
    }

    cout << "*******************************************" << endl;

}

void displayCollec(const set<string>& result)
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
