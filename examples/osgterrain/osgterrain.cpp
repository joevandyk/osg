/* OpenSceneGraph example, osgterrain.
*
*  Permission is hereby granted, free of charge, to any person obtaining a copy
*  of this software and associated documentation files (the "Software"), to deal
*  in the Software without restriction, including without limitation the rights
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*  copies of the Software, and to permit persons to whom the Software is
*  furnished to do so, subject to the following conditions:
*
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*  THE SOFTWARE.
*/

#include <OpenThreads/Block>

#include <osg/Group>
#include <osg/Geode>
#include <osg/ShapeDrawable>
#include <osg/Texture2D>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/CoordinateSystemNode>
#include <osg/ClusterCullingCallback>
#include <osg/ArgumentParser>


#include <osgDB/FileUtils>
#include <osgDB/ReadFile>

#include <osgUtil/GLObjectsVisitor>

#include <osgText/FadeText>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/StateSetManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>

#include <osgTerrain/TerrainTile>
#include <osgTerrain/GeometryTechnique>
#include <osgTerrain/Layer>

#include <iostream>


typedef std::vector< osg::ref_ptr<osg::GraphicsThread> > GraphicsThreads;



class LoadAndCompileOperation : public osg::Operation
{
public:

    LoadAndCompileOperation(const std::string& filename, GraphicsThreads& graphicsThreads, osg::RefBlockCount* block):
        Operation("Load and compile Operation", false),
        _filename(filename),
        _graphicsThreads(graphicsThreads),
        _block(block) {}

    virtual void operator () (osg::Object* object)
    {
        // osg::notify(osg::NOTICE)<<"LoadAndCompileOperation "<<_filename<<std::endl;

        _loadedModel = osgDB::readNodeFile(_filename);
        if (_loadedModel.valid() && !_graphicsThreads.empty())
        {
            osg::ref_ptr<osgUtil::GLObjectsOperation> compileOperation = new osgUtil::GLObjectsOperation(_loadedModel.get());

            for(GraphicsThreads::iterator gitr = _graphicsThreads.begin();
                gitr != _graphicsThreads.end();
                ++gitr)
            {
                (*gitr)->add( compileOperation.get() );
                // requiresBarrier = true;
            }
        }
        
        if (_block.valid()) _block->completed();

        // osg::notify(osg::NOTICE)<<"done LoadAndCompileOperation "<<_filename<<std::endl;
    }
    
    std::string                 _filename;
    GraphicsThreads             _graphicsThreads;
    osg::ref_ptr<osg::Node>     _loadedModel;
    osg::ref_ptr<osg::RefBlockCount> _block;

};

class MasterOperation : public osg::Operation
{
public:

    typedef std::set<std::string> Files;
    typedef std::map<std::string, osg::ref_ptr<osg::Node> >  FilenameNodeMap;
    typedef std::vector< osg::ref_ptr<osg::Node> >  Nodes;


    MasterOperation(const std::string& filename):
        Operation("Master reading operation",true),
        _filename(filename)
    {
    }
    
    /** Set the OperationQueue that the MasterOperation can use to place tasks like file loading on for other processes to handle.*/ 
    void setOperationQueue(osg::OperationQueue* oq) { _operationQueue = oq; }
    
    osg::OperationQueue* getOperationQueue() { return _operationQueue.get(); }
    
    bool readMasterFile(Files& files) const
    {
        std::ifstream fin(_filename.c_str());
        if (fin)
        {
            osgDB::Input fr;
            fr.attach(&fin);

            bool readFilename;

            while(!fr.eof())
            {
                bool itrAdvanced = false;
                if (fr.matchSequence("file %s") || fr.matchSequence("file %w") )
                {
                    files.insert(fr[1].getStr());
                    fr += 2;
                    itrAdvanced = true;
                    readFilename = true;
                }

                if (!itrAdvanced)
                {
                    ++fr;
                }
            }
            
            return readFilename;
        }
        return false;
    }
    
    bool open(osg::Group* group)
    {
        Files files;
        readMasterFile(files);
        for(Files::iterator itr = files.begin();
            itr != files.end();
            ++itr)
        {
            osg::Node* model = osgDB::readNodeFile(*itr);
            if (model)
            {
                osg::notify(osg::NOTICE)<<"open: Loaded file "<<*itr<<std::endl;
                group->addChild(model);
                _existingFilenameNodeMap[*itr] = model;
            }
        }
        
        return true;
    }
    

    virtual void operator () (osg::Object* callingObject)
    {
        // decided which method to call according to whole has called me.
        osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(callingObject);

        if (viewer) update(viewer->getSceneData());
        else load();
    }

    void load()
    {
        //osg::notify(osg::NOTICE)<<"void load(Object)"<<std::endl;
        Files filesA;
        Files filesB;
        
        readMasterFile(filesB);
        // osg::notify(osg::NOTICE)<<"First read "<<filesA.size()<<std::endl;

        // itererate until the master file is stable
        do
        {
            OpenThreads::Thread::microSleep(100000);

            filesB.swap(filesA);
            filesB.clear();
            readMasterFile(filesB);

            // osg::notify(osg::NOTICE)<<"second read "<<filesB.size()<<std::endl;
            
        } while (filesA!=filesB);

        Files files;
        files.swap(filesB);

        // osg::notify(osg::NOTICE)<<"Now equal "<<files.size()<<std::endl;


        Files newFiles;
        Files removedFiles;

        // find out which files are new, and which ones have been removed.
        {            
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);

            for(Files::iterator fitr = files.begin();
                fitr != files.end();
                ++fitr)
            {
                if (_existingFilenameNodeMap.count(*fitr)==0) newFiles.insert(*fitr);
            }

            for(FilenameNodeMap::iterator litr = _existingFilenameNodeMap.begin();
                litr != _existingFilenameNodeMap.end();
                ++litr)
            {
                if (files.count(litr->first)==0)
                {
                    removedFiles.insert(litr->first);
                }
            }
        }
        
#if 0
        if (!newFiles.empty() || !removedFiles.empty())
        {
            osg::notify(osg::NOTICE)<<std::endl<<std::endl<<"void operator () files.size()="<<files.size()<<std::endl;
        }
#endif

        // first load the new files.
        FilenameNodeMap nodesToAdd;
        if (!newFiles.empty())
        {

            typedef std::vector< osg::ref_ptr<osg::GraphicsThread> > GraphicsThreads;
            GraphicsThreads threads;
        
            for(unsigned int i=0; i<= osg::GraphicsContext::getMaxContextID(); ++i)
            {
                osg::GraphicsContext* gc = osg::GraphicsContext::getCompileContext(i);
                osg::GraphicsThread* gt = gc ? gc->getGraphicsThread() : 0;
                if (gt) threads.push_back(gt);
            }

            bool requiresBarrier = false;


            if (_operationQueue.valid())
            {
                // osg::notify(osg::NOTICE)<<"Using OperationQueue"<<std::endl;

                _endOfLoadBlock = new osg::RefBlockCount(newFiles.size());
                
                _endOfLoadBlock->reset();
     
                typedef std::list< osg::ref_ptr<LoadAndCompileOperation> > LoadAndCompileList;
                LoadAndCompileList loadAndCompileList;
     
                for(Files::iterator nitr = newFiles.begin();
                    nitr != newFiles.end();
                    ++nitr)
                {
                    // osg::notify(osg::NOTICE)<<"Adding LoadAndCompileOperation "<<*nitr<<std::endl;

                    osg::ref_ptr<LoadAndCompileOperation> loadAndCompile = new LoadAndCompileOperation( *nitr, threads, _endOfLoadBlock.get() );
                    loadAndCompileList.push_back(loadAndCompile);
                    _operationQueue->add( loadAndCompile.get() );
                }

#if 1
                osg::ref_ptr<osg::Operation> operation;
                while ((operation=_operationQueue->getNextOperation()).valid())
                {
                    // osg::notify(osg::NOTICE)<<"Local running of operation"<<std::endl;
                    (*operation)(0);
                }
#endif                                
                // osg::notify(osg::NOTICE)<<"Waiting for completion of LoadAndCompile operations"<<std::endl;
                _endOfLoadBlock->block();
                // osg::notify(osg::NOTICE)<<"done ... Waiting for completion of LoadAndCompile operations"<<std::endl;
                
                for(LoadAndCompileList::iterator litr = loadAndCompileList.begin();
                    litr != loadAndCompileList.end();
                    ++litr)
                {
                    if ((*litr)->_loadedModel.valid())
                    {
                        nodesToAdd[(*litr)->_filename] = (*litr)->_loadedModel;
                        requiresBarrier = true;
                    }
                }

            }
            
            else
            {
                for(Files::iterator nitr = newFiles.begin();
                    nitr != newFiles.end();
                    ++nitr)
                {
                    osg::ref_ptr<osg::Node> loadedModel = osgDB::readNodeFile(*nitr);

                    if (loadedModel.get())
                    {
                        nodesToAdd[*nitr] = loadedModel;

                        osg::ref_ptr<osgUtil::GLObjectsOperation> compileOperation = new osgUtil::GLObjectsOperation(loadedModel.get());

                        for(GraphicsThreads::iterator gitr = threads.begin();
                            gitr != threads.end();
                            ++gitr)
                        {
                            (*gitr)->add( compileOperation.get() );
                            requiresBarrier = true;
                        }
                    }
                }
            }
                        
            if (requiresBarrier)
            {
                _endOfCompilebarrier = new osg::BarrierOperation(threads.size()+1);
                _endOfCompilebarrier->setKeep(false);
                
                for(GraphicsThreads::iterator gitr = threads.begin();
                    gitr != threads.end();
                    ++gitr)
                {
                    (*gitr)->add(_endOfCompilebarrier.get());
                }
                
                // osg::notify(osg::NOTICE)<<"Waiting for Compile to complete"<<std::endl;

                // wait for the graphics threads to complete.
                _endOfCompilebarrier->block();
                
                // osg::notify(osg::NOTICE)<<"done ... Waiting for Compile to complete"<<std::endl;
            }
        }
        
        bool requiresBlock = false;
        
        // pass the locally peppared data to MasterOperations shared data
        // so that updated thread can merge these changes with the main scene 
        // graph.  This merge is carried out via the update(..) method.
        if (!removedFiles.empty() || !nodesToAdd.empty())        
        {
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
            _nodesToRemove.swap(removedFiles);
            _nodesToAdd.swap(nodesToAdd);
            requiresBlock = true;
        }

        // now block so we don't try to load anything till the new data has been merged
        // otherwise _existingFilenameNodeMap will get out of sync.
        if (requiresBlock)
        {
            _updatesMergedBlock.block();
        }
        else
        {
            OpenThreads::Thread::YieldCurrentThread();
        }

    }
    
    // merge the changes with the main scene graph.
    void update(osg::Node* scene)
    {
        // osg::notify(osg::NOTICE)<<"void update(Node*)"<<std::endl;

        osg::Group* group = dynamic_cast<osg::Group*>(scene);
        if (!group)
        {
            osg::notify(osg::NOTICE)<<"Error, MasterOperation::update(Node*) can only work with a Group as Viewer::getSceneData()."<<std::endl;
            return;
        }
    
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
        
        if (!_nodesToRemove.empty() || !_nodesToAdd.empty())
        {
            osg::notify(osg::NOTICE)<<"update().................. "<<std::endl;
        }

        if (!_nodesToRemove.empty())
        {
            for(Files::iterator itr = _nodesToRemove.begin();
                itr != _nodesToRemove.end();
                ++itr)
            {
                FilenameNodeMap::iterator fnmItr = _existingFilenameNodeMap.find(*itr);
                if (fnmItr != _existingFilenameNodeMap.end())
                {
                    osg::notify(osg::NOTICE)<<"  update():removing "<<*itr<<std::endl;
                
                    group->removeChild(fnmItr->second.get());
                    _existingFilenameNodeMap.erase(fnmItr);
                }
            }

            _nodesToRemove.clear();
        }
        
        if (!_nodesToAdd.empty())
        {
            for(FilenameNodeMap::iterator itr = _nodesToAdd.begin();
                itr != _nodesToAdd.end();
                ++itr)
            {
                osg::notify(osg::NOTICE)<<"  update():inserting "<<itr->first<<std::endl;
                group->addChild(itr->second.get());
                _existingFilenameNodeMap[itr->first] = itr->second;
            }
            
            _nodesToAdd.clear();
        }

        _updatesMergedBlock.release();

    }
    
    // add release implementation so that any thread cancellation can
    // work even when blocks and barriers are used.
    virtual void release()
    {
        if (_operationQueue.valid()) _operationQueue->removeAllOperations();

        _updatesMergedBlock.release();
        if (_endOfCompilebarrier.valid()) _endOfCompilebarrier.release();
        if (_endOfLoadBlock.valid()) _endOfLoadBlock.release();
    }

    
    std::string                         _filename;
    
    OpenThreads::Mutex                  _mutex;
    FilenameNodeMap                     _existingFilenameNodeMap;
    Files                               _nodesToRemove;
    FilenameNodeMap                     _nodesToAdd;
    OpenThreads::Block                  _updatesMergedBlock;

    osg::ref_ptr<osg::BarrierOperation> _endOfCompilebarrier;
    osg::ref_ptr<osg::RefBlockCount>    _endOfLoadBlock;
    
    osg::ref_ptr<osg::OperationQueue>   _operationQueue;
};

class FilterHandler : public osgGA::GUIEventHandler 
{
public: 

    FilterHandler(osgTerrain::GeometryTechnique* gt):
        _gt(gt) {}

    bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
    {
        if (!_gt) return false;

        switch(ea.getEventType())
        {
        case(osgGA::GUIEventAdapter::KEYDOWN):
            {
                if (ea.getKey() == 'g')
                {
                    osg::notify(osg::NOTICE)<<"Gaussian"<<std::endl;
                    _gt->setFilterMatrixAs(osgTerrain::GeometryTechnique::GAUSSIAN);
                    return true;
                }
                else if (ea.getKey() == 's')
                {
                    osg::notify(osg::NOTICE)<<"Smooth"<<std::endl;
                    _gt->setFilterMatrixAs(osgTerrain::GeometryTechnique::SMOOTH);
                    return true;
                }
                else if (ea.getKey() == 'S')
                {
                    osg::notify(osg::NOTICE)<<"Sharpen"<<std::endl;
                    _gt->setFilterMatrixAs(osgTerrain::GeometryTechnique::SHARPEN);
                    return true;
                }
                else if (ea.getKey() == '+')
                {
                    _gt->setFilterWidth(_gt->getFilterWidth()*1.1);
                    osg::notify(osg::NOTICE)<<"Filter width = "<<_gt->getFilterWidth()<<std::endl;
                    return true;
                }
                else if (ea.getKey() == '-')
                {
                    _gt->setFilterWidth(_gt->getFilterWidth()/1.1);
                    osg::notify(osg::NOTICE)<<"Filter width = "<<_gt->getFilterWidth()<<std::endl;
                    return true;
                }
                else if (ea.getKey() == '>')
                {
                    _gt->setFilterBias(_gt->getFilterBias()+0.1);
                    osg::notify(osg::NOTICE)<<"Filter bias = "<<_gt->getFilterBias()<<std::endl;
                    return true;
                }
                else if (ea.getKey() == '<')
                {
                    _gt->setFilterBias(_gt->getFilterBias()-0.1);
                    osg::notify(osg::NOTICE)<<"Filter bias = "<<_gt->getFilterBias()<<std::endl;
                    return true;
                }
                break;
            }
        default:
            break;
        }
        return false;

    }

protected:

    osg::observer_ptr<osgTerrain::GeometryTechnique> _gt;

};



class LayerHandler : public osgGA::GUIEventHandler 
{
public: 

    LayerHandler(osgTerrain::Layer* layer):
        _layer(layer) {}

    bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
    {
        if (!_layer) return false;

        float scale = 1.2;

        switch(ea.getEventType())
        {
        case(osgGA::GUIEventAdapter::KEYDOWN):
            {
                if (ea.getKey() == 'q')
                {
                    _layer->transform(0.0, scale);
                    return true;
                }
                else if (ea.getKey() == 'a')
                {
                    _layer->transform(0.0, 1.0f/scale);
                    return true;
                }
                break;
            }
        default:
            break;
        }
        return false;

    }

protected:

    osg::observer_ptr<osgTerrain::Layer> _layer;
};

int main(int argc, char** argv)
{
    osg::ArgumentParser arguments(&argc, argv);

    // construct the viewer.
    osgViewer::Viewer viewer(arguments);

    // set up the camera manipulators.
    {
        osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;

        keyswitchManipulator->addMatrixManipulator( '1', "Trackball", new osgGA::TrackballManipulator() );
        keyswitchManipulator->addMatrixManipulator( '2', "Flight", new osgGA::FlightManipulator() );
        keyswitchManipulator->addMatrixManipulator( '3', "Drive", new osgGA::DriveManipulator() );
        keyswitchManipulator->addMatrixManipulator( '4', "Terrain", new osgGA::TerrainManipulator() );

        std::string pathfile;
        char keyForAnimationPath = '5';
        while (arguments.read("-p",pathfile))
        {
            osgGA::AnimationPathManipulator* apm = new osgGA::AnimationPathManipulator(pathfile);
            if (apm || !apm->valid()) 
            {
                unsigned int num = keyswitchManipulator->getNumMatrixManipulators();
                keyswitchManipulator->addMatrixManipulator( keyForAnimationPath, "Path", apm );
                keyswitchManipulator->selectMatrixManipulator(num);
                ++keyForAnimationPath;
            }
        }

        viewer.setCameraManipulator( keyswitchManipulator.get() );
    }


    // add the state manipulator
    viewer.addEventHandler( new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()) );

    // add the stats handler
    viewer.addEventHandler(new osgViewer::StatsHandler);

    // add the record camera path handler
    viewer.addEventHandler(new osgViewer::RecordCameraPathHandler);


    double x = 0.0;
    double y = 0.0;
    double w = 1.0;
    double h = 1.0;

    unsigned int numLoadThreads = 1;
    while (arguments.read("--load-threads",numLoadThreads)) { }

    osg::ref_ptr<MasterOperation> masterOperation;
    std::string masterFilename;
    while(arguments.read("-m",masterFilename))
    {
        masterOperation = new MasterOperation(masterFilename);
    }
    

    osg::ref_ptr<osgTerrain::TerrainTile> terrainTile = new osgTerrain::TerrainTile;
    osg::ref_ptr<osgTerrain::Locator> locator = new osgTerrain::Locator;
    osg::ref_ptr<osgTerrain::ValidDataOperator> validDataOperator = new osgTerrain::NoDataValue(0.0);
    osg::ref_ptr<osgTerrain::Layer> lastAppliedLayer;

    locator->setCoordinateSystemType(osgTerrain::Locator::GEOCENTRIC);
    locator->setTransformAsExtents(-osg::PI, -osg::PI*0.5, osg::PI, osg::PI*0.5);

    unsigned int layerNum = 0;

    std::string filterName;

    osgTerrain::Layer::Filter filter = osgTerrain::Layer::LINEAR;

    bool readParameter = false;
    float minValue, maxValue;
    float scale = 1.0f;
    float offset = 0.0f;

    int pos = 1;
    while(pos<arguments.argc())
    {
        readParameter = false;
        std::string filename;
        
        if (arguments.read(pos, "--layer",layerNum)) 
        {
            osg::notify(osg::NOTICE)<<"Set layer number to "<<layerNum<<std::endl;
            readParameter = true;
        }

        else if (arguments.read(pos, "-b"))
        {
            terrainTile->setTreatBoundariesToValidDataAsDefaultValue(true);
        }
        
        else if (arguments.read(pos, "-e",x,y,w,h))
        {
            // define the extents.
            locator->setCoordinateSystemType(osgTerrain::Locator::GEOCENTRIC);
            locator->setTransformAsExtents(x,y,x+w,y+h);
            readParameter = true;
        }

        else if (arguments.read(pos, "--transform",offset, scale) || arguments.read(pos, "-t",offset, scale))
        {
            // define the extents.
            readParameter = true;
        }

        else if (arguments.read(pos, "--cartesian",x,y,w,h))
        {
            // define the extents.
            locator->setCoordinateSystemType(osgTerrain::Locator::PROJECTED);
            locator->setTransformAsExtents(x,y,x+w,y+h);
        }

        else if (arguments.read(pos, "--hf",filename))
        {
            readParameter = true;
            
            osg::notify(osg::NOTICE)<<"--hf "<<filename<<std::endl;

            osg::ref_ptr<osg::HeightField> hf = osgDB::readHeightFieldFile(filename);
            if (hf.valid())
            {
                osg::ref_ptr<osgTerrain::HeightFieldLayer> hfl = new osgTerrain::HeightFieldLayer;
                hfl->setHeightField(hf.get());
                
                hfl->setLocator(locator.get());
                hfl->setValidDataOperator(validDataOperator.get());
                hfl->setFilter(filter);
                
                if (offset!=0.0f || scale!=1.0f)
                {
                    hfl->transform(offset,scale);
                }
                
                terrainTile->setElevationLayer(hfl.get());
                
                lastAppliedLayer = hfl.get();
                
                osg::notify(osg::NOTICE)<<"created osgTerrain::HeightFieldLayer"<<std::endl;
            }
            else
            {
                osg::notify(osg::NOTICE)<<"failed to create osgTerrain::HeightFieldLayer"<<std::endl;
            }
            
            scale = 1.0f;
            offset = 0.0f;
            
        }

        else if (arguments.read(pos, "-d",filename) || arguments.read(pos, "--elevation-image",filename))
        {
            readParameter = true;
            osg::notify(osg::NOTICE)<<"--elevation-image "<<filename<<std::endl;

            osg::ref_ptr<osg::Image> image = osgDB::readImageFile(filename);
            if (image.valid())
            {
                osg::ref_ptr<osgTerrain::ImageLayer> imageLayer = new osgTerrain::ImageLayer;
                imageLayer->setImage(image.get());
                imageLayer->setLocator(locator.get());
                imageLayer->setValidDataOperator(validDataOperator.get());
                imageLayer->setFilter(filter);
                
                if (offset!=0.0f || scale!=1.0f)
                {
                    imageLayer->transform(offset,scale);
                }
                
                terrainTile->setElevationLayer(imageLayer.get());
                
                lastAppliedLayer = imageLayer.get();

                osg::notify(osg::NOTICE)<<"created Elevation osgTerrain::ImageLayer"<<std::endl;
            }
            else
            {
                osg::notify(osg::NOTICE)<<"failed to create osgTerrain::ImageLayer"<<std::endl;
            }

            scale = 1.0f;
            offset = 0.0f;
            
        }
        
        else if (arguments.read(pos, "-c",filename) || arguments.read(pos, "--image",filename))
        {
            readParameter = true;
            osg::notify(osg::NOTICE)<<"--image "<<filename<<" x="<<x<<" y="<<y<<" w="<<w<<" h="<<h<<std::endl;

            osg::ref_ptr<osg::Image> image = osgDB::readImageFile(filename);
            if (image.valid())
            {
                osg::ref_ptr<osgTerrain::ImageLayer> imageLayer = new osgTerrain::ImageLayer;
                imageLayer->setImage(image.get());
                imageLayer->setLocator(locator.get());
                imageLayer->setValidDataOperator(validDataOperator.get());
                imageLayer->setFilter(filter);
                
                if (offset!=0.0f || scale!=1.0f)
                {
                    imageLayer->transform(offset,scale);
                }

                terrainTile->setColorLayer(layerNum, imageLayer.get());

                lastAppliedLayer = imageLayer.get();

                osg::notify(osg::NOTICE)<<"created Color osgTerrain::ImageLayer"<<std::endl;
            }
            else
            {
                osg::notify(osg::NOTICE)<<"failed to create osgTerrain::ImageLayer"<<std::endl;
            }

            scale = 1.0f;
            offset = 0.0f;
            
        }

        else if (arguments.read(pos, "--filter",filterName))
        {
            readParameter = true;
            
            if (filterName=="NEAREST")
            {
                osg::notify(osg::NOTICE)<<"--filter "<<filterName<<std::endl;
                filter = osgTerrain::Layer::NEAREST;
            }
            else if (filterName=="LINEAR") 
            {
                filter = osgTerrain::Layer::LINEAR;
                osg::notify(osg::NOTICE)<<"--filter "<<filterName<<std::endl;
            }
            else
            {
                osg::notify(osg::NOTICE)<<"--filter "<<filterName<<" unrecognized filter name, please use LINEAER or NEAREST."<<std::endl;
            }            

            if (terrainTile->getColorLayer(layerNum))
            {
                terrainTile->getColorLayer(layerNum)->setFilter(filter);
            }
            
        }

        else if (arguments.read(pos, "--tf",minValue, maxValue))
        {
            readParameter = true;

            osg::ref_ptr<osg::TransferFunction1D> tf = new osg::TransferFunction1D;
            
            tf->setInputRange(minValue, maxValue);
            
            tf->allocate(6);
            tf->setValue(0, osg::Vec4(1.0,1.0,1.0,1.0));
            tf->setValue(1, osg::Vec4(1.0,0.0,1.0,1.0));
            tf->setValue(2, osg::Vec4(1.0,0.0,0.0,1.0));
            tf->setValue(3, osg::Vec4(1.0,1.0,0.0,1.0));
            tf->setValue(4, osg::Vec4(0.0,1.0,1.0,1.0));
            tf->setValue(5, osg::Vec4(0.0,1.0,0.0,1.0));
            
            osg::notify(osg::NOTICE)<<"--tf "<<minValue<<" "<<maxValue<<std::endl;

            terrainTile->setColorLayer(layerNum, new osgTerrain::ContourLayer(tf.get()));
        }
        else
        {
            ++pos;
        }

    }
    

    osg::ref_ptr<osg::Group> scene = new osg::Group;

    if (terrainTile.valid() && (terrainTile->getElevationLayer() || terrainTile->getColorLayer(0)))
    {
        osg::notify(osg::NOTICE)<<"Terrain created"<<std::endl;
    
        scene->addChild(terrainTile.get());

        osg::ref_ptr<osgTerrain::GeometryTechnique> geometryTechnique = new osgTerrain::GeometryTechnique;
        terrainTile->setTerrainTechnique(geometryTechnique.get());
        viewer.addEventHandler(new FilterHandler(geometryTechnique.get()));
        viewer.addEventHandler(new LayerHandler(lastAppliedLayer.get()));
    }

    if (masterOperation.valid())
    {
        osg::notify(osg::NOTICE)<<"Master operation created"<<std::endl;

        masterOperation->open(scene.get());
    }
    
    if (scene->getNumChildren()==0)
    {
        osg::notify(osg::NOTICE)<<"No model created, please specify terrain or master file on command line."<<std::endl;
        return 0;
    }
    
    viewer.setSceneData(scene.get());


    // start operation thread if a master file has been used.
    osg::ref_ptr<osg::OperationThread> masterOperationThread;
    
    typedef std::list< osg::ref_ptr<osg::OperationThread> > OperationThreadList;
    OperationThreadList generalThreadList;
    
    if (masterOperation.valid()) 
    {
        masterOperationThread = new osg::OperationThread;
        masterOperationThread->startThread();
        
        masterOperationThread->add(masterOperation.get());

//        if (numLoadThreads>0)
        {
            osg::ref_ptr<osg::OperationQueue> operationQueue = new osg::OperationQueue;
            masterOperation->setOperationQueue(operationQueue.get());

            for(unsigned int i=0; i<numLoadThreads; ++i)
            {        
                osg::ref_ptr<osg::OperationThread> thread = new osg::OperationThread;
                thread->setOperationQueue(operationQueue.get());
                thread->startThread();
                generalThreadList.push_back(thread);
            }
        }
        
        viewer.addUpdateOperation(masterOperation.get());
    }
    
    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);
    
    // enable the use of compile contexts and associated threads.
    osg::DisplaySettings::instance()->setCompileContextsHint(true);

    // realize the graphics windows.
    viewer.realize();

    // run the viewers main loop
    return viewer.run();

}
