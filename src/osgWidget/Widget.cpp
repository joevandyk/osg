// -*-c++-*- osgWidget - Code by: Jeremy Moles (cubicool) 2007-2008
// $Id: Widget.cpp 64 2008-06-30 21:32:00Z cubicool $

#include <osg/io_utils>
#include <osg/Math>
#include <osg/BlendFunc>
#include <osg/TexMat>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgWidget/WindowManager>

// Don't use these macros! :) They're simply for internal optimization!
#define MACRO_WIDGET_X(v) (*v)[LL].x()
#define MACRO_WIDGET_Y(v) (*v)[LL].y()
#define MACRO_WIDGET_W(v) (*v)[LR].x() - (*v)[LL].x()
#define MACRO_WIDGET_H(v) (*v)[UL].y() - (*v)[LL].y()

namespace osgWidget {

osg::ref_ptr<PointArray> Widget::_norms;

Widget::Widget(const std::string& name, point_type w, point_type h):
_parent    (0),
_index     (0),
_layer     (LAYER_LOW),
_padLeft   (0.0f),
_padRight  (0.0f),
_padTop    (0.0f),
_padBottom (0.0f),
_valign    (VA_CENTER),
_halign    (HA_CENTER),
_coordMode (CM_ABSOLUTE),
_canFill   (false),
_canClone  (true),
_isManaged (false),
_isStyled  (false),
_minWidth  (w),
_minHeight (h) {
    _name = name.size() ? name : generateRandomName("Widget");

    if(!_norms.valid()) {
        _norms = new PointArray(1);

        (*_norms)[0].set(0.0f, 0.0f, 1.0f);
        (*_norms)[0].normalize();
    }

    TexCoordArray* texs = new TexCoordArray(4);

    // Fill our texture coordinates with null stuff for now, since we aren't using them
    // until an Image is set at some later point.
    std::fill(texs->begin(), texs->end(), osg::Vec2(0.0f, 0.0f));

    setUseDisplayList(false);
    setDataVariance(osg::Object::DYNAMIC);
    setVertexArray(new PointArray(4));
    setColorArray(new ColorArray(4));
    setNormalArray(_norms.get());
    setTexCoordArray(0, texs);
    setNormalBinding(osg::Geometry::BIND_OVERALL);
    setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

    setDimensions(0.0f, 0.0f, w, h);
    setColor(1.0f, 1.0f, 1.0f, 1.0f);

    getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
    getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
}

Widget::Widget(const Widget& widget, const osg::CopyOp& co):
osg::Geometry  (widget, co),
EventInterface (widget),
StyleInterface (widget),
_parent        (0),
_index         (0),
_layer         (widget._layer),
_padLeft       (widget._padLeft),
_padRight      (widget._padRight),
_padTop        (widget._padTop),
_padBottom     (widget._padBottom),
_valign        (widget._valign),
_halign        (widget._halign),
_coordMode     (widget._coordMode),
_canFill       (widget._canFill),
_canClone      (widget._canClone),
_isManaged     (false),
_isStyled      (widget._isStyled),
_minWidth      (widget._minWidth),
_minHeight     (widget._minHeight) {
}

// This takes an integer value and translates it into a value that will be added to
// the parent window's Z value.
point_type Widget::_calculateZ(unsigned int layer) const {
    point_type zRange = 0.0f;

    if(_parent) zRange = _parent->getZRange();

    return (static_cast<point_type>(layer) / static_cast<point_type>(LAYER_TOP + 1)) * zRange;

}

WindowManager* Widget::_getWindowManager() const {
    if(!_parent) return 0;

    return _parent->getWindowManager();
}

osg::Image* Widget::_getImage() const {
    const osg::Texture2D* texture = _texture();

    if(texture) return const_cast<osg::Image*>(texture->getImage(0));

    return 0;
}

void Widget::managed(WindowManager* wm) {
    if(!wm->isInvertedY()) return;

    osg::Matrix s = osg::Matrix::scale(1.0f, -1.0f, 1.0f);
    osg::Matrix t = osg::Matrix::translate(0.0f, -1.0, 0.0f);

    getOrCreateStateSet()->setTextureAttributeAndModes(
        0,
        new osg::TexMat(t * s),
        osg::StateAttribute::ON
    );
}

void Widget::setDimensions(point_type x, point_type y, point_type w, point_type h, point_type z) {
    if(w != -1.0f && w < _minWidth) {
        warn()
            << "Widget [" << _name
            << "] was asked to set it's width to " << w
            << ", but the minimum width is " << _minWidth
            << "." << std::endl
        ;

        w = _minWidth;
    }

    if(h != -1.0f && h < _minHeight) {
        warn()
            << "Widget [" << _name
            << "] was asked to set it's height to " << h
            << ", but the minimum height is " << _minHeight
            << "." << std::endl
        ;

        h = _minHeight;
    }

    PointArray* verts = _verts();

    if(_coordMode == CM_ABSOLUTE) {
        // If any of our values are 0, replace them with the current value.
        // We could just call getWidth(), etc., but all those dynamic_casts could eventually
        // get expensive, so we just use the already-created verts() array directly.
        if(x < 0.0f) x = MACRO_WIDGET_X(verts);
        if(y < 0.0f) y = MACRO_WIDGET_Y(verts);
        if(w < 0.0f) w = MACRO_WIDGET_W(verts);
        if(h < 0.0f) h = MACRO_WIDGET_H(verts);
    }

    else {
        if(x < 0.0f) x = _relCoords[0];
        if(y < 0.0f) y = _relCoords[1];
        if(w < 0.0f) w = _relCoords[2];
        if(h < 0.0f) h = _relCoords[3];
    }

    if(z < 0.0f) z = _calculateZ(_layer);

    // Now, we need to determine if the dimensions are actually percentage of the parent's
    // size, rather than an absolute values. The Widget must be parented for this to be
    // valid, however.
    if(_coordMode == CM_RELATIVE) {
        XYCoord size;
        
        if(_parent) size = _parent->getSize();

        if(x >= 0.0f && x <= 1.0f) {
            _relCoords[0] = x;
            
            x = size.x() * x;
        }

        if(y >= 0.0f && y <= 1.0f) {
            _relCoords[1] = y;

            y = size.y() * y;
        }
        
        if(w >= 0.0f && w <= 1.0f) {
            _relCoords[2] = w;
            
            w = size.x() * w;
        }
        
        if(h >= 0.0f && h <= 1.0f) {
            _relCoords[3] = h;
            
            h = size.y() * h;
        }
    }

    (*verts)[LL].set(x,     y,     z);
    (*verts)[LR].set(x + w, y,     z);
    (*verts)[UR].set(x + w, y + h, z);
    (*verts)[UL].set(x,     y + h, z);
}

void Widget::setColor(color_type r, color_type g, color_type b, color_type a, Corner p) {
    ColorArray* cols = _cols();

    if(p == ALL_CORNERS) {
        (*cols)[LL].set(r, g, b, a);
        (*cols)[LR].set(r, g, b, a);
        (*cols)[UR].set(r, g, b, a);
        (*cols)[UL].set(r, g, b, a);
    }

    else (*cols)[convertCorner(p)].set(r, g, b, a);
}

void Widget::addColor(color_type r, color_type g, color_type b, color_type a, Corner p) {
    ColorArray* cols = _cols();
    
    if(p == ALL_CORNERS) {
        (*cols)[LL] += Color(r, g, b, a);
        (*cols)[LR] += Color(r, g, b, a);
        (*cols)[UR] += Color(r, g, b, a);
        (*cols)[UL] += Color(r, g, b, a);
    }

    else (*cols)[convertCorner(p)] += Color(r, g, b, a);
}

void Widget::setTexCoord(texcoord_type tx, texcoord_type ty, Corner p) {
    TexCoordArray* texs = _texs();

    if(p == ALL_CORNERS) {
        (*texs)[LL].set(tx, ty);
        (*texs)[LR].set(tx, ty);
        (*texs)[UR].set(tx, ty);
        (*texs)[UL].set(tx, ty);
    }

    else (*texs)[convertCorner(p)].set(tx, ty);
}

void Widget::setTexCoordRegion(point_type x, point_type y, point_type w, point_type h) {
    osg::Image* image = _image();

    if(!image) return;

    point_type tw = image->s();
    point_type th = image->t();

    TexCoordArray* texs = _texs();

    // Set the LOWER_LEFT point.
    XYCoord t(x / tw, y / tw);

    (*texs)[UL] = t;
    
    // Set the LOWER_RIGHT point.
    t += XYCoord(w / tw, 0.0f);

    (*texs)[UR] = t;

    // Set the UPPER_RIGHT point.
    t += XYCoord(0.0f, h / th);

    (*texs)[LR] = t;

    // Set the UPPER_LEFT point.
    t += XYCoord(-(w / tw), 0.0f);

    (*texs)[LL] = t;
}

void Widget::setTexCoordWrapHorizontal() {
    osg::Image*     image   = _image();
    osg::Texture2D* texture = _texture();

    if(!image || !texture || image->s() == 0.0f) return;

    texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);

    setTexCoord(getWidth() / image->s(), 0.0f, LOWER_RIGHT);
    setTexCoord(getWidth() / image->s(), 1.0f, UPPER_RIGHT);
}

void Widget::setTexCoordWrapVertical() {
    osg::Image*     image   = _image();
    osg::Texture2D* texture = _texture();

    if(!image || !texture || image->t() == 0.0f) return;
    
    texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);

    setTexCoord(0.0f, getHeight() / image->t(), UPPER_LEFT);
    setTexCoord(1.0f, getHeight() / image->t(), UPPER_RIGHT);
}

XYCoord Widget::localXY(double _x, double _y) const {
    if(!_parent) return XYCoord(_x, _y);

    return _parent->localXY(_x, _y) - getOrigin();
}

bool Widget::setImage(osg::Image* image, bool setTexCoords) {
    if(!image) {
        warn() << "Widget [" << _name << "] cannot use a NULL image." << std::endl;

        return false;
    }

    osg::Texture2D* texture = new osg::Texture2D();
    
    texture->setDataVariance(osg::Object::DYNAMIC);
    texture->setImage(0, image);

    getOrCreateStateSet()->setTextureAttributeAndModes(
        0,
        texture,
        osg::StateAttribute::ON
    );

    if(setTexCoords) {
        setTexCoord(0.0f, 0.0f, LOWER_LEFT);
        setTexCoord(1.0f, 0.0f, LOWER_RIGHT);
        setTexCoord(1.0f, 1.0f, UPPER_RIGHT);
        setTexCoord(0.0f, 1.0f, UPPER_LEFT);
    }

    return true;
}

bool Widget::setImage(const std::string& filePath, bool setTexCoords) {
    if(!osgDB::findDataFile(filePath).size()) {
        warn()
            << "Widget [" << _name
            << "] cannot find file " << filePath
            << " to set as it's Image." << std::endl
        ;

        return false;
    }

    return setImage(osgDB::readImageFile(filePath), setTexCoords);
}

void Widget::setPadding(point_type pad) {
    _padLeft = _padRight = _padTop = _padBottom = pad;
}

void Widget::addX(point_type x) {
    if(_coordMode == CM_ABSOLUTE) setDimensions(MACRO_WIDGET_X(_verts()) + x);

    else setDimensions(_relCoords[0] + x);
}

void Widget::addY(point_type y) {
    if(_coordMode == CM_ABSOLUTE) setDimensions(-1.0f, MACRO_WIDGET_Y(_verts()) + y);

    else setDimensions(-1.0f, _relCoords[1] + y);
}

void Widget::addWidth(point_type w) {
    if(_coordMode == CM_ABSOLUTE) setDimensions(-1.0f, -1.0f, MACRO_WIDGET_W(_verts()) + w);

    else setDimensions(-1.0f, -1.0f, _relCoords[2] + w);
}

void Widget::addHeight(point_type h) {
    if(_coordMode == CM_ABSOLUTE) setDimensions(
        -1.0f,
        -1.0f,
        -1.0f,
        MACRO_WIDGET_H(_verts()) + h
    );

    else setDimensions(-1.0f, -1.0f, -1.0f, _relCoords[3] + h);
}

void Widget::addOrigin(point_type x, point_type y) {
    if(_coordMode == CM_ABSOLUTE) {
        PointArray* verts = _verts();

        setDimensions(
            MACRO_WIDGET_X(verts) + x,
            MACRO_WIDGET_Y(verts) + y
        );
    }

    else setDimensions(_relCoords[0] + x, _relCoords[1] + y);
}

void Widget::addSize(point_type w, point_type h) {
    if(_coordMode == CM_ABSOLUTE) {
        PointArray* verts = _verts();

        setDimensions(
            -1.0f,
            -1.0f,
            MACRO_WIDGET_W(verts) + w,
            MACRO_WIDGET_H(verts) + h
        );
    }
    
    else setDimensions(-1.0f, -1.0f, _relCoords[2] + w, _relCoords[3] + h);
}

point_type Widget::getWidth() const {
    const PointArray* verts = _verts();
    
    return MACRO_WIDGET_W(verts);
}

point_type Widget::getHeight() const {
    const PointArray* verts = _verts();
    
    return MACRO_WIDGET_H(verts);
}

point_type Widget::getX() const {
    return MACRO_WIDGET_X(_verts());
}

point_type Widget::getY() const {
    return MACRO_WIDGET_Y(_verts());
}

point_type Widget::getZ() const {
    return (*_verts())[LL].z();
}

point_type Widget::getPadHorizontal() const {
    return _padLeft + _padRight;
}

point_type Widget::getPadVertical() const {
    return _padTop + _padBottom;
}

const Point& Widget::getPoint(Corner p) const {
    Corner point = p;

    if(p == ALL_CORNERS) point = UPPER_LEFT;

    return (*_verts())[convertCorner(point)];
}

const Color& Widget::getColor(Corner p) const {
    Corner point = p;

    if(p == ALL_CORNERS) point = UPPER_LEFT;

    return (*_cols())[convertCorner(point)];
}

const TexCoord& Widget::getTexCoord(Corner p) const {
    Corner point = p;

    if(p == ALL_CORNERS) point = UPPER_LEFT;

    return (*_texs())[convertCorner(point)];
}

// This converts our points back and forth depding on whether or not we're in an
// inverted-Y WindowManager.
Widget::Corner Widget::convertCorner(Corner p) const {
    const WindowManager* wm = getWindowManager();

    if(!wm || !wm->isInvertedY()) return p;
    
    if(p == UPPER_LEFT) return LOWER_LEFT;

    else if(p == UPPER_RIGHT) return LOWER_RIGHT;

    else if(p == LOWER_LEFT) return UPPER_LEFT;

    else if(p == LOWER_RIGHT) return UPPER_RIGHT;

    else return p;
}

Color Widget::getImageColorAtXY(point_type x, point_type y) const {
    const osg::Image* image = _image();

    if(!image) return Color();

    const TexCoordArray* texs = _texs();

    /*
    How do we do this? First we need to make sure our right side is larger
    than our left side and that the top side is larger the bottom; otherwise,
    they're using strange tex coords.

    Then, we find the percent area being used in both dimensions. We multiply
    the XY values by those ratios and then add those values to the "offsets."
    */

    point_type width  = fabs((*texs)[LR].x() - (*texs)[LL].x());
    point_type height = fabs((*texs)[LR].y() - (*texs)[UR].y());

    point_type X = ((x / getWidth()) * width) + (*texs)[LL].x();
    point_type Y = (((getHeight() - y) / getHeight()) * height) + (*texs)[UR].y();
    
    return image->getColor(TexCoord(X, Y));
}

bool Widget::isPaddingUniform() const {
    return
        _padLeft == _padRight &&
        _padLeft == _padTop && 
        _padLeft == _padBottom
    ;
}

}
