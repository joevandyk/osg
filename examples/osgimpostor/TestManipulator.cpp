#include "TestManipulator.h"
#include <osg/Notify>

using namespace osg;
using namespace osgGA;

TestManipulator::TestManipulator()
{
    _modelScale = 0.01f;
    _minimumZoomScale = 0.05f;
    _thrown = false;

    _distance = 1.0f;
}


TestManipulator::~TestManipulator()
{
}


void TestManipulator::setNode(osg::Node* node)
{
    _node = node;
    if (_node.get())
    {
        const osg::BoundingSphere& boundingSphere=_node->getBound();
        _modelScale = boundingSphere._radius;
    }
}


const osg::Node* TestManipulator::getNode() const
{
    return _node.get();
}


osg::Node* TestManipulator::getNode()
{
    return _node.get();
}


                                 /*ea*/
void TestManipulator::home(const GUIEventAdapter& ,GUIActionAdapter& us)
{
    if(_node.get())
    {

        const osg::BoundingSphere& boundingSphere=_node->getBound();

        computePosition(boundingSphere.center()+osg::Vec3(0.0f, 0.0f, 20.0f),
			osg::Vec3(0.0f, 1.0f, 0.0f),
			osg::Vec3(0.0f,  0.0f,  1.0f));

        us.requestRedraw();
    }
}


void TestManipulator::init(const GUIEventAdapter& ,GUIActionAdapter& )
{
    flushMouseEventStack();
}

bool TestManipulator::handle(const GUIEventAdapter& ea,GUIActionAdapter& us)
{
    switch(ea.getEventType())
    {
        case(GUIEventAdapter::PUSH):
        {
            flushMouseEventStack();
            addMouseEvent(ea);
            if (calcMovement()) us.requestRedraw();
            us.requestContinuousUpdate(false);
            _thrown = false;
            return true;
        }

        case(GUIEventAdapter::RELEASE):
        {
            if (ea.getButtonMask()==0)
            {

                if (isMouseMoving())
                {
                    if (calcMovement())
                    {
                        us.requestRedraw();
                        us.requestContinuousUpdate(true);
                        _thrown = true;
                    }
                }
                else
                {
                    flushMouseEventStack();
                    addMouseEvent(ea);
                    if (calcMovement()) us.requestRedraw();
                    us.requestContinuousUpdate(false);
                    _thrown = false;
                }

            }
            else
            {
                flushMouseEventStack();
                addMouseEvent(ea);
                if (calcMovement()) us.requestRedraw();
                us.requestContinuousUpdate(false);
                _thrown = false;
            }
            return true;
        }

        case(GUIEventAdapter::DRAG):
        {
            addMouseEvent(ea);
            if (calcMovement()) us.requestRedraw();
            us.requestContinuousUpdate(false);
            _thrown = false;
            return true;
        }

        case(GUIEventAdapter::MOVE):
        {
            return false;
        }

        case(GUIEventAdapter::KEYDOWN):
            if (ea.getKey()==' ')
            {
                flushMouseEventStack();
                _thrown = false;
                home(ea,us);
                us.requestRedraw();
                us.requestContinuousUpdate(false);
                return true;
            } 
            return false;
        case(GUIEventAdapter::FRAME):
            if (_thrown)
            {
                if (calcMovement()) us.requestRedraw();
                return true;
            }
            return false;
        default:
            return false;
    }
}


bool TestManipulator::isMouseMoving()
{
    if (_ga_t0.get()==NULL || _ga_t1.get()==NULL) return false;

    static const float velocity = 0.1f;

    float dx = _ga_t0->getXnormalized()-_ga_t1->getXnormalized();
    float dy = _ga_t0->getYnormalized()-_ga_t1->getYnormalized();
    float len = sqrtf(dx*dx+dy*dy);
    float dt = _ga_t0->time()-_ga_t1->time();

    return (len>dt*velocity);
}


void TestManipulator::flushMouseEventStack()
{
    _ga_t1 = NULL;
    _ga_t0 = NULL;
}


void TestManipulator::addMouseEvent(const GUIEventAdapter& ea)
{
    _ga_t1 = _ga_t0;
    _ga_t0 = &ea;
}

void TestManipulator::setByMatrix(const osg::Matrix& matrix)
{
    _center = matrix.getTrans();
    matrix.get(_rotation);
    _distance = 1.0f;
}

osg::Matrix TestManipulator::getMatrix() const
{
    return osg::Matrix::rotate(_rotation)*osg::Matrix::translate(_center);
}

osg::Matrix TestManipulator::getInverseMatrix() const
{
    return osg::Matrix::translate(-_center)*osg::Matrix::rotate(_rotation.inverse());
}

void TestManipulator::computePosition(const osg::Vec3& eye,const osg::Vec3& lv,const osg::Vec3& up)
{
    osg::Vec3 f(lv);
    f.normalize();
    osg::Vec3 s(f^up);
    s.normalize();
    osg::Vec3 u(s^f);
    u.normalize();
    
    osg::Matrix rotation_matrix(s[0],     u[0],     -f[0],     0.0f,
                                s[1],     u[1],     -f[1],     0.0f,
                                s[2],     u[2],     -f[2],     0.0f,
                                0.0f,     0.0f,     0.0f,      1.0f);
                   
    _center = eye+lv;
    _distance = lv.length();
    rotation_matrix.get(_rotation);
    _rotation = _rotation.inverse();
}


bool TestManipulator::calcMovement()
{

    // return if less then two events have been added.
    if (_ga_t0.get()==NULL || _ga_t1.get()==NULL) return false;

    float dx = _ga_t0->getXnormalized()-_ga_t1->getXnormalized();
    float dy = _ga_t0->getYnormalized()-_ga_t1->getYnormalized();


    // return if there is no movement.
    if (dx==0 && dy==0) return false;

    unsigned int buttonMask = _ga_t1->getButtonMask();
    if (buttonMask==GUIEventAdapter::LEFT_MOUSE_BUTTON)
    {

        // rotate camera.

        osg::Quat new_rotate;
	new_rotate.makeRotate(dx / 3.0f, osg::Vec3(0.0f, 0.0f, 1.0f));
        
        _rotation = _rotation*new_rotate;

        return true;

    }
    else if (buttonMask==GUIEventAdapter::MIDDLE_MOUSE_BUTTON)
    {

        // pan model.

	osg::Vec3 dv = osg::Vec3(0.0f, 0.0f, -500.0f) * dy;

        _center += dv;
        
        return true;

    }
    else if (buttonMask==GUIEventAdapter::RIGHT_MOUSE_BUTTON)
    {
        osg::Matrix rotation_matrix(_rotation);
    
        		
        osg::Vec3 uv = osg::Vec3(0.0f,1.0f,0.0f)*rotation_matrix;
        osg::Vec3 sv = osg::Vec3(1.0f,0.0f,0.0f)*rotation_matrix;
        osg::Vec3 fv = uv ^ sv;
        osg::Vec3 dv = fv*(dy*-500.0f)-sv*(dx*500.0f);

        _center += dv;

	return true;
    }

    return false;
}
