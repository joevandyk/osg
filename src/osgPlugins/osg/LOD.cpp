#include "osg/LOD"

#include "osgDB/Registry"
#include "osgDB/Input"
#include "osgDB/Output"

using namespace osg;
using namespace osgDB;

// forward declare functions to use later.
bool LOD_readLocalData(Object& obj, Input& fr);
bool LOD_writeLocalData(const Object& obj, Output& fw);

// register the read and write functions with the osgDB::Registry.
RegisterDotOsgWrapperProxy g_LODProxy
(
    new osg::LOD,
    "LOD",
    "Object Node LOD Group",
    &LOD_readLocalData,
    &LOD_writeLocalData
);

bool LOD_readLocalData(Object& obj, Input& fr)
{
    bool iteratorAdvanced = false;

    LOD& lod = static_cast<LOD&>(obj);

    if (fr.matchSequence("Center %f %f %f"))
    {
        Vec3 center;
        fr[1].getFloat(center[0]);
        fr[2].getFloat(center[1]);
        fr[3].getFloat(center[2]);
        lod.setCenter(center);

        iteratorAdvanced = true;
        fr+=4;
    }

    bool matchFirst = false;
    if ((matchFirst=fr.matchSequence("Ranges {")) || fr.matchSequence("Ranges %i {"))
    {

        // set up coordinates.
        int entry = fr[0].getNoNestedBrackets();

        if (matchFirst)
        {
            fr += 2;
        }
        else
        {
            //_rangeList.(capacity);
            fr += 3;
        }

        float range;
        int i=0;
        while (!fr.eof() && fr[0].getNoNestedBrackets()>entry)
        {
            if (fr[0].getFloat(range))
            {
                lod.setRange(i,range);
                ++fr;
                ++i;
            }
            else
            {
                ++fr;
            }
        }

        iteratorAdvanced = true;
        ++fr;

    }

    return iteratorAdvanced;
}


bool LOD_writeLocalData(const Object& obj, Output& fw)
{
    const LOD& lod = static_cast<const LOD&>(obj);

    fw.indent() << "Center "<< lod.getCenter() << std::endl;

    fw.indent() << "Ranges {"<< std::endl;
    fw.moveIn();
    
    for(int i=0; i<lod.getNumRanges();++i)
    {
        fw.indent() << lod.getRange(i) << std::endl;
    }
    fw.moveOut();
    fw.indent() << "}"<< std::endl;

    return true;
}
