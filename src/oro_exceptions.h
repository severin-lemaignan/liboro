
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

/** \file
 * This header defines some C++ exceptions specific to \p liboro .
 */

#ifndef ORO_EXCEPTIONS_H_
#define ORO_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

namespace oro {

    /** The string identifying a "NotFoundException" as emitted by the ontology server.
     */
    static const std::string SERVER_NOTFOUND_EXCEPTION = "com.hp.hpl.jena.shared.NotFoundException";

    /** The string identifying a "QueryParseException" as emitted by the ontology server.
     */
    static const std::string SERVER_QUERYPARSE_EXCEPTION = "com.hp.hpl.jena.query.QueryParseException";

    /** The string identifying a "ConnectorException", as emitted by liboro.
     */
    static const std::string CONNECTOR_EXCEPTION = "ConnectorException";

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
        OntologyException(const std::string& msg) : std::runtime_error(msg.c_str()) { }
};

/**
 * Generic exception for every server-related exceptions. Does not include errors related to the ontology reasonning.
 */
class OntologyServerException : public std::runtime_error {
    public:
        OntologyServerException() : std::runtime_error("OntologyServerException") { }
        OntologyServerException(const char* msg) : std::runtime_error(msg) { }
        OntologyServerException(const std::string& msg) : std::runtime_error(msg.c_str()) { }
};

/**
 * Exception for every conenctor-related problems.
 */
class ConnectorException : public std::runtime_error {
    public:
        ConnectorException() : std::runtime_error("ConnectorException") { }
        ConnectorException(const char* msg) : std::runtime_error(msg) { }
        ConnectorException(const std::string& msg) : std::runtime_error(msg.c_str()) { }
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
        ResourceNotFoundOntologyException(const std::string& msg) : OntologyException(msg.c_str()) { }
};

/**
 * Thrown when a query (currently, SPARQL) is not valid (not parsable -> syntax error).
 */
class InvalidQueryException : public OntologyException {
    public:
        InvalidQueryException() : OntologyException("Invalid query. Check your syntax!") { }
        InvalidQueryException(const char* msg) : OntologyException(msg) { }
        InvalidQueryException(const std::string& msg) : OntologyException(msg.c_str()) { }
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
