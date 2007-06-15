//
// OpenFlight� loader for OpenSceneGraph
//
//  Copyright (C) 2005-2006  Brede Johansen
//

#include <osgSim/OpenFlightOptimizer>

#include <osg/Notify>
#include <osg/Geode>

#include <osgUtil/Tessellator>
#include <osgUtil/Optimizer>

#include <vector>
#include <map>

using namespace osgSim;

void Optimizer::optimize(osg::Node* node)
{
    unsigned int options = 0;
    
    const char* env = getenv("OSG_FLIGHTUTIL_OPTIMIZER");
    if (env)
    {
        std::string str(env);

        if(str.find("OFF")!=std::string::npos) options = 0;

        if(str.find("~DEFAULT")!=std::string::npos) options ^= DEFAULT_OPTIMIZATIONS;
        else if(str.find("DEFAULT")!=std::string::npos) options |= DEFAULT_OPTIMIZATIONS;

        if(str.find("~TESSELLATE_POLYGON")!=std::string::npos) options ^= TESSELLATE_POLYGON;
        else if(str.find("TESSELLATE_POLYGON")!=std::string::npos) options |= TESSELLATE_POLYGON;

        if(str.find("~MAKE_LIT")!=std::string::npos) options ^= MAKE_LIT;
        else if(str.find("MAKE_LIT")!=std::string::npos) options |= MAKE_LIT;

        if(str.find("~MERGE_GEODES")!=std::string::npos) options ^= MERGE_GEODES;
        else if(str.find("MERGE_GEODES")!=std::string::npos) options |= MERGE_GEODES;
    }
    else
    {
        options = DEFAULT_OPTIMIZATIONS;
    }

    optimize(node,options);
}

void Optimizer::optimize(osg::Node* node, unsigned int options)
{
    if (options & TESSELLATE_POLYGON)
    {
        osg::notify(osg::INFO)<<"osgFlightUtil::Optimizer::optimize() doing TESSELLATE_POLYGON"<<std::endl;

        TessellateVisitor visitor;
        node->accept(visitor);
    }

    if (options & MAKE_LIT)
    {
        osg::notify(osg::INFO)<<"osgFlightUtil::Optimizer::optimize() doing MAKE_LIT"<<std::endl;

        MakeLitVisitor visitor;
        node->accept(visitor);
    }

    if (options & MERGE_GEODES)
    {
        osg::notify(osg::INFO)<<"osgFlightUtil::Optimizer::optimize() doing MERGE_GEODES"<<std::endl;

        MergeGeodesVisitor visitor;
        node->accept(visitor);
    }
}


void Optimizer::TessellateVisitor::apply(osg::Geode& geode)
{
    for (unsigned int i=0; i<geode.getNumDrawables(); ++i)
    {
        osg::Geometry* geometry = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
        if (geometry)
        {
            if (hasPolygons(*geometry))
            {
                // Tessellate
                osgUtil::Tessellator Tessellator;
                Tessellator.retessellatePolygons(*geometry);
            }
        }
    }
}

bool Optimizer::TessellateVisitor::hasPolygons(osg::Geometry& geometry)
{
    for (unsigned int i=0; i<geometry.getNumPrimitiveSets(); ++i)
    {
        if (geometry.getPrimitiveSet(i)->getMode() == osg::PrimitiveSet::POLYGON)
            return true;
    }

    return false;
}


void Optimizer::MakeLitVisitor::apply(osg::Geode& /*geode*/)
{
#if 0 // TODO
    osg::StateSet* stateset = geode.getStateSet();
    if (stateset)
    {
        // Not lit?
        if (!(stateset->getMode(GL_LIGHTING)&osg::StateAttribute::ON))
        {
            stateset->setMode(GL_LIGHTING, osg::StateAttribute::ON);
            for (unsigned int i=0; i<geode.getNumDrawables(); ++i)
            {
                osg::Geometry* geometry = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
                if (geometry)
                {
                    if () TODO Compare vertex array length and normal array length + normal binding.
                    {
                        // generate normals
                        osgUtil::SmoothingVisitor smoother;
                        smoother.smooth(*geometry);
                    }
                }
            }
        }
    }
#endif
}

/** Need to share stateset before merging geodes.
  */
class GeodeStateOptimizer : public osgUtil::Optimizer::StateVisitor
{
    public:

        GeodeStateOptimizer():
            osgUtil::Optimizer::StateVisitor() {}

        void optimize(osg::Group& group)
        {
            for (unsigned int i=0; i<group.getNumChildren(); ++i)
            {
                osg::Node* child = group.getChild(i);
                if (typeid(*child)==typeid(osg::Geode)) 
                {
                    osg::Geode* geode = (osg::Geode*)child;
                    addStateSet(geode->getStateSet(),geode);
                }
            }

            osgUtil::Optimizer::StateVisitor::optimize();
        }

            
};


void Optimizer::MergeGeodesVisitor::apply(osg::Group& group)
{
    mergeGeodes(group);
    traverse(group);
}


// Requires shared stateset to work. Run GeodeStateOptimizer before MergeGeodesVisitor.
struct LessGeode
{
    bool operator() (const osg::Geode* lhs,const osg::Geode* rhs) const
    {
        if (lhs->getStateSet()<rhs->getStateSet()) return true;
//      if (rhs->getStateSet()<lhs->getStateSet()) return false;
        return false;
    }
};

void Optimizer::MergeGeodesVisitor::mergeGeodes(osg::Group& group)
{
    {
        GeodeStateOptimizer gsopt;
        gsopt.optimize(group);
    }

    typedef std::vector<osg::Geode*>                      DuplicateList;
    typedef std::map<osg::Geode*,DuplicateList,LessGeode> GeodeDuplicateMap;

    GeodeDuplicateMap geodeDuplicateMap;

    for (unsigned int i=0; i<group.getNumChildren(); ++i)
    {
        osg::Node* child = group.getChild(i);
        if (typeid(*child)==typeid(osg::Geode)) 
        {
            osg::Geode* geode = (osg::Geode*)child;
            geodeDuplicateMap[geode].push_back(geode);
        }
    }

    // merge
    for(GeodeDuplicateMap::iterator itr=geodeDuplicateMap.begin();
        itr!=geodeDuplicateMap.end();
        ++itr)
    {
        if (itr->second.size()>1)
        {
            osg::Geode* lhs = itr->second[0];
            for(DuplicateList::iterator dupItr=itr->second.begin()+1;
                dupItr!=itr->second.end();
                ++dupItr)
            {
                osg::Geode* rhs = *dupItr;
                
                if (mergeGeode(*lhs,*rhs))
                {
                    group.removeChild(rhs);

                    static int co = 0;
                    osg::notify(osg::INFO)<<"merged and removed Geode "<<++co<<std::endl;
                }
            }
        }
    }
}

bool Optimizer::MergeGeodesVisitor::mergeGeode(osg::Geode& lhs, osg::Geode& rhs)
{
    for (unsigned int i=0; i<rhs.getNumDrawables(); ++i)
    {
        lhs.addDrawable(rhs.getDrawable(i));
    }

    return true;
}
