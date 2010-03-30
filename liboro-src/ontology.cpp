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

#include <algorithm>

#include "oro.h"
#include "oro_exceptions.h"

using namespace std;

namespace oro {

Ontology* Ontology::_instance = NULL;

// Protected constructor
Ontology::Ontology(IConnector& connector) : _connector(connector) {
	
	cout << "liboro v." << ORO_VERSION;
	//Initializes the random generator for later generation of unique id for concepts.
	srand(time(NULL));
	
	_bufferize = false;
	
	_buf_op_counter = 0;
	
	if (! checkOntologyServer()) {
		cerr << "\nCannot reach the ontology server! Check it is started and that the middleware link is up.\n";
		throw OntologyServerException("Cannot reach the ontology server. Abandon.");
	}
	
	//TODO : destructor required if resources need to be released.
}

//Singleton creation
Ontology* Ontology::createWithConnector(IConnector& connector){
	if (_instance == NULL)
		_instance = new Ontology(connector);
	
	cout << " - ontology initialized." << endl;

	return _instance;
}

//Singleton access
Ontology* Ontology::getInstance(){
	if (_instance != NULL)
		return _instance;
	else throw UninitializedOntologyException("the ontology is not properly initialized. Created with Ontology::createWithConnector(IConnector&) before any access attempt.");
}

bool Ontology::checkOntologyServer(){
	vector<string> result;
	
	ServerResponse res = _connector.execute("stats");
	
	if (res.status != ServerResponse::ok) return false;
	
	try {
		string version = (boost::get<map<string, string> >(res.result))["version"];
		cout << " - oro-server v." << version;
	} catch (boost::bad_get e) {
		cerr << "Internal error: oro-server answered malformed results at initialization!";
		return false;
	}
	
	return true;
}

void Ontology::evtCallback(const std::string& event_id, const server_return_types& raw_event_content){
	
	set<Concept> event_content;
	
	try {
		set<string> raw_content = boost::get<set<string> >(raw_event_content);
		
		copy(	raw_content.begin(), 
					raw_content.end(), 
					inserter(event_content, event_content.begin()));

	} catch (boost::bad_get e) {
		cerr << "A set of string is expected in the event content! I discard this event"
		<< endl;
		return;
	}
	
	//Create a new event object
	OroEvent e(event_id, event_content);
	
	//Call the liboro event subscriber callback;
	std::map<std::string, EventObserver>::iterator it;
	it = _eventObservers.find(event_id);
	
	if (it != _eventObservers.end()) {
		EventObserver& eo = _eventObservers[event_id];
		(*eo.first)(e);
	
		//If the event is a "one shot", remove it from the event list
		if (eo.second)
			_eventObservers.erase(event_id);
	}
	else cerr << "[EE] Got a callback on an event I don't know!" << endl;

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
	
	set<Statement> stmtToAdd;
		//copy(_buffer["add"].begin(), _buffer["add"].end(), stmtToAdd.begin());
	
	for(BufStatements::iterator i = _buffer["add"].begin() ; i != _buffer["add"].end() ; ++i) {
		stmtToAdd.insert(i->second);
	}
		
	if (stmtToAdd.size() > 0) add(stmtToAdd);
	
	set<Statement> stmtToRemove;
	//copy(_buffer["remove"].begin(), _buffer["remove"].end(), stmtToRemove.begin());
	for(BufStatements::iterator i = _buffer["remove"].begin() ; i != _buffer["remove"].end() ; ++i) {
		stmtToAdd.insert(i->second);
	}
	
	if (stmtToRemove.size() > 0) remove(stmtToRemove); //the order we call add and remove doesn't matter if the buffer is carefully filled through Ontology::addToBuffer. Else, if the same statement is first removed and then added, the flush operation will only retain the "remove"!
	
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
	set<Statement> tmp;
	tmp.insert(statement);
	add(tmp);
}

void Ontology::add(const set<Statement>& statements){
	
	set<string> stringified_stmts;
	set<Statement>::const_iterator iterator = statements.begin();
		
	while( iterator != statements.end() ) {
		
		if (_bufferize) addToBuffer("add", *iterator);
		else stringified_stmts.insert(iterator->to_string());
		++iterator;
	}

	if (!_bufferize) {
		ServerResponse res = _connector.execute("add", stringified_stmts);

		if (res.status == ServerResponse::failed) throw OntologyServerException("Server threw a " + res.exception_msg + " while adding statements. Server message was " + res.error_msg);
	}
}

void Ontology::remove(const Statement& statement){
	set<Statement> tmp;
	tmp.insert(statement);
	remove(tmp);
}

void Ontology::remove(const set<Statement>& statements){
	set<string> stringified_stmts;
	set<Statement>::const_iterator iterator = statements.begin();
	
	while( iterator != statements.end() ) {
		if (_bufferize) addToBuffer("remove", (Statement)*iterator);
		stringified_stmts.insert(((Statement)*iterator).to_string());
		++iterator;
	}

	if (!_bufferize) {
		ServerResponse res = _connector.execute("remove", stringified_stmts);
	
		if (res.status == ServerResponse::failed) throw OntologyServerException("Server" + res.exception_msg + " while removing statements. Server message was " + res.error_msg);
	}
}

void Ontology::addForAgent(const string& agent, const Statement& statement){
	set<Statement> tmp;
	tmp.insert(statement);
	addForAgent(agent, tmp);
}

void Ontology::addForAgent(const string& agent, const set<Statement>& statements){

	vector<server_param_types> parameters;
	set<string> stringified_stmts;
	set<Statement>::const_iterator iterator = statements.begin();
		
	while( iterator != statements.end() ) {
		stringified_stmts.insert(iterator->to_string());
		++iterator;
	}
	
	parameters.push_back(agent);
	parameters.push_back(stringified_stmts);

	ServerResponse res = _connector.execute("addForAgent", parameters);

	if (res.status == ServerResponse::failed)
		throw OntologyServerException("Server threw a " + res.exception_msg + 
		" while adding statements for agent " + agent + ". Server message was " +
		res.error_msg);
}

void Ontology::removeForAgent(const string& agent, const Statement& statement){
	set<Statement> tmp;
	tmp.insert(statement);
	removeForAgent(agent, tmp);
}

void Ontology::removeForAgent(const string& agent, const set<Statement>& statements){

	vector<server_param_types> parameters;
	set<string> stringified_stmts;
	set<Statement>::const_iterator iterator = statements.begin();
		
	while( iterator != statements.end() ) {
		stringified_stmts.insert(iterator->to_string());
		++iterator;
	}
	
	parameters.push_back(agent);
	parameters.push_back(stringified_stmts);

	ServerResponse res = _connector.execute("removeForAgent", parameters);

	if (res.status == ServerResponse::failed)
		throw OntologyServerException("Server threw a " + res.exception_msg + 
		" while removing statements for agent " + agent + ". Server message was " +
		res.error_msg);
}

bool Ontology::checkConsistency(){
	ServerResponse res = _connector.execute("checkConsistency");
	
	if (res.status == ServerResponse::failed) throw OntologyServerException(("Server" + res.exception_msg + " while checking consistency. Server message was " + res.error_msg).c_str());
	
	if(boost::get<bool>(res.result) == true) return true;
	return false;
}

void Ontology::save(const string& path){
	ServerResponse res = _connector.execute("save", path);	
		
	if (res.status == ServerResponse::failed) throw OntologyServerException("Server" + res.exception_msg + " while saving the ontology. Server message was " + res.error_msg);
	
}	

void Ontology::stats(map<string, string>& result){
	ServerResponse res = _connector.execute("stats");
	
	if (res.status == ServerResponse::failed) throw OntologyServerException(("Server " + res.exception_msg + " while fetching stats. Server message was " + res.error_msg).c_str());
	
	try {
		result = boost::get<map<string, string> >(res.result);
	} catch (boost::bad_get e) {
		throw OntologyServerException("INTERNAL ERROR! Server returned wrong type of data while fetching stats. Please contact the maintener (openrobots@laas.fr)");
	}
}

void Ontology::find(const std::string& resource, const std::set<std::string>& partial_statements, const std::set<std::string>& restrictions, std::set<Concept>& result){
	
	set<string> rawResult;
	vector<server_param_types> args;
	args.push_back(resource);
	args.push_back(partial_statements);
	args.push_back(restrictions);
	
	ServerResponse res = _connector.execute("find", args);
	
	if (res.status != ServerResponse::ok)
	{
		throw OntologyServerException("\"filtred find\" operation was not successful: server threw a " + res.exception_msg + " (" + res.error_msg +")");	
	}
	
	rawResult = boost::get<set<string> >(res.result);
	
	copy(rawResult.begin(), rawResult.end(), inserter(result, result.begin()));

}

void Ontology::find(const std::string& resource, const std::set<std::string>& partial_statements, std::set<Concept>& result){
	
	set<string> rawResult;
	vector<server_param_types> args;
	args.push_back(resource);
	args.push_back(partial_statements);

	ServerResponse res = _connector.execute("find", args);
	
	if (res.status != ServerResponse::ok)
	{
		throw OntologyServerException("\"Find\" operation was not successful: server threw a " + res.exception_msg + " (" + res.error_msg +")");	
	}
	
	rawResult = boost::get<set<string> >(res.result);
	
	copy(rawResult.begin(), rawResult.end(), inserter(result, result.begin()));
}

void Ontology::find(const std::string& resource, const std::string& partial_statement, std::set<Concept>& result){
	
	set<string> tmp;
	tmp.insert(partial_statement);
	find(resource, tmp, result);
	
}

//TODO: Some code factorization around would be welcome...
void Ontology::findForAgent(const string& agent, const std::string& resource, const std::set<std::string>& partial_statements, const std::set<std::string>& restrictions, std::set<Concept>& result){
	
	set<string> rawResult;
	vector<server_param_types> args;
	args.push_back(agent);
	args.push_back(resource);
	args.push_back(partial_statements);
	args.push_back(restrictions);
	
	ServerResponse res = _connector.execute("findForAgent", args);
	
	if (res.status != ServerResponse::ok)
	{
		throw OntologyServerException("\"filtred findForAgent\" operation was not successful: server threw a " + res.exception_msg + " (" + res.error_msg +")");	
	}
	
	rawResult = boost::get<set<string> >(res.result);
	
	copy(rawResult.begin(), rawResult.end(), inserter(result, result.begin()));
	
}

void Ontology::findForAgent(const string& agent, const std::string& resource, const std::set<std::string>& partial_statements, std::set<Concept>& result){
	
	set<string> rawResult;
	vector<server_param_types> args;
	args.push_back(agent);
	args.push_back(resource);
	args.push_back(partial_statements);

	ServerResponse res = _connector.execute("findForAgent", args);
	
	if (res.status != ServerResponse::ok)
	{
		throw OntologyServerException("\"FindForAgent\" operation was not successful: server threw a " + res.exception_msg + " (" + res.error_msg +")");	
	}
	
	rawResult = boost::get<set<string> >(res.result);
	
	copy(rawResult.begin(), rawResult.end(), inserter(result, result.begin()));
	
}

void Ontology::findForAgent(const string& agent, const std::string& resource, const std::string& partial_statement, std::set<Concept>& result){
	
	set<string> tmp;
	tmp.insert(partial_statement);
	findForAgent(agent, resource, tmp, result);	
}


void Ontology::guess(const std::string& resource, const double threshold, const std::vector<std::string>& partial_statements, std::set<std::string>& result){
	throw OntologyException("Not yet implemented!");
}

void Ontology::query(const string& var_name, const string& query, set<string>& result){
	vector<server_param_types> args;
	 args.push_back(var_name);
	 args.push_back(query);
	
	ServerResponse res = _connector.execute("query", args);
	
	if (res.status != ServerResponse::ok)
	{
		if (res.exception_msg.find(SERVER_QUERYPARSE_EXCEPTION) != string::npos)
			throw InvalidQueryException(query + "\nYour SPARQL query is invalid: " + res.error_msg);
		throw OntologyServerException("Query was not successful: server threw a " + res.exception_msg + " (" + res.error_msg +").");	
	}
	
	if (set<string>* result_p = boost::get<set<string> >(&res.result))
		result = *result_p;
}

void Ontology::getInfos(const string& resource, set<string>& result){
	ServerResponse res = _connector.execute("getInfos", resource);
	if (res.status != ServerResponse::ok)
	{
		if (res.exception_msg.find(SERVER_NOTFOUND_EXCEPTION) != string::npos)
			throw ResourceNotFoundOntologyException(resource + " does not exist in the current ontology.");
		else throw OntologyServerException("Couldn't retrieve infos on " + resource + ": server threw a " + res.exception_msg + " (" + res.error_msg +").");	
	}
	result = boost::get<set<string> >(res.result);
	
}

void Ontology::registerEvent(	OroEventObserver& callback, 
								EventType eventType, 
								EventTriggeringType triggerType, 
								const std::set<std::string>& pattern, 
								const std::string& variable_to_bind){
	
	vector<server_param_types> args;
	
	bool oneShot = false;
	
	string event_id;
	
	string eventTypeStr;
	string triggerTypeStr;
	
	switch (eventType){ //Painful, but no obvious way to workaround that...
		case FACT_CHECKING:
			eventTypeStr = NAME_OF(FACT_CHECKING);
			break;
		case NEW_INSTANCE:
			eventTypeStr = NAME_OF(NEW_INSTANCE);
			break;
		case NEW_CLASS_INSTANCE:
			eventTypeStr = NAME_OF(NEW_CLASS_INSTANCE);
			break;
	}
	
	switch (triggerType){ //Painful, but no obvious way to workaround that...
		case ON_TRUE:
			triggerTypeStr = NAME_OF(ON_TRUE);
			break;
		case ON_TRUE_ONE_SHOT:
			triggerTypeStr = NAME_OF(ON_TRUE_ONE_SHOT);
			oneShot = true;
			break;
		case ON_FALSE:
			triggerTypeStr = NAME_OF(ON_FALSE);
			break;
		case ON_FALSE_ONE_SHOT:
			triggerTypeStr = NAME_OF(ON_FALSE_ONE_SHOT);
			oneShot = true;
			break;
		case ON_TOGGLE:
			triggerTypeStr = NAME_OF(ON_TOGGLE);
			break;
	}
	
	args.push_back(eventTypeStr);
	args.push_back(triggerTypeStr);
	if (variable_to_bind != "")
		args.push_back(variable_to_bind);
	args.push_back(pattern);
	
	
	ServerResponse res = _connector.execute("registerEvent", args);
	if (res.status != ServerResponse::ok)
	{
		throw OntologyServerException("Couldn't register event: server threw a " + res.exception_msg + " (" + res.error_msg +").");
	}
	event_id = boost::get<string >(res.result);
	
	
	//Store the newly registered event in the list of event observers
	std::map<std::string, EventObserver>::iterator it;
	it = _eventObservers.find(event_id);
	
	if (it != _eventObservers.end())
		cerr << "[EE] Server returned an event id that I already know!" << endl;
	else {
		EventObserver e(&callback, oneShot);
		_eventObservers[event_id] = e;

		cout << "New event registered with id " << event_id << endl;
	}
	

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
