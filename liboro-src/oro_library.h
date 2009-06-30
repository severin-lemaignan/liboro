/** \file
 * This header defines a "library" of C++ properties and classes (or concepts) matching the ones defines in the openrobots.owl ontology.\n
 * It has been automatically generated from openrobots.owl by the "generate_oro_library_from_ontology.py" Python script.
 * If you need to regenerate this file (for your own ontology or after a modification of the ontology), just run <tt>./generate_oro_library_from_ontology.py > src/oro_library.h</tt>
 */
#ifndef ORO_LIBRARY_H_
#define ORO_LIBRARY_H_

#include "oro.h"

namespace oro {

/** This class lists all the OWL properties defined in the openrobots.owl ontology.\n
 * It provides handy shortcuts when asserting new facts on concepts.
 * \see Concept::assert(const Property& predicate, const Concept& value)
 */
class Properties {
	public:

// Object properties

		static const Property actionInvolves;

		static const Property belongsTo;

		static const Property bringsToStaticSituation;

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

		/**
		* (desires AGT PROP) means that the Agent_PartiallyTangible AGT desires
		* that the world be as the proposition PROP (represented by a
		* CycLSentence_Assertible) describes it to be. desires is implied by
		* goals, but is weaker: PROP might be some desirable state of affairs
		* that the agent is not actively working, or planning, to make/keep
		* true; e.g., world peace.
		*/
		static const Property desires;

		/**
		* Link a robot component to the action it enables.
		*/
		static const Property enables;

		static const Property hasCharacteristicLocation;

		/**
		* describe the main, significant color of the object.
		*/
		static const Property hasColor;

		/**
		* Links a robot with its components
		*/
		static const Property hasComponent;

		static const Property hasFrameOfReference;

		static const Property holds;

		static const Property holds_Underspecified;

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

		static const Property isIn;

		static const Property isNextTo;

		static const Property isOnTopOf;

		/**
		* (knowsAbout AGENT THING) means that the IntelligentAgent AGENT knows
		* something about THING. This predicate represents more than simple
		* awareness, but exactly what is known -- the content of AGENT's
		* knowledge about THING -- is not specified. For example, (knowsAbout
		* MichaelJordan Basketball_TheGame).
		* 
		* Note: many commonsense rules can conclude (knowsAbout AGENT THING) --
		* e.g., those having to do with AGENT's occupation, college degrees,
		* city of residence,... in fact, almost any assertion about AGENT can
		* lead to presumptions that he/she probably knowsAbout something.
		* knowsAbout in turn can serve (weakly) in arguments about whether or
		* not AGENT knows (q.v.) specific common assertions involving THING,
		* and in arguments about whether or not AGENT knowsValue (q.v.) of some
		* particular predicate applied to THING, etc. See also expertRegarding.
		*/
		static const Property knowsAbout;

		/**
		* This is a specialization of knowsAbout predicate with the range
		* constrained to artifacts, which is meaningful for robotics.
		*/
		static const Property knowsAboutArtifact;

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
		* binds an action to a state of the world (a Situation) which is
		* desired as a result (at short or long term) of this action.
		*/
		static const Property purposeOfAction;

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

		/**
		* Links an agent to an object visible for him.
		*/
		static const Property sees;

		/**
		* Binds a static situation (a state of the world) to the agents
		* involved in the situation.
		*/
		static const Property staticSituationInvolves;

		static const Property None;

// Datatype properties

		/**
		* True if you can calculate a "grasp" from the 3D geometry.
		*/
		static const Property canBeManipulated;

		/**
		* "has3dCoordinate" is an abstract dataproperty and only specific sub
		* properties (xCoord, yCoord, zCoord) must be instanciated.
		*/
		static const Property has3dCoordinate;

		/**
		* Bind an artifact to its 3D representation. The format of the 3D mesh
		* is not specified yet.
		*/
		static const Property has3dMesh;

		static const Property hasCharacteristicDimension;

		/**
		* The characteristic weight of the tangible facet of the object, in
		* grammes
		*/
		static const Property hasCharacteristicWeight;

		/**
		* the hue value of a color, as defined in the HSV color model.
		* Value are comprised between 0 and 360.
		* The value can be computed from RGB color representation as explained
		* in http://en.wikipedia.org/wiki/HSL_color_space
		*/
		static const Property hue;

		/**
		* Sets if an action is an atomic action, ie is a built-in primitive
		* action of a planner.
		*/
		static const Property isAtomicAction;

		/**
		* True if one of the object primary function is to hold other objects.
		* See the temporalProperty holds-Underspecified for a relationship
		* between a holder and a holded object.
		*/
		static const Property isHolder;

		static const Property isMovable;

		/**
		* True if the object is currently reachable by the robot.
		*/
		static const Property isReachable;

		static const Property isTraversable;

		static const Property xCoord;

		static const Property yCoord;

		static const Property zCoord;
};

/** This class lists all the OWL concepts defined in the openrobots.owl ontology.\n
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
		* The collection of Events (q.v.) that are carried out by some "doer".
		* Instances of Action include any event in which one or more agents
		* effect some change in the (tangible or intangible) state of the
		* world, typically by an expenditure of effort or energy. Note that it
		* is not required that any tangible object be moved, changed, produced,
		* or destroyed for an action to occur; the effects of an action might
		* be intangible. In the context of the OpenRobotOntology, doers of an
		* action are as a matter of fact instances to Agent_PartiallyTangible
		* (q.v.), even if they may not be in a broader context (e.g. a falling
		* rock that dents a car's roof). Depending upon the context, doers of
		* actions might be animate or inanimate, conscious or nonconscious. For
		* actions that are intentional, see PurposefulAction and performedBy.
		* 
		* The object of an action is asserted through the properties
		* objectOfAction and recipientOfAction.
		*/
		static const Class Action;

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

		static const Class Blue;

		static const Class Book;

		static const Class Bottle;

		static const Class Ceilling;

		static const Class Color;

		/**
		* A component is an autonomous entity which provide a behaviour (which
		* can be described as a set of actions). This behaviour is stable over
		* the time (its nature won't change). It can be composed of sub
		* components.
		*/
		static const Class Component;

		static const Class Discussion;

		/**
		* Doorway is a specialization of Portal, CavityWithWalls, and
		* Artifact_NonAgentive. Each instance of Doorway is a portal in some
		* instance of HumanOccupationConstruct (q.v.), suitable for people (and
		* perhaps vehicles) to enter and exit. Each instance of Doorway
		* includes the frame, not merely the empty space within the doorway.
		* Instances of Doorway include doorways to houses, office buildings,
		* elevators, automobiles, airplanes, and garages.
		*/
		static const Class Doorway;

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

		/**
		* Each instance of FrameOfReference is a mathematical (and hence
		* intangible) representation of the context in which certain data are
		* to be interpreted. Such contexts are typically physical, but may be
		* purely mathematical. For example, a CartesianCoordinateSystem can be
		* used to represent the positions of things on the surface of the Eath,
		* but can also be used to represent an abstract geometric space.
		*/
		static const Class FrameOfReference;

		static const Class Furniture;

		/**
		* Represents the action of an agent giving something to some other
		* agent. The "something" must be specified by the "objectOfAction"
		* property, while the "some other agent" must be specified by the
		* "recipientOfAction" property.
		*/
		static const Class Give;

		static const Class Green;

		/**
		* Represents the action for an agent to help another agent. The agent
		* which recieves help may be specified with the "recipientOfAction"
		* property.
		*/
		static const Class Help;

		/**
		* Represents the action of an agent holding an object specified by the
		* objectOfAction property.
		* The immediate consequence of this action is a StaticSituation where
		* an object is holded by the agent. Cf the object property
		* "holds-underspecified".
		* The part of the agent which holds the object may be specified wtih
		* the "recipientOfAction" property.
		*/
		static const Class Hold;

		/**
		* Each instance of HolderGripperComponent is function which control an
		* object that can apply pressure to another object and thereby grip it
		* in such a way that its motion is restricted. For instance, a human
		* hand.
		*/
		static const Class HolderGripperComponent;

		static const Class Human;

		static const Class LocalisationAwareRobot;

		static const Class LocalisationComponent;

		/**
		* Location is use in its very broad meaning of "a spatial-localized
		* thing". Hence an object is a location, an agent is a location, etc.
		*/
		static const Class Location;

		/**
		* Represents the action of an agent looking at the thing specified by
		* the ObjectOfAction property.
		*/
		static const Class LookAt;

		static const Class MotionComponent;

		static const Class MotionPlanner;

		/**
		* Represents an action consisting in a move to a place or an object as
		* specified by the objectOfAction property.
		* If no destination is specified, this concept represent the general
		* fact of moving.
		*/
		static const Class Move;

		static const Class MovingRobot;

		static const Class Object;

		static const Class Object_SupportingFurniture;

		static const Class Obstacle;

		static const Class Opening;

		static const Class Orange;

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

		/**
		* A robot able to go alone to a given location
		*/
		static const Class PositionableRobot;

		/**
		* A specialization of both Action and AtLeastPartiallyMentalEvent. Each
		* instance of PurposefulAction is an action consciously, volitionally,
		* and purposefully done by (see performedBy) at least one actor.
		*/
		static const Class PurposefulAction;

		/**
		* Represents the action of an agent pushing an object specified by the
		* objectOfAction property.
		* The place where the object is pushed (its destination) may be
		* specified by the "recipientOfAction" property.
		*/
		static const Class Push;

		/**
		* Represtens the action of an agent putting (or placing) something
		* somewhere.
		* The "something" is specified by the "objectOfAction" property while
		* the "somewhere" is specified by the "recipientOfAction" property.
		*/
		static const Class Put;

		static const Class RaisonningComponent;

		static const Class Red;

		/**
		* A special kind of "Talk" action which represents the "request"
		* communication act. The content of the request is specified by the
		* "objectOfAction" property.
		*/
		static const Class Request;

		static const Class Robot;

		/**
		* Represents the action of an agent who sees something which must be
		* specified by the objectOfAction property.
		*/
		static const Class See;

		static const Class SensingComponent;

		static const Class Shelf;

		static const Class Shelter;

		/**
		* Represents the action of an agent showing something to some other
		* agent.
		* The "something" must be specified by the "objectOfAction" property,
		* while the "some other agent" may be specified by the
		* "recipientOfAction" property.
		*/
		static const Class Show;

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

		/**
		* The action of taking (or grabbing or grasping) something specified by
		* the objectOfAction property.
		*/
		static const Class Take;

		/**
		* Represents the action for an agent of talking (or dialoging) with
		* another agent which must be specified with the "recipientOfAction"
		* property.
		* The object of discussion may be specified with the "objectOfAction"
		* property.
		*/
		static const Class Talk;

		/**
		* Represents the action of an agent which enters in contact with the
		* object specified by the ObjectOfAction property.
		*/
		static const Class Touch;

		static const Class Trashbin;

		static const Class Violet;

		static const Class VisualMark;

		static const Class Walk;

		static const Class Wall;

		static const Class Window;

		static const Class Yellow;

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

		static const Class None;
};


}

#endif /* ORO_LIBRARY_H_ */
