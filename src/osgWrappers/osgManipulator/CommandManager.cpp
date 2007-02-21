// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/StaticMethodInfo>
#include <osgIntrospection/Attributes>

#include <osgManipulator/Command>
#include <osgManipulator/CommandManager>
#include <osgManipulator/Constraint>
#include <osgManipulator/Dragger>
#include <osgManipulator/Selection>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osgManipulator::CommandManager)
	I_BaseType(osg::Referenced);
	I_Constructor0(____CommandManager,
	               "",
	               "");
	I_Method2(bool, connect, IN, osgManipulator::Dragger &, dragger, IN, osgManipulator::Selection &, selection,
	          Properties::VIRTUAL,
	          __bool__connect__Dragger_R1__Selection_R1,
	          "Connect a dragger to a selection. ",
	          "The selection will begin listening to commands generated by the dragger. This can be called multiple times to connect many selections to a dragger.");
	I_Method2(bool, connect, IN, osgManipulator::Dragger &, dragger, IN, osgManipulator::Constraint &, constrain,
	          Properties::VIRTUAL,
	          __bool__connect__Dragger_R1__Constraint_R1,
	          "",
	          "");
	I_Method1(bool, disconnect, IN, osgManipulator::Dragger &, dragger,
	          Properties::VIRTUAL,
	          __bool__disconnect__Dragger_R1,
	          "Disconnect the selections from a dragger. ",
	          "");
	I_Method1(void, dispatch, IN, osgManipulator::MotionCommand &, command,
	          Properties::VIRTUAL,
	          __void__dispatch__MotionCommand_R1,
	          "Dispatches a command. ",
	          "Usually called from a dragger. ");
	I_Method2(void, addSelectionsToCommand, IN, osgManipulator::MotionCommand &, command, IN, osgManipulator::Dragger &, dragger,
	          Properties::NON_VIRTUAL,
	          __void__addSelectionsToCommand__MotionCommand_R1__Dragger_R1,
	          "Add all selections connected to the dragger to the command. ",
	          "");
END_REFLECTOR
