/*
 * concept.cpp
 *
 *  Created on: 2 févr. 2009
 *      Author: slemaign
 */

#include "oro.h"

using namespace std;

namespace oro {

Ontology* Ontology::_instance = NULL;

// Private constructor
Ontology::Ontology(IConnector& connector) : _connector(connector) {
	
	//Initializes the random generator for later generation of unique id for concepts.
	srand(time(NULL));
	
	//TODO : destructor required if resources need to be released.
}

//Singleton creation
Ontology* Ontology::createWithConnector(IConnector& connector){
	if (_instance == NULL)
		_instance = new Ontology(connector);

	return _instance;
}

//Singleton access
Ontology* Ontology::getInstance(){
	if (_instance != NULL)
		return _instance;
	else throw UninitializedOntologyException();
}

void Ontology::add(const Statement& statement){
	//vector<string> tmp(1, statement.to_string());
	//tmp.push_back(statement.to_string());
	_connector.execute("add", vector<string>(1, statement.to_string()));
}

void Ontology::add(const std::vector<Statement>& statements){
	
	vector<string> stringified_stmts;
	vector<Statement>::const_iterator iterator = statements.begin();
	
	while( iterator != statements.end() ) {
		stringified_stmts.push_back(((Statement)*iterator).to_string());
		++iterator;
	}

	_connector.execute("add", stringified_stmts);
}

void Ontology::remove(const Statement& statement){
	_connector.execute("remove", vector<string>(1, statement.to_string()));
}

void Ontology::remove(const std::vector<Statement>& statements){
	vector<string> stringified_stmts;
	vector<Statement>::const_iterator iterator = statements.begin();
	
	while( iterator != statements.end() ) {
		stringified_stmts.push_back(((Statement)*iterator).to_string());
		++iterator;
	}

	_connector.execute("remove", stringified_stmts);
}

bool Ontology::checkConsistency(){
	_connector.execute("check_consistency");
}	

int Ontology::find(const std::string& resource, const std::vector<std::string>& partial_statements, const std::vector<std::string>& restrictions, std::vector<Concept>& result){
	throw OntologyException("Not yet implemented!");
}

int Ontology::find(const std::string& resource, const std::vector<std::string>& partial_statements, std::vector<Concept>& result){
	throw OntologyException("Not yet implemented!");
}

int Ontology::find(const std::string& resource, const std::string& partial_statement, std::vector<Concept>& result){
	throw OntologyException("Not yet implemented!");
}


int Ontology::guess(const std::string& resource, const double threshold, const std::vector<std::string>& partial_statements, std::vector<std::string>& result){
	throw OntologyException("Not yet implemented!");
}

int Ontology::query(const std::string& var_name, const std::string& query, std::vector<std::string>& result){
	throw OntologyException("Not yet implemented!");
}

		
string Ontology::newId(int length)
	{
		string result;

		for(int i=0; i<length; i++)
		{
			result += (char)(rand() % 26 + 97); //ASCII codes of letters starts at 98 for "a"
		}

		return result;
	}

}