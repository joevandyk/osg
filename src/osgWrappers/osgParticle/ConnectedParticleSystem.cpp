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
#include <osg/Object>
#include <osg/RenderInfo>
#include <osgParticle/ConnectedParticleSystem>
#include <osgParticle/Particle>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osgParticle::ConnectedParticleSystem)
	I_DeclaringFile("osgParticle/ConnectedParticleSystem");
	I_BaseType(osgParticle::ParticleSystem);
	I_Constructor0(____ConnectedParticleSystem,
	               "",
	               "");
	I_ConstructorWithDefaults2(IN, const osgParticle::ConnectedParticleSystem &, copy, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY,
	                           ____ConnectedParticleSystem__C5_ConnectedParticleSystem_R1__C5_osg_CopyOp_R1,
	                           "",
	                           "");
	I_Method0(osg::Object *, cloneType,
	          Properties::VIRTUAL,
	          __osg_Object_P1__cloneType,
	          "Clone the type of an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop,
	          Properties::VIRTUAL,
	          __osg_Object_P1__clone__C5_osg_CopyOp_R1,
	          "Clone an object, with Object* return type. ",
	          "Must be defined by derived classes. ");
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj,
	          Properties::VIRTUAL,
	          __bool__isSameKindAs__C5_osg_Object_P1,
	          "",
	          "");
	I_Method0(const char *, libraryName,
	          Properties::VIRTUAL,
	          __C5_char_P1__libraryName,
	          "return the name of the object's library. ",
	          "Must be defined by derived classes. The OpenSceneGraph convention is that the namespace of a library is the same as the library name. ");
	I_Method0(const char *, className,
	          Properties::VIRTUAL,
	          __C5_char_P1__className,
	          "return the name of the object's class type. ",
	          "Must be defined by derived classes. ");
	I_Method1(osgParticle::Particle *, createParticle, IN, const osgParticle::Particle *, ptemplate,
	          Properties::VIRTUAL,
	          __Particle_P1__createParticle__C5_Particle_P1,
	          "Create a new particle from the specified template (or the default one if ptemplate is null). ",
	          "");
	I_Method1(void, reuseParticle, IN, int, i,
	          Properties::VIRTUAL,
	          __void__reuseParticle__int,
	          "Reuse the i-th particle. ",
	          "");
	I_Method1(void, drawImplementation, IN, osg::RenderInfo &, renderInfo,
	          Properties::VIRTUAL,
	          __void__drawImplementation__osg_RenderInfo_R1,
	          "Draw the connected particles as either a line or a quad strip, depending upon viewing distance. . ",
	          "");
	I_Method0(const osgParticle::Particle *, getStartParticle,
	          Properties::NON_VIRTUAL,
	          __C5_osgParticle_Particle_P1__getStartParticle,
	          "Get the (const) particle from where the line or quadstrip starts to be drawn. ",
	          "");
	I_Method0(osgParticle::Particle *, getStartParticle,
	          Properties::NON_VIRTUAL,
	          __osgParticle_Particle_P1__getStartParticle,
	          "Get the particle from where the line or quadstrip starts to be drawn. ",
	          "");
	I_Method1(void, setMaxNumberOfParticlesToSkip, IN, unsigned int, maxNumberofParticlesToSkip,
	          Properties::NON_VIRTUAL,
	          __void__setMaxNumberOfParticlesToSkip__unsigned_int,
	          "Set the maximum numbers of particles to be skipped during the predraw filtering. ",
	          "");
	I_Method0(unsigned int, getMaxNumberOfParticlesToSkip,
	          Properties::NON_VIRTUAL,
	          __unsigned_int__getMaxNumberOfParticlesToSkip,
	          "Get the maximum numbers of particles to be skipped during the predraw filtering. ",
	          "");
	I_SimpleProperty(unsigned int, MaxNumberOfParticlesToSkip, 
	                 __unsigned_int__getMaxNumberOfParticlesToSkip, 
	                 __void__setMaxNumberOfParticlesToSkip__unsigned_int);
	I_SimpleProperty(osgParticle::Particle *, StartParticle, 
	                 __osgParticle_Particle_P1__getStartParticle, 
	                 0);
END_REFLECTOR

