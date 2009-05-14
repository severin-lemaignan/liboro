//
// C++ Interface: oro_exceptions
//
// Description: 
//
//
// Author: Severin Lemaignan,B69,7844 <slemaign@goedel>, (C) 2009
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef ORO_EXCEPTIONS_H_
#define ORO_EXCEPTIONS_H_

#include <stdexcept>

namespace oro {

/*********************************************************
 *                      Exceptions                       *
 *********************************************************/

/**
 * Generic exception for everything related to the handling of ontologies. Does not include errors related to the server itself.
 */
class OntologyException : public std::runtime_error {
	public:
		OntologyException() : std::runtime_error("An exception occured related to the ontology.") { }
		OntologyException(const char* msg) : std::runtime_error(msg) { }
};

/**
 * Generic exception for every server-related exceptions. Does not include errors related to the ontology reasonning.
 */
class OntologyServerException : public std::runtime_error {
	public:
		OntologyServerException() : std::runtime_error("OntologyServerException") { }
		OntologyServerException(const char* msg) : std::runtime_error(msg) { }
};

/**
 * Generic exception when the ontology loose its consistency (<=> semantic error).
 */
class OntologySemanticException : public OntologyException {
	public:
		OntologySemanticException() : OntologyException("An consistency exception occured on the ontology.") { }
		OntologySemanticException(const char* msg) : OntologyException(msg) { }
};

/**
 * Thrown if two statements are to be asserted for the same (subject, predicate) couple when the predicate is functional.
 */
class FunctionalPredicateException : public OntologySemanticException {
	public:
		FunctionalPredicateException() : OntologySemanticException("Error: two statements involving the same functional predicate!") { }
		FunctionalPredicateException(const char* msg) : OntologySemanticException(msg) { }
};

/**
 * Thrown if an individual is set to have two different types (classes) which are disjoint.
 */
class DisjointTypesException : public OntologySemanticException {
	public:
		DisjointTypesException() : OntologySemanticException("An individual may not have two disjoint types.") { }
		DisjointTypesException(const char* msg) : OntologySemanticException(msg) { }
};

/**
 * Thrown by the method Concept::is() if the requested property is not with a boolean range.
 */
class NonBooleanDatatypePropertyException : public OntologyException {
	public:
		NonBooleanDatatypePropertyException() : OntologyException("The datatype property invoked from a Concept.is(property) is not boolean.") { }
		NonBooleanDatatypePropertyException(const char* msg) : OntologyException(msg) { }
};

/**
 * Thrown if ontology is accessed but not yet initialized.
 */
class UninitializedOntologyException : public OntologyException {
	public:
		UninitializedOntologyException() : OntologyException("Ontology not yet initialized! you must specify a port name and a server name!") { }
		UninitializedOntologyException(const char* msg) : OntologyException(msg) { }
};

/**
 * Thrown when a resource is referenced but not found (a wrong class or property name, for instance).
 */
class ResourceNotFoundOntologyException : public OntologyException {
	public:
		ResourceNotFoundOntologyException() : OntologyException("A resource was not found. Wrong class or property name?") { }
		ResourceNotFoundOntologyException(const char* msg) : OntologyException(msg) { }
};

/**
 * Thrown when a statement is not valid (for instance, contain only 2 tokens).
 */
class InvalidStatementException : public OntologyException {
	public:
		InvalidStatementException() : OntologyException("A statement must contain precisely 3 tokens (the subject, the predicate and the object)") { }
		InvalidStatementException(const char* msg) : OntologyException(msg) { }
};

}

#endif /* ORO_EXCEPTIONS_H_ */