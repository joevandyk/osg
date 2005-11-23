/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2005 Robert Osfield 
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
#include <osg/GLExtensions>
#include <osg/GL>
#include <osg/GLU>
#include <osg/Notify>
#include <osg/buffered_value>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <string>
#include <vector>
#include <set>

bool osg::isGLExtensionSupported(unsigned int contextID, const char *extension)
{
    typedef std::set<std::string>  ExtensionSet;
    static osg::buffered_object<ExtensionSet> s_extensionSetList;
    static osg::buffered_object<std::string> s_rendererList;
    static osg::buffered_value<int> s_initializedList;

    ExtensionSet& extensionSet = s_extensionSetList[contextID];
    std::string& rendererString = s_rendererList[contextID];

    // if not already set up, initialize all the per graphic context values.
    if (!s_initializedList[contextID])
    {
        s_initializedList[contextID] = 1;
    
        // set up the renderer
        const GLubyte* renderer = glGetString(GL_RENDERER);
        rendererString = renderer ? (const char*)renderer : "";

        // get the extension list from OpenGL.
        const char* extensions = (const char*)glGetString(GL_EXTENSIONS);
        if (extensions==NULL) return false;

        // insert the ' ' delimiated extensions words into the extensionSet.
        const char *startOfWord = extensions;
        const char *endOfWord;
        while ((endOfWord = strchr(startOfWord,' '))!=NULL)
        {
            extensionSet.insert(std::string(startOfWord,endOfWord));
            startOfWord = endOfWord+1;
        }
        if (*startOfWord!=0) extensionSet.insert(std::string(startOfWord));
        
        osg::notify(INFO)<<"OpenGL extensions supported by installed OpenGL drivers are:"<<std::endl;
        for(ExtensionSet::iterator itr=extensionSet.begin();
            itr!=extensionSet.end();
            ++itr)
        {
            osg::notify(INFO)<<"    "<<*itr<<std::endl;
        }
            
    }

    // true if extension found in extensionSet.
    bool result = extensionSet.find(extension)!=extensionSet.end();

    // now see if extension is in the extension disabled list
    bool extensionDisabled = false;
    if (result)
    {

        const std::string& disableString = getGLExtensionDisableString();
        if (!disableString.empty())
        {
                    
            std::string::size_type pos=0;
            while ( pos!=std::string::npos && (pos=disableString.find(extension,pos))!=std::string::npos )
            {
                std::string::size_type previousColon = disableString.find_last_of(':',pos);
                std::string::size_type previousSemiColon = disableString.find_last_of(';',pos);
                
                std::string renderer = "";
                if (previousColon!=std::string::npos)
                {
                    if (previousSemiColon==std::string::npos) renderer = disableString.substr(0,previousColon);
                    else if (previousSemiColon<previousColon) renderer = disableString.substr(previousSemiColon+1,previousColon-previousSemiColon-1);
                }

                if (!renderer.empty())
                {
                
                    // remove leading spaces if they exist.
                    std::string::size_type leadingSpaces = renderer.find_first_not_of(' ');
                    if (leadingSpaces==std::string::npos) renderer = ""; // nothing but spaces
                    else if (leadingSpaces!=0) renderer.erase(0,leadingSpaces);

                    // remove trailing spaces if they exist.
                    std::string::size_type trailingSpaces = renderer.find_last_not_of(' ');
                    if (trailingSpaces!=std::string::npos) renderer.erase(trailingSpaces+1,std::string::npos);

                }
                            
                if (renderer.empty())
                {
                    extensionDisabled = true;
                    break;
                }
                
                if (rendererString.find(renderer)!=std::string::npos)
                {
                    extensionDisabled = true;
                    break;
                    
                }
                
                // move the position in the disable string along so that the same extension is found multiple times
                ++pos;
            }

        }
    }

    if (result)
    {
        if (!extensionDisabled) osg::notify(INFO)<<"OpenGL extension '"<<extension<<"' is supported."<<std::endl;
        else osg::notify(INFO)<<"OpenGL extension '"<<extension<<"' is supported by OpenGL\ndriver but has been disabled by osg::getGLExtensionDisableString()."<<std::endl;
    }
    else osg::notify(INFO)<<"OpenGL extension '"<<extension<<"' is not supported."<<std::endl;


    return result && !extensionDisabled;
}

void osg::setGLExtensionDisableString(const std::string& disableString)
{
    getGLExtensionDisableString() = disableString;
}

std::string& osg::getGLExtensionDisableString()
{
    static const char* envVar = getenv("OSG_GL_EXTENSION_DISABLE");
    static std::string s_GLExtensionDisableString(envVar?envVar:"Nothing defined");
    return s_GLExtensionDisableString;
}


bool osg::isGLUExtensionSupported(unsigned int contextID, const char *extension)
{
    typedef std::set<std::string>  ExtensionSet;
    static osg::buffered_object<ExtensionSet> s_extensionSetList;
    static osg::buffered_object<std::string> s_rendererList;
    static osg::buffered_value<int> s_initializedList;

    ExtensionSet& extensionSet = s_extensionSetList[contextID];
    std::string& rendererString = s_rendererList[contextID];

    // if not already set up, initialize all the per graphic context values.
    if (!s_initializedList[contextID])
    {
        s_initializedList[contextID] = 1;
    
        // set up the renderer
        const GLubyte* renderer = glGetString(GL_RENDERER);
        rendererString = renderer ? (const char*)renderer : "";

        // get the extension list from OpenGL.
        const char* extensions = (const char*)gluGetString(GLU_EXTENSIONS);
        if (extensions==NULL) return false;

        // insert the ' ' delimiated extensions words into the extensionSet.
        const char *startOfWord = extensions;
        const char *endOfWord;
        while ((endOfWord = strchr(startOfWord,' '))!=NULL)
        {
            extensionSet.insert(std::string(startOfWord,endOfWord));
            startOfWord = endOfWord+1;
        }
        if (*startOfWord!=0) extensionSet.insert(std::string(startOfWord));
        
        osg::notify(INFO)<<"OpenGL extensions supported by installed OpenGL drivers are:"<<std::endl;
        for(ExtensionSet::iterator itr=extensionSet.begin();
            itr!=extensionSet.end();
            ++itr)
        {
            osg::notify(INFO)<<"    "<<*itr<<std::endl;
        }
            
    }

    // true if extension found in extensionSet.
    bool result = extensionSet.find(extension)!=extensionSet.end();

    if (result) osg::notify(INFO)<<"OpenGL utility library extension '"<<extension<<"' is supported."<<std::endl;
    else osg::notify(INFO)<<"OpenGL utility library extension '"<<extension<<"' is not supported."<<std::endl;

    return result;
}

#if defined(WIN32)
    #define WIN32_LEAN_AND_MEAN
    #ifndef NOMINMAX
        #define NOMINMAX
    #endif // NOMINMAX
    #include <windows.h>
#elif defined(__APPLE__)
    #include <mach-o/dyld.h>
#else
    #include <dlfcn.h>
#endif

void* osg::getGLExtensionFuncPtr(const char *funcName)
{
#if defined(WIN32)

    return (void*)wglGetProcAddress(funcName);

#elif defined(__APPLE__)

    std::string temp( "_" );
    temp += funcName;    // Mac OS X prepends an underscore on function names
    if ( NSIsSymbolNameDefined( temp.c_str() ) )
    {
        NSSymbol symbol = NSLookupAndBindSymbol( temp.c_str() );
        return NSAddressOfSymbol( symbol );
    } else
        return NULL;

#elif defined (__sun) 

     static void *handle = dlopen((const char *)0L, RTLD_LAZY);
     return dlsym(handle, funcName);
    
#elif defined (__sgi)

     static void *handle = dlopen((const char *)0L, RTLD_LAZY);
     return dlsym(handle, funcName);

#elif defined (__FreeBSD__)

    return dlsym( RTLD_DEFAULT, funcName );

#else // all other unixes

    return dlsym(0, funcName);

#endif
}
