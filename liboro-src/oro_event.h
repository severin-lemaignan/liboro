
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

#ifndef ORO_EVENT_H_
#define ORO_EVENT_H_

#include <string>
#include <set>
#include <boost/variant.hpp>

#define PERMANENT_EVENT true
#define ONE_SHOT_EVENT false

namespace oro {

class Concept;

/**
 * A null-type, used to assert that an event didn't return any content.
 */
struct Null {};

/**
 * The list of all possible return types for an event.
 * This could be extended in the future.
 */
typedef boost::variant<	Null,
						std::set<Concept>
						 > event_content_types;

/** Constants that defines the type of event that is to be registered.
 *
 * <ul>
 * <li>FACT_CHECKING 
 * The pattern to watch must be a {@linkplain laas.openrobots.ontology.PartialStatement partial statement}.
 * If, when evaluated, it returns true (ie, at least one asserted or inferred 
 * statement match the pattern), the event is fired.
 * </li> 
 * <li>NEW_INSTANCE * 
 * The event pattern for this type of event is first a variable and then a 
 * set of {@linkplain laas.openrobots.ontology.PartialStatement partial statement}. 
 * The event is triggered when a new statement matches this set.
 * 
 * The server return the list of instance bound to the variable.
 * </li>
 * <li>NEW_CLASS_INSTANCE
 * The event is triggered when a new instance of a given class appear in the 
 * ontology server.
 * </li>
 * 
 * </ul>
 */
//When adding new event type, remember to update as well the implementation of Ontology::subscribe
enum EventType {FACT_CHECKING, NEW_CLASS_INSTANCE, NEW_INSTANCE};

/** Constants that defines the way an event occuring in the ontology is triggered.\n
* 
* <ul>
*  <li>\p ON_TRUE : the event is triggered each time the corresponding watch 
* expression <em>becomes</em> true.</li>
*  <li>\p ON_TRUE_ONE_SHOT : the event is triggered the first time the 
* corresponding watch expression <em>becomes</em> true. The watcher is then 
* deleted.</li>
*  <li>\p ON_FALSE : the event is triggered each time the corresponding watch 
* expression <em>becomes</em> false.</li>
*  <li>\p ON_FALSE_ONE_SHOT : the event is triggered the first time the 
* corresponding watch expression <em>becomes</em> false. The watcher is then 
* deleted.</li>
*  <li>\p ON_TOGGLE : the event is triggered each time the corresponding watch 
* expression <em>becomes</em> true or false.</li>
* </ul>
* 
* \see subscribe(const std::string&, EventTriggeringType, const std::string&)
*/
//When adding new trigger type, remember to update as well the implementation of Ontology::subscribe
enum EventTriggeringType {	ON_TRUE, 
							ON_TRUE_ONE_SHOT, 
							ON_FALSE, 
							ON_FALSE_ONE_SHOT, 
							ON_TOGGLE};

/**
 * This is an "event" object, that is generated and sent to the event subscriber
 * when an event occurs.
 */
struct OroEvent {
	
	std::string eventId;
	
	/**
	 * The content of the event.
	 */
	event_content_types content;
	
	OroEvent(const std::string& eventId, const event_content_types& content) : 
				eventId(eventId), 
				content(content) {};

};

/**
 * Fonctor interface that receives events (ie, event callback).
 * 
 * When an event is triggered, OroEventObserver(evt) is called with the current 
 * event. Implementations of OroEventObserver should provide the desired handling
 * of the event.
 */
class OroEventObserver {

public:
	virtual void operator()(const OroEvent& evt) = 0;

};

}
#endif /* ORO_EVENT_H_ */
