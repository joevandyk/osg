// GroupRecord.cpp

#include "flt.h"
#include "Registry.h"
#include "GroupRecord.h"

using namespace flt;

////////////////////////////////////////////////////////////////////
//
//                          GroupRecord
//
////////////////////////////////////////////////////////////////////

RegisterRecordProxy<GroupRecord> g_GroupProxy;

GroupRecord::GroupRecord()
{
}


// virtual
GroupRecord::~GroupRecord()
{
}


void GroupRecord::endian()
{
    SGroup *pSGroup = (SGroup*)getData();

    ENDIAN( pSGroup->iGroupRelPriority );
    ENDIAN( pSGroup->dwFlags );
    ENDIAN( pSGroup->iSpecialId_1 );
    ENDIAN( pSGroup->iSpecialId_2 );
    ENDIAN( pSGroup->iSignificance );
}
