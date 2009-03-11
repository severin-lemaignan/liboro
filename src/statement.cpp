/*
 * statement.cpp
 *
 *  Created on: 10 mar. 2009
 *      Author: slemaign
 */

#include "oro.h"

using namespace std;

namespace oro {
	Statement::Statement(const Concept& _subject, const Property& _predicate, const Concept& _object):subject(_subject), predicate(_predicate), object(_object), literal_object(""), isObjectLiteral(false){}
	
	Statement::Statement(const Concept& _subject, const Property& _predicate, const std::string& _object):subject(_subject), predicate(_predicate), object(Concept::nothing), literal_object(_object), isObjectLiteral(true){}
		
	/**
	 * Returns a computer-friendly string describing the statement.
	 */
	std::string Statement::to_string() const {
		return subject.to_string() + " " + predicate.to_string() + " " + (isObjectLiteral?literal_object:object.to_string());
	}
}
