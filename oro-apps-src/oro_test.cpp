
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

 #include <iostream>
 #include <iterator>
 #include <boost/variant.hpp>
 
 #include "oro.h"
 #include "oro_library.h"
 #include "socket_connector.h"

 using namespace std;
 using namespace oro;
 
 class EventCallback : public OroEventObserver {

	void operator()(const OroEvent& evt) {
                cout << "Event triggered!" << endl << "Event content: " << endl;
		
		set<Concept> evt_content = boost::get<set<Concept> >(evt.content);
		
		copy(evt_content.begin(), evt_content.end(), ostream_iterator<Concept>(cout, "\n"));
	} 
 };
 
 int main(void) {
	set<Concept> result;
	set<string> partial_stmts;

	SocketConnector connector("localhost", "6969");
	Ontology *oro = Ontology::createWithConnector(connector); //actually connect the application to the ontology server. The "oro" object is here built as a singleton.

        oro->bufferize();
		//First, create some instances (ie, objects).
		Agent robot1 = Agent::create("Nice Robot", Classes::Robot); //a new instance of Agent has been created. It is named "Nice Robot" and its type (or "class") is set to be a Robot (which is a subconcept of Agent).
		Agent human = Agent::create("Young PhD", Classes::Human); //another agent...

		Object table = Object::create(Classes::Table); //here, an object is created. No name (or "label") has been set up, but the class is refined: it's not only an object, but more precisely a table.
		Object unknown_object = Object::create(); //here, an unknown object has been identified, without any more infos.
                //if no setter is available for a given property, then direct assertion can be made.
		//the list of existing properties and classes come from the oro ontology itself (from which oro_library.h/cpp is automatically generated)
		unknown_object.assertThat(Properties::isOn, table);

		Agent myself("myself"); //"myself" is a special, unique instance representing the robot itself. This instance is built from the already existing identifier "myself". Hence the constructor of the Agent class can be directly called.

		myself.sees(unknown_object);
		myself.sees(human);
	oro->flush();

	//Then, try to find back the unknown object...
	partial_stmts.insert("?mysterious oro:isAt ?table");
	partial_stmts.insert("?table rdf:type oro:Table");
	partial_stmts.insert("oro:myself oro:sees ?mysterious");

	oro->find("mysterious", partial_stmts, result);

	//display the results on std_out
	copy(result.begin(), result.end(), ostream_iterator<Concept>(cout, "\n"));
	
	/** EVENTS **/
	EventCallback ec;
	Classes::Human.onNewInstance(ec);
	Agent superman = Agent::create("Superman", Classes::Human);
	
	cout << "Sleeping for 1 sec..." << endl;
	sleep(1);


        set<string> event_pattern;
        Property flyingProp = Property("isFlying");

        event_pattern.insert( superman.id() + " isFlying true");
        oro->registerEvent(ec, FACT_CHECKING, ON_TRUE_ONE_SHOT, event_pattern, "");

        superman.assertThat(flyingProp, "true");

        cout << "Sleeping for 1 sec..." << endl;
        sleep(1);

	
	//oro->checkConsistency();

	return 0;
 }

