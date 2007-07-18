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


#include <osg/GraphicsContext>
#include <osg/Camera>
#include <osg/View>

#include <osg/FrameBufferObject>
#include <osg/Program>
#include <osg/Drawable>
#include <osg/FragmentProgram>
#include <osg/VertexProgram>

#include <OpenThreads/ReentrantMutex>

#include <osg/Notify>

#include <map>
#include <sstream>
#include <algorithm>

using namespace osg;

/////////////////////////////////////////////////////////////////////////////
//
//  GraphicsContext static method implementations
//

static ref_ptr<GraphicsContext::WindowingSystemInterface> s_WindowingSystemInterface;

void GraphicsContext::setWindowingSystemInterface(WindowingSystemInterface* callback)
{
    s_WindowingSystemInterface = callback;
    osg::notify(osg::INFO)<<"GraphicsContext::setWindowingSystemInterface() "<<s_WindowingSystemInterface.get()<<"\t"<<&s_WindowingSystemInterface<<std::endl;
}

GraphicsContext::WindowingSystemInterface* GraphicsContext::getWindowingSystemInterface()
{
    osg::notify(osg::INFO)<<"GraphicsContext::getWindowingSystemInterface() "<<s_WindowingSystemInterface.get()<<"\t"<<&s_WindowingSystemInterface<<std::endl;
    return s_WindowingSystemInterface.get();
}

GraphicsContext* GraphicsContext::createGraphicsContext(Traits* traits)
{
    if (s_WindowingSystemInterface.valid())
        return s_WindowingSystemInterface->createGraphicsContext(traits);
    else
        return 0;    
}


std::string GraphicsContext::ScreenIdentifier::displayName() const
{
    std::stringstream ostr;
    ostr<<hostName<<":"<<displayNum<<"."<<screenNum;
    return ostr.str();
}


class ContextData
{
public:

    ContextData():
        _numContexts(0) {}

    unsigned int _numContexts;
    
    void incrementUsageCount() {  ++_numContexts; }

    void decrementUsageCount()
    {
        --_numContexts;

        osg::notify(osg::INFO)<<"decrementUsageCount()"<<_numContexts<<std::endl;

        if (_numContexts <= 1 && _compileContext.valid())
        {
            osg::notify(osg::INFO)<<"resetting compileContext "<<_compileContext.get()<<" refCount "<<_compileContext->referenceCount()<<std::endl;
            
            GraphicsContext* gc = _compileContext.get();
            _compileContext = 0;
        }
    }
    
    osg::ref_ptr<osg::GraphicsContext> _compileContext;

};


typedef std::map<unsigned int, ContextData>  ContextIDMap;
static ContextIDMap s_contextIDMap;
static OpenThreads::ReentrantMutex s_contextIDMapMutex;
static GraphicsContext::GraphicsContexts s_registeredContexts;

unsigned int GraphicsContext::createNewContextID()
{
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_contextIDMapMutex);
    
    // first check to see if we can reuse contextID;
    for(ContextIDMap::iterator itr = s_contextIDMap.begin();
        itr != s_contextIDMap.end();
        ++itr)
    {
        if (itr->second._numContexts == 0)
        {

            // reuse contextID;
            itr->second._numContexts = 1;

            osg::notify(osg::INFO)<<"GraphicsContext::createNewContextID() reusing contextID="<<itr->first<<std::endl;

            return itr->first;
        }
    }

    unsigned int contextID = s_contextIDMap.size();
    s_contextIDMap[contextID]._numContexts = 1;
    
    osg::notify(osg::INFO)<<"GraphicsContext::createNewContextID() creating contextID="<<contextID<<std::endl;
    

    if ( (contextID+1) > osg::DisplaySettings::instance()->getMaxNumberOfGraphicsContexts() )
    {
        osg::notify(osg::INFO)<<"Updating the MaxNumberOfGraphicsContexts to "<<contextID+1<<std::endl;

        // update the the maximum number of graphics contexts, 
        // to ensure that texture objects and display buffers are configured to the correct size.
        osg::DisplaySettings::instance()->setMaxNumberOfGraphicsContexts( contextID + 1 );
    }
    

    return contextID;    
}

unsigned int GraphicsContext::getMaxContextID()
{
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_contextIDMapMutex);
    unsigned int maxContextID = 0;
    for(ContextIDMap::iterator itr = s_contextIDMap.begin();
        itr != s_contextIDMap.end();
        ++itr)
    {
        if (itr->first > maxContextID) maxContextID = itr->first;
    }
    return maxContextID;
}


void GraphicsContext::incrementContextIDUsageCount(unsigned int contextID)
{
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_contextIDMapMutex);
    
    osg::notify(osg::INFO)<<"GraphicsContext::incrementContextIDUsageCount("<<contextID<<") to "<<s_contextIDMap[contextID]._numContexts<<std::endl;

    s_contextIDMap[contextID].incrementUsageCount();
}

void GraphicsContext::decrementContextIDUsageCount(unsigned int contextID)
{

    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_contextIDMapMutex);

    if (s_contextIDMap[contextID]._numContexts!=0)
    {
        s_contextIDMap[contextID].decrementUsageCount();
    }
    else
    {
        osg::notify(osg::NOTICE)<<"Warning: decrementContextIDUsageCount("<<contextID<<") called on expired contextID."<<std::endl;
    } 

    osg::notify(osg::INFO)<<"GraphicsContext::decrementContextIDUsageCount("<<contextID<<") to "<<s_contextIDMap[contextID]._numContexts<<std::endl;

}


void GraphicsContext::registerGraphicsContext(GraphicsContext* gc)
{
    osg::notify(osg::INFO)<<"GraphicsContext::registerGraphicsContext "<<gc<<std::endl;

    if (!gc) return;

    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_contextIDMapMutex);

    GraphicsContexts::iterator itr = std::find(s_registeredContexts.begin(), s_registeredContexts.end(), gc);
    if (itr != s_registeredContexts.end()) s_registeredContexts.erase(itr);

    s_registeredContexts.push_back(gc);
}

void GraphicsContext::unregisterGraphicsContext(GraphicsContext* gc)
{
    osg::notify(osg::INFO)<<"GraphicsContext::unregisterGraphicsContext "<<gc<<std::endl;

    if (!gc) return;

    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_contextIDMapMutex);

    GraphicsContexts::iterator itr = std::find(s_registeredContexts.begin(), s_registeredContexts.end(), gc);
    if (itr != s_registeredContexts.end()) s_registeredContexts.erase(itr);
}

GraphicsContext::GraphicsContexts GraphicsContext::getAllRegisteredGraphicsContexts()
{
    osg::notify(osg::INFO)<<"GraphicsContext::getAllRegisteredGraphicsContexts s_registeredContexts.size()="<<s_registeredContexts.size()<<std::endl;
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_contextIDMapMutex);
    return s_registeredContexts;
}

GraphicsContext::GraphicsContexts GraphicsContext::getRegisteredGraphicsContexts(unsigned int contextID)
{
    GraphicsContexts contexts;

    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_contextIDMapMutex);
    for(GraphicsContexts::iterator itr = s_registeredContexts.begin();
        itr != s_registeredContexts.end();
        ++itr)
    {
        GraphicsContext* gc = *itr;
        if (gc->getState() && gc->getState()->getContextID()==contextID) contexts.push_back(gc);
    }

    osg::notify(osg::INFO)<<"GraphicsContext::getRegisteredGraphicsContexts "<<contextID<<" contexts.size()="<<contexts.size()<<std::endl;
    
    return contexts;
}

GraphicsContext* GraphicsContext::getOrCreateCompileContext(unsigned int contextID)
{
    osg::notify(osg::INFO)<<"GraphicsContext::createCompileContext."<<std::endl;

    {    
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_contextIDMapMutex);
        if (s_contextIDMap[contextID]._compileContext.valid()) return s_contextIDMap[contextID]._compileContext.get();
    }
    
    GraphicsContext::GraphicsContexts contexts = GraphicsContext::getRegisteredGraphicsContexts(contextID);
    if (contexts.empty()) return 0;
    
    GraphicsContext* src_gc = contexts.front();
    const osg::GraphicsContext::Traits* src_traits = src_gc->getTraits();

    osg::GraphicsContext::Traits* traits = new osg::GraphicsContext::Traits;
    traits->screenNum = src_traits->screenNum;
    traits->displayNum = src_traits->displayNum;
    traits->hostName = src_traits->hostName;
    traits->width = 100;
    traits->height = 100;
    traits->red = src_traits->red;
    traits->green = src_traits->green;
    traits->blue = src_traits->blue;
    traits->alpha = src_traits->alpha;
    traits->depth = src_traits->depth;
    traits->sharedContext = src_gc;
    traits->pbuffer = true;

    osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits);
    if (gc.valid() && gc->realize())
    {    
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_contextIDMapMutex);
        s_contextIDMap[contextID]._compileContext = gc;
        osg::notify(osg::INFO)<<"   succeded GraphicsContext::createCompileContext."<<std::endl;
        return gc.release();
    }
    else
    {
        return 0;
    }
    
}

void GraphicsContext::setCompileContext(unsigned int contextID, GraphicsContext* gc)
{
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_contextIDMapMutex);
    s_contextIDMap[contextID]._compileContext = gc;
}

GraphicsContext* GraphicsContext::getCompileContext(unsigned int contextID)
{
    // osg::notify(osg::NOTICE)<<"GraphicsContext::getCompileContext "<<contextID<<std::endl;
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_contextIDMapMutex);
    ContextIDMap::iterator itr = s_contextIDMap.find(contextID);
    if (itr != s_contextIDMap.end()) return itr->second._compileContext.get();
    else return 0;
}


/////////////////////////////////////////////////////////////////////////////
//
//  GraphicsContext standard method implementations
//
GraphicsContext::GraphicsContext():
    _clearColor(osg::Vec4(0.0f,0.0f,0.0f,1.0f)),
    _clearMask(0),
    _threadOfLastMakeCurrent(0)
{
    setThreadSafeRefUnref(true);
    _operationsBlock = new RefBlock;

    registerGraphicsContext(this);
}

GraphicsContext::GraphicsContext(const GraphicsContext&, const osg::CopyOp&):
    _clearColor(osg::Vec4(0.0f,0.0f,0.0f,1.0f)),
    _clearMask(0),
    _threadOfLastMakeCurrent(0)
{
    setThreadSafeRefUnref(true);
    _operationsBlock = new RefBlock;

    registerGraphicsContext(this);
}

GraphicsContext::~GraphicsContext()
{
    close(false);

    unregisterGraphicsContext(this);
}

void GraphicsContext::clear()
{
    if (_clearMask==0 || !_traits) return;

    glViewport(0, 0, _traits->width, _traits->height);
    glScissor(0, 0, _traits->width, _traits->height);

    glClearColor( _clearColor[0], _clearColor[1], _clearColor[2], _clearColor[3]);

    glClear( _clearMask );
}

bool GraphicsContext::realize()
{
    if (realizeImplementation())
    {
        return true;
    }
    else
    {   
        return false;
    }
}

void GraphicsContext::close(bool callCloseImplementation)
{
    osg::notify(osg::INFO)<<"close("<<callCloseImplementation<<")"<<this<<std::endl;

    // switch off the graphics thread...
    setGraphicsThread(0);
    
    
    bool sharedContextExists = false;
    
    if (_state.valid())
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(s_contextIDMapMutex);
        if (s_contextIDMap[_state->getContextID()]._numContexts>1) sharedContextExists = true;
    }

    // release all the OpenGL objects in the scene graphs associted with this 
    for(Cameras::iterator itr = _cameras.begin();
        itr != _cameras.end();
        ++itr)
    {
        Camera* camera = (*itr);
        if (camera)
        {
            osg::notify(osg::INFO)<<"Releasing GL objects for Camera="<<camera<<" _state="<<_state.get()<<std::endl;
            camera->releaseGLObjects(_state.get());
        }
    }


    if (callCloseImplementation && _state.valid() && isRealized())
    {
        osg::notify(osg::INFO)<<"Closing still viable window "<<sharedContextExists<<" _state->getContextID()="<<_state->getContextID()<<std::endl;

        makeCurrent();
        
        osg::notify(osg::INFO)<<"Doing Flush"<<std::endl;

        // flush all the OpenGL object buffer for this context.
        double availableTime = 100.0f;
        double currentTime = _state->getFrameStamp()?_state->getFrameStamp()->getReferenceTime():0.0;

        osg::FrameBufferObject::flushDeletedFrameBufferObjects(_state->getContextID(),currentTime,availableTime);
        osg::RenderBuffer::flushDeletedRenderBuffers(_state->getContextID(),currentTime,availableTime);
        osg::Texture::flushAllDeletedTextureObjects(_state->getContextID());
        osg::Drawable::flushAllDeletedDisplayLists(_state->getContextID());
        osg::Drawable::flushDeletedVertexBufferObjects(_state->getContextID(),currentTime,availableTime);
        osg::VertexProgram::flushDeletedVertexProgramObjects(_state->getContextID(),currentTime,availableTime);
        osg::FragmentProgram::flushDeletedFragmentProgramObjects(_state->getContextID(),currentTime,availableTime);
        osg::Program::flushDeletedGlPrograms(_state->getContextID(),currentTime,availableTime);
        osg::Shader::flushDeletedGlShaders(_state->getContextID(),currentTime,availableTime);

        osg::notify(osg::INFO)<<"Done Flush "<<availableTime<<std::endl;

        _state->reset();
        
        releaseContext();
    }
    
    if (callCloseImplementation) closeImplementation();

    if (_state.valid())
    {
        decrementContextIDUsageCount(_state->getContextID());
        
        _state = 0;
    }
}


bool GraphicsContext::makeCurrent()
{
    bool result = makeCurrentImplementation();
    
    if (result)
    {
        _threadOfLastMakeCurrent = OpenThreads::Thread::CurrentThread();

        // initialize extension proces, not only initializes on first
        // call, will be a non-op on subsequent calls.        
        getState()->initializeExtensionProcs();
    }
    
    return result;
}

bool GraphicsContext::makeContextCurrent(GraphicsContext* readContext)
{
    bool result = makeContextCurrentImplementation(readContext);

    if (result)
    {
        _threadOfLastMakeCurrent = OpenThreads::Thread::CurrentThread();

        // initialize extension proces, not only initializes on first
        // call, will be a non-op on subsequent calls.        
        getState()->initializeExtensionProcs();
    }
    
    return result;
}

bool GraphicsContext::releaseContext()
{
    bool result = releaseContextImplementation();
    
    _threadOfLastMakeCurrent = (OpenThreads::Thread*)(-1);
    
    return result;
}

void GraphicsContext::swapBuffers()
{
    if (isCurrent())
    {
        swapBuffersImplementation();
        clear();
    }
    else if (_graphicsThread.valid() && 
             _threadOfLastMakeCurrent == _graphicsThread.get())
    {
        _graphicsThread->add(new SwapBuffersOperation);
    }
    else
    {
        makeCurrent();
        swapBuffersImplementation();
        clear();
    }
}



void GraphicsContext::createGraphicsThread()
{
    if (!_graphicsThread)
    {
        setGraphicsThread(new GraphicsThread);
    }
}

void GraphicsContext::setGraphicsThread(GraphicsThread* gt)
{
    if (_graphicsThread==gt) return; 

    if (_graphicsThread.valid()) 
    {
        // need to kill the thread in some way...
        _graphicsThread->cancel();
        _graphicsThread->setParent(0);
    }

    _graphicsThread = gt;
    
    if (_graphicsThread.valid()) 
    {
        _graphicsThread->setParent(this);
    }
}

void GraphicsContext::add(Operation* operation)
{
    osg::notify(osg::INFO)<<"Doing add"<<std::endl;

    // aquire the lock on the operations queue to prevent anyone else for modifying it at the same time
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_operationsMutex);

    // add the operation to the end of the list
    _operations.push_back(operation);

    _operationsBlock->set(true);
}

void GraphicsContext::remove(Operation* operation)
{
    osg::notify(osg::INFO)<<"Doing remove operation"<<std::endl;

    // aquire the lock on the operations queue to prevent anyone else for modifying it at the same time
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_operationsMutex);

    for(OperationQueue::iterator itr = _operations.begin();
        itr!=_operations.end();)
    {
        if ((*itr)==operation) itr = _operations.erase(itr);
        else ++itr;
    }

    if (_operations.empty())
    {
        _operationsBlock->set(false);
    }
}

void GraphicsContext::remove(const std::string& name)
{
    osg::notify(osg::INFO)<<"Doing remove named operation"<<std::endl;
    
    // aquire the lock on the operations queue to prevent anyone else for modifying it at the same time
    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_operationsMutex);

    // find the remove all operations with specificed name
    for(OperationQueue::iterator itr = _operations.begin();
        itr!=_operations.end();)
    {
        if ((*itr)->getName()==name) itr = _operations.erase(itr);
        else ++itr;
    }

    if (_operations.empty())
    {
        _operationsBlock->set(false);
    }
}

void GraphicsContext::removeAllOperations()
{
    osg::notify(osg::INFO)<<"Doing remove all operations"<<std::endl;

    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_operationsMutex);
    _operations.clear();
    _operationsBlock->set(false);
}

void GraphicsContext::runOperations()
{
    for(OperationQueue::iterator itr = _operations.begin();
        itr != _operations.end();
        )
    {
        {
            OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_operationsMutex);
            _currentOperation = *itr;

            if (!_currentOperation->getKeep())
            {
                itr = _operations.erase(itr);

                if (_operations.empty())
                {
                    _operationsBlock->set(false);
                }
            }
            else
            {
                ++itr;
            }
        }
                
        if (_currentOperation.valid())
        {
            // osg::notify(osg::INFO)<<"Doing op "<<_currentOperation->getName()<<" "<<this<<std::endl;

            // call the graphics operation.
            (*_currentOperation)(this);

            {            
                OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_operationsMutex);
                _currentOperation = 0;
            }
        }
    }
}

void GraphicsContext::addCamera(osg::Camera* camera)
{
    _cameras.push_back(camera);
}

void GraphicsContext::removeCamera(osg::Camera* camera)
{
    for(Cameras::iterator itr = _cameras.begin();
        itr != _cameras.end();
        ++itr)
    {
        if (*itr == camera)
        {
            _cameras.erase(itr);
            return;
        }
    }
}

void GraphicsContext::resizedImplementation(int x, int y, int width, int height)
{
    if (!_traits) return;
    
    double widthChangeRatio = double(width) / double(_traits->width);
    double heigtChangeRatio = double(height) / double(_traits->height);
    double aspectRatioChange = widthChangeRatio / heigtChangeRatio; 
    
    for(Cameras::iterator itr = _cameras.begin();
        itr != _cameras.end();
        ++itr)
    {
        Camera* camera = (*itr);
        Viewport* viewport = camera->getViewport();
        if (viewport)
        {
            if (viewport->x()==0 && viewport->y()==0 &&
                viewport->width()>=_traits->width && viewport->height()>=_traits->height)
            {
                viewport->setViewport(0,0,width,height);
            }
            else
            {
                viewport->x() = static_cast<osg::Viewport::value_type>(double(viewport->x())*widthChangeRatio);
                viewport->y() = static_cast<osg::Viewport::value_type>(double(viewport->y())*heigtChangeRatio);
                viewport->width() = static_cast<osg::Viewport::value_type>(double(viewport->width())*widthChangeRatio);
                viewport->height() = static_cast<osg::Viewport::value_type>(double(viewport->height())*heigtChangeRatio);
            }
        }

        // if aspect ratio adjusted change the project matrix to suit.
        if (aspectRatioChange != 1.0)
        {
            osg::View* view = camera->getView();
            osg::View::Slave* slave = view ? view->findSlaveForCamera(camera) : 0;
            
            if (slave && camera->getReferenceFrame()==osg::Transform::RELATIVE_RF)
            {
                switch(view->getCamera()->getProjectionResizePolicy())
                {
                    case(osg::Camera::HORIZONTAL): slave->_projectionOffset *= osg::Matrix::scale(1.0/aspectRatioChange,1.0,1.0); break;
                    case(osg::Camera::VERTICAL): slave->_projectionOffset *= osg::Matrix::scale(1.0, aspectRatioChange,1.0); break;
                    default: break;
                }
            }
            else
            {
                Camera::ProjectionResizePolicy policy = view ? view->getCamera()->getProjectionResizePolicy() : camera->getProjectionResizePolicy();
                switch(policy)
                {
                    case(osg::Camera::HORIZONTAL): camera->getProjectionMatrix() *= osg::Matrix::scale(1.0/aspectRatioChange,1.0,1.0); break;
                    case(osg::Camera::VERTICAL): camera->getProjectionMatrix() *= osg::Matrix::scale(1.0, aspectRatioChange,1.0); break;
                    default: break;
                }
            }

        }    

    }
    
    _traits->x = x;
    _traits->y = y;
    _traits->width = width;
    _traits->height = height;
}
