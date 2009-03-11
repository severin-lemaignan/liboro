/*
 * ©LAAS-CNRS (2008-2009)
 *
 * contributor(s) : Séverin Lemaignan <severin.lemaignan@laas.fr>
 *
 * This software is a computer program whose purpose is to interface
 * with an ontology in a robotics context.
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

#ifndef ORO_H_
#define ORO_H_

#include <vector>
#include <string>
#include <iostream>

#include <boost/logic/tribool.hpp>

#include "oro_exceptions.h"
#include "oro_connector.h"

//TODO To be replaced with a proper lib
#include "position.h"

namespace oro {

class Concept;
class Statement;

/**
 * This represent the ontology itself. This class offers tools to look for concept, etc.
 */
class Ontology {

	public:
		
		/**
	 	* This static getter for the ontology must be called once to initialize the ontology server.
	 	* After, the ontology instance can be accessed by a simple Oro::getInstance();
		 */
		static Ontology* createWithConnector(IConnector& connector);

		/**
		 * Return a pointer to the current instance of the ontology server.
		 * The server must be initialized by Oro::getInstance(local_port_name, oro_distant_server);
		 *
		 * TODO Throws UninitializedOntologyException if called before initialization.
		*/
		static Ontology* getInstance();
		
		
		/**
		 * Adds a new statement to the ontology.\n
		 * Interface to \p laas.openrobots.ontology.OpenRobotsOntology#add(String) . Please follow the link for details.\n
		 *
		* Example:
		 * \code
		 * #include "liboro.h"
		 *
		 * using namespace std;
		 * using namespace oro;
		 * int main(void) {
		 *
		 * 		IConnector oro("myDevice", "oro");
		 *
		 * 		oro.add("gorilla rdf:type Monkey");
		 * 		oro.add("gorilla age 12^^xsd:int");
		 * 		oro.add("gorilla weight 75.2");
		 *
		 * 		return 0;
		 * }
		 * \endcode
		 */
		void add(const Statement& statement);
	
		/**
		 * Adds a set of statements to the ontology.
		 * Like \p add(String) but for sets of statements.\n
		 *
		* Example:
		 *
		 * \code
		 * #include "liboro.h";
		 *
		 * using namespace std;
		 * using namespace oro;
		 * int main(void) {
		 *
		 * 		vector<string> stmts;
		 *
		 * 		IConnector oro("myDevice", "oro");
		 *
		 * 		stmts.push_back("gorilla rdf:type Monkey");
		 * 		stmts.push_back("gorilla age 12^^xsd:int");
		 * 		stmts.push_back("gorilla weight 75.2");
		 *
		 * 		oro.add(stmts);
		 *
		 * 		return 0;
		 * }
		 * \endcode
		 */
		void add(const std::vector<Statement>& statements);
	
		/**
		 * Checks the ontology consistency.
		 * 
		 * \return \p true or \p false
		 */
		bool checkConsistency();		
		
		/**
		 * Tries to identify a resource given a set of partially defined statements plus restrictions about this resource.
		 * Wrapper for to {@link laas.openrobots.ontology.OpenRobotsOntology#find(String, Vector, Vector)}. Please follow the link for details.<br/>
		 *
		 * Code snippet:
		 *
		 * \code
		 * #include &quot;liboro.h&quot;
		 *
		 * using namespace std;
		 * using namespace openrobots;
		 * int main(void) {
		 * 		vector&lt;Concept&gt; result;
		 * 		vector&lt;string&gt; partial_stmts;
		 * 		vector&lt;string&gt; filters;
		 *
		 * 		Oro oro(&quot;myDevice&quot;, &quot;oro&quot;);
		 *
		 * 		partial_stmts.push_back("?mysterious rdf:type oro:Monkey");
		 * 		partial_stmts.push_back("?mysterious oro:weight ?value");
		 *
		 * 		filters.push_back("?value >= 50");
		 *
		 * 		oro.find(&quot;mysterious&quot;, partial_stmts, filters, result);
		 * 		return 0;
		 * }
		 * \endcode
 		*/
		int find(const std::string& resource, const std::vector<std::string>& partial_statements, const std::vector<std::string>& restrictions, std::vector<Concept>& result);

		/**
		 * Tries to identify a resource given a set of partially defined statements about this resource.
		 * Wrapper for to {@link laas.openrobots.ontology.OpenRobotsOntology#find(String, Vector)}. Please follow the link for details.<br/>
		 *
 		 * Code snippet:
		 *
		 * \code
		 * #include &quot;liboro.h&quot;
		 *
		 * using namespace std;
		 * using namespace openrobots;
		 * int main(void) {
		 * 		vector&lt;Concept&gt; result;
		 * 		vector&lt;string&gt; partial_stmts;
		 *
		 * 		Oro oro(&quot;myDevice&quot;, &quot;oro&quot;);
		 *
		 * 		partial_stmts.push_back("?mysterious oro:eats oro:banana_tree");
		 * 		partial_stmts.push_back("?mysterious oro:isFemale true^^xsd:boolean");
		 *
		 * 		oro.find(&quot;mysterious&quot;, partial_stmts, result);
		 * 		return 0;
		 * }
		 * \endcode
		 */
		int find(const std::string& resource, const std::vector<std::string>& partial_statements, std::vector<Concept>& result);

		int find(const std::string& resource, const std::string& partial_statement, std::vector<Concept>& result);


		/**
		 * Tries to approximately identify an individual given a set of known
		 * statements about this resource.<br/>
		 * Wrapper for
		 * {@link laas.openrobots.ontology.OpenRobotsOntology#guess(String, Vector, double)}
		 * . Please follow the link for details.<br/>
		 *
		 * Code snippet:
		 *
		 * \code
		 * #include &quot;liboro.h&quot;
		 *
		 * using namespace std;
		 * using namespace openrobots;
		 * int main(void) {
		 * 		vector&lt;string&gt; result;
		 * 		vector&lt;string&gt; partial_stmts;
		 *
		 * 		Oro oro(&quot;myDevice&quot;, &quot;oro&quot;);
		 *
		 * 		partial_stmts.push_back(&quot;?mysterious age \&quot;40\&quot;&circ;&circ;xsd:int&quot;);
		 * 		partial_stmts.push_back(&quot;?mysterious weight \&quot;60\&quot;&circ;&circ;xsd:double&quot;);
		 *
		 * 		oro.guess(&quot;mysterious&quot;, 0.8, partial_stmts, result);
		 * 		return 0;
		 * }
		 * \endcode
		 */
		int guess(const std::string& resource, const double threshold, const std::vector<std::string>& partial_statements, std::vector<std::string>& result);

		/**
		 * Performs a SPARQL query on the OpenRobots ontology.<br/>
		 * Wrapper for
		 * {@link laas.openrobots.ontology.OpenRobotsOntology#query(String)}. Please
		 * follow the link for details.<br/>
		 * This method can only have one variable to select. See
		 * {@link #queryAsXML(Value)} to select several variables.<br/>
		 *
 		 * Code snippet:
		 *
		 * \code
		 * #include &quot;liboro.h&quot;
		 *
		 * using namespace std;
		 * using namespace openrobots;
		 * int main(void) {
		 * 		vector&lt;string&gt; result;
		 * 		Oro oro(&quot;myDevice&quot;, &quot;oro&quot;);
		 * 		oro.query(&quot;instances&quot;, &quot;SELECT ?instances \n WHERE { \n ?instances rdf:type owl:Thing}\n&quot;, result);
		 * 		return 0;
		 * }
		 * \endcode
 		*/
		int query(const std::string& var_name, const std::string& query, std::vector<std::string>& result);
		
		
		/**
		* Generate a new random id which can be used to name new objects. Attention! no check for collision!
		*
		* @parameter length the length of the id. Default is 8 characters.
		*/
		static std::string newId(int length = 8);
		
		void remove(const std::vector<Statement>& statements);
				
		void remove(const Statement& statement);
		
	protected:
		IConnector& _connector;
		Ontology(IConnector& connector);
	private:
		static Ontology* _instance;
		
	
};

/**
 * This represent a class of the OpenRobots ontology.
 * Strictly speaking (and as assumed in OWL Full for instance), a class is a kind of concept. However, since we stay in OWL DL, for simplicity and clarity, classes and concepts won't overlap in this API.
 */
class Class {
	public:
		
		/**
		 * Create a metaclass instance (ie, a class) from its literal name, in the default namespace (oro namespace).
		 * \param name the literal name of the class.
		 * \throw ResourceNotFoundOntologyException when the name can not be matched to a class name defined in the ontology.
		 */
		Class(const std::string& name);
		
		std::string name() const {return _name;}
		
		/**
		 * Return a computer-friendly string describing the class.
		 */
		std::string to_string() const {return _name;}
		
		/**
		 * Print, in a computer-friendly way, the class.
		 */
		friend std::ostream& operator<<(std::ostream& stream, const Class& c){
			stream<<c.name();
			return stream;
		}
		
		static Class& Nothing;
		static Class& Thing;
		
	protected:
		std::string _name;
};

/**
 * This represent a property (or predicate) of the OpenRobots ontology.
 */
class Property {
	public:
		/**
		 * Create a metaproperty instance (ie, a property) from its literal name, in the default namespace (oro namespace).
		 * \param name the literal name of the property.
		 * \throw ResourceNotFoundOntologyException when the name can not be matched to a property name defined in the ontology.
		 */
		Property(const std::string& name);
		
		std::string name() const {return _name;}
		
		/**
		 * Print, in a computer-friendly way, the property.
		 */
		std::string to_string() const {return _name;}
		
		/**
		 * Print, in a computer-friendly way, the property.
		 */
		friend std::ostream& operator<<(std::ostream& stream,const Property& p){
			stream<<p.name();
			return stream;
		}
		
	protected:
		std::string _name;
};

/**
 * This represent a concept (an instance or an individual in OWL terminology) of the OpenRobots ontology.
 */
class Concept {
	public:
		Concept();
		Concept(const std::string& id);
		
		//Creates and returns a copy of the newly created concept.
		template<class T> static T create(const std::string& label) {
			T concept;
			concept.setLabel(label);

			return concept;
		}
		
		template<class T> static T create(const std::string& label, const Class& type){
			T concept;
			concept.setLabel(label);
			concept.setType(type);

			return concept;
		}
		
		template<class T> static T create(const Class& type){
			T concept;
			concept.setType(type);

			return concept;
		}
		
		/**
		 * This is a special member of the Concept class representing the semantic of the "nothing" concept. It's the unique, virtual, instance of the class Nothing.
		 * \see Class::Nothing
		 */
		static const Concept nothing;

		/**
		 * Adds a new assertion regarding the current concept to the ontology.\n
		 * It adds the triple \p [this_object] \p [the_predicate] \p [the_value] to the ontology.
		 * 
		 * \param predicate the predicate of the statement.
		 * \param value the literal representation of the object of the statement.
		 * \throw ResourceNotFoundOntologyException when the predicate name can not be matched to a property name defined in the ontology.
		 * \throw OntologySemanticException if the addition of a statement causes the ontology to become inconsistent.
		 * \see remove
		 */
		void assert(const Property& predicate, const std::string& value);
		
		/**
		 * Adds a new assertion regarding the current concept to the ontology.\n
		 * It adds the triple \p [this_object] \p [the_predicate] \p [the_value] to the ontology.
		 * 
		 * \param predicate the predicate of the statement.
		 * \param value the object of the statement.
		 * \throw ResourceNotFoundOntologyException when the predicate name can not be matched to a property name defined in the ontology.
		 * \throw OntologySemanticException if the addition of a statement causes the ontology to become inconsistent.
		 * \see remove
		 */
		void assert(const Property& predicate, const Concept& value);

		/**
		 * Removes all the assertions in the ontology which have the current concept as subject and the given property as predicate.\n Assertions with subproperties as predicate are not removed.
		 * \param predicate the property whose assertions are to be removed.
				 */
		void clear(const Property& predicate);
		
		/**
		 * Returns whether the object has or not some property, ie at least one assertion involves the current concept as subject, and the property (or any subproperty) you want to check as predicate.\n
		 * For instance,
		 * \code
		 * OroConcept robot = OroConcept.createWithType("Robot");
		 * robot.has(Property("isAt"));
		 * \endcode
		 * 
		 * \param predicate the property you want to check.
		 * \return \p true or \p false if it's possible to infer the answer, \p indeterminate in other cases.
		 */
		boost::logic::tribool has(const Property& predicate) const;
		boost::logic::tribool has(const Property& predicate, const std::string& value) const;
		boost::logic::tribool has(const Property& predicate, const Concept& value) const;

		/**
		 * Returns the ID of the concept. Beware: two different ID may refer to the same actual concept (OWL doesn't rely on the Unique Name Assumption).
		 * \return the ID of the concept.
		 */
		std::string id() const;

		boost::logic::tribool is(const Property& boolDataproperty) const;

		/**
		 * Checks the type (ie class) of a concept.
		 * \param type the class you want to check the concept against.
		 * \return \p true or \p false if it's possible to infer the answer, \p indeterminate in other cases.
		 */
		boost::logic::tribool isA(const Class& type) const {return hasType(type);}
		
		/**
		 * Alias for \p isA . 
		 * \param type the class you want to check the concept against.
		 * \return \p true or \p false if it's possible to infer the answer, \p indeterminate in other cases.
		 * \see{isA}
		*/
		boost::logic::tribool hasType(const Class& type) const;
		
		/**
		 * Set the class (or type) of a concept. If the class was previously already defined, this definition will be added.\n
		 * This method adds this assertion: \p [this_object] \p rdf:type \p [the_class] to the ontology.
		 * 
		 * \param type the class you want to check the concept against.
		 * \return \p true or \p false if it's possible to infer the answer, \p indeterminate in other cases.
		 */
		void setType(const Class& type);

		/**
		 * Sets a human-readable label for this concept.\n
		 * This method sets the \p rdfs:label annotation for this concept.
		 * 
		 * \param label the human-readable name of the concept, in plain English.
		 */
		void setLabel(const std::string& label);
		
		/**
		 * Returns the human-readable name of the concept.
		 * 
		 * \return the human-readable form of the concept name, or an empty string if no label has been defined.
		 */
		std::string label() const;

		void remove(const Property& predicate, const std::string& value);		
		void remove(const Property& predicate, const Concept& value);
		

		/**
		 * Returns a computer-friendly string describing the concept.
		 */
		std::string to_string() const {return id();}
		
		/**
		 * Print, in a computer-friendly way, the concept.
		 */
		friend std::ostream& operator<<(std::ostream& stream,const Concept& c){
			stream<<c._id;
			return stream;
		}

	protected:
		std::string _id;
		std::string _label;
		

};


/**
 * This represent an object (ie an instance of the "Object" class) of the OpenRobots ontology. An object is a spacially localized and at least partially tangible kind of thing.\n
 * This class provides common, higher-level functionnalities over the base class \p Concept .
 */
class Object : public Concept {
	public:		
		boost::logic::tribool hasAbsolutePosition();
		
		/**
		 * Returns a concept which represent the location of the current object, as far as it can be inderred from the ontology.\n
		 * If no position has been asserted or can be inferred, \p Concept::nothing is returned.
		 * 
		 * \see Concept::nothing
		 */
		Concept hasPosition();
};

//Forward declaration
class Action;

/**
 * This represent an agent (ie an instance of the "Agent" class) of the OpenRobots ontology. An agent is a special kind of object, endowed with some kind of intelligence. In the OpenRobots ontology, agents are either humans or robots.\n
 * This class provides common, higher-level functionnalities over the base classes \p Concept and \p Object .
 */
class Agent : public Object {
	public:
		void desires(const Action& action);
		void currentlyPerforms(const Action& action);
};

/**
 * This represent an action (ie an instance of the "Action" class) of the OpenRobots ontology. An action is a special kind of event, carried out by an agent.\n
 * This class provides common, higher-level functionnalities over the base class \p Concept .
 */
class Action : public Concept {
	public:
		void object(const Concept& concept);
		void recipient(const Object& concept);
};

class Statement {
	public:
		const Concept& subject;
		const Property& predicate;
		
		const Concept& object;
		const std::string literal_object;
		
		bool isObjectLiteral;
		
		
		Statement(const Concept& subject, const Property& predicate, const Concept& object);
		Statement(const Concept& subject, const Property& predicate, const std::string& object);
		
		/**
		 * Returns a computer-friendly string describing the concept.
		*/
		std::string to_string() const;
		
		/**
		 * Print, in a computer-friendly way, the statement.
		 */
		friend std::ostream& operator<<(std::ostream& stream,const Statement& stmt){
			stream<<stmt.subject<<" "<<stmt.predicate<<" " << (stmt.isObjectLiteral?stmt.literal_object:stmt.object);
			return stream;
		}

		
};
}

#endif /* ORO_H_ */