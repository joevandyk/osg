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

#include <osg/CopyOp>
#include <osg/NodeVisitor>
#include <osg/Object>
#include <osgParticle/Emitter>
#include <osgParticle/Particle>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_ABSTRACT_OBJECT_REFLECTOR(osgParticle::Emitter)
	I_DeclaringFile("osgParticle/Emitter");
	I_BaseType(osgParticle::ParticleProcessor);
	I_Constructor0(____Emitter,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osgParticle::Emitter &, copy, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____Emitter__C5_Emitter_R1__C5_osg_CopyOp_R1,
	                           "",
	                           "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "return the name of the node's library. ",
	          "");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "return the name of the node's class type. ",
	          "");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_osg_Object_P1,
	          "return true if this and obj are of the same kind of object. ",
	          "");
	I_Method1(void, accept, IN, osg::NodeVisitor &, nv,
	          Properties::VIRTUAL,
	          __void__accept__osg_NodeVisitor_R1,
	          "Visitor Pattern : calls the apply method of a NodeVisitor with this node's type. ",
	          "");
	I_Method0(const osgParticle::Particle &, getParticleTemplate,
	          Properties::NON_VIRTUAL,
	          __C5_Particle_R1__getParticleTemplate,
	          "Get the particle template. ",
	          "");
	I_Method1(void, setParticleTemplate, IN, const osgParticle::Particle &, p,
	          Properties::NON_VIRTUAL,
	          __void__setParticleTemplate__C5_Particle_R1,
	          "Set the particle template (particle is copied). ",
	          "");
	I_Method0(bool, getUseDefaultTemplate,
	          Properties::NON_VIRTUAL,
	          __bool__getUseDefaultTemplate,
	          "Return whether the particle system's default template should be used. ",
	          "");
	I_Method1(void, setUseDefaultTemplate, IN, bool, v,
	          Properties::NON_VIRTUAL,
	          __void__setUseDefaultTemplate__bool,
	          "Set whether the default particle template should be used. ",
	          "When this flag is true, the particle template is ignored, and the particle system's default template is used instead. ");
	I_ProtectedMethod1(void, process, IN, double, dt,
	                   Properties::VIRTUAL,
	                   Properties::NON_CONST,
	                   __void__process__double,
	                   "",
	                   "");
	I_ProtectedMethod1(void, emit, IN, double, dt,
	                   Properties::PURE_VIRTUAL,
	                   Properties::NON_CONST,
	                   __void__emit__double,
	                   "",
	                   "");
	I_SimpleProperty(const osgParticle::Particle &, ParticleTemplate, 
	                 __C5_Particle_R1__getParticleTemplate, 
	                 __void__setParticleTemplate__C5_Particle_R1);
	I_SimpleProperty(bool, UseDefaultTemplate, 
	                 __bool__getUseDefaultTemplate, 
	                 __void__setUseDefaultTemplate__bool);
END_REFLECTOR

