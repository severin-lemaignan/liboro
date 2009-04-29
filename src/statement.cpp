/*
 * statement.cpp
 *
 *  Created on: 10 mar. 2009
 *      Author: slemaign
 */

#include <string>
#include <sstream>

#include "oro.h"

using namespace std;

namespace oro {
	Statement::Statement(const Concept& _subject, const Property& _predicate, const Concept& _object):subject(_subject), predicate(_predicate), object(_object), literal_object(""), isObjectLiteral(false){}
	
	Statement::Statement(const Concept& _subject, const Property& _predicate, const std::string& _object):subject(_subject), predicate(_predicate), object(Concept::nothing), literal_object(_object), isObjectLiteral(true){}
		
	inline bool Statement::operator==(const Statement& stmt) const {
	
		//TODO: hyper lent. Ca peut s'ameillorer
		return (stmt.to_string() == to_string());
	}
	
	Statement Statement::create(const string stmt){
		
		string buf; // Have a buffer string
		stringstream ss(stmt); // Insert the string into a stream

		vector<string> tokens; // Create vector to hold the parts of the statement

		while (ss >> buf)
			tokens.push_back(buf);
		
		if (tokens.size() != 3)
			throw InvalidStatementException();

		//TODO This will always generate a statement with a literal object...
		return Statement(Concept(tokens[0]), Property(tokens[1]), tokens[2]);
	}
	/**
	 * Returns a computer-friendly string describing the statement.
	 */
	std::string Statement::to_string() const {
		return subject.to_string() + " " + predicate.to_string() + " " + (isObjectLiteral?literal_object:object.to_string());
	}
}
