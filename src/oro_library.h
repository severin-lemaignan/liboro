// This file has been automatically generated from openrobots.owl by "generate_oro_library_from_ontology.py"
#ifndef ORO_LIBRARY_H_
#define ORO_LIBRARY_H_

#include "oro.h"

namespace oro {

class Properties {
	public:

		static const Property belongsTo;

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

		static const Property holds_Underspecified;

		/**
		* (isAbleTo AGENT ACTION) says that AGENT it technically/physically
		* able to achieve the ACTION. It does not mean that the agent can do it
		* right now (see canUndertakeNow for this meaning). The ACTION is
		* constrainted to purposeful actions: isAbleTo does not intend to
		* express a fact like "a stone is able to sink into the water".
		*/
		static const Property isAbleTo;

		static const Property isAt;

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

		static const Property nextTo;

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

		static const Property onTopOf;

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
};

const Property Properties::belongsTo = Property("belongsTo");
const Property Properties::canUndertakeNow = Property("canUndertakeNow");
const Property Properties::currentlyPerforms = Property("currentlyPerforms");
const Property Properties::desires = Property("desires");
const Property Properties::enables = Property("enables");
const Property Properties::hasCharacteristicLocation = Property("hasCharacteristicLocation");
const Property Properties::hasColor = Property("hasColor");
const Property Properties::hasComponent = Property("hasComponent");
const Property Properties::holds_Underspecified = Property("holds_Underspecified");
const Property Properties::isAbleTo = Property("isAbleTo");
const Property Properties::isAt = Property("isAt");
const Property Properties::knowsAbout = Property("knowsAbout");
const Property Properties::knowsAboutArtifact = Property("knowsAboutArtifact");
const Property Properties::mainColorOfObject = Property("mainColorOfObject");
const Property Properties::nextTo = Property("nextTo");
const Property Properties::objectFoundInLocation = Property("objectFoundInLocation");
const Property Properties::objectOfAction = Property("objectOfAction");
const Property Properties::onTopOf = Property("onTopOf");
const Property Properties::purposeOfAction = Property("purposeOfAction");
const Property Properties::recipientOfAction = Property("recipientOfAction");
const Property Properties::sees = Property("sees");

class Classes {
	public:

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

		static const Class EnduringThing_Localized;

		static const Class Event;

		static const Class Floor;

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

		static const Class SpaceRegion_Empirical;

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

const Class Classes::Action = Class("Action");
const Class Classes::Agent = Class("Agent");
const Class Classes::Artifact = Class("Artifact");
const Class Classes::Blue = Class("Blue");
const Class Classes::Book = Class("Book");
const Class Classes::Bottle = Class("Bottle");
const Class Classes::Ceilling = Class("Ceilling");
const Class Classes::Color = Class("Color");
const Class Classes::Component = Class("Component");
const Class Classes::Doorway = Class("Doorway");
const Class Classes::EnduringThing_Localized = Class("EnduringThing_Localized");
const Class Classes::Event = Class("Event");
const Class Classes::Floor = Class("Floor");
const Class Classes::Furniture = Class("Furniture");
const Class Classes::Give = Class("Give");
const Class Classes::Green = Class("Green");
const Class Classes::Help = Class("Help");
const Class Classes::Hold = Class("Hold");
const Class Classes::HolderGripperComponent = Class("HolderGripperComponent");
const Class Classes::Human = Class("Human");
const Class Classes::LocalisationAwareRobot = Class("LocalisationAwareRobot");
const Class Classes::LocalisationComponent = Class("LocalisationComponent");
const Class Classes::Location = Class("Location");
const Class Classes::LookAt = Class("LookAt");
const Class Classes::MotionComponent = Class("MotionComponent");
const Class Classes::MotionPlanner = Class("MotionPlanner");
const Class Classes::Move = Class("Move");
const Class Classes::MovingRobot = Class("MovingRobot");
const Class Classes::Object = Class("Object");
const Class Classes::Object_SupportingFurniture = Class("Object_SupportingFurniture");
const Class Classes::Obstacle = Class("Obstacle");
const Class Classes::Opening = Class("Opening");
const Class Classes::Orange = Class("Orange");
const Class Classes::PartiallyTangible = Class("PartiallyTangible");
const Class Classes::PhysicalSupport = Class("PhysicalSupport");
const Class Classes::Place = Class("Place");
const Class Classes::PositionableRobot = Class("PositionableRobot");
const Class Classes::PurposefulAction = Class("PurposefulAction");
const Class Classes::Push = Class("Push");
const Class Classes::Put = Class("Put");
const Class Classes::RaisonningComponent = Class("RaisonningComponent");
const Class Classes::Red = Class("Red");
const Class Classes::Request = Class("Request");
const Class Classes::Robot = Class("Robot");
const Class Classes::See = Class("See");
const Class Classes::SensingComponent = Class("SensingComponent");
const Class Classes::Shelf = Class("Shelf");
const Class Classes::Show = Class("Show");
const Class Classes::Situation = Class("Situation");
const Class Classes::SpaceRegion_Empirical = Class("SpaceRegion_Empirical");
const Class Classes::SpatialThing = Class("SpatialThing");
const Class Classes::SpatialThing_Localized = Class("SpatialThing_Localized");
const Class Classes::StaticSituation = Class("StaticSituation");
const Class Classes::Table = Class("Table");
const Class Classes::Take = Class("Take");
const Class Classes::Talk = Class("Talk");
const Class Classes::Touch = Class("Touch");
const Class Classes::Trashbin = Class("Trashbin");
const Class Classes::Violet = Class("Violet");
const Class Classes::VisualMark = Class("VisualMark");
const Class Classes::Wall = Class("Wall");
const Class Classes::Window = Class("Window");
const Class Classes::Yellow = Class("Yellow");
const Class Classes::Zone = Class("Zone");
const Class Classes::ZoneOfAction = Class("ZoneOfAction");
const Class Classes::ZoneOfInteraction = Class("ZoneOfInteraction");
const Class Classes::ZoneOfInterest = Class("ZoneOfInterest");
const Class Classes::None = Class("None");

}

#endif /* ORO_LIBRARY_H_ */
