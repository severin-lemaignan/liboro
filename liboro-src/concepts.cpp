/*
 * concept.cpp
 *
 *  Created on: 2 févr. 2009
 *      Author: slemaign
 */

#include <sstream>
#include "oro.h"
#include "oro_library.h"

using namespace boost::logic;
using namespace std;

namespace oro {

/*****************************************************************************
*                    	  Class Concept					     *
/****************************************************************************/
Concept::Concept():_id(Ontology::newId()), _label(""), _class(Classes::Thing) {}

Concept::Concept(const std::string& id):_id(id), _label(""), _class(Classes::Thing) {}

Concept Concept::create(const std::string& label) {
	Concept concept;
	concept.setLabel(label);

	return concept;
}

Concept Concept::create(const std::string& label, const Class& type){
	Concept concept;
	concept.setLabel(label);
	concept.setType(type);

	return concept;
}

Concept Concept::create(const Class& type){
	Concept concept;
	concept.setType(type);

	return concept;
}

void Concept::assert(const Property& predicate, const string& value){
	//cout << "Asserting " << _label << "(id: " << _id << ")" << predicate << " " << value << endl;
	Ontology::getInstance()->add(Statement(*this, predicate, value));
}

void Concept::assert(const Property& predicate, const Concept& value){
	assert(predicate, value.id());
}

void Concept::clear(const Property& predicate){
	throw OntologyException("Not yet implemented!");
}

boost::logic::tribool Concept::has(const Property& predicate) const {
	throw OntologyException("Not yet implemented!");
}

boost::logic::tribool Concept::has(const Property& predicate, const std::string& value) const{
	
	throw OntologyException("Not yet implemented!");
	
	tribool result;
	vector<string> query = vector<string>();
	vector<string> tmpResult = vector<string>();
	
	
	//Ontology::getInstance()->query(_id + " " + predicate.name() + " " + value);
	//if (!().empty())) result = true;
	//else result = indeterminate;
	
	//TODO: if the predicate is functionnal and there is an assertion with this predicate for this subject, then the return value should be false.
	return result;
}

boost::logic::tribool Concept::has(const Property& predicate, const Concept& value) const{
	return has(predicate, value.id());
}

std::set<Concept> Concept::getObjectsForPredicate(const Property& predicate) const{
	throw OntologyException("Not yet implemented!");
}

boost::logic::tribool Concept::is(const Property& boolDataproperty) const{
	throw OntologyException("Not yet implemented!");
}

boost::logic::tribool Concept::hasType(const Class& type) const{
	throw OntologyException("Not yet implemented!");
}
void Concept::setType(const Class& type){
	assert(Property("rdf:type"), type.to_string());
	_class = type;
}

void Concept::setLabel(const std::string& label){
	assert(Property("rdfs:label"), "\"" + label + "\"");
	_label = label;
}

void Concept::remove(const Property& predicate, const std::string& value){
	Ontology::getInstance()->remove(Statement(*this, predicate, value));
}

void Concept::remove(const Property& predicate, const Concept& value){
	Ontology::getInstance()->remove(Statement(*this, predicate, value.id()));
}

const Concept Concept::nothing = Concept();
/*****************************************************************************
*                    	  Class Object					     *
/****************************************************************************/

Object Object::create() {
	Object concept;
	concept.setType(Class("Object"));

	return concept;
}

Object Object::create(const std::string& label) {
	Object concept;
	concept.setLabel(label);
	concept.setType(Class("Object"));

	return concept;
}

Object Object::create(const std::string& label, const Class& type){
	Object concept;
	concept.setLabel(label);
	concept.setType(type);

	return concept;
}

Object Object::create(const Class& type){
	Object concept;
	concept.setType(type);

	return concept;
}
		
boost::logic::tribool Object::hasAbsolutePosition(){
	throw OntologyException("Not yet implemented!");
}

Concept Object::hasPosition(){
	throw OntologyException("Not yet implemented!");
}

void Object::setColor(int hue){
	std::ostringstream o;
	
	o << hue;
	
	Ontology::getInstance()->bufferize();
	Concept color = Concept::create(Classes::Color);
	color.assert(Properties::hue, o.str());
	assert(Properties::hasColor, color);
	Ontology::getInstance()->flush();
}

void Object::setAbsolutePosition(double x, double y, double z){
	
	std::ostringstream o;
	
	Ontology::getInstance()->bufferize();
	
	Concept point = Concept::create(Classes::Point);
	o << x;
	point.assert(Properties::xCoord, o.str());
	o.str("");
	o << y;
	point.assert(Properties::yCoord, o.str());
	o.str("");
	o << z;
	point.assert(Properties::zCoord, o.str());
	
	assert(Properties::isAt, point);
	
	Ontology::getInstance()->flush();
}
		
/*****************************************************************************
*                    	  Class Agent					     *
/****************************************************************************/

Agent Agent::create() {
	Agent concept;
	concept.setType(Class("Agent"));

	return concept;
}

Agent Agent::create(const std::string& label) {
	Agent concept;
	concept.setLabel(label);
	concept.setType(Class("Agent"));

	return concept;
}
		
Agent Agent::create(const std::string& label, const Class& type){
	Agent concept;
	concept.setLabel(label);
	concept.setType(type);

	return concept;
}

Agent Agent::create(const Class& type){
	Agent concept;
	concept.setType(type);

	return concept;
}

void Agent::sees(const Concept& concept, bool asserted = true) {
	if (asserted)
		assert(Properties::sees, concept);
	else
		remove(Properties::sees, concept);
}
		
void Agent::desires(const Action& action) {
	assert(Properties::desires, action);
}

void Agent::currentlyPerforms(const Action& action){
	assert(Properties::currentlyPerforms, action);
}
		
/*****************************************************************************
*                    	  Class Action					     *
/****************************************************************************/
		
Action Action::create() {
	Action concept;
	concept.setType(Class("Action"));

	return concept;
}
		
Action Action::create(const std::string& label) {
	Action concept;
	concept.setLabel(label);
	concept.setType(Class("Action"));

	return concept;
}

Action Action::create(const std::string& label, const Class& type){
	Action concept;
	concept.setLabel(label);
	concept.setType(type);

	return concept;
}

Action Action::create(const Class& type){
	Action concept;
	concept.setType(type);

	return concept;
}
		
void Action::object(const Concept& concept) {
	assert(Properties::objectOfAction, concept);
}
void Action::recipient(const Object& object) {
	assert(Properties::recipientOfAction, object);
}

}
