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

#include <osgUtil/Optimizer>
#include <osgDB/ReadFile>

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>

#include <osgGA/TrackballManipulator>

#include <osg/Material>
#include <osg/Geode>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osg/PolygonOffset>
#include <osg/MatrixTransform>
#include <osg/Camera>

#include <osgText/Text>


osg::Camera* createHUD()
{
    // create a camera to set up the projection and model view matrices, and the subgraph to drawn in the HUD
    osg::Camera* camera = new osg::Camera;

    // set the projection matrix
    camera->setProjectionMatrix(osg::Matrix::ortho2D(0,1280,0,1024));

    // set the view matrix    
    camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
    camera->setViewMatrix(osg::Matrix::identity());

    // only clear the depth buffer
    camera->setClearMask(GL_DEPTH_BUFFER_BIT);

    // draw subgraph after main camera view.
    camera->setRenderOrder(osg::Camera::POST_RENDER);

    // we don't want the camera to grab event focus from the viewers main camera(s).
    camera->setAllowEventFocus(false);
    


    // add to this camera a subgraph to render
    {

        osg::Geode* geode = new osg::Geode();

        std::string timesFont("fonts/arial.ttf");

        // turn lighting off for the text and disable depth test to ensure its always ontop.
        osg::StateSet* stateset = geode->getOrCreateStateSet();
        stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);

        osg::Vec3 position(150.0f,800.0f,0.0f);
        osg::Vec3 delta(0.0f,-120.0f,0.0f);

        {
            osgText::Text* text = new  osgText::Text;
            geode->addDrawable( text );

            text->setFont(timesFont);
            text->setPosition(position);
            text->setText("Head Up Displays are simple :-)");

            position += delta;
        }    


        {
            osgText::Text* text = new  osgText::Text;
            geode->addDrawable( text );

            text->setFont(timesFont);
            text->setPosition(position);
            text->setText("All you need to do is create your text in a subgraph.");

            position += delta;
        }    


        {
            osgText::Text* text = new  osgText::Text;
            geode->addDrawable( text );

            text->setFont(timesFont);
            text->setPosition(position);
            text->setText("Then place an osg::Camera above the subgraph\n"
                          "to create an orthographic projection.\n");

            position += delta;
        } 

        {
            osgText::Text* text = new  osgText::Text;
            geode->addDrawable( text );

            text->setFont(timesFont);
            text->setPosition(position);
            text->setText("Set the Camera's ReferenceFrame to ABSOLUTE_RF to ensure\n"
                          "it remains independent from any external model view matrices.");

            position += delta;
        } 

        {
            osgText::Text* text = new  osgText::Text;
            geode->addDrawable( text );

            text->setFont(timesFont);
            text->setPosition(position);
            text->setText("And set the Camera's clear mask to just clear the depth buffer.");

            position += delta;
        }    

        {
            osgText::Text* text = new  osgText::Text;
            geode->addDrawable( text );

            text->setFont(timesFont);
            text->setPosition(position);
            text->setText("And finally set the Camera's RenderOrder to POST_RENDER\n"
                          "to make sure its drawn last.");

            position += delta;
        }    


        {
            osg::BoundingBox bb;
            for(unsigned int i=0;i<geode->getNumDrawables();++i)
            {
                bb.expandBy(geode->getDrawable(i)->getBound());
            }

            osg::Geometry* geom = new osg::Geometry;

            osg::Vec3Array* vertices = new osg::Vec3Array;
            float depth = bb.zMin()-0.1;
            vertices->push_back(osg::Vec3(bb.xMin(),bb.yMax(),depth));
            vertices->push_back(osg::Vec3(bb.xMin(),bb.yMin(),depth));
            vertices->push_back(osg::Vec3(bb.xMax(),bb.yMin(),depth));
            vertices->push_back(osg::Vec3(bb.xMax(),bb.yMax(),depth));
            geom->setVertexArray(vertices);

            osg::Vec3Array* normals = new osg::Vec3Array;
            normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));
            geom->setNormalArray(normals);
            geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

            osg::Vec4Array* colors = new osg::Vec4Array;
            colors->push_back(osg::Vec4(1.0f,1.0,0.8f,0.2f));
            geom->setColorArray(colors);
            geom->setColorBinding(osg::Geometry::BIND_OVERALL);

            geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));

            osg::StateSet* stateset = geom->getOrCreateStateSet();
            stateset->setMode(GL_BLEND,osg::StateAttribute::ON);
            //stateset->setAttribute(new osg::PolygonOffset(1.0f,1.0f),osg::StateAttribute::ON);
            stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

            geode->addDrawable(geom);
        }

        camera->addChild(geode);
    }

    return camera;
}

int main( int argc, char **argv )
{
    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc,argv);


    // read the scene from the list of file specified commandline args.
    osg::ref_ptr<osg::Node> scene = osgDB::readNodeFiles(arguments);
    
    // if not loaded assume no arguments passed in, try use default mode instead.
    if (!scene) scene = osgDB::readNodeFile("dumptruck.osg");
    
    
    if (!scene)
    {
        osg::notify(osg::NOTICE)<<"No model loaded"<<std::endl;
        return 1;
    }


    if (arguments.read("--Viewer"))
    {
        // construct the viewer.
        osgViewer::Viewer viewer;

        // create a HUD as slave camera attached to the master view.
        
        viewer.setUpViewAcrossAllScreens();

        osgViewer::Viewer::Windows windows;
        viewer.getWindows(windows);
        
        if (windows.empty()) return 1;
        
        osg::Camera* hudCamera = createHUD();
        
        // set up cameras to rendering on the first window available.
        hudCamera->setGraphicsContext(windows[0]);
        hudCamera->setViewport(0,0,windows[0]->getTraits()->width, windows[0]->getTraits()->height);

        viewer.addSlave(hudCamera, false);

        // set the scene to render
        viewer.setSceneData(scene.get());

        return viewer.run();

    }
    if (arguments.read("--CompositeViewer"))
    {
        // construct the viewer.
        osgViewer::CompositeViewer viewer;

        // create the main 3D view
        osgViewer::View* view = new osgViewer::View;
        viewer.addView(view);

        view->setSceneData(scene.get());
        view->setUpViewAcrossAllScreens();;
        view->setCameraManipulator(new osgGA::TrackballManipulator);

        // now create the HUD camera's view
        
        osgViewer::Viewer::Windows windows;
        viewer.getWindows(windows);
        
        if (windows.empty()) return 1;
        
        osg::Camera* hudCamera = createHUD();
        
        // set up cameras to rendering on the first window available.
        hudCamera->setGraphicsContext(windows[0]);
        hudCamera->setViewport(0,0,windows[0]->getTraits()->width, windows[0]->getTraits()->height);
        
        osgViewer::View* hudView = new osgViewer::View;
        hudView->setCamera(hudCamera);
        
        viewer.addView(hudView);

        return viewer.run();

    }
    else
    {
        // construct the viewer.
        osgViewer::Viewer viewer;

        osg::ref_ptr<osg::Group> group  = new osg::Group;

        // add the HUD subgraph.    
        if (scene.valid()) group->addChild(scene.get());
        group->addChild(createHUD());

        // set the scene to render
        viewer.setSceneData(group.get());

        return viewer.run();
    }
    
}
