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

		static const Property belongsTo;

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
		* (currentlyBodilyDoes AGENT ACTION) means that the (embodied) AGENT is
		* currently performing a non-intentional action ACTION.
		*/
		static const Property currentlyBodilyDoes;

		static const Property currentlyDoes;

		/**
		* Denotes that the DOES of the EVENT is currently doing it. It's a
		* temporaly bound version of doneBy.
		*/
		static const Property currentlyDoneBy;

		/**
		* (currentlyPerforms AGENT ACTION) means that the AGENT is currently
		* performing the intentional ACTION.
		*/
		static const Property currentlyPerforms;

		static const Property desires;

		static const Property duration;

		static const Property endingPoint;

		/**
		* describes a agent that currently *is* in a specific (static) situation
		*/
		static const Property experience;

		/**
		* An agent is said to focus on an object if and only if it looksAt it
		* AND pointsAt it.
		*/
		static const Property focusesOn;

		static const Property hasBodyPart;

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

		static const Property hasInHand;

		static const Property hasInLeftHand;

		static const Property hasInRightHand;

		static const Property hasPosture;

		static const Property hasRelativePosition;

		static const Property hasShape;

		static const Property hasThematicRoleRelationWith;

		static const Property holds;

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

		static const Property isAbove;

		static const Property isAt;

		static const Property isBelow;

		static const Property isExperienced;

		static const Property isFocusOf;

		static const Property isIn;

		/**
		* describe the symbolic position on an object relative to 'myself'.
		* Values can be:
		* {NEAR_FAR}_{FRONT LEFT RIGHT BACK}
		*/
		static const Property isLocated;

		static const Property isNextTo;

		static const Property isOn;

		static const Property leftOf;

		/**
		* Describes the status of an agent actively looking at an object
		* ('sees' doesn't imply the agent is actively looking at the object,
		* but only that the object is in the field of view of the agent).
		*/
		static const Property looksAt;

		static const Property mainColorOfObject;

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

		static const Property placeOf;

		/**
		* Describes the status of an agent physically pointing at a physical
		* location with a hand.
		*/
		static const Property pointsAt;

		/**
		* binds an action to a state of the world (a Situation) which is
		* desired as a result (at short or long term) of this action.
		*/
		static const Property purposeOfAction;

		/**
		* This property represent the "active form" of the "isReachable"
		* predicate.
		* ?o isReachable true <=> myself reaches ?o
		* 
		* It is meant to be only use through rule inference, and is added to
		* this "ROMAN" scenario specific ontology to ensure fluid interaction
		* with the Natural Language module.
		*/
		static const Property reaches;

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
		* Indicates if an enduring thing-localized is directly (ie, without
		* moving) visible for the agent (myself).
		*/
		static const Property isInFieldOfView;

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

		static const Property isSitting;

		static const Property isTraversable;

		/**
		* Indicates if an enduring thing-localized is visible (modulo a head
		* movement) for the agent (myself).
		* 
		* This means that the object may not be currently in the field of view
		* of the agent, but only requires the agent to turn the head to see it.
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
		* An "active concept" is a concept (class or instance) which is
		* considered by the robot to be active at a given time.
		* 
		* "Active" may have several meaning, like "relevant in the current
		* context" or "recently accessed".
		*/
		static const Class ActiveConcept;

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

		static const Class AudioTape;

		static const Class BodyPart;

		static const Class Book;

		static const Class Bottle;

		static const Class Box;

		static const Class Cardboardbox;

		static const Class Ceilling;

		static const Class Chair;

		static const Class Cloth;

		static const Class Color;

		/**
		* The collection of tangible objects whose primary function is to
		* contain something.
		*/
		static const Class Container;

		static const Class ContinuousTimeInterval;

		static const Class Cover;

		static const Class Cup;

		/**
		* An instance of EligibleAction is an action whose pre-conditions are
		* currently fullfit, thus could possible be started.
		*/
		static const Class EligibleAction;

		static const Class EmbodiedAgent;

		static const Class Eyes;

		static const Class Floor;

		static const Class Furniture;

		/**
		* Artifact used to play with. One notable subclass is 'Toy'.
		*/
		static const Class GameArtifact;

		static const Class Give;

		static const Class Glass;

		static const Class GraspableObject;

		static const Class Hand;

		static const Class Head;

		static const Class Human;

		static const Class Jacket;

		/**
		* A joint attention situation is a situation where an object is the
		* focus of attention of more than one agent.
		*/
		static const Class JointAttentionSituation;

		static const Class LeftHand;

		/**
		* Location is use in its very broad meaning of "a spatial-localized
		* thing". Hence an object is a location, an agent is a location, etc.
		*/
		static const Class Location;

		static const Class Manipulation;

		static const Class Object;

		static const Class Obstacle;

		static const Class Opening;

		/**
		* A PhysicalSupport is a special kind of tangible object which main
		* feature is to hold other tangible objects. It includes walls, floors,
		* tables, shelves...
		*/
		static const Class PhysicalSupport;

		static const Class Pick;

		/**
		* A specialization of EnduringThing_Localized (q.v). Each instance of
		* Place is a spatial thing which has a relatively permanent location.
		*/
		static const Class Place;

		static const Class Placemat;

		static const Class Plan;

		/**
		* A specialization of both Point and SpaceRegion_Empirical (qq.v.).
		* Each instance of Point_Empirical is a zero-dimensional object that
		* belongs to, and thus has a fixed location in, the embedding space of
		* the empirical universe (see #$TheSpatialUniverse-Empirical).
		* 
		* Examples include the location of the center of mass of the Milky Way
		* galaxy at the beginning of the 20th Century. Note that empirical
		* space points are embedded in time; if time is not a significant
		* parameter (i.e. if in an atemporal or temporally agnostic context)
		* consider using Point instead.
		*/
		static const Class Point;

		static const Class Posture;

		static const Class Put;

		static const Class Release;

		static const Class Rest;

		static const Class RightHand;

		static const Class Robot;

		static const Class Shape;

		static const Class Shelf;

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

		static const Class Table;

		static const Class Tableware;

		static const Class Tape;

		/**
		* An action considered in the specific context of robotics.
		*/
		static const Class Task;

		static const Class TimeInterval;

		static const Class TimePoint;

		/**
		* A specialization of box used to carry tools.
		*/
		static const Class Toolbox;

		static const Class Torso;

		static const Class Toy;

		/**
		* A cube, traditionnaly made of wood, meant for children to play with.
		*/
		static const Class ToyCube;

		static const Class Trashbin;

		static const Class Uncover;

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
