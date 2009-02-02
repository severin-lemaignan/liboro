/*
 * concept.cpp
 *
 *  Created on: 2 févr. 2009
 *      Author: slemaign
 */

#include "liboro.h"

using namespace yarp::os;
using namespace std;

namespace openrobots {

//		Concept::Concept(){
//			oro = *Oro::getInstance();
//		}
//		Concept Concept::createFromId(std::string src_id){
//			id = src_id;
//		}
		Concept Concept::create(std::string label){

		}
		Concept Concept::create(std::string label, std::string type){

		}
		Concept Concept::createWithType(std::string type){

		}

		void Concept::assert(std::string predicate, std::string value){

		}
		void Concept::assert(std::string predicate, Concept value){

		}

		void Concept::clear(std::string predicate){

		}

		boost::logic::tribool Concept::has(std::string predicate, std::string value){

		}
		boost::logic::tribool Concept::has(std::string predicate, Concept value){

		}

		std::string Concept::id(){

		}

		boost::logic::tribool Concept::is(std::string boolDatatype){

		}

		boost::logic::tribool Concept::hasType(std::string type){

		}
		void Concept::setType(std::string type){

		}

		void Concept::setLabel(std::string label){

		}
		std::string Concept::label(){

		}

		void Concept::remove(std::string predicate, std::string value){

		}
		void Concept::remove(std::string predicate, Concept value){

		}

}
