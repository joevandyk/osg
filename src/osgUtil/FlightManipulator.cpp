#include "osgUtil/FlightManipulator"
#include "osg/Notify"

using namespace osg;
using namespace osgUtil;

FlightManipulator::FlightManipulator()
{
    _modelScale = 0.01f;
    _velocity = 0.0f;
    _yawMode = YAW_AUTOMATICALLY_WHEN_BANKED;
}

FlightManipulator::~FlightManipulator()
{
}

void FlightManipulator::setNode(osg::Node* node)
{
    _node = node;
    if (_node.get())
    {
	const osg::BoundingSphere& boundingSphere=_node->getBound();
        _modelScale = boundingSphere._radius;
    }
}

osg::Node* FlightManipulator::getNode() const
{
    return _node.get();
}

void FlightManipulator::home(GUIEventAdapter& ea,GUIActionAdapter& us)
{
    if(_node.get() && _camera.get())
    {

	const osg::BoundingSphere& boundingSphere=_node->getBound();

        _camera->setView(boundingSphere._center+osg::Vec3( 0.0,-2.0f * boundingSphere._radius,0.0f),	// eye
                         boundingSphere._center,                                // look
                         osg::Vec3(0.0f,0.0f,1.0f));	                        // up
                         
        _velocity = 0.0f;

        us.needRedraw();

        us.needWarpPointer((ea.getXmin()+ea.getXmax())/2,(ea.getYmin()+ea.getYmax())/2);

        flushMouseEventStack();

    }
    
}

void FlightManipulator::init(GUIEventAdapter& ea,GUIActionAdapter& us)
{
    flushMouseEventStack();

    us.needContinuousUpdate(false);

    _velocity = 0.0f;

    us.needWarpPointer((ea.getXmin()+ea.getXmax())/2,(ea.getYmin()+ea.getYmax())/2);

}

bool FlightManipulator::update(GUIEventAdapter& ea,GUIActionAdapter& us)
{
    if(!_camera.get()) return false;
    
    switch(ea.getEventType())
    {
    case(GUIEventAdapter::PUSH):
        {
            
            addMouseEvent(ea);
            us.needContinuousUpdate(true);
            if (calcMovement()) us.needRedraw();

        }
        return true;
    case(GUIEventAdapter::RELEASE):
        {
            
            addMouseEvent(ea);
            us.needContinuousUpdate(true);
            if (calcMovement()) us.needRedraw();

        }
        return true;
    case(GUIEventAdapter::DRAG):
        {
            
            addMouseEvent(ea);
            us.needContinuousUpdate(true);
            if (calcMovement()) us.needRedraw();

        }
        return true;
    case(GUIEventAdapter::MOVE):
        {
            
            addMouseEvent(ea);
            us.needContinuousUpdate(true);
            if (calcMovement()) us.needRedraw();

        }
        return true;

    case(GUIEventAdapter::KEYBOARD):
        if (ea.getKey()==' ') 
        {
            flushMouseEventStack();
            home(ea,us);
            us.needRedraw();
            us.needContinuousUpdate(false);
            return true;
        }
        return false;
    case(GUIEventAdapter::FRAME):
        addMouseEvent(ea);
        if (calcMovement()) us.needRedraw();
        return true;
    case(GUIEventAdapter::RESIZE):
        {
            init(ea,us);
            us.needRedraw();
        }
        return true;
    default:
        return false;
    }
}

void FlightManipulator::flushMouseEventStack()
{
    _ga_t1 = NULL;
    _ga_t0 = NULL;
}

void FlightManipulator::addMouseEvent(GUIEventAdapter& ea)
{
    _ga_t1 = _ga_t0;
    _ga_t0 = &ea;
}

bool FlightManipulator::calcMovement()
{
    // return if less then two events have been added.
    if (_ga_t0.get()==NULL || _ga_t1.get()==NULL) return false;

    float dt = _ga_t0->time()-_ga_t1->time();


    if (dt<0.0f)
    {
	    notify(WARN) << "warning dt = "<<dt<<endl;
	    dt = 0.0f;
    }

    unsigned int buttonMask = _ga_t1->getButtonMask();
    if (buttonMask==GUIEventAdapter::LEFT_BUTTON)
    {
        // pan model.

        _velocity += dt*_modelScale*0.05f;

    }
    else if (buttonMask==GUIEventAdapter::MIDDLE_BUTTON || 
             buttonMask==(GUIEventAdapter::LEFT_BUTTON|GUIEventAdapter::RIGHT_BUTTON))
    {

        _velocity = 0.0f;

    }
    else if (buttonMask==GUIEventAdapter::RIGHT_BUTTON)
    {

        _velocity -= dt*_modelScale*0.05f;

    }
    
    float mx = (_ga_t0->getXmin()+_ga_t0->getXmax())/2.0f;
    float my = (_ga_t0->getYmin()+_ga_t0->getYmax())/2.0f;

    float dx = _ga_t0->getX()-mx;
    float dy = _ga_t0->getY()-my;

    osg::Vec3 center = _camera->getEyePoint();
    osg::Vec3 sv = _camera->getSideVector();
    osg::Vec3 lv = _camera->getLookVector();

    float pitch = -dy*0.15f*dt;
    float roll = -dx*0.1f*dt;

    osg::Matrix mat;
    mat.makeTrans(-center.x(),-center.y(),-center.z());
    mat.postRot(pitch,sv.x(),sv.y(),sv.z());
    mat.postRot(roll,lv.x(),lv.y(),lv.z());
    if (_yawMode==YAW_AUTOMATICALLY_WHEN_BANKED)
    {
        float bank = asinf(sv.z());
        float yaw = (-bank*180.0f/M_PI)*dt;
        mat.postRot(yaw,0.0f,0.0f,1.0f);
    }
    mat.postTrans(center.x(),center.y(),center.z());


    lv *= (_velocity*dt);

    mat.postTrans(lv.x(),lv.y(),lv.z());

    _camera->mult(*_camera,mat);

    return true;
}
