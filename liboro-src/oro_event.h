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

#ifndef ORO_EVENT_H_
#define ORO_EVENT_H_

#include <string>

namespace oro {

/** Constants that defines the type of event that is to be registered.
 *
 * <ul>
 * <li>FACT_CHECKING 
 * The watch pattern must be a {@linkplain laas.openrobots.ontology.PartialStatement partial statement}.
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
 * The event is triggered when a new instance of the class returned by the 
 * watch pattern is added.
 * </li>
 * 
 * </ul>
 */
//When adding new event type, remember to update as well the implementation of Ontology::subscribe
enum EventType {FACT_CHECKING, NEW_CLASS_INSTANCE, NEW_INSTANCE};

/** Constants that defines the way an event occuring in the ontology is triggered.\n
* 
* <ul>
*  <li>\p ON_TRUE : the event is triggered each time the corresponding watch expression <em>becomes</em> true.</li>
*  <li>\p ON_TRUE_ONE_SHOT : the event is triggered the first time the corresponding watch expression <em>becomes</em> true. The watcher is then deleted.</li>
*  <li>\p ON_FALSE : the event is triggered each time the corresponding watch expression <em>becomes</em> false.</li>
*  <li>\p ON_FALSE_ONE_SHOT : the event is triggered the first time the corresponding watch expression <em>becomes</em> false. The watcher is then deleted.</li>
*  <li>\p ON_TOGGLE : the event is triggered each time the corresponding watch expression <em>becomes</em> true or false.</li>
* </ul>
* 
* \see subscribe(const std::string&, EventTriggeringType, const std::string&)
*/
//When adding new trigger type, remember to update as well the implementation of Ontology::subscribe
enum EventTriggeringType {ON_TRUE, ON_TRUE_ONE_SHOT, ON_FALSE, ON_FALSE_ONE_SHOT, ON_TOGGLE};

struct OroEvent {
	
	std::string eventId;
	std::string result;
	
	OroEvent(std::string eventId, std::string result) : eventId(eventId), result(result) {};
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
   virtual void operator()(const OroEvent& evt) const = 0;
};

}
#endif /* ORO_EVENT_H_ */
