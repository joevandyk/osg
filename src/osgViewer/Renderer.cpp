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

#include <stdio.h>

#include <osg/GLExtensions>

#include <osgUtil/Optimizer>
#include <osgUtil/GLObjectsVisitor>

#include <osgViewer/Renderer>
#include <osgViewer/View>

#include <osgDB/DatabasePager>

#include <osg/io_utils>

#include <sstream>

using namespace osgViewer;

//#define DEBUG_MESSAGE osg::notify(osg::NOTICE)
#define DEBUG_MESSAGE osg::notify(osg::INFO)


OpenGLQuerySupport::OpenGLQuerySupport():
    _startTick(0),
    _initialized(false),
    _timerQuerySupported(false),
    _extensions(0),
    _previousQueryTime(0.0)
{
}

void OpenGLQuerySupport::checkQuery(osg::Stats* stats)
{
    for(QueryFrameNumberList::iterator itr = _queryFrameNumberList.begin();
        itr != _queryFrameNumberList.end();
        )
    {
        GLuint query = itr->first;
        GLint available = 0;
        _extensions->glGetQueryObjectiv(query, GL_QUERY_RESULT_AVAILABLE, &available);
        if (available)
        {
            GLuint64EXT timeElapsed = 0;
            _extensions->glGetQueryObjectui64v(query, GL_QUERY_RESULT, &timeElapsed);

            double timeElapsedSeconds = double(timeElapsed)*1e-9;
            double currentTime = osg::Timer::instance()->delta_s(_startTick, osg::Timer::instance()->tick());
            double estimatedEndTime = (_previousQueryTime + currentTime) * 0.5;
            double estimatedBeginTime = estimatedEndTime - timeElapsedSeconds;

            stats->setAttribute(itr->second, "GPU draw begin time", estimatedBeginTime);
            stats->setAttribute(itr->second, "GPU draw end time", estimatedEndTime);
            stats->setAttribute(itr->second, "GPU draw time taken", timeElapsedSeconds);


            itr = _queryFrameNumberList.erase(itr);
            _availableQueryObjects.push_back(query);
        }
        else
        {
            ++itr;
        }

    }
    _previousQueryTime = osg::Timer::instance()->delta_s(_startTick, osg::Timer::instance()->tick());
}

GLuint OpenGLQuerySupport::createQueryObject()
{
    if (_availableQueryObjects.empty())
    {
        GLuint query;
        _extensions->glGenQueries(1, &query);
        return query;
    }
    else
    {
        GLuint query = _availableQueryObjects.back();
        _availableQueryObjects.pop_back();
        return query;
    }
}

void OpenGLQuerySupport::beginQuery(int frameNumber)
{
    GLuint query = createQueryObject();
    _extensions->glBeginQuery(GL_TIME_ELAPSED, query);
    _queryFrameNumberList.push_back(QueryFrameNumberPair(query, frameNumber));        
}

void OpenGLQuerySupport::endQuery()
{
    _extensions->glEndQuery(GL_TIME_ELAPSED);
}

void OpenGLQuerySupport::initialize(osg::State* state)
{
    if (_initialized) return;

    _initialized = true;
    _extensions = osg::Drawable::getExtensions(state->getContextID(),true);
    _timerQuerySupported = _extensions && _extensions->isTimerQuerySupported();
    _previousQueryTime = osg::Timer::instance()->delta_s(_startTick, osg::Timer::instance()->tick());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//  Renderer

Renderer::Renderer(osg::Camera* camera):
    osg::GraphicsOperation("Renderer",true),
    OpenGLQuerySupport(),
    _camera(camera),
    _done(false),
    _graphicsThreadDoesCull(true)
{

    DEBUG_MESSAGE<<"Render::Render() "<<this<<std::endl;

    _lockHeld[0]  = false;
    _lockHeld[1]  = false;

    _sceneView[0] = new osgUtil::SceneView;
    _sceneView[1] = new osgUtil::SceneView;

    unsigned int sceneViewOptions = osgUtil::SceneView::HEADLIGHT;

    osg::Camera* masterCamera = _camera->getView() ? _camera->getView()->getCamera() : camera;
    osg::StateSet* stateset = masterCamera->getOrCreateStateSet();
    osgViewer::View* view = dynamic_cast<osgViewer::View*>(_camera->getView());

    osg::DisplaySettings* ds = _camera->getDisplaySettings() ?  _camera->getDisplaySettings() :
                               ((view && view->getDisplaySettings()) ?  view->getDisplaySettings() :  osg::DisplaySettings::instance());

    _sceneView[0]->setGlobalStateSet(stateset);
    _sceneView[1]->setGlobalStateSet(stateset);
    
    _sceneView[0]->setDefaults(sceneViewOptions);
    _sceneView[1]->setDefaults(sceneViewOptions);

    _sceneView[0]->setDisplaySettings(ds);
    _sceneView[1]->setDisplaySettings(ds);

    _sceneView[0]->setCamera(_camera.get(), false);
    _sceneView[1]->setCamera(_camera.get(), false);

    _currentCull = 0;
    _currentDraw = 0;

    // lock the mutex for the current cull SceneView to
    // prevent the draw traversal from reading from it before the cull traversal has been completed.
    if (!_graphicsThreadDoesCull)
    {
         _mutex[_currentCull].lock();
         _lockHeld[_currentCull] = true;
    }

    _flushOperation = new osg::FlushDeletedGLObjectsOperation(0.1);
}

Renderer::~Renderer()
{
    DEBUG_MESSAGE<<"Render::~Render() "<<this<<std::endl;
}

void Renderer::setGraphicsThreadDoesCull(bool flag)
{
    if (_graphicsThreadDoesCull==flag) return;

    _graphicsThreadDoesCull = flag;

    _currentCull = 0;
    _currentDraw = 0;

    if (_graphicsThreadDoesCull)
    {
        // need to disable any locks held by the cull
        if (_lockHeld[0])
        {
            _lockHeld[0] = false;
            _mutex[0].unlock();
        }

        if (_lockHeld[1])
        {
            _lockHeld[1] = false;
            _mutex[1].unlock();
        }

        DEBUG_MESSAGE<<"Disabling locks in Renderer"<<std::endl;
    }
    else
    {
        DEBUG_MESSAGE<<"Enable locks in Renderer"<<std::endl;
    
        // need to set a lock for cull
        _mutex[_currentCull].lock();
        _lockHeld[_currentCull] = true;
    }
}

void Renderer::updateSceneView(osgUtil::SceneView* sceneView)
{
    osg::Camera* masterCamera = _camera->getView() ? _camera->getView()->getCamera() : _camera.get();
    osg::StateSet* stateset = masterCamera->getOrCreateStateSet();

    if (sceneView->getGlobalStateSet()!=stateset)
    {
        sceneView->setGlobalStateSet(stateset);
    }
    
    osg::GraphicsContext* context = _camera->getGraphicsContext();
    osg::State* state = context ? context->getState() : 0;
    if (sceneView->getState()!=state) 
    {
        sceneView->setState(state);
    }

    osgViewer::View* view = dynamic_cast<osgViewer::View*>(_camera->getView());
    osgDB::DatabasePager* databasePager = view ? view->getDatabasePager() : 0;
    sceneView->getCullVisitor()->setDatabaseRequestHandler(databasePager);
    
    sceneView->setFrameStamp(view ? view->getFrameStamp() : state->getFrameStamp());
    
    if (databasePager) databasePager->setCompileGLObjectsForContextID(state->getContextID(), true);
    
    osg::DisplaySettings* ds = _camera->getDisplaySettings() ?  _camera->getDisplaySettings() :
                               ((view &&view->getDisplaySettings()) ?  view->getDisplaySettings() :  osg::DisplaySettings::instance());

    sceneView->setDisplaySettings(ds);

    if (view) _startTick = view->getStartTick();
}


void Renderer::cull()
{
    DEBUG_MESSAGE<<"cull()"<<std::endl;

    if (_done || _graphicsThreadDoesCull) return;

    // note we assume lock has already been aquired.
    osgUtil::SceneView* sceneView = _sceneView[_currentCull].get();

    if (sceneView)
    {
        updateSceneView(sceneView);

        // osg::notify(osg::NOTICE)<<"Culling buffer "<<_currentCull<<std::endl;

        // pass on the fusion distance settings from the View to the SceneView
        osgViewer::View* view = dynamic_cast<osgViewer::View*>(sceneView->getCamera()->getView());
        if (view) sceneView->setFusionDistance(view->getFusionDistanceMode(), view->getFusionDistanceValue());

        osg::Stats* stats = sceneView->getCamera()->getStats();
        osg::State* state = sceneView->getState();
        const osg::FrameStamp* fs = state->getFrameStamp();
        int frameNumber = fs ? fs->getFrameNumber() : 0;

        _frameNumber[_currentCull] = frameNumber;

        // do cull taversal
        osg::Timer_t beforeCullTick = osg::Timer::instance()->tick();

        sceneView->inheritCullSettings(*(sceneView->getCamera()));
        sceneView->cull();

        osg::Timer_t afterCullTick = osg::Timer::instance()->tick();

#if 0
        if (sceneView->getDynamicObjectCount()==0 && state->getDynamicObjectRenderingCompletedCallback())
        {
            // osg::notify(osg::NOTICE)<<"Completed in cull"<<std::endl;
            state->getDynamicObjectRenderingCompletedCallback()->completed(state);
        }
#endif
        if (stats && stats->collectStats("rendering"))
        {
            DEBUG_MESSAGE<<"Collecting rendering stats"<<std::endl;
        
            stats->setAttribute(frameNumber, "Cull traversal begin time", osg::Timer::instance()->delta_s(_startTick, beforeCullTick));
            stats->setAttribute(frameNumber, "Cull traversal end time", osg::Timer::instance()->delta_s(_startTick, afterCullTick));
            stats->setAttribute(frameNumber, "Cull traversal time taken", osg::Timer::instance()->delta_s(beforeCullTick, afterCullTick));
        }
    }


    // relase the mutex associated with this cull traversal, let the draw commence.
    _lockHeld[_currentCull] = false;
    _mutex[_currentCull].unlock();

    // swap which SceneView we need to do cull traversal on next.
    _currentCull = 1 - _currentCull;

    // aquire the lock for it for the new cull traversal
    _mutex[_currentCull].lock();
    _lockHeld[_currentCull] = true;

    DEBUG_MESSAGE<<"end cull() "<<this<<std::endl;
}

void Renderer::draw()
{
    DEBUG_MESSAGE<<"draw() "<<this<<std::endl;

    osgUtil::SceneView* sceneView = _sceneView[_currentDraw].get();

    osg::GraphicsContext* compileContext = osg::GraphicsContext::getCompileContext(sceneView->getState()->getContextID());
    osg::GraphicsThread* compileThread = compileContext ? compileContext->getGraphicsThread() : 0;

    if (sceneView || _done)
    {
        OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex[_currentDraw]);

        osgViewer::View* view = dynamic_cast<osgViewer::View*>(_camera->getView());
        osgDB::DatabasePager* databasePager = view ? view->getDatabasePager() : 0;

        // osg::notify(osg::NOTICE)<<"Drawing buffer "<<_currentDraw<<std::endl;

        if (_done)
        {
            osg::notify(osg::INFO)<<"Renderer::release() causing draw to exit"<<std::endl;
            return;
        }

        if (_graphicsThreadDoesCull)
        {
            osg::notify(osg::INFO)<<"Renderer::draw() completing early due to change in _graphicsThreadDoesCull flag."<<std::endl;
            return;
        }

        // osg::notify(osg::NOTICE)<<"RenderingOperation"<<std::endl;

        osg::Stats* stats = sceneView->getCamera()->getStats();
        osg::State* state = sceneView->getState();
        int frameNumber = _frameNumber[_currentDraw];

        if (!_initialized)
        {
            initialize(state);
        }

        state->setDynamicObjectCount(sceneView->getDynamicObjectCount());

        if (sceneView->getDynamicObjectCount()==0 && state->getDynamicObjectRenderingCompletedCallback())
        {
            // osg::notify(osg::NOTICE)<<"Completed in cull"<<std::endl;
            state->getDynamicObjectRenderingCompletedCallback()->completed(state);
        }

        osg::Timer_t beforeDrawTick = osg::Timer::instance()->tick();

        bool aquireGPUStats = stats && _timerQuerySupported && stats->collectStats("gpu");

        if (aquireGPUStats) 
        {
            checkQuery(stats);
        }

        // do draw traveral
        if (aquireGPUStats) 
        {
            checkQuery(stats);
            beginQuery(frameNumber);
        }

        sceneView->draw();

        double availableTime = 0.004; // 4 ms
        if (databasePager && databasePager->requiresExternalCompileGLObjects(sceneView->getState()->getContextID()))
        {
            databasePager->compileGLObjects(*(sceneView->getState()), availableTime);
        }

        if (compileThread)
        {
            compileThread->add(_flushOperation.get());
        }
        else
        {
            sceneView->flushDeletedGLObjects(availableTime);
        }

        if (aquireGPUStats)
        {
            endQuery();
            checkQuery(stats);
        }

        glFlush();


        osg::Timer_t afterDrawTick = osg::Timer::instance()->tick();

        if (stats && stats->collectStats("rendering"))
        {
            stats->setAttribute(frameNumber, "Draw traversal begin time", osg::Timer::instance()->delta_s(_startTick, beforeDrawTick));
            stats->setAttribute(frameNumber, "Draw traversal end time", osg::Timer::instance()->delta_s(_startTick, afterDrawTick));
            stats->setAttribute(frameNumber, "Draw traversal time taken", osg::Timer::instance()->delta_s(beforeDrawTick, afterDrawTick));
        }
    }

    _currentDraw = 1-_currentDraw;

    DEBUG_MESSAGE<<"end draw() "<<this<<std::endl;
}

void Renderer::cull_draw()
{
    DEBUG_MESSAGE<<"cull_draw() "<<this<<std::endl;

    osgUtil::SceneView* sceneView = _sceneView[_currentDraw].get();
    if (!sceneView || _done) return;

    updateSceneView(sceneView);

    osgViewer::View* view = dynamic_cast<osgViewer::View*>(_camera->getView());
    osgDB::DatabasePager* databasePager = view ? view->getDatabasePager() : 0;

    osg::GraphicsContext* compileContext = osg::GraphicsContext::getCompileContext(sceneView->getState()->getContextID());
    osg::GraphicsThread* compileThread = compileContext ? compileContext->getGraphicsThread() : 0;

    OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex[_currentDraw]);

    if (_done)
    {
        osg::notify(osg::INFO)<<"Render::release() causing cull_draw to exit"<<std::endl;
        return;
    }

    // osg::notify(osg::NOTICE)<<"RenderingOperation"<<std::endl;

    // pass on the fusion distance settings from the View to the SceneView
    if (view) sceneView->setFusionDistance(view->getFusionDistanceMode(), view->getFusionDistanceValue());

    osg::Stats* stats = sceneView->getCamera()->getStats();
    osg::State* state = sceneView->getState();
    const osg::FrameStamp* fs = state->getFrameStamp();
    int frameNumber = fs ? fs->getFrameNumber() : 0;

    if (!_initialized)
    {
        initialize(state);
    }

    bool aquireGPUStats = stats && _timerQuerySupported && stats->collectStats("gpu");

    if (aquireGPUStats) 
    {
        checkQuery(stats);
    }

    // do cull taversal
    osg::Timer_t beforeCullTick = osg::Timer::instance()->tick();

    sceneView->inheritCullSettings(*(sceneView->getCamera()));
    sceneView->cull();

    osg::Timer_t afterCullTick = osg::Timer::instance()->tick();

#if 0
    if (state->getDynamicObjectCount()==0 && state->getDynamicObjectRenderingCompletedCallback())
    {
        state->getDynamicObjectRenderingCompletedCallback()->completed(state);
    }
#endif

    // do draw traveral
    if (aquireGPUStats) 
    {
        checkQuery(stats);
        beginQuery(frameNumber);
    }

    sceneView->draw();

    double availableTime = 0.004; // 4 ms
    if (databasePager && databasePager->requiresExternalCompileGLObjects(sceneView->getState()->getContextID()))
    {
        databasePager->compileGLObjects(*(sceneView->getState()), availableTime);
    }

    if (compileThread)
    {
        compileThread->add(_flushOperation.get());
    }
    else
    {
        sceneView->flushDeletedGLObjects(availableTime);
    }

    if (aquireGPUStats)
    {
        endQuery();
        checkQuery(stats);
    }

    osg::Timer_t afterDrawTick = osg::Timer::instance()->tick();

    if (stats && stats->collectStats("rendering"))
    {
        DEBUG_MESSAGE<<"Collecting rendering stats"<<std::endl;

        stats->setAttribute(frameNumber, "Cull traversal begin time", osg::Timer::instance()->delta_s(_startTick, beforeCullTick));
        stats->setAttribute(frameNumber, "Cull traversal end time", osg::Timer::instance()->delta_s(_startTick, afterCullTick));
        stats->setAttribute(frameNumber, "Cull traversal time taken", osg::Timer::instance()->delta_s(beforeCullTick, afterCullTick));

        stats->setAttribute(frameNumber, "Draw traversal begin time", osg::Timer::instance()->delta_s(_startTick, afterCullTick));
        stats->setAttribute(frameNumber, "Draw traversal end time", osg::Timer::instance()->delta_s(_startTick, afterDrawTick));
        stats->setAttribute(frameNumber, "Draw traversal time taken", osg::Timer::instance()->delta_s(afterCullTick, afterDrawTick));
    }

    DEBUG_MESSAGE<<"end cull_draw() "<<this<<std::endl;

}

void Renderer::operator () (osg::Object* object)
{
    osg::GraphicsContext* context = dynamic_cast<osg::GraphicsContext*>(object);
    if (context) operator()(context);

    osg::Camera* camera = dynamic_cast<osg::Camera*>(object);
    if (camera) cull();
}

void Renderer::operator () (osg::GraphicsContext* context)
{
    if (_graphicsThreadDoesCull)
    {
        cull_draw();
    }
    else
    {
        draw();
    }
}

void Renderer::release()
{
    osg::notify(osg::INFO)<<"Renderer::release()"<<std::endl;
    _done = true;

    if (_lockHeld[0])
    {
        _lockHeld[0] = false;
        _mutex[0].unlock();
    }

    if (_lockHeld[1])
    {
        _lockHeld[1] = false;
        _mutex[1].unlock();
    }
}
