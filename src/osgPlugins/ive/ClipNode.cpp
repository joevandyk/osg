/**********************************************************************
 *
 *    FILE:            ClipNode.cpp
 *
 *    DESCRIPTION:    Read/Write osg::ClipNode in binary format to disk.
 *
 *    CREATED BY:        Auto generated by iveGenerated
 *                    and later modified by Rune Schmidt Jensen.
 *
 *    HISTORY:        Created 21.3.2003
 *
 *    Copyright 2003 VR-C
 **********************************************************************/

#include "Exception.h"
#include "ClipNode.h"
#include "ClipPlane.h"
#include "Group.h"
#include "Light.h"

using namespace ive;

void ClipNode::write(DataOutputStream* out){
    // Write ClipNode's identification.
    out->writeInt(IVECLIPNODE);
    // If the osg class is inherited by any other class we should also write this to file.
    osg::Group*  group = dynamic_cast<osg::Group*>(this);
    if(group){
        ((ive::Group*)(group))->write(out);
    }
    else
        throw Exception("ClipNode::write(): Could not cast this osg::ClipNode to an osg::Group.");
    // Write ClipNode's properties.

    out->writeUInt(getNumClipPlanes());

    for (unsigned int i=0;i<getNumClipPlanes();++i)
    {
        ((ive::ClipPlane*)getClipPlane(i))->write(out);
    }

}

void ClipNode::read(DataInputStream* in){
    // Peek on ClipNode's identification.
    int id = in->peekInt();
    if(id == IVECLIPNODE){
        // Read ClipNode's identification.
        id = in->readInt();
        // If the osg class is inherited by any other class we should also read this from file.
        osg::Group*  group = dynamic_cast<osg::Group*>(this);
        if(group){
            ((ive::Group*)(group))->read(in);
        }
        else
            throw Exception("ClipNode::read(): Could not cast this osg::ClipNode to an osg::Object.");
        // Read ClipNode's properties

        unsigned int numClipPlanes = in->readUInt();

        for (unsigned int i=0;i<numClipPlanes;++i)
        {
            osg::ClipPlane* clipPlane = new osg::ClipPlane;
            ((ive::ClipPlane*)clipPlane)->read(in);
            addClipPlane(clipPlane);
        }

    }
    else{
        throw Exception("ClipNode::read(): Expected ClipNode identification.");
    }
}
