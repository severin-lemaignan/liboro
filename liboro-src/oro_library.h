/** \file
 * This header defines a "library" of C++ properties and classes (or concepts)
 * matching the ones defines in the commonsense.oro.owl ontology.\n
 * It has been automatically generated from commonsense.oro.owl by the 
 * "generate_oro_library_from_ontology.py" Python script.
 * If you need to regenerate this file (for your own ontology or after a 
 * modification of the ontology), just run 
 * <tt>./generate_oro_library_from_ontology.py > src/oro_library.h</tt>
 */
#ifndef ORO_LIBRARY_H_
#define ORO_LIBRARY_H_

#include "oro.h"

namespace oro {

/** This class lists all the OWL properties defined in the commonsense.oro.owl
 * ontology.\n
 * It provides handy shortcuts when asserting new facts on concepts.
 * \see Concept::assert(const Property& predicate, const Concept& value)
 */
class Properties {
	public:

// Object properties

		static const Property actionInvolves;

		/**
		* Theme (or Patient) of an action: Entity undergoing the effect of some
		* action
		* 
		* Example: >Mary< fell over.
		*/
		static const Property actsOnObject;

		static const Property after;

		static const Property after_Underspecified;

		/**
		* An "agent relative property" is always relative to the point of view
		* of the agent "myself"
		*/
		static const Property agentRelativeProperty;

		static const Property behind;

		static const Property bringsToStaticSituation;

		static const Property canBePerformedBy;

		static const Property canPerform;

		/**
		* Links an agent to an action it can starts now. When required, this
		* relationship is to be computed by a planner (or inferred from a model
		* of the human).
		*/
		static const Property canUndertakeNow;

		/**
		* (currentlyPerforms AGENT ACTION) means that the AGENT is performing
		* the ACTION, purposefully or not.
		*/
		static const Property currentlyPerforms;

		static const Property desires;

		static const Property duration;

		static const Property endingPoint;

		static const Property hasCharacteristicLocation;

		/**
		* describe the main, significant color of the object.
		*/
		static const Property hasColor;

		static const Property hasFeature;

		/**
		* Goal: Entity towards which something moves.
		* 
		* Example: John went >home<
		*/
		static const Property hasGoal;

		static const Property hasRelativePosition;

		static const Property hasShape;

		static const Property hasThematicRoleRelationWith;

		static const Property holds;

		static const Property holds_Underspecified;

		static const Property inFrontOf;

		static const Property involves;

		/**
		* (isAbleTo AGENT ACTION) says that AGENT it technically/physically
		* able to achieve the ACTION. It does not mean that the agent can do it
		* right now (see canUndertakeNow for this meaning). The ACTION is
		* constrainted to purposeful actions: isAbleTo does not intend to
		* express a fact like "a stone is able to sink into the water".
		*/
		static const Property isAbleTo;

		static const Property isAt;

		static const Property isFarFrom;

		static const Property isIn;

		/**
		* Relates an agent to some object: a "near" object is closer than 1m
		* from the agent.
		*/
		static const Property isNearOf;

		static const Property isNextTo;

		static const Property isOn;

		static const Property isUnder;

		static const Property leftOf;

		static const Property mainColorOfObject;

		/**
		* Links a localizable thing to its location.
		*/
		static const Property objectFoundInLocation;

		/**
		* Binds an action to one or several objects which are targetted by the
		* action.
		* For instance, in the assertion, "I take a cup", "cup" is the
		* ObjectOfAction of "to take".
		* 
		* In the sentence "Jido shows the Eiffel tower to the tourist", the
		* object of action is the Eiffel tower and the receiver is the tourist,
		* as specified by the "receiverOfAction" property.
		*/
		static const Property objectOfAction;

		/**
		* Instigator of some action.
		* 
		* Example: >John< killed Harry.
		*/
		static const Property performedBy;

		/**
		* binds an action to a state of the world (a Situation) which is
		* desired as a result (at short or long term) of this action.
		*/
		static const Property purposeOfAction;

		/**
		* Link an action to the object or agent that is a recipient of the
		* action result.
		* Not to be confused with actionOnObject that denote that the object is
		* the main concern of the action.
		* 
		* Example: John got >Mary< a present.
		*/
		static const Property receivedBy;

		/**
		* When relevant for the considered action, this property specify the
		* agent or object whose state is eventually modified as a *result* of
		* an action. It is not be the object of action itself (specified by the
		* "objectOfAction" property).
		* 
		* For instance, in the sentence "The robot gives the cup to the human",
		* the human is the final recipient of the cup and hence the
		* recipientOfAction. In the sentence "I place the robot on the chair",
		* the chair is the recipient of action while the robot is the object of
		* the action. In the sentence "Jido shows the Eiffel tower to the
		* tourist", the recipient is the tourist and the object of action is
		* the Eiffel tower.
		*/
		static const Property recipientOfAction;

		static const Property rightOf;

		static const Property sees;

		static const Property startingPoint;

		/**
		* A ComplexTemporalPredicate (q.v.) that relates any pair of temporal
		* things such that the first starts after the second ends.
		* (startsAfterEndingOf AFTER BEFORE) means that the time point (see
		* TimePoint) at which AFTER first starts to exist or occur is
		* temporally later than (see after) the time point at which BEFORE
		* finally ceases to exist or occur. Thus it is equivalent to the form
		* (after (StartFn AFTER) (EndFn BEFORE)). That is, the startingPoint of
		* AFTER is later than the endingPoint of BEFORE.
		*/
		static const Property startsAfterEndingOf;

		/**
		* Binds a static situation (a state of the world) to the agents
		* involved in the situation.
		*/
		static const Property staticSituationInvolves;

		static const Property taskOfPlan;

		static const Property temporallyRelated;

// Datatype properties

		/**
		* Indicates if an object can be manipulated (knows a grasping point for
		* the object) by the agent myself. Thus, if the object can be
		* manipulated, it is movable as well.
		*/
		static const Property canBeManipulated;

		/**
		* Characteristic entity dimension, in centimeters
		*/
		static const Property hasCharacteristicDimension;

		/**
		* The characteristic weight of the tangible facet of the object, in
		* grammes
		*/
		static const Property hasCharacteristicWeight;

		/**
		* IsDesired is equivalent to (desires MYSELF PROP). 
		* (desire AGT PROP) means that the Agent_PartiallyTangible AGT desires
		* that the world be as the proposition PROP (represented by a
		* CycLSentence_Assertible) describes it to be. desires is implied by
		* goals, but is weaker: PROP might be some desirable state of affairs
		* that the agent is not actively working, or planning, to make/keep
		* true; e.g., world peace.
		*/
		static const Property isDesired;

		/**
		* True if one of the object primary function is to hold other objects.
		* See the temporalProperty holds-Underspecified for a relationship
		* between a holder and a holded object.
		*/
		static const Property isHolder;

		/**
		* States if a given artifact is movable by someone (not specifying who).
		*/
		static const Property isMovable;

		static const Property isMoving;

		/**
		* True if the object is currently reachable by agent myself.
		* Thus an object can not be said to be generally reachable or not.
		*/
		static const Property isReachable;

		static const Property isTraversable;

		/**
		* Indicates if an enduring thing-localized is visible for the agent
		* (myself).
		*/
		static const Property isVisible;

};

/** This class lists all the OWL concepts defined in the commonsense.oro.owl ontology.\n
 * It provides handy shortcuts when asserting the class of a concept.
 * \see Concept::isA(const Class& type) const
 */
class Classes {
	public:
		/**
		* The most generic concept available.
		*/
		static const Class Thing;

		/**
		* An action implies several thematic roles (or semantic roles):
		* 
		* We use this list of roles (Aarts 1997: 88), which are mapped to
		* specific predicates :
		* Agent: The ‘doer’ or instigator of the action denoted by the
		* predicate.
		* Patient: The ‘undergoer’ of the action or event denoted by the
		* predicate.
		* Theme: The entity that is moved by the action or event denoted by the
		* predicate.
		* Experiencer: The living entity that experiences the action or event
		* denoted by the predicate.
		* Goal: The location or entity in the direction of which something
		* moves.
		* Benefactive: The entity that benefits from the action or event
		* denoted by the predicate.
		* Source: The location or entity from which something moves
		* Instrument: The medium by which the action or event denoted by the
		* predicate is carried out.
		* Locative: The specification of the place where the action or event
		* denoted by the predicate in situated.
		*/
		static const Class Action;

		/**
		* Groups both humans and robots
		*/
		static const Class Agent;

		/**
		* A specialization of InanimateObject. Each instance of Artifact is an
		* at least partially tangible thing which was intentionally created by
		* an Agent_PartiallyTangible (or a group of Agent_PartiallyTangibles
		* working together) to serve some purpose or perform some function. In
		* order to create an instance of Artifact, it is not necessary that an
		* Agent_PartiallyTangible create the matter out of which the Artifact
		* is composed; rather, an Agent_PartiallyTangible can create an
		* instance of Artifact by assembling or modifying existing matter.
		* Examples of Artifacts include a wooden flute that's been whittled
		* from a tree branch, a sawhorse that's been put together out of boards
		* and nails, and a coin that's been minted by embossing or by melting
		* liquid silver into a mold. In addition to the obvious human artifacts
		* (buildings, tools, textiles, power lines), the collection Artifact
		* also includes certain sorts of things made by Animals, such as bird
		* nests, termite mounds, and beaver dams. Artifacts without any
		* tangible parts are excluded from the collection Artifact; they are
		* included in the collection Artifact_Intangible.
		*/
		static const Class Artifact;

		static const Class Book;

		static const Class Bottle;

		static const Class Ceilling;

		static const Class Chair;

		static const Class Color;

		/**
		* The collection of tangible objects whose primary function is to
		* contain something.
		*/
		static const Class Container;

		static const Class ContinuousTimeInterval;

		static const Class Cup;

		/**
		* A specialization of both SpatialThing_Localized and SomethingExisting
		* (qq.v.). Each instance of EnduringThing_Localized is a spatial
		* enduring thing at which an event can occur or a situation can obtain.
		* Positive examples include planets, the borders between countries,
		* human beings, rocks, and atoms. Negative example include situations,
		* events, abstract objects, and regions of space that exclusively act
		* as possible locations for other spatial objects (see SpaceRegion). An
		* important specialization of EnduringThing_Localized is Place (q.v.).
		* The salient distinction between places (instances of Place) and
		* locations (instances of EnduringThing_Localized) is that places are
		* assumed to have relatively permanent locations, whereas locations
		* need not have permanent locations. Thus, from the perspective of
		* someone standing on a beach, the crest of a breaking wave can be a
		* location at which foaming is occuring (thus an
		* EnduringThing_Localized), but it cannot be such a place (i.e. it
		* cannot be an instance of Place).
		*/
		static const Class EnduringThing_Localized;

		static const Class Event;

		static const Class Floor;

		static const Class Furniture;

		/**
		* Artifact used to play with
		*/
		static const Class GameArtifact;

		static const Class Glass;

		static const Class Human;

		/**
		* Location is use in its very broad meaning of "a spatial-localized
		* thing". Hence an object is a location, an agent is a location, etc.
		*/
		static const Class Location;

		static const Class Object;

		static const Class Object_SupportingFurniture;

		static const Class Obstacle;

		static const Class Opening;

		/**
		* A subcollection of EnduringThing_Localized and TemporalThing. Each
		* instance of PartiallyTangible has a tangible (i.e. material) part and
		* a temporal extent (i.e. it exists in time). It might or might not
		* also have an intangible part. For example, a particular copy of a
		* book is made of matter, has temporal extent, and also has an
		* intangible part: the information content of the text markings on its
		* pages.
		*/
		static const Class PartiallyTangible;

		/**
		* A PhysicalSupport is a special kind of tangible object which main
		* feature is to hold other tangible objects. It includes walls, floors,
		* tables, shelves...
		*/
		static const Class PhysicalSupport;

		/**
		* A specialization of EnduringThing_Localized (q.v). Each instance of
		* Place is a spatial thing which has a relatively permanent location.
		*/
		static const Class Place;

		static const Class Plan;

		static const Class Plan_ExpectedSituationType;

		/**
		* For ease of use, we use here "Point" as "Point-Empirical" in the Open
		* Cyc ontology. The OpenCyc URI refers to the correct one
		* ("Point-Empirical")
		*/
		static const Class Point;

		/**
		* A specialization of both Action and AtLeastPartiallyMentalEvent. Each
		* instance of PurposefulAction is an action consciously, volitionally,
		* and purposefully done by (see performedBy) at least one actor.
		*/
		static const Class PurposefulAction;

		static const Class Robot;

		static const Class Shape;

		static const Class Shelf;

		/**
		* Each instance of Situation is a state or event consisting of one or
		* more objects having certain properties or bearing certain relations
		* to each other. Notable specializations of Situation are Event and
		* StaticSituation; it is disjoint with SomethingExisting.
		*/
		static const Class Situation;

		/**
		* A specialization of both SpatialThing and IntangibleIndividual
		* (qq.v.) whose instances are regions of space that exclusively act as
		* possible locations for other spatial objects, and thus are immobile.
		* A space region might be three-, two-, one-, or zero-dimensional; and
		* spatial objects "occupy" such regions accordingly. Three-dimensional
		* space regions (see SpaceChunk) can be occupied by solid objects.
		* Two-dimensional space regions (or Surfaces) can be occupied by a
		* purely two-dimensional objects. And similarly for one-dimensional
		* space regions (Lines) and zero-dimensional space regions (Points).
		* 
		* Another important specialization of SpaceRegion is
		* SpaceRegion_Empirical, whose instances are pieces of the embedding
		* space for spatio-temporal objects (see SpatialThing_Localized).
		* 
		* For more on spatial location and occupancy, see
		* AbsoluteLocationalPredicate and its instances.
		*/
		static const Class SpaceRegion;

		/**
		* A specialization of Individual. The collection of all things that
		* have a spatial extent or location relative to some other SpatialThing
		* or in some embedding space. Note that to say that an entity is a
		* member of this collection is to remain agnostic about two issues.
		* First, a SpatialThing may be PartiallyTangible (e.g. Texas-State) or
		* wholly Intangible (e.g. ArcticCircle or a line mentioned in a
		* geometric theorem). Second, although we do insist on location
		* relative to another spatial thing or in some embedding space, a
		* SpatialThing might or might not be located in the actual physical
		* universe. It is far from clear that all SpatialThings are so located:
		* an ideal platonic circle or a trajectory through the phase space of
		* some physical system (e.g.) might not be. If the intent is to imply
		* location in the empirically observable cosmos, the user should employ
		* this collection's specialization, SpatialThing-Localized.
		*/
		static const Class SpatialThing;

		/**
		* A specialization of both SpatialThing and TemporalThing. This is the
		* collection of all spatial things, tangible or intangible, that can
		* meaningfully be said to have location or position in the empirical
		* universe.
		* 
		* Examples of spatially-localized things include all PartiallyTangible
		* things, such as trees and ships, as well as certain Intangible
		* spatial things, like the EarthsEquator. Also included are those
		* events that can be pinned-down to specific places (see
		* Event-Localized), and thus all PhysicalEvents. Excluded from this
		* collection are any SpatialThings that are not localized, such as --
		* arguably -- purely abstract geometrical figures (e.g. a Platonic
		* sphere).
		* 
		* Note that a fictional or imaginary object (such as Frodo, Captain
		* Queeg, or HAL9000-TheComputer) is typically localized with respect to
		* the universe of the fictional/imagnary context in which it is found,
		* and so is an instance of SpatialThing-Localized with respect to that
		* context.
		*/
		static const Class SpatialThing_Localized;

		/**
		* A specialization of Situation. Each instance of StaticSituation is a
		* state of affairs between two or more things, persisting statically
		* over some time interval. IE, a state of the world.
		*/
		static const Class StaticSituation;

		static const Class Table;

		static const Class Tableware;

		/**
		* An action considered in the specific context of robotics.
		*/
		static const Class Task;

		static const Class TemporalThing;

		static const Class TemporallyExtendedThing;

		static const Class TimeInterval;

		static const Class TimePoint;

		/**
		* A cube, traditionnaly made of wood, meant for children to play with.
		*/
		static const Class ToyCube;

		static const Class Trashbin;

		/**
		* A magnetic video support
		*/
		static const Class VideoTape;

		static const Class VisualMark;

		static const Class Wall;

		static const Class Window;

		static const Class Zone;

		/**
		* Defines the portion of space in which an agent can act. It primary
		* relates to actions like taking something.
		*/
		static const Class ZoneOfAction;

		static const Class ZoneOfInteraction;

		/**
		* A specific region of space which has an interest for some purpose.
		* For instance, a table can have several zone of interests describing
		* parts of the table interessant for the robot (plates, zons in front
		* of chairs,etc.)
		*/
		static const Class ZoneOfInterest;
};


}

#endif /* ORO_LIBRARY_H_ */
