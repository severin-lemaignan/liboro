
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
		
		//Problem: the third argument may contain spaces
		
		//if (tokens.size() != 3)
		//	throw InvalidStatementException();
		
		//TODO Ugly... if the last token contains a semicolon or the predicate is subClassOf, we assume it's a concept. Need to be improved!
		if(tokens[2].find(":") != string::npos){
			tokens[2] = tokens[2].substr(tokens[2].find(":"));
			object = Concept(tokens[2]);
			isObjectLiteral = false;
		}
		else if (tokens[1] == "rdfs:subClassOf") {
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
