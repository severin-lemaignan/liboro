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

#include <string>
#include <sstream>

#include "oro.h"

using namespace std;

namespace oro {
	Statement::Statement(const Concept& _subject, const Property& _predicate, const Concept& _object):subject(_subject), predicate(_predicate), object(_object), literal_object(""), isObjectLiteral(false){
		_originalStmt = subject.to_string() + " " + predicate.to_string() + " " + (isObjectLiteral?literal_object:object.to_string());
	}
	
	Statement::Statement(const Concept& _subject, const Property& _predicate, const std::string& _object):subject(_subject), predicate(_predicate), object(Concept::nothing), literal_object(_object), isObjectLiteral(true){
		_originalStmt = subject.to_string() + " " + predicate.to_string() + " " + (isObjectLiteral?literal_object:object.to_string());
	}
		
	/**
	 * Creates a new statement from its literal string representation.
	*/
	Statement::Statement(const string& stmt) : subject(""), predicate(""), object(""), literal_object(""), isObjectLiteral(false){
		
		_originalStmt = stmt;
				
		string buf = stmt; // Have a buffer string
		//stringstream ss(stmt); // Insert the string into a stream

		vector<string> tokens; // Create vector to hold the parts of the statement

		//while (ss >> buf) //nice and efficient to tokenize a string on the spaces
		//	tokens.push_back(buf);
		
		//pas super élégant...
		int cutAt;
		
		//First token (subject)
		cutAt = buf.find(" ");
		tokens.push_back(buf.substr(0,cutAt));
		
		//if(tokens[0].find(":") != string::npos){
		//	tokens[0] = tokens[0].substr(tokens[0].find(":"));
		//}
		
		subject = Concept(tokens[0]);
		
		//Second token (predicate)
		buf = buf.substr(cutAt+1);
		cutAt = buf.find(" ");
		tokens.push_back(buf.substr(0,cutAt));
		
		predicate = Property(tokens[1]);
				
		//Third token (object)
		buf = buf.substr(cutAt+1);
		tokens.push_back(buf);
		
		//if (tokens.size() != 3)
		//	throw InvalidStatementException();
		
		//TODO Ugly... if the last token simply contains a semicolon, we assume it's a concept. Need to be improved!
		if(tokens[2].find(":") != string::npos){
			tokens[2] = tokens[2].substr(tokens[2].find(":"));
			object = Concept(tokens[2]);
			isObjectLiteral = false;
		}
		else {
			object = tokens[2];
			isObjectLiteral = true;
		}
	}
	/**
	 * Returns a computer-friendly string describing the statement.
	 */
	std::string Statement::to_string() const {
		//return subject.to_string() + " " + predicate.to_string() + " " + (isObjectLiteral?literal_object:object.to_string());
		return _originalStmt;
	}
}
