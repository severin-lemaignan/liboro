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
	
	_bufferize = false;
	
	_buf_op_counter = 0;
	
	//TODO : destructor required if resources need to be released.
}

//Singleton creation
Ontology* Ontology::createWithConnector(IConnector& connector){
	if (_instance == NULL)
		_instance = new Ontology(connector);
	
	cout << "liboro v." << ORO_VERSION << " - ontology initialized" << endl;

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
	
	//cout << "End of bufferization" << endl;
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

		//here we remove all the previous requests to "add" this very statement, since the "remove" cancel them.	
		BufStatements::iterator i = _buffer["add"].find(stmt.to_string());

		if( i != _buffer["add"].end()) {
			_buffer["add"].erase(i);
		} else _buffer["remove"].insert(pair<string, Statement>(stmt.to_string(), stmt));
	}
	
}

void Ontology::add(const Statement& statement){
	add(vector<Statement>(1, statement));	
}

//TODO replace vectors by sets. It's stupid to use vectors.
void Ontology::add(const vector<Statement>& statements){
	
	vector<string> stringified_stmts;
	vector<Statement>::const_iterator iterator = statements.begin();
		
	while( iterator != statements.end() ) {
		
		if (_bufferize) addToBuffer("add", *iterator);
		else stringified_stmts.push_back(iterator->to_string());
		
		//cout << iterator->to_string() << endl;
		
		++iterator;
	}

	if (!_bufferize) {
		ServerResponse res = _connector.execute("add", stringified_stmts);

		if (res.status == ServerResponse::failed) throw OntologyServerException("Server threw a " + res.exception_msg + " while adding statements. Server message was " + res.error_msg);
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
	
		if (res.status == ServerResponse::failed) throw OntologyServerException("Server" + res.exception_msg + " while removing statements. Server message was " + res.error_msg);
	}
}

bool Ontology::checkConsistency(){
	ServerResponse res = _connector.execute("checkConsistency");
	
	if (res.status == ServerResponse::failed) throw OntologyServerException(("Server" + res.exception_msg + " while checking consistency. Server message was " + res.error_msg).c_str());
	
	if(res.result[0] == "true") return true;
	return false;
}

void Ontology::save(const string& path){
	_connector.execute("save", vector<string>(1, path));
}	

void Ontology::stats(vector<string>& result){
	ServerResponse res = _connector.execute("stats");
	
	if (res.status == ServerResponse::failed) throw OntologyServerException(("Server" + res.exception_msg + " while fetching stats. Server message was " + res.error_msg).c_str());
	
	vector<string> rawResult = res.result;
	
	vector<string>::iterator itRawResult;
	for(itRawResult = rawResult.begin(); itRawResult != rawResult.end(); itRawResult++)
	{
		result.push_back(*(itRawResult));
	}
}	


void Ontology::find(const std::string& resource, const std::vector<std::string>& partial_statements, const std::vector<std::string>& restrictions, std::vector<Concept>& result){
	vector<vector<string> > args;
	vector<string> rawResult;
	args.push_back(vector<string>(1,resource));
	args.push_back(partial_statements);
	args.push_back(restrictions);
	
	ServerResponse res = _connector.execute("filtredFind", args);
	
	if (res.status != ServerResponse::ok)
	{
		throw OntologyServerException("\"filtredFind\" operation was not successful: server threw a " + res.exception_msg + " (" + res.error_msg +")");	
	}
	
	rawResult = res.result;
	
	vector<string>::iterator itRawResult;
	for(itRawResult = rawResult.begin(); itRawResult != rawResult.end(); itRawResult++)
	{
		result.push_back(Concept(*(itRawResult)));
	}
}

void Ontology::find(const std::string& resource, const std::vector<std::string>& partial_statements, std::vector<Concept>& result){
	
	vector<vector<string> > args;
	vector<string> rawResult;
	args.push_back(vector<string>(1,resource));
	args.push_back(partial_statements);
	
	ServerResponse res = _connector.execute("find", args);
	
	if (res.status != ServerResponse::ok)
	{
		throw OntologyServerException("\"Find\" operation was not successful: server threw a " + res.exception_msg + " (" + res.error_msg +")");	
	}
	
	rawResult = res.result;
	
	vector<string>::iterator itRawResult;
	for(itRawResult = rawResult.begin(); itRawResult != rawResult.end(); itRawResult++)
	{
		result.push_back(Concept(*(itRawResult)));
	}
	
	
}

void Ontology::find(const std::string& resource, const std::string& partial_statement, std::vector<Concept>& result){
	
	find(resource, vector<string>(1,partial_statement), result);
	
}


int Ontology::guess(const std::string& resource, const double threshold, const std::vector<std::string>& partial_statements, std::vector<std::string>& result){
	throw OntologyException("Not yet implemented!");
}

int Ontology::query(const std::string& var_name, const std::string& query, std::vector<std::string>& result){
	vector<string> args;
	 args.push_back(var_name);
	 args.push_back(query);
	
	ServerResponse res = _connector.execute("query", args);
	
	if (res.status != ServerResponse::ok)
	{
		if (res.exception_msg.find(SERVER_QUERYPARSE_EXCEPTION) != string::npos)
			throw InvalidQueryException(query + "\nYour SPARQL query is invalid: " + res.error_msg);
		throw OntologyServerException("Query was not successful: server threw a " + res.exception_msg + " (" + res.error_msg +")");	
	}
	result = res.result;
}

void Ontology::getInfos(const string& resource, vector<std::string>& result){
	ServerResponse res = _connector.execute("getInfos", vector<string>(1, resource));
	if (res.status != ServerResponse::ok)
	{
		if (res.exception_msg.find(SERVER_NOTFOUND_EXCEPTION) != string::npos)
			throw ResourceNotFoundOntologyException(resource + " does not exist in the current ontology.");
		else throw OntologyServerException("Couldn't retrieve infos on " + resource + ": server threw a " + res.exception_msg + ".");	
	}
	result = res.result;
	
}

void Ontology::subscribe(const std::string& watchExpression, EventTriggeringType triggerType, const std::string& portToTrigger){
	vector<string> args;
	string port(portToTrigger);
	args.push_back(watchExpression);
	switch (triggerType){
		case ON_TRUE:
			args.push_back("ON_TRUE");
			break;
		case ON_TRUE_ONE_SHOT:
			args.push_back("ON_TRUE_ONE_SHOT");
			break;
		case ON_FALSE:
			args.push_back("ON_FALSE");
			break;
		case ON_FALSE_ONE_SHOT:
			args.push_back("ON_FALSE_ONE_SHOT");
			break;
		case ON_TOGGLE:
			args.push_back("ON_TOGGLE");
			break;
		default:
			args.push_back("UNSUPPORTED_TRIGGER_TYPE");
			break;
	}
	
	if (port.find("0/") != 0) port = "/" + port;
	
	args.push_back(port);
	
	_connector.execute("subscribe", args);
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