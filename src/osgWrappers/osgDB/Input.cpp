// ***************************************************************************
//
//   Generated automatically by genwrapper.
//   Please DO NOT EDIT this file!
//
// ***************************************************************************

#include <osgIntrospection/ReflectionMacros>
#include <osgIntrospection/TypedMethodInfo>
#include <osgIntrospection/Attributes>

#include <osg/Image>
#include <osg/Node>
#include <osg/Object>
#include <osg/Uniform>
#include <osg/test/Drawable>
#include <osg/test/StateAttribute>
#include <osgDB/Input>
#include <osgDB/ReaderWriter>
#include <osgDB/Registry>

// Must undefine IN and OUT macros defined in Windows headers
#ifdef IN
#undef IN
#endif
#ifdef OUT
#undef OUT
#endif

BEGIN_OBJECT_REFLECTOR(osgDB::Input)
	I_BaseType(osgDB::FieldReaderIterator);
	I_Constructor0();
	I_Method1(void, setOptions, IN, const osgDB::ReaderWriter::Options *, options);
	I_Method0(const osgDB::ReaderWriter::Options *, getOptions);
	I_Method1(osg::Object *, readObjectOfType, IN, const osg::Object &, compObj);
	I_Method1(osg::Object *, readObjectOfType, IN, const osgDB::basic_type_wrapper &, btw);
	I_Method0(osg::Object *, readObject);
	I_Method0(osg::Image *, readImage);
	I_Method0(osg::Drawable *, readDrawable);
	I_Method0(osg::StateAttribute *, readStateAttribute);
	I_Method0(osg::Uniform *, readUniform);
	I_Method0(osg::Node *, readNode);
	I_Method1(osg::Object *, readObject, IN, const std::string &, fileName);
	I_Method1(osg::Image *, readImage, IN, const std::string &, fileName);
	I_Method1(osg::Node *, readNode, IN, const std::string &, fileName);
	I_Method1(osg::Object *, getObjectForUniqueID, IN, const std::string &, uniqueID);
	I_Method2(void, regisiterUniqueIDForObject, IN, const std::string &, uniqueID, IN, osg::Object *, obj);
	I_Property(const osgDB::ReaderWriter::Options *, Options);
END_REFLECTOR

