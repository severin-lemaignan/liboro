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

#include <sstream>
#include "oro.h"
#include "oro_library.h"

using namespace boost::logic;
using namespace std;

namespace oro {

/*******************************************************************************
*                       	  Class Concept					                   *
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
