#include <osgGA/TrackballManipulator>
#include <osg/Notify>

using namespace osg;
using namespace osgGA;

TrackballManipulator::TrackballManipulator()
{
    _modelScale = 0.01f;
    _minimumZoomScale = 0.05f;
    _thrown = false;

    _distance = 1.0f;
}


TrackballManipulator::~TrackballManipulator()
{
}


void TrackballManipulator::setNode(osg::Node* node)
{
    _node = node;
    if (_node.get())
    {
        const osg::BoundingSphere& boundingSphere=_node->getBound();
        _modelScale = boundingSphere._radius;
    }
}


const osg::Node* TrackballManipulator::getNode() const
{
    return _node.get();
}


osg::Node* TrackballManipulator::getNode()
{
    return _node.get();
}


                                 /*ea*/
void TrackballManipulator::home(const GUIEventAdapter& ,GUIActionAdapter& us)
{
    if(_node.get() && _camera.get())
    {

        const osg::BoundingSphere& boundingSphere=_node->getBound();

        _camera->setView(boundingSphere._center+osg::Vec3( 0.0,-3.5f * boundingSphere._radius,0.0f),
                        boundingSphere._center,
                        osg::Vec3(0.0f,0.0f,1.0f));

        computeLocalDataFromCamera();

        us.requestRedraw();
    }

}


void TrackballManipulator::init(const GUIEventAdapter& ,GUIActionAdapter& )
{
    flushMouseEventStack();
    
    computeLocalDataFromCamera();
}


void TrackballManipulator::getUsage(osg::ApplicationUsage& usage) const
{
    usage.addKeyboardMouseBinding("Trackball: Space","Reset the viewing position to home");
    usage.addKeyboardMouseBinding("Trackball: +","When in stereo, increase the fusion distance");
    usage.addKeyboardMouseBinding("Trackball: -","When in stereo, reduse the fusion distance");
}

bool TrackballManipulator::handle(const GUIEventAdapter& ea,GUIActionAdapter& us)
{
    if(!_camera.get()) return false;

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
            } else if (ea.getKey()=='+')
            {
                _camera->setFusionDistanceRatio(_camera->getFusionDistanceRatio()*1.25f);
                return true;
            }
            else if (ea.getKey()=='-')
            {
                _camera->setFusionDistanceRatio(_camera->getFusionDistanceRatio()/1.25f);
                return true;
            }
// this is quick hack to test out othographic projection.            
//             else if (ea.getKey()=='O')
//             {
//                 float dist = _camera->getLookDistance();
//                 _camera->setOrtho(-dist,dist,-dist,dist,-dist,dist);
//                 return true;
//             }
            return false;
        case(GUIEventAdapter::FRAME):
            _camera->setFusionDistanceMode(osg::Camera::PROPORTIONAL_TO_LOOK_DISTANCE);
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


bool TrackballManipulator::isMouseMoving()
{
    if (_ga_t0.get()==NULL || _ga_t1.get()==NULL) return false;

    static const float velocity = 0.1f;

    float dx = _ga_t0->getXnormalized()-_ga_t1->getXnormalized();
    float dy = _ga_t0->getYnormalized()-_ga_t1->getYnormalized();
    float len = sqrtf(dx*dx+dy*dy);
    float dt = _ga_t0->time()-_ga_t1->time();

    return (len>dt*velocity);
}


void TrackballManipulator::flushMouseEventStack()
{
    _ga_t1 = NULL;
    _ga_t0 = NULL;
}


void TrackballManipulator::addMouseEvent(const GUIEventAdapter& ea)
{
    _ga_t1 = _ga_t0;
    _ga_t0 = &ea;
}



void TrackballManipulator::computeLocalDataFromCamera()
{
    // maths from gluLookAt/osg::Matrix::makeLookAt
    osg::Vec3 f(_camera->getCenterPoint()-_camera->getEyePoint());
    f.normalize();
    osg::Vec3 s(f^_camera->getUpVector());
    s.normalize();
    osg::Vec3 u(s^f);
    u.normalize();

    osg::Matrix rotation_matrix(s[0],     u[0],     -f[0],     0.0f,
                                s[1],     u[1],     -f[1],     0.0f,
                                s[2],     u[2],     -f[2],     0.0f,
                                0.0f,     0.0f,     0.0f,      1.0f);
                   
    _center = _camera->getCenterPoint();
    _distance = _camera->getLookDistance();
    _rotation.set(rotation_matrix);
    _rotation = _rotation.inverse();
     
}

void TrackballManipulator::computeCameraFromLocalData()
{
    osg::Matrix new_rotation;
    new_rotation.makeRotate(_rotation);
    
    osg::Vec3 up = osg::Vec3(0.0f,1.0f,0.0) * new_rotation;
    osg::Vec3 eye = (osg::Vec3(0.0f,0.0f,_distance) * new_rotation) + _center;

    _camera->setLookAt(eye,_center,up);
}


bool TrackballManipulator::calcMovement()
{

    // return if less then two events have been added.
    if (_ga_t0.get()==NULL || _ga_t1.get()==NULL) return false;

    float dx = _ga_t0->getXnormalized()-_ga_t1->getXnormalized();
    float dy = _ga_t0->getYnormalized()-_ga_t1->getYnormalized();


    // return if there is no movement.
    if (dx==0 && dy==0) return false;

    float focalLength = (_camera->getCenterPoint()-_camera->getEyePoint()).length();
    unsigned int buttonMask = _ga_t1->getButtonMask();
    if (buttonMask==GUIEventAdapter::LEFT_MOUSE_BUTTON)
    {

        // rotate camera.

        osg::Vec3 axis;
        float angle;

        float px0 = _ga_t0->getXnormalized();
        float py0 = _ga_t0->getYnormalized();
        
        float px1 = _ga_t1->getXnormalized();
        float py1 = _ga_t1->getYnormalized();
        

        trackball(axis,angle,px1,py1,px0,py0);

        osg::Quat new_rotate;
        new_rotate.makeRotate(angle,axis);
        
        _rotation = _rotation*new_rotate;

        computeCameraFromLocalData();

        return true;

    }
    else if (buttonMask==GUIEventAdapter::MIDDLE_MOUSE_BUTTON ||
        buttonMask==(GUIEventAdapter::LEFT_MOUSE_BUTTON|GUIEventAdapter::RIGHT_MOUSE_BUTTON))
    {

        // pan model.

        float scale = -0.5f*focalLength;

        osg::Vec3 uv = _camera->getUpVector();
        osg::Vec3 sv = _camera->getSideVector();
        osg::Vec3 dv = uv*(dy*scale)+sv*(dx*scale);

        _center += dv;
        
        computeCameraFromLocalData();

        return true;

    }
    else if (buttonMask==GUIEventAdapter::RIGHT_MOUSE_BUTTON)
    {

        // zoom model.

        float fd = focalLength;
        float scale = 1.0f+dy;
        if (fd*scale>_modelScale*_minimumZoomScale)
        {

            _distance *= scale;

            computeCameraFromLocalData();

        }
        else
        {

            // notify(DEBUG_INFO) << "Pushing forward"<<std::endl;
            // push the camera forward.
            float scale = -fd;
            osg::Vec3 dv = _camera->getLookVector()*(dy*scale);

            _center += dv;

            computeCameraFromLocalData();

        }

        return true;

    }

    return false;
}


/*
 * This size should really be based on the distance from the center of
 * rotation to the point on the object underneath the mouse.  That
 * point would then track the mouse as closely as possible.  This is a
 * simple example, though, so that is left as an Exercise for the
 * Programmer.
 */
const float TRACKBALLSIZE = 0.8f;

/*
 * Ok, simulate a track-ball.  Project the points onto the virtual
 * trackball, then figure out the axis of rotation, which is the cross
 * product of P1 P2 and O P1 (O is the center of the ball, 0,0,0)
 * Note:  This is a deformed trackball-- is a trackball in the center,
 * but is deformed into a hyperbolic sheet of rotation away from the
 * center.  This particular function was chosen after trying out
 * several variations.
 *
 * It is assumed that the arguments to this routine are in the range
 * (-1.0 ... 1.0)
 */
void TrackballManipulator::trackball(osg::Vec3& axis,float& angle, float p1x, float p1y, float p2x, float p2y)
{
    /*
     * First, figure out z-coordinates for projection of P1 and P2 to
     * deformed sphere
     */

    osg::Vec3 uv = _camera->getUpVector();
    osg::Vec3 sv = _camera->getSideVector();
    osg::Vec3 lv = _camera->getLookVector();

    osg::Vec3 p1 = sv*p1x+uv*p1y-lv*tb_project_to_sphere(TRACKBALLSIZE,p1x,p1y);
    osg::Vec3 p2 = sv*p2x+uv*p2y-lv*tb_project_to_sphere(TRACKBALLSIZE,p2x,p2y);

    /*
     *  Now, we want the cross product of P1 and P2
     */

// Robert,
//
// This was the quick 'n' dirty  fix to get the trackball doing the right 
// thing after fixing the Quat rotations to be right-handed.  You may want
// to do something more elegant.
//   axis = p1^p2;
axis = p2^p1;
    axis.normalize();

    /*
     *  Figure out how much to rotate around that axis.
     */
    float t = (p2-p1).length() / (2.0*TRACKBALLSIZE);

    /*
     * Avoid problems with out-of-control values...
     */
    if (t > 1.0) t = 1.0;
    if (t < -1.0) t = -1.0;
    angle = inRadians(asin(t));

}


/*
 * Project an x,y pair onto a sphere of radius r OR a hyperbolic sheet
 * if we are away from the center of the sphere.
 */
float TrackballManipulator::tb_project_to_sphere(float r, float x, float y)
{
    float d, t, z;

    d = sqrt(x*x + y*y);
                                 /* Inside sphere */
    if (d < r * 0.70710678118654752440)
    {
        z = sqrt(r*r - d*d);
    }                            /* On hyperbola */
    else
    {
        t = r / 1.41421356237309504880;
        z = t*t / d;
    }
    return z;
}
