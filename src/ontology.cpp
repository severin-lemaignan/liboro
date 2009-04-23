/*
 * ontology.cpp
 *
 *  Created on: 2 févr. 2009
 *      Author: slemaign
 */

#include "oro.h"
#include "oro_exceptions.h"

using namespace std;

namespace oro {

Ontology* Ontology::_instance = NULL;

// Protected constructor
Ontology::Ontology(IConnector& connector) : _connector(connector) {
	
	//Initializes the random generator for later generation of unique id for concepts.
	srand(time(NULL));
	
	//TODO : destructor required if resources need to be released.
}

//Singleton creation
Ontology* Ontology::createWithConnector(IConnector& connector){
	if (_instance == NULL)
		_instance = new Ontology(connector);
	
	cout << "liboro v.0.1 - ontology initialized" << endl;

	return _instance;
}

//Singleton access
Ontology* Ontology::getInstance(){
	if (_instance != NULL)
		return _instance;
	else throw UninitializedOntologyException("the ontology is not properly initialized. Created with Ontology::createWithConnector(IConnector&) before any access attempt.");
}

void Ontology::add(const Statement& statement){
	add(vector<Statement>(1, statement));	
}

void Ontology::add(const std::vector<Statement>& statements){
	
	vector<string> stringified_stmts;
	vector<Statement>::const_iterator iterator = statements.begin();
	
	while( iterator != statements.end() ) {
		stringified_stmts.push_back(((Statement)*iterator).to_string());
		++iterator;
	}

	ServerResponse res = _connector.execute("add", stringified_stmts);

	if (res.status == ServerResponse::failed) throw OntologyServerException(("Server" + res.exception_msg + " while adding statements. Server message was " + res.error_msg).c_str());
}

void Ontology::remove(const Statement& statement){
	remove(vector<Statement>(1, statement));
}

void Ontology::remove(const std::vector<Statement>& statements){
	vector<string> stringified_stmts;
	vector<Statement>::const_iterator iterator = statements.begin();
	
	while( iterator != statements.end() ) {
		stringified_stmts.push_back(((Statement)*iterator).to_string());
		++iterator;
	}

	ServerResponse res = _connector.execute("remove", stringified_stmts);
	
	if (res.status == ServerResponse::failed) throw OntologyServerException(("Server" + res.exception_msg + " while removing statements. Server message was " + res.error_msg).c_str());
}

bool Ontology::checkConsistency(){
	_connector.execute("check_consistency");
}	

void Ontology::save(const std::string& path){
	_connector.execute("save", vector<string>(1, path));
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