#include <vector>
#include <string>
#include <iostream>
#include <iterator>

#include "oro.h"
#include "oro_library.h"
#include "oro_connector.h"
#include "socket_connector.h"
#include "dummy_connector.h"

using namespace std;

using namespace oro;


void display(const vector<string>& result)
{

	    cout << "Results:" << endl;

	    copy(result.begin(), result.end(), ostream_iterator<string>(cout, "\n")); //ce n'est pas moi qui ait écrit ça

}

/**
 * A "fake" HRI module. Returns a ordered list of actions to start from a user request.
 * @return A vector of actions to start.
 */
vector<Concept> hriModule(void){
	cout << "--> \"Take a bottle on the table and put it in the trash bin\"" << endl;
	
	//Transform the order "Take a bottle on the table and put it in the trash bin"
 
	//TODO MagicModule::analyse();
	
	Object obj1 = Concept::create<Object>(Classes::Bottle); //a bottle, we don't know yet which one
  //since it's "a bottle", we are happy with a under-specified instance.
	
 
	Object obj2 = Concept::create<Object>(Classes::Table); //a table, we don't know yet which one
  //Since it's "the table", we trigger now a disambiguation
	
	//TODO SuperModule::which(obj2);
 
	obj1.assertThat(Properties::onTopOf, obj2);
 
	Object obj3 = Concept::create<Object>(Classes::Trashbin);
	//TODO SuperModule::which(obj3);
 
	Action action1 = Action::create<Action>(Classes::Take);
	action1.object(obj1);
 
	Action action2 = Action::create<Action>(Classes::Put);
	action2.object(obj1); //put the bottle...
	action2.recipient(obj3); //...in the trashbin
	
	vector<Concept> res;
	res.push_back(action1);
	res.push_back(action2);	
}

void plannerModule(vector<Concept>) {

	

}

int main(void) {
	
	//Create a connector to the ontology server.
	oro::SocketConnector connector("localhost", 6969);
	//oro::DummyConnector connector();
	
	//Instanciate the ontology with this connector.
	Ontology* oro = Ontology::createWithConnector(connector);
	
	cout << "********* TEST ORO *********" << endl;
	cout << "(need a server with openrobots.owl as base ontology)" << endl;
	
	cout << "\n*** TEST 1: Jidology ***" << endl;
	
	vector<Concept> res = hriModule();
	
	plannerModule(res);

	
	cout << "*******************************" << endl;

}
