#include "osg/LineSegment"

using namespace osg;

const bool LineSegment::intersectAndClip(Vec3& s,Vec3& e,const BoundingBox& bb)
{
    // compate s and e against the xMin to xMax range of bb.
    if (s.x()<=e.x())
    {

        // trivial reject of segment wholely outside.
        if (e.x()<bb.xMin()) return false;
        if (s.x()>bb.xMax()) return false;

        if (s.x()<bb.xMin())
        {
            // clip s to xMin.
            s = s+(e-s)*(bb.xMin()-s.x())/(e.x()-s.x());
        }

        if (e.x()>bb.xMax())
        {
            // clip e to xMax.
            e = s+(e-s)*(bb.xMax()-s.x())/(e.x()-s.x());
        }
    }
    else
    {
        if (s.x()<bb.xMin()) return false;
        if (e.x()>bb.xMax()) return false;

        if (e.x()<bb.xMin())
        {
            // clip s to xMin.
            e = s+(e-s)*(bb.xMin()-s.x())/(e.x()-s.x());
        }

        if (s.x()>bb.xMax())
        {
            // clip e to xMax.
            s = s+(e-s)*(bb.xMax()-s.x())/(e.x()-s.x());
        }
    }

    // compate s and e against the yMin to yMax range of bb.
    if (s.y()<=e.y())
    {

        // trivial reject of segment wholely outside.
        if (e.y()<bb.yMin()) return false;
        if (s.y()>bb.yMax()) return false;

        if (s.y()<bb.yMin())
        {
            // clip s to yMin.
            s = s+(e-s)*(bb.yMin()-s.y())/(e.y()-s.y());
        }

        if (e.y()>bb.yMax())
        {
            // clip e to yMax.
            e = s+(e-s)*(bb.yMax()-s.y())/(e.y()-s.y());
        }
    }
    else
    {
        if (s.y()<bb.yMin()) return false;
        if (e.y()>bb.yMax()) return false;

        if (e.y()<bb.yMin())
        {
            // clip s to yMin.
            e = s+(e-s)*(bb.yMin()-s.y())/(e.y()-s.y());
        }

        if (s.y()>bb.yMax())
        {
            // clip e to yMax.
            s = s+(e-s)*(bb.yMax()-s.y())/(e.y()-s.y());
        }
    }

    // compate s and e against the zMin to zMax range of bb.
    if (s.z()<=e.z())
    {

        // trivial reject of segment wholely outside.
        if (e.z()<bb.zMin()) return false;
        if (s.z()>bb.zMax()) return false;

        if (s.z()<bb.zMin())
        {
            // clip s to zMin.
            s = s+(e-s)*(bb.zMin()-s.z())/(e.z()-s.z());
        }

        if (e.z()>bb.zMax())
        {
            // clip e to zMax.
            e = s+(e-s)*(bb.zMax()-s.z())/(e.z()-s.z());
        }
    }
    else
    {
        if (s.z()<bb.zMin()) return false;
        if (e.z()>bb.zMax()) return false;

        if (e.z()<bb.zMin())
        {
            // clip s to zMin.
            e = s+(e-s)*(bb.zMin()-s.z())/(e.z()-s.z());
        }

        if (s.z()>bb.zMax())
        {
            // clip e to zMax.
            s = s+(e-s)*(bb.zMax()-s.z())/(e.z()-s.z());
        }
    }

    return true;
}


const bool LineSegment::intersect(const BoundingBox& bb) const
{
    if (!bb.isValid()) return false;

    Vec3 s=_s,e=_e;
    return intersectAndClip(s,e,bb);
}


const bool LineSegment::intersect(const BoundingBox& bb,float& r1,float& r2) const
{
    if (!bb.isValid()) return false;

    Vec3 s=_s,e=_e;
    bool result = intersectAndClip(s,e,bb);
    if (result)
    {
        float len = (_e-_s).length();
        if (len>0.0f)
        {
            float inv_len = 1.0f/len;
            r1 = (s-_s).length()*inv_len;
            r2 = (e-_e).length()*inv_len;
        }
        else
        {
            r1 = 0.0f;
            r2 = 0.0f;
        }
    }
    return result;
}


const bool LineSegment::intersect(const BoundingSphere& bs,float& r1,float& r2) const
{
    Vec3 sm = _s-bs._center;
    float c = sm.length2()-bs._radius*bs._radius;

    Vec3 se = _e-_s;
    float a = se.length2();

    float b = sm*se*2.0f;

    float d = b*b-4.0f*a*c;

    if (d<0.0f) return false;

    d = sqrtf(d);

    float div = 1.0f/(2.0f*a);

    r1 = (-b-d)*div;
    r2 = (-b+d)*div;

    if (r1<=0.0f && r2<=0.0f) return false;

    if (r1>=1.0f && r2>=1.0f) return false;

    return true;
}


const bool LineSegment::intersect(const BoundingSphere& bs) const
{
    Vec3 sm = _s-bs._center;
    float c = sm.length2()-bs._radius*bs._radius;
    if (c<0.0f) return true;

    Vec3 se = _e-_s;
    float a = se.length2();

    float b = (sm*se)*2.0f;

    float d = b*b-4.0f*a*c;

    if (d<0.0f) return false;

    d = sqrtf(d);

    float div = 1.0f/(2.0f*a);

    float r1 = (-b-d)*div;
    float r2 = (-b+d)*div;

    if (r1<=0.0f && r2<=0.0f) return false;

    if (r1>=1.0f && r2>=1.0f) return false;

    return true;
}


const bool LineSegment::intersect(const Vec3& v1,const Vec3& v2,const Vec3& v3,float& r)
{
    if (v1==v2 || v2==v3 || v1==v3) return false;

    Vec3 vse = _e-_s;

    Vec3 v12 = v2-v1;
    Vec3 n12 = v12^vse;
    float ds12 = (_s-v1)*n12;
    float d312 = (v3-v1)*n12;
    if (d312>=0.0f)
    {
        if (ds12<0.0f) return false;
        if (ds12>d312) return false;
    }
    else                         // d312 < 0
    {
        if (ds12>0.0f) return false;
        if (ds12<d312) return false;
    }

    Vec3 v23 = v3-v2;
    Vec3 n23 = v23^vse;
    float ds23 = (_s-v2)*n23;
    float d123 = (v1-v2)*n23;
    if (d123>=0.0f)
    {
        if (ds23<0.0f) return false;
        if (ds23>d123) return false;
    }
    else                         // d123 < 0
    {
        if (ds23>0.0f) return false;
        if (ds23<d123) return false;
    }

    Vec3 v31 = v1-v3;
    Vec3 n31 = v31^vse;
    float ds31 = (_s-v3)*n31;
    float d231 = (v2-v3)*n31;
    if (d231>=0.0f)
    {
        if (ds31<0.0f) return false;
        if (ds31>d231) return false;
    }
    else                         // d231 < 0
    {
        if (ds31>0.0f) return false;
        if (ds31<d231) return false;
    }

    float r3 = ds12/d312;
    float r1 = ds23/d123;
    float r2 = ds31/d231;

    //    float rt = r1+r2+r3;

    Vec3 in = v1*r1+v2*r2+v3*r3;

    float length = vse.length();
    vse /= length;
    float d = (in-_s)*vse;

    if (d<0.0f) return false;
    if (d>length) return false;

    r = d/length;

    return true;
}
