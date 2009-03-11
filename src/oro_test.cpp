#include <vector>
#include <string>
#include <iostream>
#include <iterator>

#include "oro.h"
#include "oro_connector.h"
#include "yarp_connector.h"

using namespace std;

using oro::Ontology;
using oro::Concept;
using oro::Class;
using oro::Property;
using oro::Action;
using oro::Agent;
using oro::Object;

void display(const vector<string>& result)
{

	    cout << "Results:" << endl;

	    copy(result.begin(), result.end(), ostream_iterator<string>(cout, "\n")); //ce n'est pas moi qui ait écrit ça

}

int main(void) {
	
	//Create a connector to the ontology server.
	oro::IConnector yarp = oro::YarpConnector("myDevice", "oro");
	
	//Instanciate the ontology with this connector.
	Ontology* oro = Ontology::createWithConnector(yarp);
	
	cout << "********* TEST ORO *********" << endl;
	cout << "(need a server with onto_test.owl as base ontology)" << endl;
	
	cout << "\n*** TEST 1: Jidology ***" << endl;
	cout << "--> \"Take a bottle on the table and put it in the trash bin\"" << endl;
	
	//Transform the order "Take a bottle on the table and put it in the trash bin"
 
	Object obj1 = Concept::create<Object>(Class("Bottle")); //a bottle, we don't know yet which one
  //since it's "a bottle", we are happy with a under-specified instance.
 
	Object obj2 = Concept::create<Object>(Class("Table")); //a table, we don't know yet which one
  //Since it's "the table", we trigger now a disambiguation now
	
	//TODO SuperModule::which(obj2);
 
	obj1.assert(Property("onTopOf"), obj2);
 
	Object obj3 = Concept::create<Object>(Class("TrashBin"));
	//TODO SuperModule::which(obj3);
 
	Action action1 = Action::create<Action>(Class("Take"));
	action1.object(obj1);
 
	Action action2 = Action::create<Action>(Class("Put"));
	action2.object(obj1); //put the bottle...
	action2.recipient(obj3); //...in the trashbin
	
	cout << action2;

}