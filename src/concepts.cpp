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
Concept::Concept():_id(Ontology::newId()), _label("") {}

Concept::Concept(const std::string& id):_id(id), _label(""){}

void Concept::assert(const Property& predicate, const std::string& value){
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

std::string Concept::id() const{
	return _id;
}

boost::logic::tribool Concept::is(const Property& boolDataproperty) const{

}

boost::logic::tribool Concept::hasType(const Class& type) const{
	
	
	throw OntologyException("Not yet implemented!");
}
void Concept::setType(const Class& type){
	assert(Property("rdf:type"), type.to_string());
}

void Concept::setLabel(const std::string& label){
	assert(Property("rdfs:label"), "\"" + label + "\"");
	_label = label;
}
std::string Concept::label() const{
	return _label;
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
	Concept color = Concept::create<Concept>(Classes::Color);
	color.assert(Property("hue"), o.str());
	assert(Properties::hasColor, color);
	Ontology::getInstance()->flush();
}

void Object::setAbsolutePosition(double x, double y, double z){
	
	std::ostringstream o;
	
	Ontology::getInstance()->bufferize();
	
	Concept point = Concept::create<Concept>(Classes::Point);
	o << x;
	point.assert(Property("xCoord"), o.str());
	o.str("");
	o << y;
	point.assert(Property("yCoord"), o.str());
	o.str("");
	o << z;
	point.assert(Property("zCoord"), o.str());
	
	assert(Properties::isAt, point);
	
	Ontology::getInstance()->flush();
}
		
/*****************************************************************************
*                    	  Class Agent					     *
/****************************************************************************/
void Agent::desires(const Action& action) {
	throw OntologyException("Not yet implemented!");
}

void Agent::currentlyPerforms(const Action& action){
	throw OntologyException("Not yet implemented!");
}
		
/*****************************************************************************
*                    	  Class Action					     *
/****************************************************************************/
void Action::object(const Concept& concept) {
	assert(Properties::objectOfAction, concept);
}
void Action::recipient(const Object& object) {
	assert(Properties::recipientOfAction, object);
}

}
