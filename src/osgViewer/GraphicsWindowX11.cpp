/* -*-c++-*- OpenSceneGraph - Copyright (C) 1998-2006 Robert Osfield 
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

/* Note, elements of GraphicsWindowX11 have used Prodcer/RenderSurface_X11.cpp as both
 * a guide to use of X11/GLX and copiying directly in the case of setBorder().
 * These elements are license under OSGPL as above, with Copyright (C) 2001-2004  Don Burns.
 */

#include <osgViewer/GraphicsWindowX11>

using namespace osgViewer;

class GraphicsContextX11 : public osg::GraphicsContext
{
    public:

        GraphicsContextX11(osg::GraphicsContext::Traits* traits):
            _display(0),
            _parent(0),
            _window(0)
        {
            _traits = traits;
        }
    
        /** Realise the GraphicsContext implementation, 
          * Pure virtual - must be implemented by concrate implementations of GraphicsContext. */
        virtual bool realizeImplementation() { osg::notify(osg::NOTICE)<<"GraphicsWindow::realizeImplementation() not implemented."<<std::endl; return false; }

        /** Return true if the graphics context has been realised, and is ready to use, implementation.
          * Pure virtual - must be implemented by concrate implementations of GraphicsContext. */
        virtual bool isRealizedImplementation() const  { osg::notify(osg::NOTICE)<<"GraphicsWindow::isRealizedImplementation() not implemented."<<std::endl; return false; }

        /** Close the graphics context implementation.
          * Pure virtual - must be implemented by concrate implementations of GraphicsContext. */
        virtual void closeImplementation()  { osg::notify(osg::NOTICE)<<"GraphicsWindow::closeImplementation() not implemented."<<std::endl; }

        /** Make this graphics context current implementation.
          * Pure virtual - must be implemented by concrate implementations of GraphicsContext. */
        virtual void makeCurrentImplementation()  { osg::notify(osg::NOTICE)<<"GraphicsWindow::makeCurrentImplementation() not implemented."<<std::endl; }
        
        /** Make this graphics context current with specified read context implementation.
          * Pure virtual - must be implemented by concrate implementations of GraphicsContext. */
        virtual void makeContextCurrentImplementation(GraphicsContext* /*readContext*/)  { osg::notify(osg::NOTICE)<<"GraphicsWindow::makeContextCurrentImplementation(..) not implemented."<<std::endl; }

        /** Pure virtual, Bind the graphics context to associated texture implementation.
          * Pure virtual - must be implemented by concrate implementations of GraphicsContext. */
        virtual void bindPBufferToTextureImplementation(GLenum /*buffer*/)  { osg::notify(osg::NOTICE)<<"GraphicsWindow::void bindPBufferToTextureImplementation(..) not implemented."<<std::endl; }

        /** Swap the front and back buffers implementation.
          * Pure virtual - must be implemented by Concrate implementations of GraphicsContext. */
        virtual void swapBuffersImplementation()  { osg::notify(osg::NOTICE)<<"GraphicsWindow:: swapBuffersImplementation() not implemented."<<std::endl; }
        
    protected:
        
        Display* _display;
        Window _parent;
        Window _window;

};


bool GraphicsWindowX11::createVisualInfo()
{
    typedef std::vector<int> Attributes;
    Attributes attributes;
    
    attributes.push_back(GLX_USE_GL);
    
    attributes.push_back(GLX_RGBA);
    
    if (_traits->_doubleBuffer) attributes.push_back(GLX_DOUBLEBUFFER);
    
    if (_traits->_quadBufferStereo) attributes.push_back(GLX_STEREO);
    
    attributes.push_back(GLX_RED_SIZE); attributes.push_back(_traits->_red);
    attributes.push_back(GLX_GREEN_SIZE); attributes.push_back(_traits->_green);
    attributes.push_back(GLX_BLUE_SIZE); attributes.push_back(_traits->_blue);
    attributes.push_back(GLX_DEPTH_SIZE); attributes.push_back(_traits->_depth);
    
    if (_traits->_alpha) { attributes.push_back(GLX_ALPHA_SIZE); attributes.push_back(_traits->_alpha); }
    
    if (_traits->_stencil) { attributes.push_back(GLX_STENCIL_SIZE); attributes.push_back(_traits->_stencil); }

    // TODO
    //  GLX_AUX_BUFFERS
    //  GLX_ACCUM_RED_SIZE
    //  GLX_ACCUM_GREEN_SIZE
    //  GLX_SAMPLE_BUFFERS
    //  GLX_SAMPLES
    
    attributes.push_back(None);
    
    _visualInfo = glXChooseVisual( _display, _traits->_screenNum, &(attributes.front()) );

    return _visualInfo != 0;
}

void GraphicsWindowX11::setWindowDecoration(bool flag)
{
    Atom atom;
    if( (atom = XInternAtom( _display, "_MOTIF_WM_HINTS", 0 )) != None )
    {
// Hack for sending 64 bit atom to Xserver
#if defined( _MIPS_SIM) && (_MIPS_SIM == _MIPS_SIM_ABI64) || defined ( __ia64 ) || defined (__amd64 ) || defined(__x86_64__)
        struct {
            CARD32 flags0;
            CARD32 flags1;
            CARD32 functions0;
            CARD32 functions1;
            CARD32 decorations0;
            CARD32 decorations1;
            INT32 input_mode0;
            INT32 input_mode1;
            CARD32 status0;
            CARD32 status1;
        } wmHints;

#if defined( __ia64 ) || defined (__amd64)  || defined (__x86_64__)
        wmHints.flags0       = (1L << 1);
        wmHints.functions0   = 0;
        wmHints.decorations0 = flag;
        wmHints.input_mode0  = 0;

#else
        wmHints.flags1       = (1L << 1);
        wmHints.functions1   = 0;
        wmHints.decorations1 = flag;
        wmHints.input_mode1  = 0;
#endif

#else

        struct {
            CARD32 flags;
            CARD32 functions;
            CARD32 decorations;
            INT32 input_mode;
            CARD32 status;
        } wmHints;

        wmHints.flags       = (1L << 1);
        wmHints.functions   = 0;
        wmHints.decorations = flag;
        wmHints.input_mode  = 0;
#endif

        XUnmapWindow(_display, _window );
        XChangeProperty( _display, _window, atom, atom, 32, PropModeReplace, (unsigned char *)&wmHints,  5 );
        XMapWindow(_display, _window );

        XFlush(_display);
        XSync(_display,0);

#if 0
        // now update the window dimensions to account for any size changes made by the window manager,
        XGetWindowAttributes( _display, _window, &watt );
        _traits->_width = watt.width;
        _traits->_height = watt.height;
#endif

    }
    else
        osg::notify(osg::NOTICE)<<"Error: GraphicsWindowX11::setBorder(" << flag << ") - couldn't change decorations." << std::endl;
}

void GraphicsWindowX11::init()
{
    if (!_traits || _initialized) return;

    const char* displayString = _traits->_hostName.c_str();
    _display = XOpenDisplay(displayString);
    
    unsigned int screen = _traits->_screenNum;

    if (!_display)
    {
        osg::notify(osg::NOTICE)<<"Error: Unable to open display \"" << XDisplayName(displayString) << "\".  Is the DISPLAY environmental variable set?"<<std::endl;
        return;
    }

     // Query for GLX extension
    int errorBase, eventBase;
    if( glXQueryExtension( _display, &errorBase, &eventBase)  == False )
    {
        osg::notify(osg::NOTICE)<<"Error: " << XDisplayName(displayString) <<" has no GLX extension." << std::endl;

        XCloseDisplay( _display );
        _display = 0;
        return;
    }
    
    // osg::notify(osg::NOTICE)<<"GLX extension, errorBase="<<errorBase<<" eventBase="<<eventBase<<std::endl;

    if (!createVisualInfo())
    {
        osg::notify(osg::NOTICE)<<"Error: Not able to create requested visual." << std::endl;
        return;
    }
    
    // need to pick up from traits
    GLXContext sharedGLContext = 0;
    
    _glxContext = glXCreateContext( _display, _visualInfo, sharedGLContext, GL_TRUE );
    
    if (!_glxContext)
    {
        osg::notify(osg::NOTICE)<<"Error: Unable to create OpenGL graphics context."<<std::endl;
        return;
    }
    
    _parent = RootWindow( _display, screen );

    XWindowAttributes watt;
    XGetWindowAttributes( _display, _parent, &watt );
    // unsigned int parentWindowHeight = watt.height;
    osg::notify(osg::NOTICE)<<"First watt.x = "<<watt.x<<" watt.y="<<watt.y<<std::endl;

    XSetWindowAttributes swatt;
    swatt.colormap = XCreateColormap( _display, _parent, _visualInfo->visual, AllocNone);
    //swatt.colormap = DefaultColormap( _dpy, 10 );
    swatt.background_pixel = 0;
    swatt.border_pixel = 0;
    swatt.event_mask =  0;
    unsigned long mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;

    bool overrideRedirect = false;
    if (overrideRedirect)
    {
        swatt.override_redirect = true;
        mask |= CWOverrideRedirect;
    }

    _window = XCreateWindow( _display, _parent,
                             _traits->_x,
                             _traits->_y,
                             _traits->_width, _traits->_height, 0,
                             _visualInfo->depth, InputOutput,
                             _visualInfo->visual, mask, &swatt );
                             
    if (!_window)
    {
        osg::notify(osg::NOTICE)<<"Error: Unable to create Window."<<std::endl;
        return;
    }

    // This positions the window at _windowX, _windowY
    XSizeHints sh;
    sh.flags = 0;
    sh.flags |= USSize;
    sh.flags &= 0x7;
    sh.flags |= USPosition;
    sh.flags &= 0xB;
    sh.x = _traits->_x;
    sh.y = _traits->_y;
    sh.width  = _traits->_width;
    sh.height = _traits->_height;
    XSetStandardProperties( _display, _window, _traits->_windowName.c_str(), _traits->_windowName.c_str(), None, 0, 0, &sh);

#if 1
    setWindowDecoration(_traits->_windowDecoration);
#else
    setWindowDecoration(true);
#endif
    // Create  default Cursor
    _defaultCursor = XCreateFontCursor( _display, XC_left_ptr );


    // Create Null Cursor
    {
        Pixmap pixmap;
        static char buff[2] = {0,0};
        static XColor ncol = {0,0,0,0,DoRed|DoGreen|DoBlue,0};

        pixmap = XCreateBitmapFromData( _display, _parent, buff, 1, 1);
        _nullCursor = XCreatePixmapCursor( _display, pixmap, pixmap, &ncol, &ncol, 0, 0 );
    }

#if 1
    _currentCursor = _defaultCursor;
#else
    _currentCursor = _nullCursor;
#endif

    {
        XDefineCursor( _display, _window, _currentCursor );
        XFlush(_display);
        XSync(_display,0);
    }

    XSelectInput( _display, _window, ExposureMask | StructureNotifyMask | 
                                     KeyPressMask | KeyReleaseMask |
                                     PointerMotionMask  | ButtonPressMask | ButtonReleaseMask);

    XFlush( _display );
    XSync( _display, 0 );

    // now update the window dimensions to account for any size changes made by the window manager,
    XGetWindowAttributes( _display, _window, &watt );
    _traits->_width = watt.width;
    _traits->_height = watt.height;
    
    //osg::notify(osg::NOTICE)<<"After sync apply.x = "<<watt.x<<" watt.y="<<watt.y<<" width="<<watt.width<<" height="<<watt.height<<std::endl;


    _initialized = true;
}

bool GraphicsWindowX11::realizeImplementation()
{
    if (_realized) return true;

    if (!_initialized) init();
    
    if (!_initialized) return false;
    
    XMapWindow( _display, _window );
    Window temp = _window;
    XSetWMColormapWindows( _display, _window, &temp, 1);
    
    makeCurrent();
    
    _realized = true;

    return true;
}

void GraphicsWindowX11::makeCurrentImplementation()
{
    glXMakeCurrent( _display, _window, _glxContext );
}


void GraphicsWindowX11::closeImplementation()
{
    //glXDestroyContext(_display, _glxContext );
    XDestroyWindow(_display, _window);

    XFlush( _display );
    XSync( _display,0 );
    
    _window = 0;
    _parent = 0;
    
    if(_visualInfo)
    {
        XFree(_visualInfo);
        _visualInfo = 0;
    }

    _initialized = false;
    _realized = false;
}

void GraphicsWindowX11::swapBuffersImplementation()
{
    glXSwapBuffers(_display, _window); 
}

void GraphicsWindowX11::checkEvents()
{
    // osg::notify(osg::NOTICE)<<"Check events"<<std::endl;    
    while( XPending(_display) )
    {
        XEvent ev;
        XNextEvent( _display, &ev );

        switch( ev.type )
        {
            case Expose :
                osg::notify(osg::NOTICE)<<"Expose x="<<ev.xexpose.x<<" y="<<ev.xexpose.y<<" width="<<ev.xexpose.width<<", height="<<ev.xexpose.height<<std::endl;
                break;

            case GravityNotify :
                osg::notify(osg::NOTICE)<<"GravityNotify"<<std::endl;
                break;

            case UnmapNotify :
                osg::notify(osg::NOTICE)<<"UnmapNotify"<<std::endl;
                break;

            case ReparentNotify:
                osg::notify(osg::NOTICE)<<"ReparentNotify"<<std::endl;
                break;

            case DestroyNotify :
                osg::notify(osg::NOTICE)<<"DestroyNotify"<<std::endl;
                _realized =  false;
                break;
                
            case ConfigureNotify :
            {
                osg::notify(osg::NOTICE)<<"ConfigureNotify x="<<ev.xconfigure.x<<" y="<<ev.xconfigure.y<<" width="<<ev.xconfigure.width<<", height="<<ev.xconfigure.height<<std::endl;
                XWindowAttributes watt;
                XGetWindowAttributes( _display, _window, &watt );
                osg::notify(osg::NOTICE)<<"After sync apply.x = "<<watt.x<<" watt.y="<<watt.y<<" width="<<watt.width<<" height="<<watt.height<<std::endl;

                _traits->_x = ev.xconfigure.x;
                _traits->_y = ev.xconfigure.y;
                _traits->_width = ev.xconfigure.width;
                _traits->_height = ev.xconfigure.height;
                // need to dispatch resize event.
                break;
            }
            
            case MapNotify :
            {
                osg::notify(osg::NOTICE)<<"MapNotify"<<std::endl;
                XWindowAttributes watt;
                do
                XGetWindowAttributes(_display, _window, &watt );
                while( watt.map_state != IsViewable );
                
                osg::notify(osg::NOTICE)<<"MapNotify x="<<watt.x<<" y="<<watt.y<<" width="<<watt.width<<", height="<<watt.height<<std::endl;
                _traits->_width = watt.width;
                _traits->_height = watt.height;

                break;
            }

           case MotionNotify :
           {
                int  wx, wy;
                Window win = 0L;
                if( ev.xmotion.same_screen )
                {
                    wx = ev.xmotion.x;
                    wy = ev.xmotion.y;
                }
                else
                {
                    // the mouse in on another screen so need to compute the
                    // coordinates of the mouse position relative to an absolute position
                    // then take away the position of the original window/screen to get
                    // the coordinates relative to the original position.
                    Window root;
                    int rx, ry;
                    unsigned int buttons;

                    int screenOrigin_x = 0;
                    int screenOrigin_y = 0;
                    int i;
                    for(i= 0; i < ScreenCount(_display); i++ )
                    {
                        if( XQueryPointer( _display, RootWindow(_display, i),
                              &root, &win, &rx, &ry, &wx, &wy, &buttons) )
                        {
                            break;
                        }

                        screenOrigin_x += DisplayWidth(_display, i);
                    }

                    for(i= 0; i < static_cast<int>(_traits->_screenNum); i++ )
                    {
                        screenOrigin_x -= DisplayWidth(_display, i);
                    }

                    int dest_x_return, dest_y_return;
                    Window child_return;
                    Bool result = XTranslateCoordinates(_display, _window, _parent, 0, 0, &dest_x_return, &dest_y_return, &child_return);

                    wx += (screenOrigin_x - dest_x_return);
                    wy += (screenOrigin_y - dest_y_return);
                }
                

                float mx = wx;
                float my = wy;
                transformMouseXY(mx, my);
                getEventQueue()->mouseMotion(mx, my);

                // osg::notify(osg::NOTICE)<<"MotionNotify wx="<<wx<<" wy="<<wy<<" mx="<<mx<<" my="<<my<<std::endl;

                break;
            }
            
            case ButtonPress :
            {
                if( ev.xbutton.button == Button4 )
                {
                    getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP);
                }
                else if( ev.xbutton.button == Button5)
                {
                    getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN);
                }
                else
                {
                    float mx = ev.xbutton.x;
                    float my = ev.xmotion.y;
                    transformMouseXY(mx, my);
                    getEventQueue()->mouseButtonPress(mx, my, ev.xbutton.button);
                }
                break;
            }
            
            case ButtonRelease :
            {
                if( ev.xbutton.button == Button4 )
                {
                    getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP);
                }
                else if( ev.xbutton.button == Button5)
                {
                    getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN);
                }
                else
                {
                    float mx = ev.xbutton.x;
                    float my = ev.xmotion.y;
                    transformMouseXY(mx, my);
                    getEventQueue()->mouseButtonRelease(mx, my, ev.xbutton.button);
                }
                break;
            }
            
            case KeyPress:
            {
                int keySymbol = 0;
                unsigned int modifierMask = 0;
                adaptKey(ev.xkey, keySymbol, modifierMask);

                getEventQueue()->keyPress(keySymbol);
                getEventQueue()->getCurrentEventState()->setModKeyMask(modifierMask);
                break;
            }
            
            case KeyRelease:
            {
                int keySymbol = 0;
                unsigned int modifierMask = 0;
                adaptKey(ev.xkey, keySymbol, modifierMask);
                
                getEventQueue()->keyRelease(keySymbol);
                getEventQueue()->getCurrentEventState()->setModKeyMask(modifierMask);
                break;
            }
            
            default:
                osg::notify(osg::NOTICE)<<"Other event"<<std::endl;
                break;
                
        }
    }
}

void GraphicsWindowX11::grabFocus()
{
    XSetInputFocus( _display, _window, RevertToNone, CurrentTime );
    XFlush(_display); XSync(_display,0);
}

void GraphicsWindowX11::grabFocusIfPointerInWindow()
{
    Window win, root;
    int wx, wy, rx, ry;
    unsigned int buttons;

    if( XQueryPointer( _display, _window,
          &root, &win, &rx, &ry, &wx, &wy, &buttons))
    {
        grabFocus();
    }
}


void GraphicsWindowX11::transformMouseXY(float& x, float& y)
{
    if (getEventQueue()->getUseFixedMouseInputRange())
    {
        osgGA::GUIEventAdapter* eventState = getEventQueue()->getCurrentEventState();
        x = eventState->getXmin() + (eventState->getXmax()-eventState->getXmin())*x/float(_traits->_width);
        y = eventState->getYmin() + (eventState->getYmax()-eventState->getYmin())*y/float(_traits->_height);
    }
}

void GraphicsWindowX11::adaptKey(XKeyEvent& keyevent, int& keySymbol, unsigned int& modifierMask)
{
    // KeySym ks = XKeycodeToKeysym( _display, keyevent.keycode, 0 );

    static XComposeStatus state;
    unsigned char keybuf[32];
    XLookupString( &keyevent, (char *)keybuf, sizeof(keybuf), NULL, &state );

    modifierMask = 0;
    if( keyevent.state & ShiftMask )
    {
        modifierMask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
    }
    if( keyevent.state & LockMask )
    {
        modifierMask |= osgGA::GUIEventAdapter::MODKEY_CAPS_LOCK;
    }
    if( keyevent.state & ControlMask )
    {
        modifierMask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
    }
    if( keyevent.state & Mod1Mask )
    {
        modifierMask |= osgGA::GUIEventAdapter::MODKEY_ALT;
    }
    if( keyevent.state & Mod2Mask )
    {
        modifierMask |= osgGA::GUIEventAdapter::MODKEY_NUM_LOCK;
    }
    if( keyevent.state & Mod4Mask )
    {
        modifierMask |= osgGA::GUIEventAdapter::MODKEY_META;
    }

    keySymbol = keybuf[0];
}

struct X11WindowingSystemInterface : public osg::GraphicsContext::WindowingSystemInterface
{

    X11WindowingSystemInterface()
    {
    }

    virtual unsigned int getNumScreens(const osg::GraphicsContext::ScreenIdentifier& si) 
    {
        const char* displayString = si._hostName.c_str();
        Display* display = XOpenDisplay(displayString);
        if(display)
        {
            unsigned int numScreens = ScreenCount(display); 
            XCloseDisplay(display);

            return numScreens;
        }
        else
        {
            osg::notify(osg::NOTICE) << "Unable to open display \"" << XDisplayName(displayString) << "\".  Is the DISPLAY environmental variable set?"<<std::endl;
            return 0;
        }
    }

    virtual void getScreenResolution(const osg::GraphicsContext::ScreenIdentifier& si, unsigned int& width, unsigned int& height)
    {
        const char* displayString = si._hostName.c_str();
        Display* display = XOpenDisplay(displayString);
        if(display)
        {
            width = DisplayWidth(display, si._screenNum);
            height = DisplayHeight(display, si._screenNum);
            XCloseDisplay(display);
        }
        else
        {
            osg::notify(osg::NOTICE) << "Unable to open display \"" << XDisplayName(displayString) << "\".  Is the DISPLAY environmental variable set?"<<std::endl;
            width = 0;
            height = 0;
        }
    }

    virtual osg::GraphicsContext* createGraphicsContext(osg::GraphicsContext::Traits* traits)
    {
        if (traits->_pbuffer)
        {
            return new GraphicsContextX11(traits);
        }
        else
        {
            return new GraphicsWindowX11(traits);
        }
    }

};

struct RegisterWindowingSystemInterfaceProxy
{
    RegisterWindowingSystemInterfaceProxy()
    {
        osg::GraphicsContext::setWindowingSystemInterface(new X11WindowingSystemInterface);
    }

    ~RegisterWindowingSystemInterfaceProxy()
    {
        osg::GraphicsContext::setWindowingSystemInterface(0);
    }
};

RegisterWindowingSystemInterfaceProxy createWindowingSystemInterfaceProxy;
