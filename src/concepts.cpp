
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

#include <sstream>
#include "oro.h"
#include "oro_library.h"

using namespace boost::logic;
using namespace std;

namespace oro {

/*******************************************************************************
*                       	  Class Concept                                *
*******************************************************************************/
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

void Concept::assertThat(const Property& predicate, const string& value){
	Ontology::getInstance()->add(Statement(*this, predicate, value));
}

void Concept::assertThat(const Property& predicate, const Concept& value){
	assertThat(predicate, value.id());
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
	
	//TODO: if the predicate is functionnal and there is an assertion with this
	// predicate for this subject, then the return value should be false.
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
	assertThat(Property("rdf:type"), type.to_string());
	_class = type;
}

void Concept::setLabel(const std::string& label){
	assertThat(Property("rdfs:label"), "\"" + label + "\"");
	_label = label;
}

void Concept::remove(const Property& predicate, const std::string& value){
	Ontology::getInstance()->remove(Statement(*this, predicate, value));
}

void Concept::remove(const Property& predicate, const Concept& value){
	Ontology::getInstance()->remove(Statement(*this, predicate, value.id()));
}

const Concept Concept::nothing = Concept();
/*******************************************************************************
*                            	  Class Object					               *
*******************************************************************************/

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

/*******************************************************************************
*                           	  Class Agent					               *
*******************************************************************************/

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

void Agent::sees(const Concept& concept, bool asserted) {
	if (asserted)
		assertThat(Properties::sees, concept);
	else
		remove(Properties::sees, concept);
}
		
void Agent::desires(const Action& action) {
	assertThat(Properties::desires, action);
}

void Agent::currentlyPerforms(const Action& action){
	assertThat(Properties::currentlyPerforms, action);
}
		
/*******************************************************************************
*                           	  Class Action					               *
*******************************************************************************/
		
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
	assertThat(Properties::objectOfAction, concept);
}
void Action::recipient(const Object& object) {
	assertThat(Properties::recipientOfAction, object);
}

}
