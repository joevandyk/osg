#include <osgText/Text>
#include <osgText/Font>

#include <iostream>
#include <string>

#include <osg/Vec3>
#include <osg/Vec4>

#include <osgDB/Registry>
#include <osgDB/Input>
#include <osgDB/Output>
#include <osgDB/ParameterOutput>

bool Text_readLocalData(osg::Object &obj, osgDB::Input &fr);
bool Text_writeLocalData(const osg::Object &obj, osgDB::Output &fw);

osgDB::RegisterDotOsgWrapperProxy Text_Proxy
(
    new osgText::Text,
    "Text",
    "Object Drawable Text",
    Text_readLocalData,
    Text_writeLocalData
);

bool Text_readLocalData(osg::Object &obj, osgDB::Input &fr)
{
    osgText::Text &text = static_cast<osgText::Text &>(obj);
    bool itAdvanced = false;


    if (fr.matchSequence("font %w"))
    { 
        text.setFont(fr[1].getStr());
        fr += 2;
        itAdvanced = true;
        
    }

    if (fr[0].matchWord("fontSize"))
    {
        unsigned int width;
        unsigned int height;
        if (fr[1].getUInt(width) && fr[2].getUInt(height))
        {
            text.setFontSize(width,height);
            fr += 3;
            itAdvanced = true;
        }
    }

    if (fr[0].matchWord("characterSize"))
    {
        float height;
        float aspectRatio;
        if (fr[1].getFloat(height) && fr[2].getFloat(aspectRatio))
        {
            text.setCharacterSize(height,aspectRatio);
            fr += 3;
            itAdvanced = true;
        }
    }

    // maximum dimentsions of text box.
    if (fr[0].matchWord("maximumWidth"))
    {
        float width;
        if (fr[1].getFloat(width))
        {
            text.setMaximumWidth(width);
            fr += 2;
            itAdvanced = true;
        }
    }

    if (fr[0].matchWord("maximumHeight"))
    {
        float height;
        if (fr[1].getFloat(height))
        {
            text.setMaximumWidth(height);
            fr += 2;
            itAdvanced = true;
        }
    }

    if (fr.matchSequence("alignment %w"))
    {
        std::string str = fr[1].getStr();
        if      (str=="LEFT_TOP") text.setAlignment(osgText::Text::LEFT_TOP);
        else if (str=="LEFT_CENTER") text.setAlignment(osgText::Text::LEFT_CENTER);
        else if (str=="LEFT_BOTTOM") text.setAlignment(osgText::Text::LEFT_BOTTOM);
        else if (str=="CENTER_TOP") text.setAlignment(osgText::Text::CENTER_TOP);
        else if (str=="CENTER_CENTER") text.setAlignment(osgText::Text::CENTER_CENTER);
        else if (str=="CENTER_BOTTOM") text.setAlignment(osgText::Text::CENTER_BOTTOM);
        else if (str=="RIGHT_TOP") text.setAlignment(osgText::Text::RIGHT_TOP);
        else if (str=="RIGHT_CENTER") text.setAlignment(osgText::Text::RIGHT_CENTER);
        else if (str=="RIGHT_BOTTOM") text.setAlignment(osgText::Text::RIGHT_BOTTOM);
        else if (str=="BASE_LINE") text.setAlignment(osgText::Text::BASE_LINE);
        fr += 2;
        itAdvanced = true;
    }
    
    if (fr.matchSequence("axisAlignment %w"))
    {
        std::string str = fr[1].getStr();
        if      (str=="XY_PLANE") text.setAxisAlignment(osgText::Text::XY_PLANE);
        else if (str=="XZ_PLANE") text.setAxisAlignment(osgText::Text::XZ_PLANE);
        else if (str=="YZ_PLANE") text.setAxisAlignment(osgText::Text::YZ_PLANE);
        else if (str=="SCREEN") text.setAxisAlignment(osgText::Text::SCREEN);
        fr += 2;
        itAdvanced = true;
    }

    if (fr.matchSequence("rotation"))
    {
        osg::Vec4 rotation;
        if (fr[1].getFloat(rotation.x()) && fr[2].getFloat(rotation.y()) && fr[3].getFloat(rotation.z()) && fr[4].getFloat(rotation.w()))
        {
            text.setRotation(rotation);
            fr += 4;
            itAdvanced = true;
        }
    }

    if (fr.matchSequence("layout %w"))
    {
        std::string str = fr[1].getStr();
        if      (str=="LEFT_TO_RIGHT") text.setLayout(osgText::Text::LEFT_TO_RIGHT);
        else if (str=="RIGHT_TO_LEFT") text.setLayout(osgText::Text::RIGHT_TO_LEFT);
        else if (str=="VERTICAL") text.setLayout(osgText::Text::VERTICAL);
        fr += 2;
        itAdvanced = true;
    }


    // position
    if (fr[0].matchWord("position"))
    {
        osg::Vec3 p;
        if (fr[1].getFloat(p.x()) && fr[2].getFloat(p.y()) && fr[3].getFloat(p.z()))
        {
            text.setPosition(p);
            fr += 4;
            itAdvanced = true;
        }
    }

    // color
    if (fr[0].matchWord("color"))
    {
        osg::Vec4 c;
        if (fr[1].getFloat(c.x()) && fr[2].getFloat(c.y()) && fr[3].getFloat(c.z()) && fr[4].getFloat(c.w()))
        {
            text.setColor(c);
            fr += 4;
            itAdvanced = true;
        }
    }

    // draw mode
    if (fr[0].matchWord("drawMode"))
    {
        int i;
        if (fr[1].getInt(i)) {
            text.setDrawMode(i);
            fr += 2;
            itAdvanced = true;
        }
    }

    // text
    if (fr.matchSequence("text %s")) {
        text.setText(std::string(fr[1].getStr()));
        fr += 2;
        itAdvanced = true;
    }

    if (fr.matchSequence("text %i {"))
    {
        // pre 0.9.3 releases..
        int entry = fr[0].getNoNestedBrackets();

        int capacity;
        fr[1].getInt(capacity);

        osgText::String str;
        str.reserve(capacity);

        fr += 3;

        while (!fr.eof() && fr[0].getNoNestedBrackets()>entry)
        {
            unsigned int c;
            if (fr[0].getUInt(c))
            {
                ++fr;
                str.push_back(c);
            }
            else
            {
                ++fr;
            }
        }

        text.setText(str);

        itAdvanced = true;
        ++fr;
    }

    return itAdvanced;
}

bool Text_writeLocalData(const osg::Object &obj, osgDB::Output &fw)
{
    const osgText::Text &text = static_cast<const osgText::Text &>(obj);

    if (text.getFont())
    {
        fw.indent() << "font " << text.getFont()->getFileName() << std::endl;
    }

    // font resolution
    fw.indent() << "fontSize " << text.getFontWidth() << " " << text.getFontHeight() << std::endl;

    // charater size.
    fw.indent() << "characterSize " << text.getCharacterHeight() << " " << text.getCharacterAspectRatio() << std::endl;

    // maximum dimension of text box.
    if (text.getMaximumWidth()>0.0f)
    {
        fw.indent() << "maximumWidth " << text.getMaximumWidth() << std::endl;
    }
    
    if (text.getMaximumHeight()>0.0f)
    {
        fw.indent() << "maximumHeight " << text.getMaximumHeight() << std::endl;
    }
    
    // alignment
    fw.indent() << "alignment ";
    switch(text.getAlignment())
    {
      case osgText::Text::LEFT_TOP:        fw << "LEFT_TOP" << std::endl; break;
      case osgText::Text::LEFT_CENTER :    fw << "LEFT_CENTER" << std::endl; break;
      case osgText::Text::LEFT_BOTTOM :    fw << "LEFT_BOTTOM" << std::endl; break;
      
      case osgText::Text::CENTER_TOP:      fw << "CENTER_TOP" << std::endl; break;
      case osgText::Text::CENTER_CENTER:   fw << "CENTER_CENTER" << std::endl; break;
      case osgText::Text::CENTER_BOTTOM:   fw << "CENTER_BOTTOM" << std::endl; break;
      
      case osgText::Text::RIGHT_TOP:       fw << "RIGHT_TOP" << std::endl; break;
      case osgText::Text::RIGHT_CENTER:    fw << "RIGHT_CENTER" << std::endl; break;
      case osgText::Text::RIGHT_BOTTOM:    fw << "RIGHT_BOTTOM" << std::endl; break;
      
      case osgText::Text::BASE_LINE:       fw << "BASE_LINE" << std::endl; break;
    };


    // axis alignment
    fw.indent() << "axisAlignment ";
    switch(text.getAxisAlignment())
    {
      case osgText::Text::XY_PLANE: fw << "XY_PLANE" << std::endl; break;
      case osgText::Text::XZ_PLANE: fw << "XZ_PLANE" << std::endl; break;
      case osgText::Text::YZ_PLANE: fw << "YZ_PLANE" << std::endl; break;
      case osgText::Text::SCREEN:   fw << "SCREEN" << std::endl; break;
    };

    if (!text.getRotation().zeroRotation())
    {
        fw.indent() << "rotation " << text.getRotation() << std::endl;
    }


    // layout
    fw.indent() << "layout ";
    switch(text.getLayout())
    {
      case osgText::Text::LEFT_TO_RIGHT: fw << "LEFT_TO_RIGHT" << std::endl; break;
      case osgText::Text::RIGHT_TO_LEFT: fw << "RIGHT_TO_LEFT" << std::endl; break;
      case osgText::Text::VERTICAL: fw << "VERTICAL" << std::endl; break;
    };


    // position
    osg::Vec3 p = text.getPosition();
    fw.indent() << "position " << p.x() << " " << p.y() << " " << p.z() << std::endl;

    // color
    osg::Vec4 c = text.getColor();
    fw.indent() << "color " << c.x() << " " << c.y() << " " << c.z() << " " << c.w() << std::endl;

    // draw mode
    fw.indent() << "drawMode " << text.getDrawMode() << std::endl;


    // text
    const osgText::String& textstring = text.getText();
    bool isACString = true;
    for(osgText::String::const_iterator itr=textstring.begin();
        itr!=textstring.end() && isACString;
        ++itr)
    {
        if (*itr==0 || *itr>256) isACString=false;
    }
    if (isACString)
    {
        std::string str;
        std::copy(textstring.begin(),textstring.end(),std::back_inserter(str));
        fw.indent() << "text " << fw.wrapString(str) << std::endl;
    }
    else
    {
        // do it the hardway...output each character as an int
        fw.indent() << "text "<<textstring.size()<<std::endl;;
        osgDB::writeArray(fw,textstring.begin(),textstring.end());
    }

    return true;
}
