#include "osg/LightSource"

#include "osgDB/Registry"
#include "osgDB/Input"
#include "osgDB/Output"

using namespace osg;
using namespace osgDB;

// forward declare functions to use later.
bool LightSource_readLocalData(Object& obj, Input& fr);
bool LightSource_writeLocalData(const Object& obj, Output& fw);

// register the read and write functions with the osgDB::Registry.
RegisterDotOsgWrapperProxy g_LightSourceProxy
(
    new osg::LightSource,
    "LightSource",
    "Object Node LightSource",
    &LightSource_readLocalData,
    &LightSource_writeLocalData
);

bool LightSource_readLocalData(Object& obj, Input& fr)
{
    bool iteratorAdvanced = false;

    LightSource& lightsource = static_cast<LightSource&>(obj);

    if (fr[0].matchWord("referenceFrame")) {
        if (fr[1].matchWord("RELATIVE_TO_ABSOLUTE")) {
            lightsource.setReferenceFrame(LightSource::RELATIVE_TO_ABSOLUTE);
            fr += 2;
            iteratorAdvanced = true;
        }
        if (fr[1].matchWord("RELATIVE_TO_PARENTS")) {
            lightsource.setReferenceFrame(LightSource::RELATIVE_TO_PARENTS);
            fr += 2;
            iteratorAdvanced = true;
        }
    }

    StateAttribute* light=fr.readStateAttribute();
    if (light)
    {
        lightsource.setLight(light);
        iteratorAdvanced = true;
    }

    return iteratorAdvanced;
}


bool LightSource_writeLocalData(const Object& obj, Output& fw)
{
    const LightSource& lightsource = static_cast<const LightSource&>(obj);

    fw.indent() << "referenceFrame ";
    switch (lightsource.getReferenceFrame()) {
        case LightSource::RELATIVE_TO_ABSOLUTE:
            fw << "RELATIVE_TO_ABSOLUTE\n";
            break;
        case LightSource::RELATIVE_TO_PARENTS:
        default:
            fw << "RELATIVE_TO_PARENTS\n";
    };

    if (lightsource.getLight()) fw.writeObject(*lightsource.getLight());

    return true;
}
