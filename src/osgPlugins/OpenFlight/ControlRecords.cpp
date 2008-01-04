/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
 *
 * This library is open source and may be redistributed and/or modified under  
 * the terms of the OpenSceneGraph Public License (OSGPL) version 0.0 or 
 * (at your option) any later version.  The full license is in LICENSE file
 * included with this distribution, and on the openscenegraph.org website.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * OpenSceneGraph Public License for more details.
*/

//
// OpenFlight� loader for OpenSceneGraph
//
//  Copyright (C) 2005-2007  Brede Johansen
//

#include <assert.h>
#include <osg/Geode>
#include <osg/Geometry>
#include "Registry.h"
#include "Document.h"
#include "RecordInputStream.h"

namespace flt {



/** PushLevel
*/
class PushLevel : public Record
{
    public:

        PushLevel() {}

        META_Record(PushLevel)

        virtual void readRecord(RecordInputStream& /*in*/, Document& document)
        {
            document.pushLevel();
        }

    protected:

        virtual ~PushLevel() {}
};

RegisterRecordProxy<PushLevel> g_PushLevel(PUSH_LEVEL_OP);


/** PophLevel
*/
class PopLevel : public Record
{
    public:

        PopLevel() {}

        META_Record(PopLevel)

        virtual void read(RecordInputStream& /*in*/, Document& document)
        {
            // Finally call dispose() for primary with push, pop level pair. 
            PrimaryRecord* primary = document.getTopOfLevelStack();
            if (primary)
            {
                primary->dispose(document);
            }

            document.popLevel();
        }

    protected:

        virtual ~PopLevel() {}
};

RegisterRecordProxy<PopLevel> g_PopLevel(POP_LEVEL_OP);


/** PushSubface
*/
class PushSubface : public Record
{
    public:

        PushSubface() {}

        META_Record(PushSubface)

        virtual void read(RecordInputStream& /*in*/, Document& document)
        {
            document.pushSubface();
        }

    protected:

        virtual ~PushSubface() {}
};

RegisterRecordProxy<PushSubface> g_PushSubface(PUSH_SUBFACE_OP);


/** PopSubface
*/
class PopSubface : public Record
{
    public:

        PopSubface() {}

        META_Record(PopSubface)

        virtual void read(RecordInputStream& /*in*/, Document& document)
        {
            document.popSubface();
        }

    protected:

        virtual ~PopSubface() {}
};

RegisterRecordProxy<PopSubface> g_PopSubface(POP_SUBFACE_OP);


/** PushExtension
*/
class PushExtension : public Record
{
    public:

        PushExtension() {}

        META_Record(PushExtension)

        virtual void read(RecordInputStream& in, Document& document)
        {
            readRecord(in,document);
            document.pushExtension();
        }

    protected:

        virtual ~PushExtension() {}
};

RegisterRecordProxy<PushExtension> g_PushExtension(PUSH_EXTENSION_OP);


/** PopExtension
*/
class PopExtension : public Record
{
    public:

        PopExtension() {}

        META_Record(PopExtension)

        virtual void read(RecordInputStream& in, Document& document)
        {
            readRecord(in,document);
            document.popExtension();
        }

    protected:

        virtual ~PopExtension() {}
};

RegisterRecordProxy<PopExtension> g_PopExtension(POP_EXTENSION_OP);


/** PushAttribute - Reserved subtree
*/
class PushAttribute : public Record
{
    public:

        PushAttribute() {}

        META_Record(PushAttribute)

        virtual void read(RecordInputStream& in, Document& document)
        {
            readRecord(in,document);
        }

    protected:

        virtual ~PushAttribute() {}
};

RegisterRecordProxy<PushAttribute> g_PushAttribute(PUSH_ATTRIBUTE_OP);


/** PopAttribute
*/
class PopAttribute : public Record
{
    public:

        PopAttribute() {}

        META_Record(PopAttribute)

        virtual void read(RecordInputStream& in, Document& document)
        {
            readRecord(in,document);
        }

    protected:

        virtual ~PopAttribute() {}
};

RegisterRecordProxy<PopAttribute> g_PopAttribute(POP_ATTRIBUTE_OP);


} // end namespace



