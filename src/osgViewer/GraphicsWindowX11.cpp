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
 * a guide to use of X11/GLX and copying directly in the case of setBorder().
 * These elements are licensed under OSGPL as above, with Copyright (C) 2001-2004  Don Burns.
 */

#include <osgViewer/api/X11/GraphicsWindowX11>
#include <osgViewer/api/X11/PixelBufferX11>

#include <osg/DeleteHandler>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <X11/Xmd.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>

#include <X11/Xmd.h>        /* For CARD16 */

#ifdef OSGVIEWER_USE_XRANDR
#include <X11/extensions/Xrandr.h>
#endif

#include <unistd.h>

using namespace osgViewer;

class X11KeyboardMap
{
    public:

        X11KeyboardMap()
        {
            _keymap[XK_Escape       ] = osgGA::GUIEventAdapter::KEY_Escape;
            _keymap[XK_F1           ] = osgGA::GUIEventAdapter::KEY_F1;
            _keymap[XK_F2           ] = osgGA::GUIEventAdapter::KEY_F2;
            _keymap[XK_F3           ] = osgGA::GUIEventAdapter::KEY_F3;
            _keymap[XK_F4           ] = osgGA::GUIEventAdapter::KEY_F4;
            _keymap[XK_F5           ] = osgGA::GUIEventAdapter::KEY_F5;
            _keymap[XK_F6           ] = osgGA::GUIEventAdapter::KEY_F6;
            _keymap[XK_F7           ] = osgGA::GUIEventAdapter::KEY_F7;
            _keymap[XK_F8           ] = osgGA::GUIEventAdapter::KEY_F8;
            _keymap[XK_F9           ] = osgGA::GUIEventAdapter::KEY_F9;
            _keymap[XK_F10          ] = osgGA::GUIEventAdapter::KEY_F10;
            _keymap[XK_F11          ] = osgGA::GUIEventAdapter::KEY_F11;
            _keymap[XK_F12          ] = osgGA::GUIEventAdapter::KEY_F12;
            _keymap[XK_quoteleft    ] = '"';
            _keymap[XK_1            ] = '1';
            _keymap[XK_2            ] = '2';
            _keymap[XK_3            ] = '3';
            _keymap[XK_4            ] = '4';
            _keymap[XK_5            ] = '5';
            _keymap[XK_6            ] = '6';
            _keymap[XK_7            ] = '7';
            _keymap[XK_8            ] = '8';
            _keymap[XK_9            ] = '9';
            _keymap[XK_0            ] = '0';
            _keymap[XK_minus        ] = '-';
            _keymap[XK_equal        ] = '=';
            _keymap[XK_BackSpace    ] = osgGA::GUIEventAdapter::KEY_BackSpace;
            _keymap[XK_Tab          ] = osgGA::GUIEventAdapter::KEY_Tab;
            _keymap[XK_a            ] = 'A';
            _keymap[XK_b            ] = 'B';
            _keymap[XK_c            ] = 'C';
            _keymap[XK_d            ] = 'D';
            _keymap[XK_e            ] = 'E';
            _keymap[XK_f            ] = 'F';
            _keymap[XK_g            ] = 'G';
            _keymap[XK_h            ] = 'H';
            _keymap[XK_i            ] = 'I';
            _keymap[XK_j            ] = 'J';
            _keymap[XK_k            ] = 'K';
            _keymap[XK_l            ] = 'L';
            _keymap[XK_m            ] = 'M';
            _keymap[XK_n            ] = 'N';
            _keymap[XK_o            ] = 'O';
            _keymap[XK_p            ] = 'P';
            _keymap[XK_q            ] = 'Q';
            _keymap[XK_r            ] = 'R';
            _keymap[XK_s            ] = 'S';
            _keymap[XK_t            ] = 'T';
            _keymap[XK_u            ] = 'U';
            _keymap[XK_v            ] = 'V';
            _keymap[XK_w            ] = 'W';
            _keymap[XK_x            ] = 'X';
            _keymap[XK_y            ] = 'Y';
            _keymap[XK_z            ] = 'Z';
            _keymap[XK_bracketleft  ] = '(';
            _keymap[XK_bracketright ] = ')';
            _keymap[XK_backslash    ] = '\\';
            _keymap[XK_Caps_Lock    ] = osgGA::GUIEventAdapter::KEY_Caps_Lock;
            _keymap[XK_semicolon    ] = ';';
            _keymap[XK_apostrophe   ] = '\'';
            _keymap[XK_Return       ] = osgGA::GUIEventAdapter::KEY_Return;
            _keymap[XK_comma        ] = ',';
            _keymap[XK_period       ] = '.';
            _keymap[XK_slash        ] = '/';
            _keymap[XK_space        ] = ' ';
            _keymap[XK_Shift_L      ] = osgGA::GUIEventAdapter::KEY_Shift_L;
            _keymap[XK_Shift_R      ] = osgGA::GUIEventAdapter::KEY_Shift_R;
            _keymap[XK_Control_L    ] = osgGA::GUIEventAdapter::KEY_Control_L;
            _keymap[XK_Control_R    ] = osgGA::GUIEventAdapter::KEY_Control_R;
            _keymap[XK_Meta_L       ] = osgGA::GUIEventAdapter::KEY_Meta_L;
            _keymap[XK_Meta_R       ] = osgGA::GUIEventAdapter::KEY_Meta_R;
            _keymap[XK_Alt_L        ] = osgGA::GUIEventAdapter::KEY_Alt_L;
            _keymap[XK_Alt_R        ] = osgGA::GUIEventAdapter::KEY_Alt_R;
            _keymap[XK_Super_L      ] = osgGA::GUIEventAdapter::KEY_Super_L;
            _keymap[XK_Super_R      ] = osgGA::GUIEventAdapter::KEY_Super_R;
            _keymap[XK_Hyper_L      ] = osgGA::GUIEventAdapter::KEY_Hyper_L;
            _keymap[XK_Hyper_R      ] = osgGA::GUIEventAdapter::KEY_Hyper_R;
            _keymap[XK_Menu         ] = osgGA::GUIEventAdapter::KEY_Menu;
            _keymap[XK_Print        ] = osgGA::GUIEventAdapter::KEY_Print;
            _keymap[XK_Scroll_Lock  ] = osgGA::GUIEventAdapter::KEY_Scroll_Lock;
            _keymap[XK_Pause        ] = osgGA::GUIEventAdapter::KEY_Pause;
            _keymap[XK_Home         ] = osgGA::GUIEventAdapter::KEY_Home;
            _keymap[XK_Page_Up      ] = osgGA::GUIEventAdapter::KEY_Page_Up;
            _keymap[XK_End          ] = osgGA::GUIEventAdapter::KEY_End;
            _keymap[XK_Page_Down    ] = osgGA::GUIEventAdapter::KEY_Page_Down;
            _keymap[XK_Delete       ] = osgGA::GUIEventAdapter::KEY_Delete;
            _keymap[XK_Insert       ] = osgGA::GUIEventAdapter::KEY_Insert;
            _keymap[XK_Left         ] = osgGA::GUIEventAdapter::KEY_Left;
            _keymap[XK_Up           ] = osgGA::GUIEventAdapter::KEY_Up;
            _keymap[XK_Right        ] = osgGA::GUIEventAdapter::KEY_Right;
            _keymap[XK_Down         ] = osgGA::GUIEventAdapter::KEY_Down;
            _keymap[XK_Num_Lock     ] = osgGA::GUIEventAdapter::KEY_Num_Lock;
            _keymap[XK_KP_Divide    ] = osgGA::GUIEventAdapter::KEY_KP_Divide;
            _keymap[XK_KP_Multiply  ] = osgGA::GUIEventAdapter::KEY_KP_Multiply;
            _keymap[XK_KP_Subtract  ] = osgGA::GUIEventAdapter::KEY_KP_Subtract;
            _keymap[XK_KP_Add       ] = osgGA::GUIEventAdapter::KEY_KP_Add;
            _keymap[XK_KP_Home      ] = osgGA::GUIEventAdapter::KEY_KP_Home;
            _keymap[XK_KP_Up        ] = osgGA::GUIEventAdapter::KEY_KP_Up;
            _keymap[XK_KP_Page_Up   ] = osgGA::GUIEventAdapter::KEY_KP_Page_Up;
            _keymap[XK_KP_Left      ] = osgGA::GUIEventAdapter::KEY_KP_Left;
            _keymap[XK_KP_Begin     ] = osgGA::GUIEventAdapter::KEY_KP_Begin;
            _keymap[XK_KP_Right     ] = osgGA::GUIEventAdapter::KEY_KP_Right;
            _keymap[XK_KP_End       ] = osgGA::GUIEventAdapter::KEY_KP_End;
            _keymap[XK_KP_Down      ] = osgGA::GUIEventAdapter::KEY_KP_Down;
            _keymap[XK_KP_Page_Down ] = osgGA::GUIEventAdapter::KEY_KP_Page_Down;
            _keymap[XK_KP_Insert    ] = osgGA::GUIEventAdapter::KEY_KP_Insert;
            _keymap[XK_KP_Delete    ] = osgGA::GUIEventAdapter::KEY_KP_Delete;
            _keymap[XK_KP_Enter     ] = osgGA::GUIEventAdapter::KEY_KP_Enter;
     
        }

        ~X11KeyboardMap() {}

        int remapKey(int key)
        {
            KeyMap::iterator itr = _keymap.find(key);
            if (itr == _keymap.end()) return key;
            else return itr->second;
        }
      
    protected:
    

        typedef std::map<int, int> KeyMap;
        KeyMap _keymap;
};

static int remapX11Key(int key)
{
    static X11KeyboardMap s_x11KeyboardMap;
    return s_x11KeyboardMap.remapKey(key);
}

// Functions to handle key maps of type char[32] as contained in
// an XKeymapEvent or returned by XQueryKeymap().
static inline bool keyMapGetKey(const char* map, unsigned int key)
{
    return (map[(key & 0xff) / 8] & (1 << (key & 7))) != 0;
}

static inline void keyMapSetKey(char* map, unsigned int key)
{
    map[(key & 0xff) / 8] |= (1 << (key & 7));
}

static inline void keyMapClearKey(char* map, unsigned int key)
{
    map[(key & 0xff) / 8] &= ~(1 << (key & 7));
}

GraphicsWindowX11::~GraphicsWindowX11()
{
    close(true);
}

Display* GraphicsWindowX11::getDisplayToUse() const
{
    if (_threadOfLastMakeCurrent==0)
    {
        return _display;
    }
    
    if (OpenThreads::Thread::CurrentThread()==_threadOfLastMakeCurrent) 
    {
        return _display;
    }
    else 
    {
        return _eventDisplay;
    }
}

bool GraphicsWindowX11::createVisualInfo()
{
    if( _window != 0 )
    {
        XWindowAttributes watt;
        XGetWindowAttributes( _display, _window, &watt );
        XVisualInfo temp;
        temp.visualid = XVisualIDFromVisual(watt.visual);
        int n;
        _visualInfo = XGetVisualInfo( _display, VisualIDMask, &temp, &n );
    }
    else
    {

        typedef std::vector<int> Attributes;
        Attributes attributes;
        
        attributes.push_back(GLX_USE_GL);
        
        attributes.push_back(GLX_RGBA);
        
        if (_traits->doubleBuffer) attributes.push_back(GLX_DOUBLEBUFFER);
        
        if (_traits->quadBufferStereo) attributes.push_back(GLX_STEREO);
        
        attributes.push_back(GLX_RED_SIZE); attributes.push_back(_traits->red);
        attributes.push_back(GLX_GREEN_SIZE); attributes.push_back(_traits->green);
        attributes.push_back(GLX_BLUE_SIZE); attributes.push_back(_traits->blue);
        attributes.push_back(GLX_DEPTH_SIZE); attributes.push_back(_traits->depth);
        
        if (_traits->alpha) { attributes.push_back(GLX_ALPHA_SIZE); attributes.push_back(_traits->alpha); }
        
        if (_traits->stencil) { attributes.push_back(GLX_STENCIL_SIZE); attributes.push_back(_traits->stencil); }
    
    #if defined(GLX_SAMPLE_BUFFERS) && defined (GLX_SAMPLES)
    
        if (_traits->sampleBuffers) { attributes.push_back(GLX_SAMPLE_BUFFERS); attributes.push_back(_traits->sampleBuffers); }
        if (_traits->sampleBuffers) { attributes.push_back(GLX_SAMPLES); attributes.push_back(_traits->samples); }
    
    #endif
        // TODO
        //  GLX_AUX_BUFFERS
        //  GLX_ACCUM_RED_SIZE
        //  GLX_ACCUM_GREEN_SIZE
        //  GLX_SAMPLE_BUFFERS
        //  GLX_SAMPLES
        
        attributes.push_back(None);
        
        _visualInfo = glXChooseVisual( _display, _traits->screenNum, &(attributes.front()) );
    }

    return _visualInfo != 0;
}
#define MWM_HINTS_FUNCTIONS   (1L << 0)
#define MWM_HINTS_DECORATIONS (1L << 1)
#define MWM_HINTS_INPUT_MODE  (1L << 2)
#define MWM_HINTS_STATUS      (1L << 3)

#define MWM_DECOR_ALL         (1L<<0)
#define MWM_DECOR_BORDER      (1L<<1)
#define MWM_DECOR_RESIZEH     (1L<<2)
#define MWM_DECOR_TITLE       (1L<<3)
#define MWM_DECOR_MENU        (1L<<4)
#define MWM_DECOR_MINIMIZE    (1L<<5)
#define MWM_DECOR_MAXIMIZE    (1L<<6)

#define MWM_FUNC_ALL          (1L<<0)
#define MWM_FUNC_RESIZE       (1L<<1)
#define MWM_FUNC_MOVE         (1L<<2)
#define MWM_FUNC_MINIMIZE     (1L<<3)
#define MWM_FUNC_MAXIMIZE     (1L<<4)
#define MWM_FUNC_CLOSE        (1L<<5)


bool GraphicsWindowX11::checkAndSendEventFullScreenIfNeeded(Display* display, int x, int y, int width, int height, bool windowDecoration)
{
  osg::GraphicsContext::WindowingSystemInterface *wsi = osg::GraphicsContext::getWindowingSystemInterface();
  if (wsi == NULL) {
    osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot toggle window fullscreen." << std::endl;
    return false;
  }

  unsigned int    screenWidth;
  unsigned int    screenHeight;

  wsi->getScreenResolution(*_traits, screenWidth, screenHeight);
  bool isFullScreen = x == 0 && y == 0 && width == (int)screenWidth && height == (int)screenHeight && !windowDecoration;

  Atom netWMStateAtom = XInternAtom(display, "_NET_WM_STATE", True);
  Atom netWMStateFullscreenAtom = XInternAtom(display,
                                              "_NET_WM_STATE_FULLSCREEN", True);

  if (netWMStateAtom != None && netWMStateFullscreenAtom != None) {
    XEvent xev;
    xev.xclient.type = ClientMessage;
    xev.xclient.serial = 0;
    xev.xclient.send_event = True;
    xev.xclient.window = _window;
    xev.xclient.message_type = netWMStateAtom;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = isFullScreen ? 1 : 0;
    xev.xclient.data.l[1] = netWMStateFullscreenAtom;
    xev.xclient.data.l[2] = 0;

    XSendEvent(display, RootWindow(display, DefaultScreen(display)),
               False,  SubstructureRedirectMask | SubstructureNotifyMask, &xev);
    return true;
  }
  return false;
}

bool GraphicsWindowX11::setWindowDecorationImplementation(bool flag)
{
    Display* display = getDisplayToUse();

    XMapWindow(display, _window );

    checkAndSendEventFullScreenIfNeeded(display, _traits->x, _traits->y, _traits->width, _traits->height, flag);
    struct
    {
        unsigned long flags;
        unsigned long functions;
        unsigned long decorations;
        long          inputMode;
        unsigned long status;
    } wmHints;

    Atom atom;
    bool result = false;
    if( (atom = XInternAtom( display, "_MOTIF_WM_HINTS", 0 )) != None )
    {
        
        wmHints.flags = 0;
        wmHints.functions = MWM_FUNC_ALL;
        wmHints.decorations = MWM_DECOR_ALL;
        wmHints.inputMode = 0;
        wmHints.status = 0;        
        
        if (!flag)
        {
            wmHints.flags = MWM_HINTS_DECORATIONS;
            wmHints.decorations = 0;
        }
        else
        {
            wmHints.flags |= MWM_HINTS_FUNCTIONS;
            if (_traits.valid() && !_traits->supportsResize) wmHints.functions |= MWM_FUNC_RESIZE;
        }
        XChangeProperty( display, _window, atom, atom, 32, PropModeReplace, (unsigned char *)&wmHints,  5 );
        result = true;
    }
    else
    {
        osg::notify(osg::NOTICE)<<"Error: GraphicsWindowX11::setBorder(" << flag << ") - couldn't change decorations." << std::endl;
        result = false;
    }

    XFlush(display);
    XSync(display,0);
    // add usleep here to give window manager a chance to handle the request, if
    // we don't add this sleep then any X11 calls right afterwards can produce
    // X11 errors.
    usleep(100000);
    return result;
}

bool GraphicsWindowX11::setWindowRectangleImplementation(int x, int y, int width, int height)
{
    if (!_initialized) return false;
    
    Display* display = getDisplayToUse();
    
    XMoveResizeWindow(display, _window, x, y, width, height);
    
    XFlush(display);
    XSync(display, 0);

    checkAndSendEventFullScreenIfNeeded(display, x, y, width, height, _traits->windowDecoration);

    // add usleep here to give window manager a chance to handle the request, if
    // we don't add this sleep then any X11 calls right afterwards can produce
    // X11 errors.
    usleep(100000);

    
    return true;
}

void GraphicsWindowX11::setWindowName(const std::string& name)
{
    if( _window == 0) return;

    //    char *slist[] = { name.c_str(), 0L };
    //    XTextProperty xtp;

    //    XStringListToTextProperty( slist, 1, &xtp );

    Display* display = getDisplayToUse();
    if( !display ) return;

    //    XSetWMName( display, _window, &xtp );
    XStoreName( display, _window, name.c_str() );
    XSetIconName( display, _window, name.c_str() );

    XFlush(display); 
    XSync(display,0);

    _traits->windowName = name;
}

void GraphicsWindowX11::setCursor(MouseCursor mouseCursor)
{
    Cursor newCursor = getOrCreateCursor(mouseCursor);
    if (newCursor == _currentCursor) return;

    _currentCursor = newCursor;
    if (!_window) return;
    Display* display = getDisplayToUse();
    if (!display) return;
    XDefineCursor( display, _window, _currentCursor );
    XFlush(display);
    XSync(display, 0);

    _traits->useCursor = (_currentCursor != getOrCreateCursor(NoCursor));
}

Cursor GraphicsWindowX11::getOrCreateCursor(MouseCursor mouseCursor)
{
    std::map<MouseCursor,Cursor>::iterator i = _mouseCursorMap.find(mouseCursor);
    if (i != _mouseCursorMap.end()) return i->second;

    Display* display = getDisplayToUse();
    if (!display) return None;

    switch (mouseCursor) {
    case NoCursor:
    {
        // create an empty mouse cursor, note that it is safe to destroy the Pixmap just past cursor creation
        // since the resource in the x server is reference counted.
        char buff[2] = {0,0};
        XColor ncol = {0,0,0,0,DoRed|DoGreen|DoBlue,0};
        Pixmap pixmap = XCreateBitmapFromData( display, _parent, buff, 1, 1);
        _mouseCursorMap[mouseCursor] = XCreatePixmapCursor( display, pixmap, pixmap, &ncol, &ncol, 0, 0 );
        XFreePixmap(display, pixmap);
        // Important to have the pixmap and the buffer still available when the request is sent to the server ...
        XFlush(display);
        XSync(display, 0);
        break;
    }
    case RightArrowCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_left_ptr );
        break;
    case LeftArrowCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_top_left_arrow );
        break;
    case InfoCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_hand1 );
        break;
    case DestroyCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_pirate );
        break;
    case HelpCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_question_arrow );
        break;
    case CycleCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_exchange );
        break;
    case SprayCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_spraycan );
        break;
    case WaitCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_watch );
        break;
    case TextCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_xterm );
        break;
    case CrosshairCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_crosshair );
        break;
    case UpDownCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_sb_v_double_arrow );
        break;
    case LeftRightCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_sb_h_double_arrow );
        break;
    case TopSideCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_top_side );
        break;
    case BottomSideCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_bottom_side );
        break;
    case LeftSideCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_left_side );
        break;
    case RightSideCursor:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_right_side );
        break;
    case TopLeftCorner:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_top_left_corner );
        break;
    case TopRightCorner:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_top_right_corner );
        break;
    case BottomRightCorner:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_bottom_right_corner );
        break;
    case BottomLeftCorner:
        _mouseCursorMap[mouseCursor] = XCreateFontCursor( display, XC_bottom_left_corner );
        break;

    case InheritCursor:
    default:
        _mouseCursorMap[mouseCursor] = None;
        break;
    };
    return _mouseCursorMap[mouseCursor];
}

void GraphicsWindowX11::init()
{
    if (_initialized) return;

    if (!_traits)
    {
        _valid = false;
        return;
    }

    // getEventQueue()->setCurrentEventState(osgGA::GUIEventAdapter::getAccumulatedEventState().get());

    WindowData* inheritedWindowData = dynamic_cast<WindowData*>(_traits->inheritedWindowData.get());
    Window windowHandle = inheritedWindowData ? inheritedWindowData->_window : 0;

    _ownsWindow = windowHandle == 0;



    _display = XOpenDisplay(_traits->displayName().c_str());

    if (!_display)
    {
        osg::notify(osg::NOTICE)<<"Error: Unable to open display \"" << XDisplayName(_traits->displayName().c_str()) << "\"."<<std::endl;
        _valid = false;
        return;
    }

    // Query for GLX extension
    int errorBase, eventBase;
    if( glXQueryExtension( _display, &errorBase, &eventBase)  == False )
    {
        osg::notify(osg::NOTICE)<<"Error: " << XDisplayName(_traits->displayName().c_str()) <<" has no GLX extension." << std::endl;

        XCloseDisplay( _display );
        _display = 0;
        _valid = false;
        return;
    }

    // osg::notify(osg::NOTICE)<<"GLX extension, errorBase="<<errorBase<<" eventBase="<<eventBase<<std::endl;

    if (!createVisualInfo())
    {
        _traits->red /= 2; 
        _traits->green /= 2; 
        _traits->blue /= 2; 
        _traits->alpha /= 2; 
        _traits->depth /= 2; 
        
        osg::notify(osg::INFO)<<"Relaxing traits"<<std::endl;

        if (!createVisualInfo())
        {
            osg::notify(osg::NOTICE)<<"Error: Not able to create requested visual." << std::endl;
            XCloseDisplay( _display );
            _display = 0;
            _valid = false;
            return;
        }    
    }

    GLXContext sharedContextGLX = NULL;

    // get any shared GLX contexts    
    GraphicsWindowX11* graphicsWindowX11 = dynamic_cast<GraphicsWindowX11*>(_traits->sharedContext);
    if (graphicsWindowX11) 
    {
        sharedContextGLX = graphicsWindowX11->getGLXContext();
    }
    else
    {
        PixelBufferX11* pixelBufferX11 = dynamic_cast<PixelBufferX11*>(_traits->sharedContext);
        if (pixelBufferX11 && pixelBufferX11->valid())
        {
            sharedContextGLX = pixelBufferX11->getGLXContext();
        }
    }
    
    _glxContext = glXCreateContext( _display, _visualInfo, sharedContextGLX, True );
    
    if (!_glxContext)
    {
        osg::notify(osg::NOTICE)<<"Error: Unable to create OpenGL graphics context."<<std::endl;
        XCloseDisplay( _display );
        _display = 0;
        _valid = false;
        return;
    }

    _initialized = _ownsWindow ? createWindow() : setWindow(windowHandle);
    _valid = _initialized;

    if (_valid == false)
    {
        XCloseDisplay( _display );
        _display = 0;
    }
}

bool GraphicsWindowX11::createWindow()
{
    unsigned int screen = _traits->screenNum;

    _eventDisplay = XOpenDisplay(_traits->displayName().c_str());

    _parent = RootWindow( _display, screen );

    XWindowAttributes watt;
    XGetWindowAttributes( _display, _parent, &watt );
    // unsigned int parentWindowHeight = watt.height;

    XSetWindowAttributes swatt;
    swatt.colormap = XCreateColormap( _display, _parent, _visualInfo->visual, AllocNone);
    //swatt.colormap = DefaultColormap( _dpy, 10 );
    swatt.background_pixel = 0;
    swatt.border_pixel = 0;
    swatt.event_mask =  0;
    unsigned long mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;

    if (_traits->overrideRedirect)
    {
        swatt.override_redirect = true;
        mask |= CWOverrideRedirect;
        
        osg::notify(osg::INFO)<<"Setting override redirect"<<std::endl;
    }

    _window = XCreateWindow( _display, _parent,
                             _traits->x,
                             _traits->y,
                             _traits->width, _traits->height, 0,
                             _visualInfo->depth, InputOutput,
                             _visualInfo->visual, mask, &swatt );                          

    if (!_window)
    {
        osg::notify(osg::NOTICE)<<"Error: Unable to create Window."<<std::endl;
        _glxContext = 0;
        return false;
    }


    // This positions the window at _windowX, _windowY
    XSizeHints sh;
    sh.flags = 0;
    sh.flags |= USSize;
    sh.flags &= 0x7;
    sh.flags |= USPosition;
    sh.flags &= 0xB;
    sh.x = _traits->x;
    sh.y = _traits->y;
    sh.width  = _traits->width;
    sh.height = _traits->height;
    XSetStandardProperties( _display, _window, _traits->windowName.c_str(), _traits->windowName.c_str(), None, 0, 0, &sh);

    setWindowDecoration(_traits->windowDecoration);

    useCursor(_traits->useCursor);

    _deleteWindow = XInternAtom (_display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(_display, _window, &_deleteWindow, 1); 


    XFlush( _display );
    XSync( _display, 0 );

    // now update the window dimensions to account for any size changes made by the window manager,
    XGetWindowAttributes( _display, _window, &watt );
    
    if (_traits->width != watt.width && _traits->height != watt.height)
    {
        resized( _traits->x, _traits->y, _traits->width, _traits->height );
    }
        
    //osg::notify(osg::NOTICE)<<"After sync apply.x = "<<watt.x<<" watt.y="<<watt.y<<" width="<<watt.width<<" height="<<watt.height<<std::endl;


    XSelectInput( _eventDisplay, _window, ExposureMask | StructureNotifyMask | 
                                     KeyPressMask | KeyReleaseMask |
                                     PointerMotionMask | ButtonPressMask | ButtonReleaseMask |
                                     KeymapStateMask | FocusChangeMask | EnterWindowMask );

    XFlush( _eventDisplay );
    XSync( _eventDisplay, 0 );
    rescanModifierMapping();

    return true;
}

bool GraphicsWindowX11::setWindow(Window window)
{
    if (_initialized)
    {
        osg::notify(osg::NOTICE) << "GraphicsWindowX11::setWindow() - Window already created; it cannot be changed";
        return false;
    }
    
    if (window==0)
    {
        osg::notify(osg::NOTICE) << "GraphicsWindowX11::setWindow() - Invalid window handle passed ";
        return false;
    }
    
    _window = window;
    if (_window==0)
    {
        osg::notify(osg::NOTICE) << "GraphicsWindowX11::setWindow() - Unable to retrieve native window handle";
        return false;
    }

    XWindowAttributes watt;
    XGetWindowAttributes( _display, _window, &watt );
    _traits->x = watt.x;
    _traits->y = watt.y;
    _traits->width = watt.width;
    _traits->height = watt.height;

    _parent = DefaultRootWindow( _display );

    //_traits->supportsResize = false;
    _traits->windowDecoration = false;

    if (_traits->windowName.size()) setWindowName(_traits->windowName);

    _eventDisplay = XOpenDisplay(_traits->displayName().c_str());

    XFlush( _eventDisplay );
    XSync( _eventDisplay, 0 );

    return true;
}

bool GraphicsWindowX11::realizeImplementation()
{
    if (_realized)
    {
        osg::notify(osg::NOTICE)<<"GraphicsWindowX11::realizeImplementation() Already realized"<<std::endl;
        return true;
    }

    if (!_initialized) init();
    
    if (!_initialized) return false;
    
    XMapWindow( _display, _window );
    
//    Window temp = _window;
//    XSetWMColormapWindows( _display, _window, &temp, 1);
    
    _realized = true;

    return true;
}

bool GraphicsWindowX11::makeCurrentImplementation()
{
    if (!_realized)
    {
        osg::notify(osg::NOTICE)<<"Warning: GraphicsWindow not realized, cannot do makeCurrent."<<std::endl;
        return false;
    }

    // osg::notify(osg::NOTICE)<<"GraphicsWindowX11::makeCurrentImplementation "<<this<<" "<<OpenThreads::Thread::CurrentThread()<<std::endl;
    // osg::notify(osg::NOTICE)<<"   glXMakeCurrent ("<<_display<<","<<_window<<","<<_glxContext<<std::endl;

    return glXMakeCurrent( _display, _window, _glxContext )==True;
}

bool GraphicsWindowX11::releaseContextImplementation()
{
    if (!_realized)
    {
        osg::notify(osg::NOTICE)<<"Warning: GraphicsWindow not realized, cannot do release context."<<std::endl;
        return false;
    }

    // osg::notify(osg::NOTICE)<<"GraphicsWindowX11::releaseContextImplementation() "<<this<<" "<<OpenThreads::Thread::CurrentThread()<<std::endl;
    // osg::notify(osg::NOTICE)<<"   glXMakeCurrent ("<<_display<<std::endl;

    return glXMakeCurrent( _display, None, NULL )==True;
}


void GraphicsWindowX11::closeImplementation()
{
    // osg::notify(osg::NOTICE)<<"Closing GraphicsWindowX11"<<std::endl;

    if (_eventDisplay)
    {
        XCloseDisplay( _eventDisplay );
        _eventDisplay = 0;
    }

    if (_display)
    {
        if (_glxContext)
        {
            glXDestroyContext(_display, _glxContext );
        }
    
        if (_window && _ownsWindow)
        {
            XDestroyWindow(_display, _window);
        }

        XFlush( _display );
        XSync( _display,0 );
    }
    
    _window = 0;
    _parent = 0;
    _glxContext = 0;

    if (_visualInfo)
    {
        XFree(_visualInfo);
        _visualInfo = 0;
    }


    if (_display)
    {
        XCloseDisplay( _display );
        _display = 0;
    }

    _initialized = false;
    _realized = false;
    _valid = false;
}

void GraphicsWindowX11::swapBuffersImplementation()
{
    if (!_realized) return;

    // osg::notify(osg::NOTICE)<<"swapBuffersImplementation "<<this<<" "<<OpenThreads::Thread::CurrentThread()<<std::endl;

    glXSwapBuffers(_display, _window);

    while( XPending(_display) )
    {
        XEvent ev;
        XNextEvent( _display, &ev );

        switch( ev.type )
        {
            case ClientMessage:
            {
                if (static_cast<Atom>(ev.xclient.data.l[0]) == _deleteWindow)
                {
                    osg::notify(osg::INFO)<<"DeleteWindow event received"<<std::endl;
                    getEventQueue()->closeWindow();
                }
            }
        }
    }
}

void GraphicsWindowX11::checkEvents()
{
    if (!_realized) return;

    Display* display = _eventDisplay;

    double baseTime = _timeOfLastCheckEvents;
    double eventTime = baseTime;
    double resizeTime = eventTime;
    _timeOfLastCheckEvents = getEventQueue()->getTime();
    
    // osg::notify(osg::NOTICE)<<"GraphicsWindowX11::checkEvents() : getEventQueue()->getCurrentEventState()->getGraphicsContext()="<<getEventQueue()->getCurrentEventState()->getGraphicsContext()<<std::endl;

    int windowX = _traits->x;
    int windowY = _traits->y;
    int windowWidth = _traits->width;
    int windowHeight = _traits->height;

    bool destroyWindowRequested = false;
    
    Time firstEventTime = 0;
     
         // osg::notify(osg::NOTICE)<<"Check events"<<std::endl;    
    while( XPending(display) )
    {
        XEvent ev;
        XNextEvent( display, &ev );

        switch( ev.type )
        {
            case ClientMessage:
            {
                osg::notify(osg::NOTICE)<<"ClientMessage event received"<<std::endl;
                if (static_cast<Atom>(ev.xclient.data.l[0]) == _deleteWindow)
                {
                    osg::notify(osg::NOTICE)<<"DeleteWindow event received"<<std::endl;
                    // FIXME only do if _ownsWindow ?
                    destroyWindowRequested = true;
                    getEventQueue()->closeWindow(eventTime);
                }
            }
            case Expose :
                osg::notify(osg::INFO)<<"Expose x="<<ev.xexpose.x<<" y="<<ev.xexpose.y<<" width="<<ev.xexpose.width<<", height="<<ev.xexpose.height<<std::endl;
                break;

            case GravityNotify :
                osg::notify(osg::INFO)<<"GravityNotify event received"<<std::endl;
                break;

            case ReparentNotify:
                osg::notify(osg::INFO)<<"ReparentNotify event received"<<std::endl;
                break;

            case DestroyNotify :
                osg::notify(osg::NOTICE)<<"DestroyNotify event received"<<std::endl;
                _realized =  false;
                _valid = false;
                break;
                
            case ConfigureNotify :
            {
                osg::notify(osg::INFO)<<"ConfigureNotify x="<<ev.xconfigure.x<<" y="<<ev.xconfigure.y<<" width="<<ev.xconfigure.width<<", height="<<ev.xconfigure.height<<std::endl;

                if (windowX != ev.xconfigure.x || 
                    windowX != ev.xconfigure.y ||
                    windowWidth != ev.xconfigure.width ||
                    windowHeight != ev.xconfigure.height)
                {
                    resizeTime = eventTime;

                    windowX = ev.xconfigure.x;
                    windowY = ev.xconfigure.y;
                    windowWidth = ev.xconfigure.width;
                    windowHeight = ev.xconfigure.height;
                }

                break;
            }
            
            case MapNotify :
            {
                osg::notify(osg::INFO)<<"MapNotify"<<std::endl;
                XWindowAttributes watt;
                do
                    XGetWindowAttributes(display, _window, &watt );
                while( watt.map_state != IsViewable );
                
                osg::notify(osg::INFO)<<"MapNotify x="<<watt.x<<" y="<<watt.y<<" width="<<watt.width<<", height="<<watt.height<<std::endl;

                if (windowWidth != watt.width || windowHeight != watt.height)
                {
                    resizeTime = eventTime;

                    windowWidth = watt.width;
                    windowHeight = watt.height;
                }

                break;
            }

            case FocusIn :
                osg::notify(osg::INFO)<<"FocusIn event received"<<std::endl;
                flushKeyEvents();
                break;

            case UnmapNotify :
            case FocusOut :
            {
                osg::notify(osg::INFO)<<"FocusOut/UnmapNotify event received"<<std::endl;
                if (ev.type == FocusOut && ev.xfocus.mode != NotifyNormal) break;

                char modMap[32];
                getModifierMap(modMap);

                // release normal (non-modifier) keys
                for (unsigned int key = 8; key < 256; key++)
                {
                    bool isModifier = keyMapGetKey(modMap, key);
                    if (!isModifier) forceKey(key, eventTime, false);
                }

                // release modifier keys
                for (unsigned int key = 8; key < 256; key++)
                {
                    bool isModifier = keyMapGetKey(modMap, key);
                    if (isModifier) forceKey(key, eventTime, false);
                }
                break;
            }

            case EnterNotify :
                osg::notify(osg::INFO)<<"EnterNotify event received"<<std::endl;
                _modifierState = ev.xcrossing.state;
                syncLocks();
                break;

            case KeymapNotify :
            {
                osg::notify(osg::INFO)<<"KeymapNotify event received"<<std::endl;

                // KeymapNotify is guaranteed to directly follow either a FocusIn or
                // an EnterNotify event. We are only interested in the FocusIn case.
                if (_lastEventType != FocusIn) break;

                char modMap[32];
                getModifierMap(modMap);
                syncLocks();

                char keyMap[32];
                XQueryKeymap(_eventDisplay, keyMap);

                // release normal (non-modifier) keys
                for (unsigned int key = 8; key < 256; key++)
                {
                    bool isModifier = keyMapGetKey(modMap, key);
                    if (isModifier) continue;
                    bool isPressed = keyMapGetKey(keyMap, key);
                    if (!isPressed) forceKey(key, eventTime, false);
                }

                // press/release modifier keys
                for (unsigned int key = 8; key < 256; key++)
                {
                    bool isModifier = keyMapGetKey(modMap, key);
                    if (!isModifier) continue;
                    bool isPressed = keyMapGetKey(keyMap, key);
                    forceKey(key, eventTime, isPressed);
                }

                // press normal keys
                for (unsigned int key = 8; key < 256; key++)
                {
                    bool isModifier = keyMapGetKey(modMap, key);
                    if (isModifier) continue;
                    bool isPressed = keyMapGetKey(keyMap, key);
                    if (isPressed) forceKey(key, eventTime, true);
                }
                break;
            }

            case MappingNotify :
                osg::notify(osg::INFO)<<"MappingNotify event received"<<std::endl;
                if (ev.xmapping.request == MappingModifier) rescanModifierMapping();
                break;

            case MotionNotify :
            {
                if (firstEventTime==0) firstEventTime = ev.xmotion.time;
                Time relativeTime = ev.xmotion.time - firstEventTime;
                eventTime = baseTime + static_cast<double>(relativeTime)*0.001;
           
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
                    for(i= 0; i < ScreenCount(display); i++ )
                    {
                        if( XQueryPointer( display, RootWindow(display, i),
                              &root, &win, &rx, &ry, &wx, &wy, &buttons) )
                        {
                            break;
                        }

                        screenOrigin_x += DisplayWidth(display, i);
                    }

                    for(i= 0; i < static_cast<int>(_traits->screenNum); i++ )
                    {
                        screenOrigin_x -= DisplayWidth(display, i);
                    }

                    int dest_x_return, dest_y_return;
                    Window child_return;
                    XTranslateCoordinates(display, _window, _parent, 0, 0, &dest_x_return, &dest_y_return, &child_return);

                    wx += (screenOrigin_x - dest_x_return);
                    wy += (screenOrigin_y - dest_y_return);
                }
                

                float mx = wx;
                float my = wy;
                transformMouseXY(mx, my);
                getEventQueue()->mouseMotion(mx, my, eventTime);

                // osg::notify(osg::NOTICE)<<"MotionNotify wx="<<wx<<" wy="<<wy<<" mx="<<mx<<" my="<<my<<std::endl;

                break;
            }
            
            case ButtonPress :
            {
                if (firstEventTime==0) firstEventTime = ev.xmotion.time;
                Time relativeTime = ev.xmotion.time - firstEventTime;
                eventTime = baseTime + static_cast<double>(relativeTime)*0.001;

                if( ev.xbutton.button == Button4 )
                {
                    getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP, eventTime);
                }
                else if( ev.xbutton.button == Button5)
                {
                    getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN, eventTime);
                }
                else
                {
                    float mx = ev.xbutton.x;
                    float my = ev.xmotion.y;
                    transformMouseXY(mx, my);
                    getEventQueue()->mouseButtonPress(mx, my, ev.xbutton.button, eventTime);
                }
                break;
            }
            
            case ButtonRelease :
            {
                if (firstEventTime==0) firstEventTime = ev.xmotion.time;
                Time relativeTime = ev.xmotion.time - firstEventTime;
                eventTime = baseTime + static_cast<double>(relativeTime)*0.001;

                if( ev.xbutton.button == Button4 )
                {
                    getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_UP, eventTime);
                }
                else if( ev.xbutton.button == Button5)
                {
                    getEventQueue()->mouseScroll(osgGA::GUIEventAdapter::SCROLL_DOWN, eventTime);
                }
                else
                {
                    float mx = ev.xbutton.x;
                    float my = ev.xmotion.y;
                    transformMouseXY(mx, my);
                    getEventQueue()->mouseButtonRelease(mx, my, ev.xbutton.button, eventTime);
                }
                break;
            }
            
            case KeyPress:
            {
                if (firstEventTime==0) firstEventTime = ev.xmotion.time;
                Time relativeTime = ev.xmotion.time - firstEventTime;
                eventTime = baseTime + static_cast<double>(relativeTime)*0.001;

                _modifierState = ev.xkey.state;
                keyMapSetKey(_keyMap, ev.xkey.keycode);
                int keySymbol = 0;
                adaptKey(ev.xkey, keySymbol);

                getEventQueue()->keyPress(keySymbol, eventTime);
                break;
            }
            
            case KeyRelease:
            {
                if (firstEventTime==0) firstEventTime = ev.xmotion.time;
                Time relativeTime = ev.xmotion.time - firstEventTime;
                eventTime = baseTime + static_cast<double>(relativeTime)*0.001;
#if 1                
                // Check for following KeyPress events and see if
                // the pair are the result of auto-repeat. If so, drop
                // this one on the floor, to be consistent with
                // Windows and Mac ports. The idea comes from libSDL sources. 
                XEvent nextev;
                if (XPending(display))
                {
                    XPeekEvent(display, &nextev);
                    if ((nextev.type == KeyPress)
                        && (nextev.xkey.keycode == ev.xkey.keycode)
                        && (nextev.xmotion.time - ev.xmotion.time < 2))
                    {
                        break;
                    }
                }
#endif
                _modifierState = ev.xkey.state;
                keyMapClearKey(_keyMap, ev.xkey.keycode);
                int keySymbol = 0;
                adaptKey(ev.xkey, keySymbol);
                
                getEventQueue()->keyRelease(keySymbol, eventTime);
                break;
            }
            
            default:
                osg::notify(osg::NOTICE)<<"Other event "<<ev.type<<std::endl;
                break;
                
        }
        _lastEventType = ev.type;
    }

    if (windowX != _traits->x || 
        windowY != _traits->y ||
        windowWidth != _traits->width ||
        windowHeight != _traits->height)
    {
        resized(windowX, windowY, windowWidth, windowHeight); 
        getEventQueue()->windowResize(windowX, windowY, windowWidth, windowHeight, resizeTime);
    }
    
#if 0
    if (destroyWindowRequested)
    {
        close();
    }
#endif
}

void GraphicsWindowX11::grabFocus()
{
    Display* display = getDisplayToUse();

    XSetInputFocus( display, _window, RevertToNone, CurrentTime );
    XFlush(display); 
    XSync(display,0);
}

void GraphicsWindowX11::grabFocusIfPointerInWindow()
{
    Window win, root;
    int wx, wy, rx, ry;
    unsigned int buttons;

    Display* display = getDisplayToUse();

    if( XQueryPointer( display, _window,
          &root, &win, &rx, &ry, &wx, &wy, &buttons))
    {
#if 0
        if (wx>=0 && wx<_traits->width &&
            wy>=0 && wy<_traits->height)
        {
            grabFocus();
        }
#else        
        grabFocus();
#endif
    }
}

void GraphicsWindowX11::transformMouseXY(float& x, float& y)
{
    if (getEventQueue()->getUseFixedMouseInputRange())
    {
        osgGA::GUIEventAdapter* eventState = getEventQueue()->getCurrentEventState();
        x = eventState->getXmin() + (eventState->getXmax()-eventState->getXmin())*x/float(_traits->width);
        y = eventState->getYmin() + (eventState->getYmax()-eventState->getYmin())*y/float(_traits->height);
    }
}

void GraphicsWindowX11::adaptKey(XKeyEvent& keyevent, int& keySymbol)
{
    Display* display = _eventDisplay;
 
    unsigned char keybuf[32];
    XLookupString( &keyevent, (char *)keybuf, sizeof(keybuf), NULL, NULL );

    KeySym ks = XKeycodeToKeysym( display, keyevent.keycode, 0 );
    int remappedKey = remapX11Key(ks);
    if (remappedKey & 0xff00) 
    {
        // special keyboard character
        keySymbol = remappedKey;
    }
    else
    {
        // normal ascii key
        keySymbol = keybuf[0];
    }
}

// Function to inject artificial key presses/releases.
void GraphicsWindowX11::forceKey(int key, double time, bool state)
{
    if (!(state ^ keyMapGetKey(_keyMap, key))) return; // already pressed/released

    XKeyEvent event;
    event.serial = 0;
    event.send_event = True;
    event.display = _eventDisplay;
    event.window = _window;
    event.subwindow = 0;
    event.time = 0;
    event.x = 0;
    event.y = 0;
    event.x_root = 0;
    event.y_root = 0;
    event.state = getModifierMask() | (_modifierState & (LockMask | _numLockMask));
    event.keycode = key;
    event.same_screen = True;

    int keySymbol = 0;
    if (state)
    {
        event.type = KeyPress;
        adaptKey(event, keySymbol);
        getEventQueue()->keyPress(keySymbol, time);
        keyMapSetKey(_keyMap, key);
    }
    else
    {
        event.type = KeyRelease;
        adaptKey(event, keySymbol);
        getEventQueue()->keyRelease(keySymbol, time);
        keyMapClearKey(_keyMap, key);
    }
}

void GraphicsWindowX11::syncLocks()
{
    unsigned int mask = getEventQueue()->getCurrentEventState()->getModKeyMask();

    if (_modifierState & LockMask)
        mask |= osgGA::GUIEventAdapter::MODKEY_CAPS_LOCK;
    else
        mask &= ~osgGA::GUIEventAdapter::MODKEY_CAPS_LOCK;

    if (_modifierState & _numLockMask)
        mask |= osgGA::GUIEventAdapter::MODKEY_NUM_LOCK;
    else
        mask &= ~osgGA::GUIEventAdapter::MODKEY_NUM_LOCK;

    getEventQueue()->getCurrentEventState()->setModKeyMask(mask);
}

void GraphicsWindowX11::rescanModifierMapping()
{
    XModifierKeymap *mkm = XGetModifierMapping(_eventDisplay);
    KeyCode *m = mkm->modifiermap;
    KeyCode numlock = XKeysymToKeycode(_eventDisplay, XK_Num_Lock);
    _numLockMask = 0;
    for (int i = 0; i < mkm->max_keypermod * 8; i++, m++)
    {
        if (*m == numlock)
        {
            _numLockMask = 1 << (i / mkm->max_keypermod);
            break;
        }
    }
}

void GraphicsWindowX11::flushKeyEvents()
{
    XEvent e;
    while (XCheckMaskEvent(_eventDisplay, KeyPressMask|KeyReleaseMask, &e))
        continue;
}

// Returns char[32] keymap with bits for every modifier key set.
void GraphicsWindowX11::getModifierMap(char* keymap) const
{
    memset(keymap, 0, 32);
    XModifierKeymap *mkm = XGetModifierMapping(_eventDisplay);
    KeyCode *m = mkm->modifiermap;
    for (int i = 0; i < mkm->max_keypermod * 8; i++, m++)
    {
        if (*m) keyMapSetKey(keymap, *m);
    }
}

int GraphicsWindowX11::getModifierMask() const
{
    int mask = 0;
    XModifierKeymap *mkm = XGetModifierMapping(_eventDisplay);
    for (int i = 0; i < mkm->max_keypermod * 8; i++)
    {
        unsigned int key = mkm->modifiermap[i];
        if (key && keyMapGetKey(_keyMap, key))
        {
            mask |= 1 << (i / mkm->max_keypermod);
        }
    }
    return mask;
}

void GraphicsWindowX11::requestWarpPointer(float x,float y)
{
    Display* display = _eventDisplay; // getDisplayToUse();

    XWarpPointer( display, 
                  None,
                  _window, 
                  0, 0, 0, 0,
                  static_cast<int>(x), static_cast<int>(y) );

    XFlush(display);
    XSync(display, 0);
    
    getEventQueue()->mouseWarped(x,y);
}

extern "C" 
{

typedef int (*X11ErrorHandler)(Display*, XErrorEvent*);

int X11ErrorHandling(Display* display, XErrorEvent* event)
{
    osg::notify(osg::NOTICE)<<"Got an X11ErrorHandling call display="<<display<<" event="<<event<<std::endl;

    char buffer[256];
    XGetErrorText( display, event->error_code, buffer, 256);

    osg::notify(osg::NOTICE) << buffer << std::endl;
    osg::notify(osg::NOTICE) << "Major opcode: " << (int)event->request_code << std::endl;
    osg::notify(osg::NOTICE) << "Minor opcode: " << (int)event->minor_code << std::endl;
    osg::notify(osg::NOTICE) << "Error code: " << (int)event->error_code << std::endl;
    osg::notify(osg::NOTICE) << "Request serial: " << event->serial << std::endl;
    osg::notify(osg::NOTICE) << "Current serial: " << NextRequest( display ) - 1 << std::endl;

    switch( event->error_code )
    {
        case BadValue:
            osg::notify(osg::NOTICE) << "  Value: " << event->resourceid << std::endl;
            break;

        case BadAtom:
            osg::notify(osg::NOTICE) << "  AtomID: " << event->resourceid << std::endl;
            break;

        default:
            osg::notify(osg::NOTICE) << "  ResourceID: " << event->resourceid << std::endl;
            break;
    }
    return 0;
}

}

class X11WindowingSystemInterface : public osg::GraphicsContext::WindowingSystemInterface
{
#ifdef OSGVIEWER_USE_XRANDR
    // TODO: Investigate whether or not Robert thinks we should store/restore the original
    // resolution in the destructor; I'm not sure the other ones do this, and it may be the
    // responsibility of the user.
    bool _setScreen(const osg::GraphicsContext::ScreenIdentifier& si, unsigned int width, unsigned height, double rate) {
        Display* display = XOpenDisplay(si.displayName().c_str());
        
        if(display)
        {
            XRRScreenConfiguration* sc = XRRGetScreenInfo(display, RootWindow(display, si.screenNum));

            if(!sc)
            {
                osg::notify(osg::NOTICE) << "Unable to create XRRScreenConfiguration on display \"" << XDisplayName(si.displayName().c_str()) << "\"."<<std::endl;
                return false;
            }

            int      numScreens = 0;
            int      numRates   = 0;
            Rotation currentRot = 0;
            bool     okay       = false;

            XRRConfigRotations(sc, &currentRot);
            
            // If the width or height are zero, use our defaults.
            if(!width || !height)
            {
                getScreenResolution(si, width, height);
            }

            // If this somehow fails, okay will still be false, no iteration will take place below,
            // and the sc pointer will still be freed later on.
            XRRScreenSize* ss = XRRConfigSizes(sc, &numScreens);

            for(int i = 0; i < numScreens; i++)
            {
                if(ss[i].width == static_cast<int>(width) && ss[i].height == static_cast<int>(height))
                {
                    short* rates     = XRRConfigRates(sc, i, &numRates);
                    bool   rateFound = false;
                    
                    // Search for our rate in the list of acceptable rates given to us by Xrandr.
                    // If it's not found, rateFound will still be false and the call will never
                    // be made to XRRSetScreenConfigAndRate since the rate will be invalid.
                    for(int r = 0; r < numRates; r++)
                    {
                        if(rates[r] == static_cast<short>(rate))
                        {
                            rateFound = true;
                            break;
                        }
                    }

                    if(rate > 0.0f && !rateFound)
                    {
                        osg::notify(osg::NOTICE) << "Unable to find valid refresh rate " << rate << " on display \"" << XDisplayName(si.displayName().c_str()) << "\"."<<std::endl;
                    }
                    else if(XRRSetScreenConfigAndRate(display, sc, DefaultRootWindow(display), i, currentRot, static_cast<short>(rate), CurrentTime) != RRSetConfigSuccess)
                    {
                        osg::notify(osg::NOTICE) << "Unable to set resolution to " << width << "x" << height << " on display \"" << XDisplayName(si.displayName().c_str()) << "\"."<<std::endl;
                    }
                    else
                    {
                        okay = true;
                        break;
                    }
                }
            }
    
            XRRFreeScreenConfigInfo(sc);
    
            return okay;
        }
        else
        {
            osg::notify(osg::NOTICE) << "Unable to open display \"" << XDisplayName(si.displayName().c_str()) << "\"."<<std::endl;
            return false;
        }
    }
#endif

protected:
    bool _errorHandlerSet;
    

public:
    X11WindowingSystemInterface()
    {
        osg::notify(osg::INFO)<<"X11WindowingSystemInterface()"<<std::endl;


        // Install an X11 error handler, if the application has not already done so.
        
        // Set default handler, and get pointer to current handler.
        X11ErrorHandler currentHandler = XSetErrorHandler(NULL);
        
        // Set our handler, and get pointer to default handler.
        X11ErrorHandler defHandler = XSetErrorHandler(X11ErrorHandling);

        if ( currentHandler == defHandler )
        {
            // No application error handler, use ours.
            // osg::notify(osg::INFO)<<"Set osgViewer X11 error handler"<<std::endl;
            _errorHandlerSet = 1;
        }
        else
        {
            // Application error handler exists, leave it set.
            // osg::notify(osg::INFO)<<"Existing application X11 error handler set"<<std::endl;
            _errorHandlerSet = 0;
            XSetErrorHandler(currentHandler);
        }
    
#if 0
        if (XInitThreads() == 0)
        {
            osg::notify(osg::NOTICE) << "Error: XInitThreads() failed. Aborting." << std::endl;
            exit(1);
        }
        else
        {
            osg::notify(osg::INFO) << "X11WindowingSystemInterface, xInitThreads() multi-threaded X support initialized.\n";
        }
#endif        
    }

    ~X11WindowingSystemInterface()
    {
        if (osg::Referenced::getDeleteHandler())
        {
            osg::Referenced::getDeleteHandler()->setNumFramesToRetainObjects(0);
            osg::Referenced::getDeleteHandler()->flushAll();
        }

        //osg::notify(osg::NOTICE)<<"~X11WindowingSystemInterface()"<<std::endl;

        // Unset our X11 error handler, providing the application has not replaced it.

        if ( _errorHandlerSet )
        {
            X11ErrorHandler currentHandler = XSetErrorHandler(NULL);
            if ( currentHandler == X11ErrorHandling )
            {
                // osg::notify(osg::INFO)<<"osgViewer X11 error handler removed"<<std::endl;
            }
            else
            {
                // Not our error handler, leave it set.
                // osg::notify(osg::INFO)<<"Application X11 error handler left"<<std::endl;
                XSetErrorHandler(currentHandler);
            }
        }
    }

    virtual unsigned int getNumScreens(const osg::GraphicsContext::ScreenIdentifier& si) 
    {
        Display* display = XOpenDisplay(si.displayName().c_str());
        if(display)
        {
            unsigned int numScreens = ScreenCount(display); 
            XCloseDisplay(display);

            return numScreens;
        }
        else
        {
            osg::notify(osg::NOTICE) << "A Unable to open display \"" << XDisplayName(si.displayName().c_str()) << "\""<<std::endl;
            return 0;
        }
    }

    virtual void getScreenResolution(const osg::GraphicsContext::ScreenIdentifier& si, unsigned int& width, unsigned int& height)
    {
        Display* display = XOpenDisplay(si.displayName().c_str());
        if(display)
        {
            width = DisplayWidth(display, si.screenNum);
            height = DisplayHeight(display, si.screenNum);
            XCloseDisplay(display);
        }
        else
        {
            osg::notify(osg::NOTICE) << "Unable to open display \"" << XDisplayName(si.displayName().c_str()) << "\"."<<std::endl;
            width = 0;
            height = 0;
        }
    }

    virtual bool setScreenResolution(const osg::GraphicsContext::ScreenIdentifier& si, unsigned int width, unsigned int height)
    {
#ifdef OSGVIEWER_USE_XRANDR
        return _setScreen(si, width, height, 0.0f);
#else
        osg::notify(osg::NOTICE) << "You must build osgViewer with Xrandr 1.2 or higher for setScreenResolution support!" << std::endl;
        return false;
#endif
    }

    virtual bool setScreenRefreshRate(const osg::GraphicsContext::ScreenIdentifier& si, double rate)
    {
#ifdef OSGVIEWER_USE_XRANDR
        return _setScreen(si, 0, 0, rate);
#else
        osg::notify(osg::NOTICE) << "You must build osgViewer with Xrandr 1.2 or higher for setScreenRefreshRate support!" << std::endl;
        return false;
#endif
    }

    virtual osg::GraphicsContext* createGraphicsContext(osg::GraphicsContext::Traits* traits)
    {
        if (traits->pbuffer)
        {
#if 1
            osg::ref_ptr<osgViewer::PixelBufferX11> pbuffer = new PixelBufferX11(traits);
            if (pbuffer->valid()) return pbuffer.release();
            else return 0;
#else
            osg::ref_ptr<osgViewer::GraphicsWindowX11> window = new GraphicsWindowX11(traits);
            if (window->valid()) return window.release();
            else return 0;
#endif
        }
        else
        {
            osg::ref_ptr<osgViewer::GraphicsWindowX11> window = new GraphicsWindowX11(traits);
            if (window->valid()) return window.release();
            else return 0;
        }
    }

};

struct RegisterWindowingSystemInterfaceProxy
{
    RegisterWindowingSystemInterfaceProxy()
    {
        osg::notify(osg::INFO)<<"RegisterWindowingSystemInterfaceProxy()"<<std::endl;
        osg::GraphicsContext::setWindowingSystemInterface(new X11WindowingSystemInterface);
    }

    ~RegisterWindowingSystemInterfaceProxy()
    {
        osg::notify(osg::INFO)<<"~RegisterWindowingSystemInterfaceProxy()"<<std::endl;

        if (osg::Referenced::getDeleteHandler())
        {
            osg::Referenced::getDeleteHandler()->setNumFramesToRetainObjects(0);
            osg::Referenced::getDeleteHandler()->flushAll();
        }

        osg::GraphicsContext::setWindowingSystemInterface(0);

    }
};

RegisterWindowingSystemInterfaceProxy createWindowingSystemInterfaceProxy;

// declare C entry point for static compilation.
extern "C" void graphicswindow_X11(void)
{
    osg::GraphicsContext::setWindowingSystemInterface(new X11WindowingSystemInterface);
}


void GraphicsWindowX11::raiseWindow()
{
    Display* display = getDisplayToUse();
    XWindowAttributes winAttrib;

    Window root_return, parent_return, *children;
    unsigned int nchildren, i=0;
    XTextProperty windowName;
    bool xraise = false;
    

    XQueryTree(display, _parent, &root_return, &parent_return, &children, &nchildren);
    while (!xraise &&  i<nchildren)
    {
    XGetWMName(display,children[i++],&windowName);
        if ((windowName.nitems != 0) && (strcmp(_traits->windowName.c_str(),(const char *)windowName.value) == 0)) xraise = true;
    }
    if (xraise) XRaiseWindow(display,_window);
    else 
    {
    XGetWindowAttributes(display, _window, &winAttrib);
    XReparentWindow(display, _window, _parent, winAttrib.x, winAttrib.y);
    }
    XFree(children);

    XFlush(display); 
    XSync(display,0);
}
