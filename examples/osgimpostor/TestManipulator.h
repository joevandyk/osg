//C++ header - Open Scene Graph - Copyright (C) 1998-2002 Robert Osfield
//Distributed under the terms of the GNU Library General Public License (LGPL)
//as published by the Free Software Foundation.

#ifndef OSGGA_TESTMANIPULATOR
#define OSGGA_TESTMANIPULATOR 1

#include <osgGA/MatrixManipulator>
#include <osg/Quat>

class TestManipulator : public osgGA::MatrixManipulator
{
    public:

        TestManipulator();
        virtual ~TestManipulator();

        /** set the position of the matrix manipulator using a 4x4 Matrix.*/
        virtual void setByMatrix(const osg::Matrix& matrix);

        /** set the position of the matrix manipulator using a 4x4 Matrix.*/
        virtual void setByInverseMatrix(const osg::Matrix& matrix) { setByMatrix(osg::Matrix::inverse(matrix)); }

        /** get the position of the manipulator as 4x4 Matrix.*/
        virtual osg::Matrix getMatrix() const;

        /** get the position of the manipulator as a inverse matrix of the manipulator, typically used as a model view matrix.*/
        virtual osg::Matrix getInverseMatrix() const;

        /** Attach a node to the manipulator. 
            Automatically detaches previously attached node.
            setNode(NULL) detaches previously nodes.
            Is ignored by manipulators which do not require a reference model.*/
        virtual void setNode(osg::Node*);

        /** Return node if attached.*/
        virtual const osg::Node* getNode() const;

        /** Return node if attached.*/
        virtual osg::Node* getNode();

        /** Move the camera to the default position. 
            May be ignored by manipulators if home functionality is not appropriate.*/
        virtual void home(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);
        
        /** Start/restart the manipulator.*/
        virtual void init(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);


        /** handle events, return true if handled, false otherwise.*/
        virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& us);

    private:

        /** Reset the internal GUIEvent stack.*/
        void flushMouseEventStack();
        /** Add the current mouse GUIEvent to internal stack.*/
        void addMouseEvent(const osgGA::GUIEventAdapter& ea);

        void computePosition(const osg::Vec3& eye,const osg::Vec3& lv,const osg::Vec3& up);

        /** For the give mouse movement calculate the movement of the camera.
            Return true is camera has moved and a redraw is required.*/
        bool calcMovement();
        
        /** Check the speed at which the mouse is moving.
            If speed is below a threshold then return false, otherwise return true.*/
        bool isMouseMoving();

        // Internal event stack comprising last three mouse events.
        osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t1;
        osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t0;

        osg::ref_ptr<osg::Node>       _node;

        float _modelScale;
        float _minimumZoomScale;

        bool _thrown;
        
        osg::Vec3   _center;
        osg::Quat   _rotation;
        float       _distance;

};

#endif
