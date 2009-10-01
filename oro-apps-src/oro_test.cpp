 /*
 * ©LAAS-CNRS (2008-2009)
 *
 * contributor(s) : Séverin Lemaignan <severin.lemaignan@laas.fr>
 *
 * This software is a computer program whose purpose is to interface
 * with an ontology server in a robotics context.
 *
 * This software is governed by the CeCILL  license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * "http://www.cecill.info".
 *
 * As a counterpart to the access to the source code and  rights to copy,
 * modify and redistribute granted by the license, users are provided only
 * with a limited warranty  and the software's author,  the holder of the
 * economic rights,  and the successive licensors  have only  limited
 * liability.
 *
 * In this respect, the user's attention is drawn to the risks associated
 * with loading,  using,  modifying and/or developing or reproducing the
 * software by the user in light of its specific status of free software,
 * that may mean  that it is complicated to manipulate,  and  that  also
 * therefore means  that it is reserved for developers  and  experienced
 * professionals having in-depth computer knowledge. Users are therefore
 * encouraged to load and test the software's suitability as regards their
 * requirements in conditions enabling the security of their systems and/or
 * data to be ensured and,  more generally, to use and operate it in the
 * same conditions as regards security.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
*/

 #include <iostream>
 #include <iterator>
 #include "oro.h"
 #include "oro_library.h"
 #include "socket_connector.h"

 using namespace std;
 using namespace oro;
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
                unknown_object.setColor(212); //the Object class offers some high-level setters like setColor. See the class documentation for the list.
                //if no setter is available for a given property, then direct assertion can be made.
                //the list of existing properties and classes come from the oro ontology itself (from which oro_library.h/cpp is automatically generated)
                unknown_object.assertThat(Properties::isOnTopOf, table);
 
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
 
                return 0;
 }

