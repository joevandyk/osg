#include <osg/LineStipple>

#include <osgDB/Registry>
#include <osgDB/Input>
#include <osgDB/Output>

using namespace osg;
using namespace osgDB;

// forward declare functions to use later.
bool LineStipple_readLocalData(Object& obj, Input& fr);
bool LineStipple_writeLocalData(const Object& obj, Output& fw);


// register the read and write functions with the osgDB::Registry.
RegisterDotOsgWrapperProxy g_LineStippleProxy
(
    new osg::LineStipple,
    "LineStipple",
    "Object StateAttribute LineStipple",
    &LineStipple_readLocalData,
    &LineStipple_writeLocalData
);


bool LineStipple_readLocalData(Object& obj, Input& fr)
{
    bool iteratorAdvanced = false;

    LineStipple& linestipple = static_cast<LineStipple&>(obj);
    
    int ref = linestipple.getFactor();
    if (fr[0].matchWord("factor") && fr[1].getInt(ref))
    {
        linestipple.setFactor(ref);
        fr+=2;
        iteratorAdvanced = true;
    }

    osg::uint mask = linestipple.getPattern();
    if (fr[0].matchWord("functionMask") && fr[1].getUInt(mask))
    {
        linestipple.setPattern(mask);
        fr+=2;
        iteratorAdvanced = true;
    }

    return iteratorAdvanced;
}

bool LineStipple_writeLocalData(const Object& obj,Output& fw)
{
    const LineStipple& linestipple = static_cast<const LineStipple&>(obj);

    fw.indent() << "factor " << linestipple.getFactor() << std::endl;
    fw.indent() << "pattern 0x" << std::hex << linestipple.getPattern() << std::dec << std::endl;

    return true;
}

