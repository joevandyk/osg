#include <osg/io_utils>
#include <osgWidget/WindowManager>
#include <osgWidget/Input>

namespace osgWidget {

Input::Input(const std::string& name, const std::string& label, unsigned int size):
Label        (name, label),
_xoff        (0.0f), 
_yoff        (0.0f),
_index       (0),
_size        (0),
_cursorIndex (0),
_maxSize     (size),
_cursor      (new Widget("cursor")) {
    _text->setAlignment(osgText::Text::LEFT_BOTTOM_BASE_LINE);
    _text->setKerningType(osgText::KERNING_NONE);

    // Make the cursor un-copyable.
    _cursor->setCanClone(false);
    _cursor->setDataVariance(osg::Object::DYNAMIC);
    _cursor->setColor(0.0f, 0.0f, 0.0f, 0.0f);

    setEventMask(
        // For showing/hiding the "cursor."
        EVENT_MASK_FOCUS |
        // For keypresses, obviously.
        EVENT_MASK_KEY |
        // For "click" focusing.
        EVENT_MOUSE_PUSH
    );

    _offsets.resize(size, 0.0f);

    _text->getText().resize(size, ' ');
    _text->update();
}

void Input::_calculateSize(const XYCoord& size) {
    // An Input cannot currently set it's own size RELIABLY until the osgText implementation
    // is dratiscally improved. I'm getting wildly crazy results. :(
    // point_type height = size.y() > _cursor->getHeight() ? size.y() : _cursor->getHeight();
    point_type width  = size.x() + _cursor->getWidth();
    point_type height = _cursor->getHeight();

    if(width > getWidth()) setWidth(osg::round(width));

    if(height > getHeight()) setHeight(osg::round(height));
}

void Input::_calculateCursorOffsets() {
    // Determine the "offset"
    const osgText::Text::TextureGlyphQuadMap& tgqm = _text->getTextureGlyphQuadMap();

    const osgText::Text::TextureGlyphQuadMap::const_iterator tgqmi = tgqm.begin();
    
    const osgText::Text::GlyphQuads& gq = tgqmi->second;

    point_type accum = 0.0f;

    std::ostream& os = warn() << "_offsets[ ";
    
    for(unsigned int i = 0; i < _maxSize; i++) {
        osg::Vec2 ul = gq.getCoords()[0 + (i * 4)];
        osg::Vec2 ll = gq.getCoords()[1 + (i * 4)];
        osg::Vec2 lr = gq.getCoords()[2 + (i * 4)];
        osg::Vec2 ur = gq.getCoords()[3 + (i * 4)];
        
        accum += osg::round(lr.x() - ll.x());

        _offsets[i] = accum;

        os << _offsets[i] << " (" << static_cast<char>(_text->getText()[i]) << ") ";
    }

    os << "]" << std::endl;
}

bool Input::focus(WindowManager*) {
    _cursor->setColor(1.0f, 1.0f, 1.0f, 0.5f);

    return true;
}

bool Input::unfocus(WindowManager*) {
    _cursor->setColor(0.0f, 0.0f, 0.0f, 0.0f);

    return true;
}

void Input::parented(Window* parent) {
    Label::parented(parent);

    _cursor->setSize(2.0f, _text->getCharacterHeight());

    if(_cursorIndex) parent->getGeode()->setDrawable(_cursorIndex, _cursor.get());

    else _cursorIndex = parent->addDrawableAndGetIndex(_cursor.get());
}

void Input::positioned() {
    point_type ln = static_cast<point_type>(_text->getLineCount());

    ln = ln == 0.0f ? 1.0f : ln;

    point_type th =
        (_text->getCharacterHeight() * ln) +
        (_text->getLineSpacing() * (ln - 1.0f))
    ;

    point_type x = getX() + _xoff;
    point_type y = getY() + th + _yoff;

    // XYCoord size = getTextSize();

    _text->setPosition(osg::Vec3(x, y, _calculateZ(LAYER_MIDDLE)));
    
    point_type xoffset = _index > 0 ? _offsets[_index - 1] : 0.0f;

    _cursor->setOrigin(x + xoffset + 1.0f, y - _cursor->getHeight() + 1.0f);
    _cursor->setZ(_calculateZ(LAYER_MIDDLE));
}

bool Input::keyUp(int key, int mask, WindowManager*) {
    return false;
}

bool Input::keyDown(int key, int mask, WindowManager*) {
    /*
    osgText::String& s = _text->getText();

    if(key == osgGA::GUIEventAdapter::KEY_BackSpace) {
        if(_index >= 1) {
            // s.erase(s.begin() + (_index - 1));

            s[_index - 1] = ' ';

            _text->update();
            
            _calculateCursorOffsets();

            _index--;
        }
    }

    else {
        if(key > 255 || _index >= _maxSize) return false;

        // else if(_index < s.size()) s.insert(s.begin() + _index, key);
        // else if(_index == s.size()) s.push_back(key);

        s[_index] = key;
        
        _text->update();
        
        _calculateCursorOffsets();

        _index++;
    }

    // _text->update();

    _calculateSize(getTextSize());

    getParent()->resize();
    */

    warn() << "Input is disabled until some changes can be made to osgText (09/02/2008)..." << std::endl;

    return false;
}

}
