// CommentRecord.h

#ifndef __FLT_COMMENT_RECORD_H
#define __FLT_COMMENT_RECORD_H


#include "opcodes.h"
#include "Record.h"
#include "RecordVisitor.h"


namespace flt {


////////////////////////////////////////////////////////////////////
//
//                    CommentRecord
//
////////////////////////////////////////////////////////////////////

typedef struct CommentTag
{
	SRecHeader	RecHeader;
    // TODO
} SComment;



class CommentRecord : public AncillaryRecord
{
    public:

        CommentRecord();

        virtual Record* clone() const { return new CommentRecord(); }
        virtual const char* className() const { return "CommentRecord"; }
        virtual int classOpcode() const { return COMMENT_OP; }
        virtual void accept(RecordVisitor& rv) { rv.apply(*this); }
//      virtual void traverse(RecordVisitor& rv);

    protected:

        virtual ~CommentRecord();

        virtual void endian();

};


}; // end namespace flt

#endif

