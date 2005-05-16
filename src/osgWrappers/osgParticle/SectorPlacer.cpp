// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/Attributes>

#include <osg/CopyOp>
#include <osg/Object>
#include <osgParticle/Particle>
#include <osgParticle/SectorPlacer>
#include <osgParticle/range>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osgParticle::SectorPlacer)
	I_BaseType(osgParticle::CenteredPlacer);
	I_Constructor0();
	I_ConstructorWithDefaults2(IN, const osgParticle::SectorPlacer &, copy, , IN, const osg::CopyOp &, copyop, osg::CopyOp::SHALLOW_COPY);
	I_Method0(const osgParticle::rangef &, getRadiusRange);
	I_Method1(void, setRadiusRange, IN, const osgParticle::rangef &, r);
	I_Method2(void, setRadiusRange, IN, float, r1, IN, float, r2);
	I_Method0(const osgParticle::rangef &, getPhiRange);
	I_Method1(void, setPhiRange, IN, const osgParticle::rangef &, r);
	I_Method2(void, setPhiRange, IN, float, r1, IN, float, r2);
	I_Method0(osg::Object *, cloneType);
	I_Method1(osg::Object *, clone, IN, const osg::CopyOp &, copyop);
	I_Method1(bool, isSameKindAs, IN, const osg::Object *, obj);
	I_Method0(const char *, libraryName);
	I_Method0(const char *, className);
	I_Method1(void, place, IN, osgParticle::Particle *, P);
	I_Property(const osgParticle::rangef &, PhiRange);
	I_Property(const osgParticle::rangef &, RadiusRange);
END_REFLECTOR

