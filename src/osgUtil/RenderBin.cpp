/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2005 Robert Osfield 
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
#include <osgUtil/RenderBin>
#include <osgUtil/RenderStage>
#include <osgUtil/Statistics>

#include <osg/Notify>
#include <osg/ApplicationUsage>

#include <algorithm>

using namespace osg;
using namespace osgUtil;


class RenderBinPrototypeList : public osg::Referenced, public std::map< std::string, osg::ref_ptr<RenderBin> > 
{
    public:
        RenderBinPrototypeList() {}
        ~RenderBinPrototypeList() {}
};

// register a RenderStage prototype with the RenderBin prototype list.
RegisterRenderBinProxy s_registerRenderBinProxy("RenderBin",new RenderBin(RenderBin::getDefaultRenderBinSortMode()));
RegisterRenderBinProxy s_registerDepthSortedBinProxy("DepthSortedBin",new RenderBin(RenderBin::SORT_BACK_TO_FRONT));


static RenderBinPrototypeList* renderBinPrototypeList()
{
    static osg::ref_ptr<RenderBinPrototypeList> s_renderBinPrototypeList = new  RenderBinPrototypeList;
    return s_renderBinPrototypeList.get();
}

RenderBin* RenderBin::getRenderBinPrototype(const std::string& binName)
{
    RenderBinPrototypeList* list = renderBinPrototypeList();
    if (list)
    {
        RenderBinPrototypeList::iterator itr = list->find(binName);
        if (itr != list->end()) return itr->second.get();
    }
    return NULL;
}

RenderBin* RenderBin::createRenderBin(const std::string& binName)
{
    RenderBinPrototypeList* list = renderBinPrototypeList();
    if (list)
    {
        RenderBin* prototype = getRenderBinPrototype(binName);
        if (prototype) return dynamic_cast<RenderBin*>(prototype->clone(osg::CopyOp::DEEP_COPY_ALL));
    }
    
    osg::notify(osg::WARN) <<"Warning: RenderBin \""<<binName<<"\" implemention not found, using default RenderBin as a fallback."<<std::endl;
    return new RenderBin;
}

void RenderBin::addRenderBinPrototype(const std::string& binName,RenderBin* proto)
{
    RenderBinPrototypeList* list = renderBinPrototypeList();
    if (list && proto)
    {
        (*list)[binName] = proto;
    }
}

void RenderBin::removeRenderBinPrototype(RenderBin* proto)
{
    RenderBinPrototypeList* list = renderBinPrototypeList();
    if (list && proto)
    {
        RenderBinPrototypeList::iterator itr = list->find(proto->className());
        if (itr != list->end()) list->erase(itr);
    }
}

static bool s_defaultBinSortModeInitialized = false;
static RenderBin::SortMode s_defaultBinSortMode = RenderBin::SORT_BY_STATE;
static osg::ApplicationUsageProxy RenderBin_e0(osg::ApplicationUsage::ENVIRONMENTAL_VARIABLE,"OSG_DEFAULT_BIN_SORT_MODE <type>","SORT_BY_STATE | SORT_BY_STATE_THEN_FRONT_TO_BACK | SORT_FRONT_TO_BACK | SORT_BACK_TO_FRONT");

void RenderBin::setDefaultRenderBinSortMode(RenderBin::SortMode mode)
{
    s_defaultBinSortModeInitialized = true;
    s_defaultBinSortMode = mode;
}


RenderBin::SortMode RenderBin::getDefaultRenderBinSortMode()
{
    if (!s_defaultBinSortModeInitialized)
    {
        s_defaultBinSortModeInitialized = true;
        
        const char* str = getenv("OSG_DEFAULT_BIN_SORT_MODE");
        if (str)
        {
            if (strcmp(str,"SORT_BY_STATE")==0) s_defaultBinSortMode = RenderBin::SORT_BY_STATE;
            else if (strcmp(str,"SORT_BY_STATE_THEN_FRONT_TO_BACK")==0) s_defaultBinSortMode = RenderBin::SORT_BY_STATE_THEN_FRONT_TO_BACK;
            else if (strcmp(str,"SORT_FRONT_TO_BACK")==0) s_defaultBinSortMode = RenderBin::SORT_FRONT_TO_BACK;
            else if (strcmp(str,"SORT_BACK_TO_FRONT")==0) s_defaultBinSortMode = RenderBin::SORT_BACK_TO_FRONT;
        }
    }
    
    return s_defaultBinSortMode;
}

RenderBin::RenderBin()
{
    _binNum = 0;
    _parent = NULL;
    _stage = NULL;
    _sortMode = getDefaultRenderBinSortMode();
}

RenderBin::RenderBin(SortMode mode)
{
    _binNum = 0;
    _parent = NULL;
    _stage = NULL;
    _sortMode = mode;
}

RenderBin::RenderBin(const RenderBin& rhs,const CopyOp& copyop):
        Object(rhs,copyop),
        _binNum(rhs._binNum),
        _parent(rhs._parent),
        _stage(rhs._stage),
        _bins(rhs._bins),
        _stateGraphList(rhs._stateGraphList),
        _renderLeafList(rhs._renderLeafList),
        _sortMode(rhs._sortMode),
        _sortCallback(rhs._sortCallback),
        _drawCallback(rhs._drawCallback)
{

}

RenderBin::~RenderBin()
{
}

void RenderBin::reset()
{
    _stateGraphList.clear();
    _bins.clear();
}

void RenderBin::sort()
{
    for(RenderBinList::iterator itr = _bins.begin();
        itr!=_bins.end();
        ++itr)
    {
        itr->second->sort();
    }
    
    if (_sortCallback.valid()) 
    {
        _sortCallback->sortImplementation(this);
    }
    else sortImplementation();
}

void RenderBin::setSortMode(SortMode mode)
{
    _sortMode = mode;
}

void RenderBin::sortImplementation()
{
    switch(_sortMode)
    {
        case(SORT_BY_STATE):
            sortByState();
            break;
        case(SORT_BY_STATE_THEN_FRONT_TO_BACK):
            sortByStateThenFrontToBack();
            break;
        case(SORT_FRONT_TO_BACK):
            sortFrontToBack();
            break;
        case(SORT_BACK_TO_FRONT):
            sortBackToFront();
            break;
        default:
            break;
    }
}

struct SortByStateFunctor
{
    bool operator() (const StateGraph* lhs,const StateGraph* rhs) const
    {
        return (*(lhs->_stateset)<*(rhs->_stateset));
    }
};

void RenderBin::sortByState()
{
    //osg::notify(osg::NOTICE)<<"sortByState()"<<std::endl;
    // actually we'll do nothing right now, as fine grained sorting by state
    // appears to cost more to do than it saves in draw.  The contents of
    // the StateGraph leaves is already coarse grained sorted, this
    // sorting is as a function of the cull traversal.
    // cout << "doing sortByState "<<this<<endl;
}


struct StateGraphFrontToBackSortFunctor
{
    bool operator() (const StateGraph* lhs,const StateGraph* rhs) const
    {
        return (lhs->_minimumDistance<rhs->_minimumDistance);
    }
};

void RenderBin::sortByStateThenFrontToBack()
{
    for(StateGraphList::iterator itr=_stateGraphList.begin();
        itr!=_stateGraphList.end();
        ++itr)
    {
        (*itr)->sortFrontToBack();
        (*itr)->getMinimumDistance();
    }
    std::sort(_stateGraphList.begin(),_stateGraphList.end(),StateGraphFrontToBackSortFunctor());
}

struct FrontToBackSortFunctor
{
    bool operator() (const RenderLeaf* lhs,const RenderLeaf* rhs) const
    {
        return (lhs->_depth<rhs->_depth);
    }
};

    
void RenderBin::sortFrontToBack()
{
    copyLeavesFromStateGraphListToRenderLeafList();

    // now sort the list into acending depth order.
    std::sort(_renderLeafList.begin(),_renderLeafList.end(),FrontToBackSortFunctor());
    
//    cout << "sort front to back"<<endl;
}

struct BackToFrontSortFunctor
{
    bool operator() (const RenderLeaf* lhs,const RenderLeaf* rhs) const
    {
        return (rhs->_depth<lhs->_depth);
    }
};

void RenderBin::sortBackToFront()
{
    copyLeavesFromStateGraphListToRenderLeafList();

    // now sort the list into acending depth order.
    std::sort(_renderLeafList.begin(),_renderLeafList.end(),BackToFrontSortFunctor());

//    cout << "sort back to front"<<endl;
}

void RenderBin::copyLeavesFromStateGraphListToRenderLeafList()
{
    _renderLeafList.clear();

    int totalsize=0;
    StateGraphList::iterator itr;
    for(itr=_stateGraphList.begin();
        itr!=_stateGraphList.end();
        ++itr)
    {
        totalsize += (*itr)->_leaves.size();
    }

    _renderLeafList.reserve(totalsize);
    
    // first copy all the leaves from the render graphs into the leaf list.
    for(itr=_stateGraphList.begin();
        itr!=_stateGraphList.end();
        ++itr)
    {
        for(StateGraph::LeafList::iterator dw_itr = (*itr)->_leaves.begin();
            dw_itr != (*itr)->_leaves.end();
            ++dw_itr)
        {
            _renderLeafList.push_back(dw_itr->get());
        }
    }
    
    // empty the render graph list to prevent it being drawn along side the render leaf list (see drawImplementation.)
    _stateGraphList.clear();
}

RenderBin* RenderBin::find_or_insert(int binNum,const std::string& binName)
{
    // search for appropriate bin.
    RenderBinList::iterator itr = _bins.find(binNum);
    if (itr!=_bins.end()) return itr->second.get();

    // create a renderin bin and insert into bin list.
    RenderBin* rb = RenderBin::createRenderBin(binName);
    if (rb)
    {

        RenderStage* rs = dynamic_cast<RenderStage*>(rb);
        if (rs)
        {
            rs->_binNum = binNum;
            rs->_parent = NULL;
            rs->_stage = rs;
            _stage->addPreRenderStage(rs);
        }
        else
        {
            rb->_binNum = binNum;
            rb->_parent = this;
            rb->_stage = _stage;
            _bins[binNum] = rb;
        }
    }
    return rb;
}

void RenderBin::draw(osg::State& state,RenderLeaf*& previous)
{
    if (_drawCallback.valid()) 
    {
        _drawCallback->drawImplementation(this,state,previous);
    }
    else drawImplementation(state,previous);
}

void RenderBin::drawImplementation(osg::State& state,RenderLeaf*& previous)
{
    // osg::notify(osg::NOTICE)<<"begin RenderBin::drawImplementation "<<className()<<" sortMode "<<getSortMode()<<std::endl;

    // draw first set of draw bins.
    RenderBinList::iterator rbitr;
    for(rbitr = _bins.begin();
        rbitr!=_bins.end() && rbitr->first<0;
        ++rbitr)
    {
        rbitr->second->draw(state,previous);
    }
    

    // draw fine grained ordering.
    for(RenderLeafList::iterator rlitr= _renderLeafList.begin();
        rlitr!= _renderLeafList.end();
        ++rlitr)
    {
        RenderLeaf* rl = *rlitr;
        rl->render(state,previous);
        previous = rl;
    }


    // draw coarse grained ordering.
    for(StateGraphList::iterator oitr=_stateGraphList.begin();
        oitr!=_stateGraphList.end();
        ++oitr)
    {

        for(StateGraph::LeafList::iterator dw_itr = (*oitr)->_leaves.begin();
            dw_itr != (*oitr)->_leaves.end();
            ++dw_itr)
        {
            RenderLeaf* rl = dw_itr->get();
            rl->render(state,previous);
            previous = rl;

        }
    }


    // draw post bins.
    for(;
        rbitr!=_bins.end();
        ++rbitr)
    {
        rbitr->second->draw(state,previous);
    }

    //osg::notify(osg::NOTICE)<<"end RenderBin::drawImplementation "<<className()<<std::endl;
}

// stats
bool RenderBin::getStats(Statistics* primStats)
{
    // different by return type - collects the stats in this renderrBin
    bool somestats=false;

    // draw fine grained ordering.
    for(RenderLeafList::iterator dw_itr = _renderLeafList.begin();
        dw_itr != _renderLeafList.end();
        ++dw_itr)
    {
        RenderLeaf* rl = *dw_itr;
        Drawable* dw= rl->_drawable;
        primStats->addDrawable(); // number of geosets
        if (rl->_modelview.get())
        {
            primStats->addMatrix(); // number of matrices
        }
        
        if (dw)
        {
              // then tot up the primtive types and no vertices.
              dw->accept(*primStats); // use sub-class to find the stats for each drawable
        }
        somestats = true;

    }

    for(StateGraphList::iterator oitr=_stateGraphList.begin();
        oitr!=_stateGraphList.end();
        ++oitr)
    {
        
        for(StateGraph::LeafList::iterator dw_itr = (*oitr)->_leaves.begin();
            dw_itr != (*oitr)->_leaves.end();
            ++dw_itr)
        {
            RenderLeaf* rl = dw_itr->get();
            Drawable* dw= rl->_drawable;
            primStats->addDrawable(); // number of geosets
            if (rl->_modelview.get()) primStats->addMatrix(); // number of matrices
            if (dw)
            {
                // then tot up the primtive types and no vertices.
                dw->accept(*primStats); // use sub-class to find the stats for each drawable
            }
        }
        somestats=true;
    }
    return somestats;
}

void RenderBin::getPrims(Statistics* primStats)
{
    static int ndepth;
    ndepth++;
    for(RenderBinList::iterator itr = _bins.begin();
        itr!=_bins.end();
        ++itr)
    {
        primStats->addBins(1);
        itr->second->getPrims(primStats);
    }
    getStats(primStats);
    ndepth--;

}

bool RenderBin::getPrims(Statistics* primStats, int nbin)
{ // collect stats for array of bins, maximum nbin 
    // (which will be modified on next call if array of primStats is too small);
    // return 1 for OK;
    static int ndepth;
    bool ok=false;
    ndepth++;
    int nb=primStats[0].getBins();
    if (nb<nbin)
    { // if statement to protect against writing to bins beyond the maximum seen before
        primStats[nb].setBinNo(nb);
        primStats[nb].setDepth(ndepth);
        getStats(primStats+nb);
    }
    primStats[0].addBins(1);
    for(RenderBinList::iterator itr = _bins.begin();
        itr!=_bins.end();
        ++itr)
    {
        if (itr->second->getPrims(primStats, nbin)) ok = true;
    }
    ok=true;
    ndepth--;
    return ok;
}
