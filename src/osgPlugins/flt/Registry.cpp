
#include <osg/Node>
#include <osg/Group>
#include <osg/Notify>

#include <algorithm>
#include <set>

#include "Record.h"
#include "Input.h"
#include "FltFile.h"
#include "Registry.h"

using namespace flt;

// static
Registry* Registry::instance()
{
    static Registry s_nodeFactory;
    return &s_nodeFactory;
}


void Registry::addPrototype(Record* rec)
{
    if (rec==0L) return;

    osg::notify(osg::INFO) << "flt::Registry::addPrototype("<< rec->className()<<")\n";
    int op = rec->classOpcode();
    _recordProtoMap[op] = rec;
}


Record* Registry::getPrototype(const int opcode)
{
    RecordProtoMap::iterator itr = _recordProtoMap.find(opcode);
    if (itr != _recordProtoMap.end()) {
        return (*itr).second.get();
    }
    osg::notify( osg::WARN )
	<< "flt::Registry::addPrototype: Unkown opcode: " << opcode << "\n";

    return NULL;

}

///////////////////////////////////////////////////////////////////


void Registry::addTexture(const std::string& name, osg::StateSet* texture)
{
    if (texture == NULL) return;
    _textureMap[name] = texture;
}


osg::StateSet* Registry::getTexture(const std::string name)
{
    TextureMap::iterator itr = _textureMap.find(name);
    if (itr != _textureMap.end())
        return (*itr).second.get();

    return NULL;
}


void Registry::addFltFile(const std::string& name, FltFile* file)
{
    if (file == NULL) return;
    _fltFileMap[name] = file;
}


FltFile* Registry::getFltFile(const std::string& name)
{
    FltFileMap::iterator itr = _fltFileMap.find(name);
    if (itr != _fltFileMap.end())
        return (*itr).second.get();

    return NULL;
}

