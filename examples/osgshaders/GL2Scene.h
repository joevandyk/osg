/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2003 Robert Osfield 
 * Copyright (C) 2003 3Dlabs Inc. Ltd.
 *
 * This application is open source and may be redistributed and/or modified   
 * freely and without restriction, both in commericial and non commericial applications,
 * as long as this copyright notice is maintained.
 * 
 * This application is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*/

/* file:	examples/osgshaders/GL2Scene.h
 * author:	Mike Weiblen 2003-09-18
 *
 * See http://www.3dlabs.com/opengl2/ for more information regarding
 * the OpenGL Shading Language.
*/

#include <osg/Node>
#include <osg/Referenced>
#include <osg/ref_ptr>

#include <osgGL2/ProgramObject>

class GL2Scene : public osg::Referenced
{
    public:
	GL2Scene();

	osg::ref_ptr<osg::Group> getRootNode() { return _rootNode; }
	void reloadShaderSource();
	void toggleShaderEnable();

    protected:
	~GL2Scene();

    private:	/*methods*/
	osg::ref_ptr<osg::Group> buildScene();

    private:	/*data*/
	osg::ref_ptr<osg::Group> _rootNode;
	std::vector<osgGL2::ProgramObjectPtr> _progObjList;
	bool _shadersEnabled;
};

typedef osg::ref_ptr<GL2Scene> GL2ScenePtr;

/*EOF*/

