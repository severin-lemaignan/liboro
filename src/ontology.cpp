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
	
	_buf_op_counter = 0;
	
	//TODO : destructor required if resources need to be released.
}

//Singleton creation
Ontology* Ontology::createWithConnector(IConnector& connector){
	if (_instance == NULL)
		_instance = new Ontology(connector);
	
	cout << "liboro v.0.2.2 - ontology initialized" << endl;

	return _instance;
}

//Singleton access
Ontology* Ontology::getInstance(){
	if (_instance != NULL)
		return _instance;
	else throw UninitializedOntologyException("the ontology is not properly initialized. Created with Ontology::createWithConnector(IConnector&) before any access attempt.");
}

void Ontology::bufferize(){
	_bufferize = true;
	_buf_op_counter++;
}

void Ontology::flush(){
	if (!_bufferize) return;
			
	if (!(_buf_op_counter--)) return; //more that one on-going bufferization operation? decrement the counter and return.
	
	cout << "End of bufferization" << endl;
	_bufferize = false;
	
	vector<Statement> stmtToAdd;
	stmtToAdd.reserve(_buffer["add"].size());
	//copy(_buffer["add"].begin(), _buffer["add"].end(), stmtToAdd.begin());
	
	for(BufStatements::iterator i = _buffer["add"].begin() ; i != _buffer["add"].end() ; ++i) {
		stmtToAdd.push_back(i->second);
	}
		
	add(stmtToAdd);
	
	vector<Statement> stmtToRemove;
	stmtToRemove.reserve(_buffer["remove"].size());
	//copy(_buffer["remove"].begin(), _buffer["remove"].end(), stmtToRemove.begin());
	for(BufStatements::iterator i = _buffer["remove"].begin() ; i != _buffer["remove"].end() ; ++i) {
		stmtToAdd.push_back(i->second);
	}
	
	remove(stmtToRemove); //the order we call add and remove doesn't matter if the buffer is carefully filled through Ontology::addToBuffer. Else, if the same statement is first removed and then added, the flush operation will only retain the "remove"!
	
	_buffer["add"].clear();
	_buffer["remove"].clear();
}

void Ontology::addToBuffer(const string action, const Statement& stmt) {
	
	//TODO replace vectors by sets or lists. It's stupid to use vectors.
	if (action == "add") {

		//here we remove all the previous requests to "remove" this very statement, since the "add" cancel them.	
		BufStatements::iterator i = _buffer["remove"].find(stmt.to_string());
		
		if( i != _buffer["remove"].end()) {
				_buffer["remove"].erase(i);
		} else _buffer["add"].insert(pair<string, Statement>(stmt.to_string(), stmt));
	}
	
	else if (action == "remove") {

		//here we remove all the previous requests to "remove" this very statement, since the "add" cancel them.	
		BufStatements::iterator i = _buffer["add"].find(stmt.to_string());

		if( i != _buffer["add"].end()) {
			_buffer["add"].erase(i);
		} else _buffer["remove"].insert(pair<string, Statement>(stmt.to_string(), stmt));
	}
	
}

void Ontology::add(const Statement& statement){
	add(vector<Statement>(1, statement));	
}

//TODO replace vectors by sets or lists. It's stupid to use vectors.
void Ontology::add(const std::vector<Statement>& statements){
	
	vector<string> stringified_stmts;
	vector<Statement>::const_iterator iterator = statements.begin();
	
	while( iterator != statements.end() ) {
		
		if (_bufferize) addToBuffer("add", (Statement)*iterator);
		else stringified_stmts.push_back(((Statement)*iterator).to_string());
		
		++iterator;
	}

	if (!_bufferize) {
		ServerResponse res = _connector.execute("add", stringified_stmts);

		if (res.status == ServerResponse::failed) throw OntologyServerException(("Server" + res.exception_msg + " while adding statements. Server message was " + res.error_msg).c_str());
	}
}

void Ontology::remove(const Statement& statement){
	remove(vector<Statement>(1, statement));
}

//TODO replace vectors by sets or lists. It's stupid to use vectors.
void Ontology::remove(const std::vector<Statement>& statements){
	vector<string> stringified_stmts;
	vector<Statement>::const_iterator iterator = statements.begin();
	
	while( iterator != statements.end() ) {
		if (_bufferize) addToBuffer("remove", (Statement)*iterator);
		stringified_stmts.push_back(((Statement)*iterator).to_string());
		++iterator;
	}

	if (!_bufferize) {
		ServerResponse res = _connector.execute("remove", stringified_stmts);
	
		if (res.status == ServerResponse::failed) throw OntologyServerException(("Server" + res.exception_msg + " while removing statements. Server message was " + res.error_msg).c_str());
	}
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