/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2003 Robert Osfield 
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

#ifndef DATASET_H
#define DATASET_H 1

#include <osg/Node>
#include <osg/Matrixd>
#include <osg/BoundingBox>
#include <osg/Image>
#include <osg/Shape>

#include <osgTerrain/CoordinateSystem>

#include <set>

#include <gdal_priv.h>

class DataSet : public osg::Referenced
{
    public:

        class Source;
        

        struct SpatialProperties
        {
            SpatialProperties():
                _numValuesX(0),
                _numValuesY(0),
                _numValuesZ(0) {}
        
            SpatialProperties(const SpatialProperties& sp):
                _cs(sp._cs),
                _geoTransform(sp._geoTransform),
                _extents(sp._extents),
                _numValuesX(sp._numValuesX),
                _numValuesY(sp._numValuesY),
                _numValuesZ(sp._numValuesZ) {}

            SpatialProperties(osgTerrain::CoordinateSystem* cs, const osg::BoundingBox& extents):
                _cs(cs),
                _extents(extents),
                _numValuesX(0),
                _numValuesY(0),
                _numValuesZ(0) {}
        
            SpatialProperties& operator = (const SpatialProperties& sp)
            {
                if (&sp==this) return *this;
                
                _cs = sp._cs;
                _geoTransform = sp._geoTransform;
                _extents = sp._extents;
                _numValuesX = sp._numValuesX;
                _numValuesY = sp._numValuesY;
                _numValuesZ = sp._numValuesZ;
                
                return *this;
            }

            osg::ref_ptr<osgTerrain::CoordinateSystem>  _cs;
            osg::Matrixd                                _geoTransform;
            osg::BoundingBox                            _extents;
            unsigned int                                _numValuesX;
            unsigned int                                _numValuesY;
            unsigned int                                _numValuesZ;
        };

        struct DestinationData : public osg::Referenced, SpatialProperties
        {
        
            DestinationData():
                _minDistance(0.0),
                _maxDistance(FLT_MAX) {}
        
            
            typedef std::vector< osg::ref_ptr<osg::Node> > ModelList;
            
            float                                       _minDistance;
            float                                       _maxDistance;

            osg::ref_ptr<osg::Image>                    _image;
            osg::ref_ptr<osg::HeightField>              _heightField;
            ModelList                                   _models;
        };

        struct SourceData : public osg::Referenced, public SpatialProperties
        {
        
            SourceData(Source* source=0):
                _source(source),
                _hasGCPs(false),
                _gdalDataSet(0) {}
                
                
            virtual ~SourceData()
            {
                if (_gdalDataSet) GDALClose(_gdalDataSet);
            }

            static SourceData* readData(Source* source);
            
            osg::BoundingBox getExtents(const osgTerrain::CoordinateSystem* cs) const;
            
            const SpatialProperties& computeSpatialProperties(osgTerrain::CoordinateSystem* cs) const;

            bool intersects(const SpatialProperties& sp) const;

            void read(DestinationData& destination);
            
            void readImage(DestinationData& destination);
            void readHeightField(DestinationData& destination);
            void readModels(DestinationData& destination);

            Source*                                     _source;
            
            bool                                        _hasGCPs;
            
            osg::ref_ptr<osg::Node>                     _model;
            GDALDataset*                                _gdalDataSet;
            
            typedef std::map<osgTerrain::CoordinateSystem*,SpatialProperties> SpatialPropertiesMap;
            mutable SpatialPropertiesMap _spatialPropertiesMap;
            
            
        };


        class Source : public osg::Referenced, public SpatialProperties
        {
        public:
        
            enum Type
            {
                IMAGE,
                HEIGHT_FIELD,
                MODEL
            };
            
            enum ParameterPolicy
            {
                PREFER_CONFIG_SETTINGS,
                PREFER_FILE_SETTINGS
            };

            Source():
                _type(IMAGE),
                _sortValue(0.0),
                _temporaryFile(false),
                _coordinateSystemPolicy(PREFER_FILE_SETTINGS),
                _geoTransformPolicy(PREFER_FILE_SETTINGS)
                {}
        
            Source(Type type, const std::string& filename):
                _type(type),
                _sortValue(0.0),
                _filename(filename),
                _temporaryFile(false),
                _coordinateSystemPolicy(PREFER_FILE_SETTINGS),
                _geoTransformPolicy(PREFER_FILE_SETTINGS)
                {}

            void setSortValue(double s) { _sortValue = s; }
            double getSortValue() const { return _sortValue; }

            void setSortValueFromSourceDataResolution();

            void setType(Type type) { _type = type; }
            Type getType() const { return _type; }

            void setFileName(const std::string& filename) { _filename = filename; }
            const std::string& getFileName() const { return _filename; }

            void setTemporaryFile(bool temporaryFile) { _temporaryFile = temporaryFile; }
            bool getTemporaryFile() const { return _temporaryFile; }

            void setCoordinateSystemPolicy(ParameterPolicy policy) { _coordinateSystemPolicy = policy; }
            void setCoordinateSystem(osgTerrain::CoordinateSystem* cs) { _cs = cs; }
            osgTerrain::CoordinateSystem* getCoordinateSystem() { return  _cs.get(); }

            
            void setGeoTransformPolicy(ParameterPolicy policy)  { _geoTransformPolicy = policy; }

            void setGeoTransform(osg::Matrixd& transform) { _geoTransform = transform; }
            osg::Matrixd& getGeoTransform() { return _geoTransform; }
            
            
            void setSourceData(SourceData* data) { _sourceData = data; if (_sourceData.valid()) _sourceData->_source = this; }
            SourceData* getSourceData() { return _sourceData.get(); }
            
            bool intersects(const SpatialProperties& sp) const
            {
                return  _sourceData.valid()?_sourceData->intersects(sp):false;
            }

            void loadSourceData();

            
            bool needReproject(const osgTerrain::CoordinateSystem* cs) const;

            bool needReproject(const osgTerrain::CoordinateSystem* cs, double minResolution, double maxResolution) const;
            
            Source* doReproject(const std::string& filename, osgTerrain::CoordinateSystem* cs, double targetResolution=0.0) const;
           
            void buildOverviews();
            
            
            struct ResolutionPair
            {
                ResolutionPair():
                    _resX(0.0),_resY(0.0) {}

                ResolutionPair(double x,double y):
                    _resX(x),_resY(y) {}
                    
                bool operator < (const ResolutionPair& rhs) const
                {
                    double minLHS = osg::minimum(_resX,_resY);
                    double minRHS = osg::minimum(rhs._resX,rhs._resY);
                    return minLHS<minRHS;
                }
            
                double _resX;
                double _resY;
            };
            
            typedef std::vector<ResolutionPair> ResolutionList;
            
            void addRequiredResolution(double resX, double resY) { _requiredResolutions.push_back(ResolutionPair(resX,resY)); }
            
            void setRequiredResolutions(ResolutionList& resolutions) { _requiredResolutions = resolutions; }
            
            ResolutionList& getRequiredResolutions() { return _requiredResolutions; }
            
            const ResolutionList& getRequiredResolutions() const { return _requiredResolutions; }
            
            void consolodateRequiredResolutions();

        protected:
        

            Type                                        _type;

            double                                      _sortValue;
        
            std::string                                 _filename;
            bool                                        _temporaryFile;
            
            ParameterPolicy                             _coordinateSystemPolicy;
            ParameterPolicy                             _geoTransformPolicy;
            
            osg::ref_ptr<SourceData>                    _sourceData;
            
            ResolutionList                              _requiredResolutions;
                
        };
        
        enum CompositeType
        {
            GROUP,
            LOD,
            PAGED_LOD
        };
        
        class CompositeSource : public osg::Referenced, public SpatialProperties
        {
        public:
            
            CompositeSource(CompositeType type=GROUP):_type(type) {};
            
            typedef std::vector< osg::ref_ptr<Source> > SourceList;
            typedef std::vector< osg::ref_ptr< CompositeSource> > ChildList;
            
            void setType(CompositeType type) { _type = type; }
            CompositeType getType() const { return _type; }

            void setSortValueFromSourceDataResolution();

            void sort();

            class iterator
            {
            public:

                enum IteratorMode
                {
                    ACTIVE,
                    ALL
                };
            

                iterator(CompositeSource* composite=0,IteratorMode mode=ALL):
                    _iteratorMode(mode)
                {
                    if (composite) 
                    {
                        _positionStack.push_back(IteratorPosition(composite));
                    }
                }
                
                iterator(const iterator& rhs):
                    _positionStack(rhs._positionStack) {}

                iterator& operator = (const iterator& rhs)
                {
                    if (&rhs==this) return *this;
                    _positionStack = rhs._positionStack;
                }
                
                bool operator == (const iterator& rhs) const
                {
                    return _positionStack == rhs._positionStack;
                }

                bool operator != (const iterator& rhs) const
                {
                    return _positionStack != rhs._positionStack;
                }

                bool valid() const
                {
                    return !_positionStack.empty() && _positionStack.back().valid();
                }
                                    
                CompositeSource& operator *()
                {
                    return *(valid()?_positionStack.back().current():0);
                }
                
                CompositeSource* operator ->()
                {
                    return valid()?_positionStack.back().current():0;
                }
                
                const CompositeSource& operator *() const
                {
                    return *(valid()?_positionStack.back().current():0);
                }
                
                const CompositeSource* operator ->() const
                {
                    return valid()?_positionStack.back().current():0;
                }

                iterator& operator++()
                {
                    advance(); 
                    return *this;
                }
                
                iterator operator++(int)
                {
                    iterator tmp=*this; 
                    advance(); 
                    return tmp; 
                }
                
                bool advance()
                {
                    if (_positionStack.empty()) return false;
                    
                    // simple advance to the next source
                    if (_positionStack.back().advance())
                    {
                        if (_positionStack.back().current())
                        {
                            _positionStack.push_back(IteratorPosition(_positionStack.back().current()));
                            return advance();
                        }
                    }
 
                    _positionStack.pop_back();
                    return advance();
                }

                
            protected:
            
                struct IteratorPosition
                {
                
                    IteratorPosition(CompositeSource* composite):
                        _composite(composite),
                        _index(-1) {}

                    IteratorPosition(const IteratorPosition& rhs):
                        _composite(rhs._composite),
                        _index(rhs._index) {}

                    IteratorPosition& operator = (const IteratorPosition& rhs)
                    {
                        _composite = rhs._composite;
                        _index = rhs._index;
                        return *this;
                    }

                    bool operator == (const IteratorPosition& rhs) const
                    {
                        return _composite == rhs._composite && _index == rhs._index;
                    }
                
                    bool operator != (const IteratorPosition& rhs) const
                    {
                        return _composite != rhs._composite || _index != rhs._index;
                    }

                    CompositeSource* current()
                    {
                        if (_index==-1) return _composite;
                        else return  (_index>=0 && _index < (int)_composite->_children.size())?_composite->_children[_index].get():0;
                    }

                    const CompositeSource* current() const
                    {
                        if (_index==-1) return _composite;
                        else return  (_index>=0 && _index < (int)_composite->_children.size())?_composite->_children[_index].get():0;
                    }

                    bool valid() const
                    {
                        return _composite && 
                               _index < (int)_composite->_children.size();
                    }
                    
                    inline bool advance()
                    {
                        return advanceToNextChild(*_composite,_index);
                    }

                    inline bool isActive(const CompositeSource& composite,int index)
                    {
                        return true;
                    }

                    inline bool advanceToNextChild(CompositeSource& composite, int& index)
                    {
                        ++index;
                        while (index<(int)composite._children.size()) 
                        {
                            if (isActive(composite,index)) return true;
                            ++index;
                        }
                        return false;
                    }

                    CompositeSource*                _composite;
                    int                             _index;
                };

                typedef std::vector<IteratorPosition> PositionStack;
                IteratorMode                        _iteratorMode;
                PositionStack                       _positionStack;
            };
        

            template<class T>
            class base_source_iterator
            {
            public:
            

                base_source_iterator(CompositeSource* composite=0, T advancer=T()):
                    _advancer(advancer),
                    _compositeIterator(composite),
                    _sourceIndex(-1)
                {
                    advance();
                }
                
                base_source_iterator(const base_source_iterator& rhs):
                    _advancer(rhs._advancer),
                    _compositeIterator(rhs._compositeIterator),
                    _sourceIndex(rhs._sourceIndex) {}

                base_source_iterator& operator = (const base_source_iterator& rhs)
                {
                    if (&rhs==this) return *this;
                    _advancer = rhs._advancer;
                    _compositeIterator = rhs._compositeIterator;
                    _sourceIndex = rhs._sourceIndex;
                }
                
                bool operator == (const base_source_iterator& rhs) const
                {
                    return _compositeIterator == rhs._compositeIterator &&
                           _sourceIndex == rhs._sourceIndex;
                }

                bool operator != (const base_source_iterator& rhs) const
                {
                    return _compositeIterator != rhs._compositeIterator ||
                           _sourceIndex != rhs._sourceIndex;
                }

                bool valid() const
                {
                    return _compositeIterator.valid() && _sourceIndex < (int)_compositeIterator->_sourceList.size();
                }
                                    
                osg::ref_ptr<Source>& operator *()
                {
                    return valid()?_compositeIterator->_sourceList[_sourceIndex]:_nullSource;
                }
                
                osg::ref_ptr<Source>* operator ->()
                {
                    return &(valid()?_compositeIterator->_sourceList[_sourceIndex]:_nullSource);
                }
                
                base_source_iterator& operator++()
                {
                    advance();
                    return *this;
                }
                
                base_source_iterator operator++(int)
                {
                    base_source_iterator tmp=*this; 
                    advance(); 
                    return tmp; 
                }
                
                bool advance()
                {
                    if (!_compositeIterator.valid()) return false;
                    
                    if (_advancer.advanceToNextSource(*_compositeIterator,_sourceIndex)) return true;

                    // at end of current CompositeSource, so need to advance to new one.
                    _sourceIndex = -1;
                    ++_compositeIterator;
                    return advance();
                }
                               
            protected:
            
                T                       _advancer;
                iterator                _compositeIterator;
                int                     _sourceIndex;
                osg::ref_ptr<Source>    _nullSource;

            };

            struct DefaultSourceAdvancer
            {
                DefaultSourceAdvancer() {}
                
                bool isActive(const CompositeSource& composite,int index)
                {
                    return true;
                }

                inline bool advanceToNextSource(const CompositeSource& composite, int& index)
                {
                    return ++index<(int)composite._sourceList.size();
                }
            };
        
            struct LODSourceAdvancer
            {
                LODSourceAdvancer(float targetResolution=0.0f):
                    _targetResolution(targetResolution) {}
                
                inline bool advanceToNextSource(const CompositeSource& composite, int& index)
                {
                    if (composite.getType()==GROUP)
                    {
                        return (++index<(int)composite._sourceList.size());
                    }
                    else
                    {
                        if (composite._sourceList.empty()) return false;
                        if (index!=-1) return false; // we've already traversed this composite, only ever one valid LOD.
                    
                        // find source with resolution closest to target
                        int foundIndex = 0;
                        float closestResolution = fabsf(composite._sourceList[0]->getSortValue()-_targetResolution);
                        for(int i=1;i<(int)composite._sourceList.size();++i)
                        {
                            float delta = fabsf(composite._sourceList[i]->getSortValue()-_targetResolution);
                            if (delta<closestResolution)
                            {
                                foundIndex = i;
                                closestResolution = delta;
                            }
                        }
                        if (foundIndex==index) return false;
                        index = foundIndex;
                        return true;
                    }
                }
                
                float _targetResolution;
            };

            typedef base_source_iterator<DefaultSourceAdvancer> source_iterator;
            typedef base_source_iterator<LODSourceAdvancer>     source_lod_iterator;
           
            CompositeType   _type;
            SourceList      _sourceList;
            ChildList       _children;
        };
        
        
        class DestinationTile : public osg::Referenced, public SpatialProperties
        {
        public:
        
        
            enum Position
            {
                LEFT        = 0,
                LEFT_BELOW  = 1,
                BELOW       = 2,
                BELOW_RIGHT = 3,
                RIGHT       = 4,
                RIGHT_ABOVE = 5,
                ABOVE       = 6,
                ABOVE_LEFT  = 7,
                NUMBER_OF_POSITIONS = 8
            };
        
        
            DestinationTile();

            void setNeighbours(DestinationTile* left, DestinationTile* left_below, 
                               DestinationTile* below, DestinationTile* below_right,
                               DestinationTile* right, DestinationTile* right_above,
                               DestinationTile* above, DestinationTile* above_left);
                               
            void checkNeighbouringTiles();
            
            void setMaximumImagerySize(unsigned int maxNumColumns,unsigned int maxNumRows)
            {
                _imagery_maxNumColumns = maxNumColumns;
                _imagery_maxNumRows = maxNumRows;
            }

            void setMaximumTerrainSize(unsigned int maxNumColumns,unsigned int maxNumRows)
            {
                _terrain_maxNumColumns = maxNumColumns;
                _terrain_maxNumRows = maxNumRows;
            }
            
            void computeMaximumSourceResolution(CompositeSource* sourceGraph);

            bool computeImageResolution(unsigned int& numColumns, unsigned int& numRows, double& resX, double& resY);
            bool computeTerrainResolution(unsigned int& numColumns, unsigned int& numRows, double& resX, double& resY);
            
            void allocate();
            
            void addRequiredResolutions(CompositeSource* sourceGraph);

            void readFrom(CompositeSource* sourceGraph);
            
            void equalizeCorner(Position position);
            void equalizeEdge(Position position);
            

            void equalizeBoundaries();

            osg::Node* createScene();
            
            
            std::string                                 _name;
            
            osg::ref_ptr<DestinationData>               _imagery;
            osg::ref_ptr<DestinationData>               _terrain;
            osg::ref_ptr<DestinationData>               _models;
            
            DestinationTile*                            _neighbour[8];
            bool                                        _equalized[8];
            
            
            unsigned int                                _imagery_maxNumColumns;
            unsigned int                                _imagery_maxNumRows;
            float                                       _imagery_maxSourceResolutionX;
            float                                       _imagery_maxSourceResolutionY;
            
            unsigned int                                _terrain_maxNumColumns;
            unsigned int                                _terrain_maxNumRows;
            float                                       _terrain_maxSourceResolutionX;
            float                                       _terrain_maxSourceResolutionY;
            
            unsigned int                                _level;
            unsigned int                                _X;
            unsigned int                                _Y;

        };

        class CompositeDestination : public osg::Referenced, public SpatialProperties
        {
        public:   
        
            CompositeDestination():
                _type(GROUP),
                _maxVisibleDistance(FLT_MAX) {}
        
            CompositeDestination(osgTerrain::CoordinateSystem* cs, const osg::BoundingBox& extents):
                SpatialProperties(cs,extents),
                _type(GROUP),
                _maxVisibleDistance(FLT_MAX) {}
          
            void addRequiredResolutions(CompositeSource* sourceGraph);
            
            void readFrom(CompositeSource* sourceGraph);

            void equalizeBoundaries();

            osg::Node* createScene();

            typedef std::vector< osg::ref_ptr<DestinationTile> > TileList;
            typedef std::vector< osg::ref_ptr<CompositeDestination> > ChildList;

            CompositeType   _type;
            TileList        _tiles;
            ChildList       _children;
            float           _maxVisibleDistance;
            
        };

    public:


        DataSet();

        void addSource(Source* source);
        void addSource(CompositeSource* composite);

        void loadSources();
        
        void setDestinationCoordinateSystem(osgTerrain::CoordinateSystem* cs) { _coordinateSystem = cs; }
        void setDestinationExtents(const osg::BoundingBox& extents) { _extents = extents; }
        void setDestinationGeoTransform(const osg::Matrixd& geoTransform) { _geoTransform = geoTransform; }
        
        void setDestinationTileBaseName(const std::string& basename) { _tileBasename = basename; }
        void setDestinationTileExtension(const std::string& extension) { _tileExtension = _tileExtension; }
        
        CompositeDestination* createDestinationGraph(osgTerrain::CoordinateSystem* cs,
                                                     const osg::BoundingBox& extents,
                                                     unsigned int maxImageSize,
                                                     unsigned int maxTerrainSize,
                                                     unsigned int currentLevel,
                                                     unsigned int currentX,
                                                     unsigned int currentY,
                                                     unsigned int maxNumLevels);
        
        
        void computeDestinationGraphFromSources(unsigned int numLevels);
        void updateSourcesForDestinationGraphNeeds();
        void populateDestinationGraphFromSources();
        
        void createDestination(unsigned int numLevels);
        
        void writeDestination(const std::string& filename);
        
        osg::Node* getDestinationRootNode() { return _rootNode.get(); }


    protected:

        virtual ~DataSet() {}
        
        void init();

        osg::ref_ptr<CompositeSource>               _sourceGraph;
        
        osg::ref_ptr<CompositeDestination>          _destinationGraph;

        osg::ref_ptr<osgTerrain::CoordinateSystem>  _coordinateSystem;
        osg::Matrixd                                _geoTransform;
        osg::BoundingBox                            _extents;
        std::string                                 _tileBasename;
        std::string                                 _tileExtension;
        osg::Vec4                                   _defaultColour;
        
        
        osg::ref_ptr<osg::Node>                     _rootNode;
        
};

#endif
