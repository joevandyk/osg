//C++ header - Open Scene Graph Simulation - Copyright (C) 1998-2002 Robert Osfield
// Distributed under the terms of the GNU General Public License (GPL)
// as published by the Free Software Foundation.
//
// All software using osgSim must be GPL'd or excempted via the 
// purchase of the Open Scene Graph Professional License (OSGPL)
// for further information contact robert@openscenegraph.com.

#ifndef OSGSIM_LIGHTPOINTDRAWABLE
#define OSGSIM_LIGHTPOINTDRAWABLE 1

#include <osgSim/Export>

#include <osg/Drawable>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Depth>
#include <osg/BlendFunc>
#include <osg/ColorMask>
#include <osg/Point>
#include <osg/Endian>

#include <vector>

namespace osgSim {


class OSGSIM_EXPORT LightPointDrawable : public osg::Drawable
{
    public :

        LightPointDrawable();
        
        /** Copy constructor using CopyOp to manage deep vs shallow copy.*/
        LightPointDrawable(const LightPointDrawable&,const osg::CopyOp& copyop=osg::CopyOp::SHALLOW_COPY);

        virtual osg::Object* cloneType() const { return new LightPointDrawable(); }
        virtual osg::Object* clone(const osg::CopyOp&) const { return new LightPointDrawable(); }
        virtual bool isSameKindAs(const osg::Object* obj) const { return dynamic_cast<const LightPointDrawable*>(obj)!=NULL; }
        virtual const char* className() const { return "LightPointDrawable"; }

        
        //typedef std::pair<unsigned long,osg::Vec3> ColorPosition;
        struct ColorPosition
        {
            unsigned long first;
            osg::Vec3 second;
            ColorPosition() {}
            ColorPosition(unsigned long f,const osg::Vec3& s):first(f),second(s) {}
        };
        
        void reset();

        inline unsigned long asRGBA(const osg::Vec4& color) const
        {
            return _endian==osg::BigEndian?color.asABGR():color.asRGBA();
        }

        inline void addOpaqueLightPoint(unsigned int pointSize,const osg::Vec3& position,const osg::Vec4& color)
        {
            if (pointSize>=_sizedOpaqueLightPointList.size()) _sizedOpaqueLightPointList.resize(pointSize+1);
            _sizedOpaqueLightPointList[pointSize].push_back(ColorPosition(asRGBA(color),position));
        }

        inline void addAdditiveLightPoint(unsigned int pointSize,const osg::Vec3& position,const osg::Vec4& color)
        {
            if (pointSize>=_sizedAdditiveLightPointList.size()) _sizedAdditiveLightPointList.resize(pointSize+1);
            _sizedAdditiveLightPointList[pointSize].push_back(ColorPosition(asRGBA(color),position));
        }

        inline void addBlendedLightPoint(unsigned int pointSize,const osg::Vec3& position,const osg::Vec4& color)
        {
            if (pointSize>=_sizedBlendedLightPointList.size()) _sizedBlendedLightPointList.resize(pointSize+1);
            _sizedBlendedLightPointList[pointSize].push_back(ColorPosition(asRGBA(color),position));
        }
        
        /** draw LightPoints. */
        virtual void drawImplementation(osg::State& state) const;


        void setReferenceTime(double time)
        {
            _referenceTime = time;
            _referenceTimeInterval = 0.0;
        }

        void updateReferenceTime(double time)
        {
            _referenceTimeInterval = osg::clampAbove(time-_referenceTime,0.0);
            _referenceTime = time;
        }
        
        double getReferenceTime() const { return _referenceTime; }
        double getReferenceTimeInterval() const { return _referenceTimeInterval; }
        
    protected:
    
        virtual bool computeBound() const;

        ~LightPointDrawable() {}
        
        osg::Endian                     _endian;
               
        double                          _referenceTime;
        double                          _referenceTimeInterval;
        
        typedef std::vector<ColorPosition>  LightPointList;
        typedef std::vector<LightPointList> SizedLightPointList;

        SizedLightPointList             _sizedOpaqueLightPointList;
        SizedLightPointList             _sizedAdditiveLightPointList;
        SizedLightPointList             _sizedBlendedLightPointList;

        osg::ref_ptr<osg::Depth>        _depthOff;
        osg::ref_ptr<osg::Depth>        _depthOn;
        osg::ref_ptr<osg::BlendFunc>    _blendOne;
        osg::ref_ptr<osg::BlendFunc>    _blendOneMinusSrcAlpha;
        osg::ref_ptr<osg::ColorMask>    _colorMaskOff;

        
};

}

#endif
