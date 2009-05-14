/*
 * ©LAAS-CNRS (2008-2009)
 *
 * contributor(s) : Séverin Lemaignan <severin.lemaignan@laas.fr>
 *
 * This software is a computer program whose purpose is to interface
 * with an ontology in a robotics context.
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

#include <vector>
#include <string>
#include <iostream>
#include <iterator>

#include "liboro.h"
#include "concepts.h"

using namespace std;
using namespace openrobots;

void display(const vector<string>& result)
{

	    cout << "Results:" << endl;

	    copy(result.begin(), result.end(), ostream_iterator<string>(cout, "\n")); //ce n'est pas moi qui ait écrit ça

}

int main(void) {

	vector<string> result;


	Oro& oro = Oro::getInstance("myDevice", "oro");

	cout << "********* TEST ORO *********" << endl;
	cout << "(need a server with onto_test.owl as base ontology)" << endl;


	cout << "=> add()" << endl;

	oro.add("gorilla rdf:type Monkey");
	oro.add("gorilla age 12^^xsd:int");
	oro.add("gorilla weight 75.2");



    cout << "<= add() ok!" << endl << endl;

	cout << "=> add() multiple" << endl;

	vector<string> stmts;

	stmts.push_back("gorilla rdf:type Monkey");
	stmts.push_back("gorilla age 12^^xsd:int");
	stmts.push_back("gorilla weight 75.2");

	oro.add(stmts);

    cout << "<= add() multiple ok!" << endl << endl;


	cout << "=> getInfos()" << endl;

	oro.getInfos("gorilla", result);

    display(result);

    cout << "<= getInfos() ok!" << endl << endl;

    cout << "=> getHumanReadableInfos()" << endl;

    	oro.getHumanReadableInfos("gorilla", result);

        display(result);

        cout << "<= getHumanReadableInfos() ok!" << endl << endl;

    cout << "=> query()" << endl;

    oro.query("instances", "SELECT ?instances \n WHERE { \n ?instances rdf:type owl:Thing}\n", result);

    display(result);

    cout << "<= query() ok!" << endl << endl;

    cout << "=> find()" << endl;

    vector<string> partial_stmts;

	partial_stmts.push_back("?mysterious oro:eats oro:banana_tree");
	partial_stmts.push_back("?mysterious oro:isFemale true^^xsd:boolean");

	oro.find("mysterious", partial_stmts, result);

    display(result);

    cout << "<= find() ok!" << endl << endl;

    cout << "=> filtredFind()" << endl;

        partial_stmts.clear();
        vector<string> filters;

    	partial_stmts.push_back("?mysterious oro:eats oro:banana_tree");
    	partial_stmts.push_back("?mysterious oro:isFemale true^^xsd:boolean");

    	filters.push_back("?value >= 50");

    	oro.find("mysterious", partial_stmts, filters, result);

        display(result);

    cout << "<= filtredFind() ok!" << endl << endl;


    cout << "=> guess()" << endl;

    partial_stmts.clear();

	partial_stmts.push_back("?mysterious age \"40\"^^xsd:int");
	partial_stmts.push_back("?mysterious weight \"60\"^^xsd:double");

	oro.guess("mysterious", 0.8, partial_stmts, result);

    display(result);

    cout << "<= guess() ok!" << endl << endl;

	return 0;
}
