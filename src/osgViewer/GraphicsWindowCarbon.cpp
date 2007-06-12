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

#ifdef __APPLE__
#include <osg/observer_ptr>
#include <osgViewer/api/Carbon/GraphicsWindowCarbon>
#include <Carbon/Carbon.h>
#include <OpenGL/OpenGL.h>
using namespace osgViewer;


// Carbon-Eventhandler to handle the click in the close-widget and the resize of windows

static pascal OSStatus GraphicsWindowEventHandler(EventHandlerCallRef nextHandler, EventRef event, void* userData) 
{
    WindowRef           window;
    Rect                bounds;
    OSStatus            result = eventNotHandledErr; /* report failure by default */
    
    
    result = CallNextEventHandler (nextHandler, event);

    GraphicsWindowCarbon* w = (GraphicsWindowCarbon*)userData;
    if (!w)
        return result;
    
    GetEventParameter(event, kEventParamDirectObject, typeWindowRef, NULL,
                         sizeof(window), NULL, &window);
                         
    switch(GetEventClass(event))
    {
        case kEventClassTablet:
        case kEventClassMouse:
            if (w->handleMouseEvent(event))
                result = noErr;
            break;
       
        case kEventClassKeyboard:
            if (w->handleKeyboardEvent(event))
                result = noErr;
            break;
            
        case kEventClassWindow: {
        
            switch (GetEventKind(event))
                {
                    case kEventWindowBoundsChanging:
                        // left the code for live-resizing, but it is not used, because of window-refreshing issues...
                        GetEventParameter( event, kEventParamCurrentBounds, typeQDRectangle, NULL, sizeof(Rect), NULL, &bounds );
                        
                        w->resized(bounds.left, bounds.top, bounds.right - bounds.left, bounds.bottom - bounds.top);
                        w->getEventQueue()->windowResize(bounds.left, bounds.top, bounds.right - bounds.left, bounds.bottom - bounds.top, w->getEventQueue()->getTime());
                        w->requestRedraw();
                        result = noErr;
                        break;
                            
                    case kEventWindowBoundsChanged:
                        InvalWindowRect(window, GetWindowPortBounds(window, &bounds));
                        GetWindowBounds(window, kWindowContentRgn, &bounds);
                        w->resized(bounds.left, bounds.top, bounds.right - bounds.left, bounds.bottom - bounds.top);
                        w->getEventQueue()->windowResize(bounds.left, bounds.top, bounds.right - bounds.left, bounds.bottom - bounds.top, w->getEventQueue()->getTime());
                        result = noErr;
                        break;
                    
                    case kEventWindowClosed:
                        w->requestClose();
                        break;
         
                    default: 
                        break;
                }
            }
        default:
            break;
    }
    return result;
}

static bool s_quit_requested = false;

// Application eventhandler -- listens for a quit-event 
static pascal OSStatus ApplicationEventHandler(EventHandlerCallRef inHandlerCallRef, EventRef inEvent, void *inUserData)
{
    
    HICommand commandStruct;

    OSErr  err = eventNotHandledErr;

    GetEventParameter (inEvent, kEventParamDirectObject, typeHICommand, NULL, sizeof(HICommand), NULL, &commandStruct);

    switch(commandStruct.commandID) {
        case kHICommandQuit:
            s_quit_requested = true;
            err = noErr;
            break;
                    
    }
    
    return err;
}

// AppleEventHandler, listens to the Quit-AppleEvent
static pascal OSErr QuitAppleEventHandler(const AppleEvent *theAppleEvent, AppleEvent *reply, long handlerRefcon ) {
    s_quit_requested = true;
    return (noErr);
}


namespace osgViewer
{

// small helper class which maps the raw key codes to osgGA::GUIEventAdapter::Keys

class OSXKeyboardMap {

    public:
        OSXKeyboardMap()
        {
            _keymap[53                ] =  osgGA::GUIEventAdapter::KEY_Escape;
            _keymap[115                ] =  osgGA::GUIEventAdapter::KEY_Home;
            _keymap[76                ] =  osgGA::GUIEventAdapter::KEY_KP_Enter;
            _keymap[119                ] =  osgGA::GUIEventAdapter::KEY_End;
            _keymap[36                ] =  osgGA::GUIEventAdapter::KEY_Return;
            _keymap[116                ] =  osgGA::GUIEventAdapter::KEY_Page_Up;
            _keymap[121                ] = osgGA::GUIEventAdapter::KEY_Page_Down;
            _keymap[123                ] = osgGA::GUIEventAdapter::KEY_Left;
            _keymap[124                ] = osgGA::GUIEventAdapter::KEY_Right;
            _keymap[126                ] = osgGA::GUIEventAdapter::KEY_Up;
            _keymap[125                ] = osgGA::GUIEventAdapter::KEY_Down;
            _keymap[51                ] = osgGA::GUIEventAdapter::KEY_BackSpace;
            _keymap[48                ] = osgGA::GUIEventAdapter::KEY_Tab;
            _keymap[49                ] = osgGA::GUIEventAdapter::KEY_Space;
            _keymap[117                ] = osgGA::GUIEventAdapter::KEY_Delete;
            
            _keymap[122                    ] = osgGA::GUIEventAdapter::KEY_F1;
            _keymap[120                    ] = osgGA::GUIEventAdapter::KEY_F2;
            _keymap[99                    ] = osgGA::GUIEventAdapter::KEY_F3;
            _keymap[118                    ] = osgGA::GUIEventAdapter::KEY_F4;
            _keymap[96                    ] = osgGA::GUIEventAdapter::KEY_F5;
            _keymap[97                    ] = osgGA::GUIEventAdapter::KEY_F6;
            _keymap[98                    ] = osgGA::GUIEventAdapter::KEY_F7;
            _keymap[100                    ] = osgGA::GUIEventAdapter::KEY_F8;
            _keymap[101                    ] = osgGA::GUIEventAdapter::KEY_F9;
            _keymap[109                    ] = osgGA::GUIEventAdapter::KEY_F10;
            _keymap[103                    ] = osgGA::GUIEventAdapter::KEY_F11;
            _keymap[111                    ] = osgGA::GUIEventAdapter::KEY_F12;
            
            _keymap[75                    ] = osgGA::GUIEventAdapter::KEY_KP_Divide;
            _keymap[67                    ] = osgGA::GUIEventAdapter::KEY_KP_Multiply;
            _keymap[78                    ] = osgGA::GUIEventAdapter::KEY_KP_Subtract;
            _keymap[69                    ] = osgGA::GUIEventAdapter::KEY_KP_Add;
            _keymap[89                    ] = osgGA::GUIEventAdapter::KEY_KP_Home;
            _keymap[91                    ] = osgGA::GUIEventAdapter::KEY_KP_Up;
            _keymap[92                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Up;
            _keymap[86                    ] = osgGA::GUIEventAdapter::KEY_KP_Left;
            _keymap[87                    ] = osgGA::GUIEventAdapter::KEY_KP_Begin;
            _keymap[88                    ] = osgGA::GUIEventAdapter::KEY_KP_Right;
            _keymap[83                    ] = osgGA::GUIEventAdapter::KEY_KP_End;
            _keymap[84                    ] = osgGA::GUIEventAdapter::KEY_KP_Down;
            _keymap[85                    ] = osgGA::GUIEventAdapter::KEY_KP_Page_Down;
            _keymap[82                    ] = osgGA::GUIEventAdapter::KEY_KP_Insert;
            _keymap[65                    ] = osgGA::GUIEventAdapter::KEY_KP_Delete;

        }
        
        ~OSXKeyboardMap() {
        }
        
        unsigned int remapKey(unsigned int key, unsigned int rawkey)
        {
            KeyMap::iterator itr = _keymap.find(rawkey);
            if (itr == _keymap.end()) return key;
            else return itr->second;
        }
    private:
        typedef std::map<unsigned int, osgGA::GUIEventAdapter::KeySymbol> KeyMap;
        KeyMap _keymap;
};

/** remaps a native os x keycode to a GUIEventAdapter-keycode */
static unsigned int remapOSXKey(unsigned int key, unsigned int rawkey)
{
    static OSXKeyboardMap s_OSXKeyboardMap;
    return s_OSXKeyboardMap.remapKey(key,rawkey);
}

/** creates a pixelformat from a Trait */        
static AGLPixelFormat createPixelFormat(osg::GraphicsContext::Traits* traits) {

    std::vector<GLint> attributes;
    
    if (!traits->pbuffer) attributes.push_back(AGL_NO_RECOVERY);
    attributes.push_back(AGL_RGBA);
    if (!traits->pbuffer) attributes.push_back(AGL_COMPLIANT);
    
    if (traits->doubleBuffer) attributes.push_back(AGL_DOUBLEBUFFER);
    if (traits->quadBufferStereo) attributes.push_back(AGL_STEREO);
    
    attributes.push_back(AGL_RED_SIZE); attributes.push_back(traits->red);
    attributes.push_back(AGL_GREEN_SIZE); attributes.push_back(traits->green);
    attributes.push_back(AGL_BLUE_SIZE); attributes.push_back(traits->blue);
    attributes.push_back(AGL_DEPTH_SIZE); attributes.push_back(traits->depth);
    
    if (traits->alpha) { attributes.push_back(AGL_ALPHA_SIZE); attributes.push_back(traits->alpha); }
    
    if (traits->stencil) { attributes.push_back(AGL_STENCIL_SIZE); attributes.push_back(traits->stencil); }
    
    // TODO 
    // missing accumulation-buffer-stuff

#if defined(AGL_SAMPLE_BUFFERS_ARB) && defined (AGL_SAMPLES_ARB)

    if (traits->sampleBuffers) { attributes.push_back(AGL_SAMPLE_BUFFERS_ARB); attributes.push_back(traits->sampleBuffers); }
    if (traits->sampleBuffers) { attributes.push_back(AGL_SAMPLES_ARB); attributes.push_back(traits->samples); }

#endif
    attributes.push_back(AGL_NONE);
    
    return aglChoosePixelFormat(NULL, 0, &(attributes.front()));
}


#pragma mark * * * GraphicsContextCarbon * * * 

/** This is the class we need to create for pbuffers, note its not a GraphicsWindow as it won't need any of the event handling and window mapping facilities.*/
class GraphicsContextCarbon : public osg::GraphicsContext
{
    public:

        GraphicsContextCarbon(osg::GraphicsContext::Traits* traits):
            _valid(false),
            _realized(false),
            _context(NULL)
        {
            _traits = traits;
            
            _valid = true;
        }
    
        virtual bool valid() const { return _valid; }

        /** Realise the GraphicsContext implementation, 
          * Pure virtual - must be implemented by concrate implementations of GraphicsContext. */
        virtual bool realizeImplementation() 
        {
            AGLPixelFormat pixelformat = createPixelFormat(_traits.get());
            
            if (!pixelformat) {
                osg::notify(osg::WARN) << "GraphicsContext::realizeImplementation() aglChoosePixelFormat failed! " << aglErrorString(aglGetError()) << std::endl;
                return false;
            }
            
            _context = aglCreateContext (pixelformat, NULL);
            
            if (!_context) {
                osg::notify(osg::WARN) << "GraphicsContext::realizeImplementation() aglCreateContext failed! " << aglErrorString(aglGetError()) << std::endl;
                return false;
            }
            
            aglDestroyPixelFormat(pixelformat);
            
            _realized = aglCreatePBuffer (_traits->width, _traits->height, _traits->target, GL_RGBA, _traits->level, &(_pbuffer));
            if (!_realized) {
                osg::notify(osg::WARN) << "GraphicsContext::realizeImplementation() aglCreatePBuffer failed! " << aglErrorString(aglGetError()) << std::endl;
            }
            return _realized;
        }

        /** Return true if the graphics context has been realised, and is ready to use, implementation.
          * Pure virtual - must be implemented by concrate implementations of GraphicsContext. */
        virtual bool isRealizedImplementation() const  { return _realized; }

        /** Close the graphics context implementation.
          * Pure virtual - must be implemented by concrate implementations of GraphicsContext. */
        virtual void closeImplementation()  
        { 
            if (_pbuffer) aglDestroyPBuffer(_pbuffer);
            if (_context) aglDestroyContext(_context);
            _valid = _realized = false;
        }

        /** Make this graphics context current implementation.
          * Pure virtual - must be implemented by concrate implementations of GraphicsContext. */
        virtual bool makeCurrentImplementation() 
        { 
            return (_realized) ? (aglSetCurrentContext(_context) == GL_TRUE) : false;
        }
        
        /** Make this graphics context current with specified read context implementation.
          * Pure virtual - must be implemented by concrate implementations of GraphicsContext. */
        virtual bool makeContextCurrentImplementation(GraphicsContext* /*readContext*/)  { osg::notify(osg::NOTICE)<<"GraphicsWindow::makeContextCurrentImplementation(..) not implemented."<<std::endl; return false;}

         /** Release the graphics context.*/
        virtual bool releaseContextImplementation() 
        {  
             return (aglSetCurrentContext(NULL) == GL_TRUE);
        }


        /** Pure virtual, Bind the graphics context to associated texture implementation.
          * Pure virtual - must be implemented by concrate implementations of GraphicsContext. */
        virtual void bindPBufferToTextureImplementation(GLenum /*buffer*/)  { osg::notify(osg::NOTICE)<<"GraphicsWindow::void bindPBufferToTextureImplementation(..) not implemented."<<std::endl; }

        /** Swap the front and back buffers implementation.
          * Pure virtual - must be implemented by Concrate implementations of GraphicsContext. */
        virtual void swapBuffersImplementation()  
        { 
             aglSwapBuffers(_context);
        }
        
    protected:
        
        bool        _valid;
        bool        _realized;
        AGLContext    _context;
        AGLPbuffer    _pbuffer;

};

#pragma mark * * * MenubarController * * * 

/** the MenubarController class checks all open windows if they intersect with the menubar / dock and hide the menubar/dock if necessary */
class MenubarController : public osg::Referenced 
{

    public:
        MenubarController() : 
            osg::Referenced(), 
            _list(), 
            _menubarShown(false) 
        {
            // the following code will query the system for the available ect on the main-display (typically the displaying showing the menubar + the dock

            GDHandle mainScreenDevice;
            
            DMGetGDeviceByDisplayID((DisplayIDType) CGMainDisplayID(), &mainScreenDevice, true);
            GetAvailableWindowPositioningBounds (mainScreenDevice, &_availRect);
            
            // now we need the rect of the main-display including the menubar and the dock
            _mainScreenBounds = CGDisplayBounds( CGMainDisplayID() );
            
            // hide the menubar initially
            SetSystemUIMode(kUIModeAllHidden, kUIOptionAutoShowMenuBar);
        }
        
        static MenubarController* instance();
        
        void attachWindow(GraphicsWindowCarbon* win);
        void update();
        void detachWindow(GraphicsWindowCarbon* win);
        
    private: 
        typedef std::list< osg::observer_ptr< GraphicsWindowCarbon > > WindowList;
        WindowList    _list;
        bool        _menubarShown;
        Rect        _availRect;
        CGRect        _mainScreenBounds;
        
};


MenubarController* MenubarController::instance() 
{
    static osg::ref_ptr<MenubarController> s_menubar_controller = new MenubarController();
    return s_menubar_controller.get();
}


void MenubarController::attachWindow(GraphicsWindowCarbon* win)
{
    _list.push_back(win);
    update();
}


void MenubarController::detachWindow(GraphicsWindowCarbon* win) 
{
    for(WindowList::iterator i = _list.begin(); i != _list.end(); ) {
        if ((*i).get() == win)
            i = _list.erase(i);
        else 
            ++i;
    }
    update();
}

// iterate through all open windows and check, if they intersect the area occupied by the menubar/dock, and if so, hide the menubar/dock

void MenubarController::update() 
{
    OSErr error(noErr);
    unsigned int windowsCoveringMenubarArea = 0;    
    unsigned int windowsIntersectingMainScreen = 0;
    for(WindowList::iterator i = _list.begin(); i != _list.end(); ) {
        if ((*i).valid()) {
            GraphicsWindowCarbon* w = (*i).get();
            Rect windowBounds;
            error = GetWindowBounds(w->getNativeWindowRef(), kWindowStructureRgn, &windowBounds);
            
            bool intersect = !( (_mainScreenBounds.origin.x > windowBounds.right) ||
                                (_mainScreenBounds.origin.x + _mainScreenBounds.size.width < windowBounds.left) ||
                                (_mainScreenBounds.origin.y > windowBounds.bottom) ||
                                (_mainScreenBounds.origin.y + _mainScreenBounds.size.height < windowBounds.top));
            if (intersect && !error)
            {
                ++windowsIntersectingMainScreen;
                
                // the window intersects the main-screen, does it intersect with the menubar/dock?
                if (((_availRect.top > _mainScreenBounds.origin.y) && (_availRect.top > windowBounds.top)) ||
                    ((_availRect.left > _mainScreenBounds.origin.x) && (_availRect.left > windowBounds.left)) || 
                    ((_availRect.right < _mainScreenBounds.origin.x + _mainScreenBounds.size.width) && (_availRect.right < windowBounds.right)) || 
                    ((_availRect.bottom < _mainScreenBounds.origin.y + _mainScreenBounds.size.height) && (_availRect.bottom < windowBounds.bottom) ))
                {
                    ++windowsCoveringMenubarArea;
                }
            }
            
            ++i;
        }
        else
            i= _list.erase(i);
    }
    
    // see http://developer.apple.com/technotes/tn2002/tn2062.html for hiding the dock+menubar
        
    if (windowsCoveringMenubarArea && _menubarShown)
        error = SetSystemUIMode(kUIModeAllHidden, kUIOptionAutoShowMenuBar);
    
    if (!windowsCoveringMenubarArea && !_menubarShown)
        error = SetSystemUIMode(kUIModeNormal, 0);
        _menubarShown = !windowsCoveringMenubarArea;
    
    // osg::notify(osg::DEBUG_INFO) << "MenubarController:: " << windowsCoveringMenubarArea << " windows covering the menubar/dock area, " << windowsIntersectingMainScreen << " intersecting mainscreen" << std::endl;
}


#pragma mark * * * OSXWindowingSystemInterface * * * 

struct OSXCarbonWindowingSystemInterface : public osg::GraphicsContext::WindowingSystemInterface
{
    
    /** ctor, get a list of all attached displays */
    OSXCarbonWindowingSystemInterface() :
        _displayCount(0),
        _displayIds(NULL)
    {
        ProcessSerialNumber sn = { 0, kCurrentProcess };
        TransformProcessType(&sn,kProcessTransformToForegroundApplication);
        SetFrontProcess(&sn);
        
        if( CGGetActiveDisplayList( 0, NULL, &_displayCount ) != CGDisplayNoErr )
            osg::notify(osg::WARN) << "OSXCarbonWindowingSystemInterface: could not get # of screens" << std::endl;
            
        _displayIds = new CGDirectDisplayID[_displayCount];
        if( CGGetActiveDisplayList( _displayCount, _displayIds, &_displayCount ) != CGDisplayNoErr )
            osg::notify(osg::WARN) << "OSXCarbonWindowingSystemInterface: CGGetActiveDisplayList failed" << std::endl;
        
        // register application event handler and AppleEventHandler to get quit-events:
        static const EventTypeSpec menueventSpec = {kEventClassCommand, kEventCommandProcess};
        OSErr status = InstallEventHandler(GetApplicationEventTarget(), NewEventHandlerUPP(ApplicationEventHandler), 1, &menueventSpec, 0, NULL);
        status = AEInstallEventHandler( kCoreEventClass, kAEQuitApplication, NewAEEventHandlerUPP(QuitAppleEventHandler), 0, false);
    }
    
    /** dtor */
    ~OSXCarbonWindowingSystemInterface() {
        if (_displayIds)
            delete[] _displayIds;
        _displayIds = NULL;
    }
    
    /** @return a CGDirectDisplayID for a ScreenIdentifier */
    inline CGDirectDisplayID getDisplayID(const osg::GraphicsContext::ScreenIdentifier& si) {
        if (si.screenNum < _displayCount)
            return _displayIds[si.screenNum];
        else {
            osg::notify(osg::WARN) << "GraphicsWindowCarbon :: invalid screen # " << si.screenNum << ", returning main-screen instead" << std::endl;
            return _displayIds[0];
        }
    }

    /** @return count of attached screens */
    virtual unsigned int getNumScreens(const osg::GraphicsContext::ScreenIdentifier& si) 
    {
        return _displayCount;
    }

    /** returns the resolution of a specific display */
    virtual void getScreenResolution(const osg::GraphicsContext::ScreenIdentifier& si, unsigned int& width, unsigned int& height)
    {
        CGDirectDisplayID id = getDisplayID(si);
        width = CGDisplayPixelsWide(id);
        height = CGDisplayPixelsHigh(id);
    }
    
    /** return the top left coord of a specific screen in global screen space */
    void getScreenTopLeft(const osg::GraphicsContext::ScreenIdentifier& si, int& x, int& y) {
        CGRect bounds = CGDisplayBounds( getDisplayID(si) );
        x = static_cast<int>(bounds.origin.x);
        y = static_cast<int>(bounds.origin.y);
        
        // osg::notify(osg::DEBUG_INFO) << "topleft of screen " << si.screenNum <<" " << bounds.origin.x << "/" << bounds.origin.y << std::endl;
    }
    
    /** helper method to get a value out of a CFDictionary */
    static double getDictDouble (CFDictionaryRef refDict, CFStringRef key)
    {
       double double_value;
       CFNumberRef number_value = (CFNumberRef) CFDictionaryGetValue(refDict, key);
       if (!number_value) // if can't get a number for the dictionary
           return -1;  // fail
       if (!CFNumberGetValue(number_value, kCFNumberDoubleType, &double_value)) // or if cant convert it
            return -1; // fail
        return double_value; // otherwise return the long value
    }

    
    /** implementation of setScreenResolution */
    virtual bool setScreenResolution(const osg::GraphicsContext::ScreenIdentifier& screenIdentifier, unsigned int width, unsigned int height) 
    { 
        CGDirectDisplayID displayID = getDisplayID(screenIdentifier);
        
        // add next line and on following line replace hard coded depth and refresh rate
        CGRefreshRate refresh =  getDictDouble (CGDisplayCurrentMode(displayID), kCGDisplayRefreshRate);  
        CFDictionaryRef display_mode_values =
            CGDisplayBestModeForParametersAndRefreshRate(
                            displayID, 
                            CGDisplayBitsPerPixel(displayID), 
                            width, height,  
                            refresh,  
                            NULL);

                                          
        CGDisplaySwitchToMode(displayID, display_mode_values);    
        return true; 
    }
    
    /** implementation of setScreenRefreshRate */
    virtual bool setScreenRefreshRate(const osg::GraphicsContext::ScreenIdentifier& screenIdentifier, double refreshRate) { 
        
        boolean_t  success(false);
        unsigned width, height;
        getScreenResolution(screenIdentifier, width, height);
        
        CGDirectDisplayID displayID = getDisplayID(screenIdentifier);
        
        // add next line and on following line replace hard coded depth and refresh rate
        CFDictionaryRef display_mode_values =
            CGDisplayBestModeForParametersAndRefreshRate(
                            displayID, 
                            CGDisplayBitsPerPixel(displayID), 
                            width, height,  
                            refreshRate,  
                            &success);

                                          
        if (success)
            CGDisplaySwitchToMode(displayID, display_mode_values);    
            
        return (success != 0);
    }

        
    
    virtual osg::GraphicsContext* createGraphicsContext(osg::GraphicsContext::Traits* traits)
    {
        if (traits->pbuffer)
        {
            osg::ref_ptr<osgViewer::GraphicsContextCarbon> pbuffer = new GraphicsContextCarbon(traits);
            if (pbuffer->valid()) return pbuffer.release();
            else return 0;
        }
        else
        {
            osg::ref_ptr<osgViewer::GraphicsWindowCarbon> window = new GraphicsWindowCarbon(traits);
            if (window->valid()) return window.release();
            else return 0;
        }
    }
    
    
    
    private:
        CGDisplayCount        _displayCount;
        CGDirectDisplayID*    _displayIds;
};

}


#pragma mark * * * GraphicsWindowCarbon * * *




void GraphicsWindowCarbon::init()
{
    _windowTitleHeight = 0;
    _closeRequested = false;
    _ownsWindow = false;
    _context = NULL;
    _window = NULL;
    _pixelFormat = createPixelFormat(_traits.get());
    if (!_pixelFormat)
        osg::notify(osg::WARN) << "GraphicsWindowCarbon::init could not create a valid pixelformat" << std::endl;
    _valid = (_pixelFormat != NULL);
    _initialized = true;
}

bool GraphicsWindowCarbon::setWindowDecorationImplementation(bool flag)
{
    _useWindowDecoration = flag;

    if (_realized)
    {
        OSErr err = noErr;
        Rect bounds;
        GetWindowBounds(getNativeWindowRef(), kWindowContentRgn, &bounds);

        if (_useWindowDecoration)
        {
            err = ChangeWindowAttributes(getNativeWindowRef(),  kWindowStandardDocumentAttributes,  kWindowNoTitleBarAttribute | kWindowNoShadowAttribute);
            SetWindowBounds(getNativeWindowRef(), kWindowContentRgn, &bounds);
        }
        else
        {
            err = ChangeWindowAttributes(getNativeWindowRef(), kWindowNoTitleBarAttribute | kWindowNoShadowAttribute, kWindowStandardDocumentAttributes);
            SetWindowBounds(getNativeWindowRef(), kWindowContentRgn, &bounds);    
        }

        if (err != noErr)
        {
            osg::notify(osg::WARN) << "GraphicsWindowCarbon::setWindowDecoration failed with " << err << std::endl;
            return false;
        }
        
        // update titlebar-height
        Rect titleRect;
        GetWindowBounds(_window, kWindowTitleBarRgn, &titleRect);
        _windowTitleHeight = abs(titleRect.bottom - titleRect.top);
        
        // sth: I don't know why I have to reattach the context to the window here, If I don't do this  I get blank areas, where the titlebar was. 
        // InvalWindowRect doesn't help here :-/
        
        aglSetDrawable(_context, 0);
        aglSetDrawable(_context, GetWindowPort(_window));
                
        MenubarController::instance()->update();
    }

    return true;
}


WindowAttributes GraphicsWindowCarbon::computeWindowAttributes(bool useWindowDecoration, bool supportsResize) {
    WindowAttributes attr; 
    
    if (useWindowDecoration) 
    {
        if (supportsResize)
            attr = (kWindowStandardDocumentAttributes | kWindowStandardHandlerAttribute);
        else
            attr = (kWindowStandardDocumentAttributes | kWindowStandardHandlerAttribute) & ~kWindowResizableAttribute;
    }
    else 
    {
        attr = kWindowNoTitleBarAttribute | kWindowNoShadowAttribute | kWindowStandardHandlerAttribute;
        if (supportsResize)
            attr |= kWindowResizableAttribute;
    }
    return attr;
}

void GraphicsWindowCarbon::installEventHandler() {

    // register window event handler to receive resize-events
    EventTypeSpec   windEventList[] = {
        { kEventClassWindow, kEventWindowBoundsChanged},
        { kEventClassWindow, kEventWindowClosed},
        
        {kEventClassMouse, kEventMouseDown},
        {kEventClassMouse, kEventMouseUp},
        {kEventClassMouse, kEventMouseMoved},
        {kEventClassMouse, kEventMouseDragged},
        {kEventClassMouse, kEventMouseWheelMoved},

        {kEventClassKeyboard, kEventRawKeyDown},
        {kEventClassKeyboard, kEventRawKeyRepeat},
        {kEventClassKeyboard, kEventRawKeyUp},
        {kEventClassKeyboard, kEventRawKeyModifiersChanged},
        {kEventClassKeyboard, kEventHotKeyPressed},
        {kEventClassKeyboard, kEventHotKeyReleased},
    };
    

    InstallWindowEventHandler(_window, NewEventHandlerUPP(GraphicsWindowEventHandler),  GetEventTypeCount(windEventList), windEventList, this, NULL);
}


bool GraphicsWindowCarbon::realizeImplementation()
{
    
    if (!_initialized) init();
    if (!_initialized) return false;
    if (!_traits) return false;
    
    setWindowDecoration(_traits->windowDecoration);
    
    // move the window to the right screen

    OSXCarbonWindowingSystemInterface* wsi = dynamic_cast<OSXCarbonWindowingSystemInterface*>(osg::GraphicsContext::getWindowingSystemInterface());
    int screenLeft(0), screenTop(0);
    if (wsi) {
        
        wsi->getScreenTopLeft((*_traits), screenLeft, screenTop);
        _traits->y += screenTop;
        _traits->x += screenLeft;
    }
    
    WindowData *windowData = _traits.get() ? dynamic_cast<WindowData*>(_traits->inheritedWindowData.get()) : 0;
    _ownsWindow = (windowData) ? (windowData->getNativeWindowRef() == NULL) : true;
    
    if (_ownsWindow) {
        
        // create the window
        Rect bounds = {_traits->y, _traits->x, _traits->y + _traits->height, _traits->x + _traits->width};
        OSStatus err = 0;
        WindowAttributes attr = computeWindowAttributes(_useWindowDecoration, _traits->supportsResize);
        
        err = CreateNewWindow(kDocumentWindowClass, attr, &bounds, &_window);

        if (err) {
            osg::notify(osg::WARN) << "GraphicsWindowCarbon::realizeImplementation() failed creating a window: " << err << std::endl;
            return false;
        } else {
            osg::notify(osg::INFO) << "GraphicsWindowCarbon::realizeImplementation() - window created with bounds(" << bounds.top << ", " << bounds.left << ", " << bounds.bottom << ", " << bounds.right << ")" << std::endl;
        }
    }
    else {
        _window = windowData->getNativeWindowRef();
    }
    
    Rect titleRect;
    GetWindowBounds(_window, kWindowTitleBarRgn, &titleRect);
    _windowTitleHeight = abs(titleRect.bottom - titleRect.top);
    
    if ((_ownsWindow) || (windowData && windowData->installEventHandler()))
        installEventHandler();
    
    // set the window title
    if (!_traits->windowName.empty()) {
        CFStringRef windowtitle = CFStringCreateWithBytes( kCFAllocatorDefault, (const UInt8*)(_traits->windowName.c_str()), _traits->windowName.length(),kCFStringEncodingUTF8, false );
        SetWindowTitleWithCFString( _window, windowtitle );
    }
    
    // create the context
    GraphicsWindowCarbon* sharedContextCarbon = dynamic_cast<GraphicsWindowCarbon*>(_traits->sharedContext);
    if (sharedContextCarbon) 
    {
        _context = aglCreateContext (_pixelFormat, sharedContextCarbon->getAGLContext());
    }
    else
    {
        _context = aglCreateContext (_pixelFormat, NULL);
    }


    if (!_context) {
        osg::notify(osg::WARN) << "GraphicsWindowCarbon::realizeImplementation failed creating a context: " << aglGetError() << std::endl;
        return false;
    }
       
    makeCurrent();

    if ((_traits->useMultiThreadedOpenGLEngine) && (OpenThreads::GetNumberOfProcessors() > 1)) {
        // enable Multi-threaded OpenGL Execution:
        CGLError cgerr = kCGLNoError;
        CGLContextObj ctx = CGLGetCurrentContext();

#if 0
        cgerr =  CGLEnable( ctx, kCGLCEMPEngine);
#else
        // the above use of kCGLCEMPEngine is not backwards compatible
        // so we'll use the raw value of it to keep things compiling on older
        // versions of OSX.
        cgerr =  CGLEnable( ctx, static_cast <CGLContextEnable>(313) );
#endif    
        if (cgerr != kCGLNoError )
        {
            osg::notify(osg::INFO) << "GraphicsWindowCarbon:: Multi-threaded OpenGL Execution not available" << std::endl;
        } 
    }
    aglSetDrawable(_context, GetWindowPort(_window));
    ShowWindow(_window);
    
    //enable vsync
    if (_traits->vsync) {
        GLint swap = 1;
        aglSetInteger (_context, AGL_SWAP_INTERVAL, &swap);
    }

    MenubarController::instance()->attachWindow(this);
    
    _realized = true;
    return _realized;
}



bool GraphicsWindowCarbon::makeCurrentImplementation()
{
        
    return (aglSetCurrentContext(_context) == GL_TRUE);
}

bool GraphicsWindowCarbon::releaseContextImplementation()
{
    if (!_realized)
    {
        osg::notify(osg::NOTICE)<<"Warning: GraphicsWindow not realized, cannot do makeCurrent."<<std::endl;
        return false;
    }

    // osg::notify(osg::NOTICE)<<"makeCurrentImplementation "<<this<<" "<<OpenThreads::Thread::CurrentThread()<<std::endl;
    // osg::notify(osg::NOTICE)<<"   glXMakeCurrent ("<<_display<<","<<_window<<","<<_glxContext<<std::endl;
    return (aglSetCurrentContext(NULL) == GL_TRUE);
}



void GraphicsWindowCarbon::closeImplementation()
{
    _valid = false;
    _realized = false;
    
    MenubarController::instance()->detachWindow(this);
    
    if (_pixelFormat)
    {
        aglDestroyPixelFormat(_pixelFormat);
        _pixelFormat = NULL;
    }
    
    if (_context) 
    {
        aglSetDrawable(_context, NULL);
        aglSetCurrentContext(NULL);
        aglDestroyContext(_context);
        _context = NULL;
    }
    
    if (_ownsWindow && _window) DisposeWindow(_window);
    _window = NULL;
}



void GraphicsWindowCarbon::swapBuffersImplementation()
{
    aglSwapBuffers(_context);
}



void GraphicsWindowCarbon::resizedImplementation(int x, int y, int width, int height)
{
    GraphicsContext::resizedImplementation(x, y, width, height);

    aglUpdateContext(_context);
    MenubarController::instance()->update();
}



bool GraphicsWindowCarbon::handleMouseEvent(EventRef theEvent)
{

    static unsigned int lastEmulatedMouseButton = 0;
    // mouse down event   
    Point wheresMyMouse;
    GetEventParameter (theEvent, kEventParamWindowMouseLocation, typeQDPoint, NULL, sizeof(wheresMyMouse), NULL, &wheresMyMouse);
    
    wheresMyMouse.v -= _windowTitleHeight;
    if (_useWindowDecoration && (wheresMyMouse.v < 0))
        return false;
    
    Point wheresMyMouseGlobal;
    GetEventParameter (theEvent, kEventParamMouseLocation, typeQDPoint, NULL, sizeof(wheresMyMouse), NULL, &wheresMyMouseGlobal);
    
    EventMouseButton mouseButton = 0;
    GetEventParameter (theEvent, kEventParamMouseButton, typeMouseButton, NULL, sizeof(mouseButton), NULL, &mouseButton);
    
    UInt32 modifierKeys;
    GetEventParameter (theEvent,kEventParamKeyModifiers,typeUInt32, NULL,sizeof(modifierKeys), NULL,&modifierKeys);
    
    
    WindowRef win;
    int fwres = FindWindow(wheresMyMouseGlobal, &win);
    if ((fwres != inContent) && (fwres > 0) && (mouseButton >= 1))
    {
        return false;
    }
    else
    {
    
        // swap right and middle buttons so that middle button is 2, right button is 3.
        if (mouseButton==3) mouseButton = 2;
        else if (mouseButton==2) mouseButton = 3;
        
        // check tablet pointer device and map it to a musebutton
        TabletProximityRec    theTabletRecord;    // The Tablet Proximity Record
        // Extract the Tablet Proximity reccord from the event.
        if(noErr == GetEventParameter(theEvent, kEventParamTabletProximityRec,
                                      typeTabletProximityRec, NULL,
                                      sizeof(TabletProximityRec),
                                      NULL, (void *)&theTabletRecord))
        {            
            osgGA::GUIEventAdapter::TabletPointerType pointerType;
            switch(theTabletRecord.pointerType)
            {
                case 1: // pen
                    pointerType = osgGA::GUIEventAdapter::PEN;
                    break;
                    
                case 2: // puck
                    pointerType = osgGA::GUIEventAdapter::PUCK;
                    break;
                    
                case 3: //eraser
                    pointerType = osgGA::GUIEventAdapter::ERASER;
                    break;

                default:
                   pointerType = osgGA::GUIEventAdapter::UNKNOWN;
                   break;
            }
            
            getEventQueue()->penProximity(pointerType, (theTabletRecord.enterProximity != 0));
        }

        switch(GetEventKind(theEvent))
        {
            case kEventMouseDown:
                {
                    float mx =wheresMyMouse.h;
                    float my =wheresMyMouse.v;
                    transformMouseXY(mx, my);
                    
                    lastEmulatedMouseButton = 0;
                    
                    if (mouseButton == 1) 
                    {
                        if( modifierKeys & cmdKey )
                        {
                            mouseButton = lastEmulatedMouseButton = 3;
                        }
                        else if( modifierKeys & optionKey )
                        {
                            mouseButton = lastEmulatedMouseButton = 2;
                        }
                    }
                    
                    getEventQueue()->mouseButtonPress(mx, my, mouseButton);
                }
                break;
            case kEventMouseUp:
                {
                    float mx =wheresMyMouse.h;
                    float my =wheresMyMouse.v;
                    transformMouseXY(mx, my);
                    if (lastEmulatedMouseButton > 0) {
                        getEventQueue()->mouseButtonRelease(mx, my, lastEmulatedMouseButton);
                        lastEmulatedMouseButton = 0;
                    }
                    else {
                        getEventQueue()->mouseButtonRelease(mx, my, mouseButton);
                    }
                }
                break;
                
            case kEventMouseDragged:
                {
                    // get pressure from the pen, only when mouse/pen is dragged
                    TabletPointRec    theTabletRecord;
                    if(noErr == GetEventParameter(theEvent,  kEventParamTabletPointRec, typeTabletPointRec, NULL, 
                                    sizeof(TabletPointRec), NULL, (void *)&theTabletRecord)) {
                    
                        getEventQueue()->penPressure(theTabletRecord.pressure / 65535.0f);
            
                    }
                    
                    float mx =wheresMyMouse.h;
                    float my =wheresMyMouse.v;
                    transformMouseXY(mx, my);
                    getEventQueue()->mouseMotion(mx, my);
                }
                break;
                
            case kEventMouseMoved:
                {
                    float mx = wheresMyMouse.h;
                    float my = wheresMyMouse.v;
                    transformMouseXY(mx, my);
                    getEventQueue()->mouseMotion(mx, my);
                }
                break;
                
            // mouse with scroll-wheels
            case kEventMouseWheelMoved:
                {
                    EventMouseWheelAxis axis;
                    SInt32 delta;
                    if (noErr == GetEventParameter( theEvent, kEventParamMouseWheelAxis, typeMouseWheelAxis, NULL, sizeof(axis), NULL, &axis )) {
                        if (noErr == GetEventParameter( theEvent, kEventParamMouseWheelDelta, typeLongInteger, NULL, sizeof(delta), NULL, &delta )) {
                            switch (axis) {
                                case kEventMouseWheelAxisX:
                                    getEventQueue()->mouseScroll( (delta > 0) ? osgGA::GUIEventAdapter::SCROLL_RIGHT : osgGA::GUIEventAdapter::SCROLL_LEFT);
                                    break;
                                case kEventMouseWheelAxisY:
                                    getEventQueue()->mouseScroll( (delta < 0) ? osgGA::GUIEventAdapter::SCROLL_DOWN : osgGA::GUIEventAdapter::SCROLL_UP);
                                    break;
                            }
                        }
                    }
                }
                break;
            
            // new trackpads and mighty mouse, (not officially documented, see http://developer.apple.com/qa/qa2005/qa1453.html )
            case 11:
                {
                    enum
                        {
                        kEventParamMouseWheelSmoothVerticalDelta       = 'saxy', // typeSInt32
                        kEventParamMouseWheelSmoothHorizontalDelta     = 'saxx' // typeSInt32
                        };
                    
                    SInt32 scroll_delta_x = 0;
                    SInt32 scroll_delta_y = 0;
                    OSErr err = noErr;
                    err = GetEventParameter( theEvent, kEventParamMouseWheelSmoothVerticalDelta, typeLongInteger, NULL, sizeof(scroll_delta_y), NULL, &scroll_delta_y );
                    err = GetEventParameter( theEvent, kEventParamMouseWheelSmoothHorizontalDelta, typeLongInteger, NULL, sizeof(scroll_delta_x), NULL, &scroll_delta_x );
                    
                    if ((scroll_delta_x != 0) || (scroll_delta_y != 0)) {
                        getEventQueue()->mouseScroll2D( scroll_delta_x, scroll_delta_y);
                    }
                }
                break;
            
            default:
                return false;
        }
    }
    
    return true;
}



bool GraphicsWindowCarbon::handleKeyboardEvent(EventRef theEvent)
{
    
    
    OSStatus status; 

    // Key modifiers, Numlock not supported...

    UInt32 modifierKeys;
    unsigned int modifierMask = 0;
    GetEventParameter (theEvent,kEventParamKeyModifiers,typeUInt32, NULL,sizeof(modifierKeys), NULL,&modifierKeys);
    
    
    if( modifierKeys & shiftKey )
    {
        modifierMask |= osgGA::GUIEventAdapter::MODKEY_SHIFT;
    }
    if( modifierKeys & alphaLock )
    {
        modifierMask |= osgGA::GUIEventAdapter::MODKEY_CAPS_LOCK;
    }
    if( modifierKeys & controlKey )
    {
        modifierMask |= osgGA::GUIEventAdapter::MODKEY_CTRL;
    }
    if( modifierKeys & optionKey )
    {
        modifierMask |= osgGA::GUIEventAdapter::MODKEY_ALT;
    }
    
    // we map the command-key to the META-key
    if( modifierKeys & cmdKey )
    {
        modifierMask |= osgGA::GUIEventAdapter::MODKEY_META;
    }
    
    
    UInt32 rawkey;
    GetEventParameter (theEvent,kEventParamKeyCode,typeUInt32, NULL,sizeof(rawkey), NULL,&rawkey);
    
    // std::cout << "key code: " << rawkey << " modifiers: " << modifierKeys << std::endl;
            
    UInt32 dataSize;
    /* jbw check return status so that we don't allocate a huge array */
    status = GetEventParameter( theEvent, kEventParamKeyUnicodes, typeUnicodeText, NULL, 0, &dataSize, NULL );
    if (status != noErr) return false;
    if (dataSize<=1) return false;
    
    UniChar* uniChars = new UniChar[dataSize+1];
    GetEventParameter( theEvent, kEventParamKeyUnicodes, typeUnicodeText, NULL, dataSize, NULL, (void*)uniChars );
    
    unsigned int keychar = remapOSXKey(static_cast<unsigned long>(uniChars[0]), rawkey);
    
    switch(GetEventKind(theEvent))
    {
        case kEventRawKeyDown:
        case kEventRawKeyRepeat:
        {
            getEventQueue()->getCurrentEventState()->setModKeyMask(modifierMask);
            getEventQueue()->keyPress(keychar);
            break;
        }
        
        case kEventRawKeyUp:
        {                 
            getEventQueue()->getCurrentEventState()->setModKeyMask(modifierMask);
            getEventQueue()->keyRelease(keychar);
            break;
        }
        
        default:
             break;
    
    }

    delete[] uniChars;

    return true;
}



void GraphicsWindowCarbon::checkEvents()
{
    if (!_realized) return;
    
    EventRef theEvent;
    EventTargetRef theTarget = GetEventDispatcherTarget();
    while (ReceiveNextEvent(0, NULL, 0,true, &theEvent)== noErr)
    {
        switch(GetEventClass(theEvent))
        {
            case kEventClassMouse: 
                    {
                    // handle the menubar
                    Point wheresMyMouse;
                    GetEventParameter (theEvent, kEventParamMouseLocation, typeQDPoint, NULL, sizeof(wheresMyMouse), NULL, &wheresMyMouse);
                    
                    EventMouseButton mouseButton = 0;
                    GetEventParameter (theEvent, kEventParamMouseButton, typeMouseButton, NULL, sizeof(mouseButton), NULL, &mouseButton);
                    
                    WindowRef win;
                    int fwres = FindWindow(wheresMyMouse, &win);

                    if ((fwres == inMenuBar) && (mouseButton >= 1)) {
                        MenuSelect(wheresMyMouse);
                        HiliteMenu(0);
                        return;
                    }
                    break;
                }

            case kEventClassApplication:
                switch (GetEventKind(theEvent)) {
                    case kEventAppQuit:
                        getEventQueue()->quitApplication();
                        break;
                }
                break;
            
            case kEventClassAppleEvent: 
                {
                    EventRecord eventRecord;
                    ConvertEventRefToEventRecord(theEvent, &eventRecord);
                    AEProcessAppleEvent(&eventRecord);
                    return;
                }
                break;

        }
        SendEventToEventTarget (theEvent, theTarget);
        ReleaseEvent(theEvent);        
    }  
    if (_closeRequested)
        close(true);
        
    if (s_quit_requested) {
        getEventQueue()->quitApplication();
        s_quit_requested = false;
    }
            
}


bool GraphicsWindowCarbon::setWindowRectangleImplementation(int x, int y, int width, int height)
{
    Rect bounds = {y, x, y + height, x + width};
    SetWindowBounds(getNativeWindowRef(), kWindowContentRgn, &bounds);
    aglUpdateContext(_context);
    MenubarController::instance()->update();
    return true;
}

void GraphicsWindowCarbon::grabFocus()
{
    SelectWindow(_window);
}



void GraphicsWindowCarbon::grabFocusIfPointerInWindow()
{
   // TODO: implement
   osg::notify(osg::ALWAYS) << "GraphicsWindowCarbon::grabFocusIfPointerInWindow" << std::endl;
}



void GraphicsWindowCarbon::transformMouseXY(float& x, float& y)
{
    if (getEventQueue()->getUseFixedMouseInputRange())
    {
        osgGA::GUIEventAdapter* eventState = getEventQueue()->getCurrentEventState();
        x = eventState->getXmin() + (eventState->getXmax()-eventState->getXmin())*x/float(_traits->width);
        y = eventState->getYmin() + (eventState->getYmax()-eventState->getYmin())*y/float(_traits->height);
    }
}






struct RegisterWindowingSystemInterfaceProxy
{
    RegisterWindowingSystemInterfaceProxy()
    {
        osg::GraphicsContext::setWindowingSystemInterface(new osgViewer::OSXCarbonWindowingSystemInterface());
    }

    ~RegisterWindowingSystemInterfaceProxy()
    {
        osg::GraphicsContext::setWindowingSystemInterface(0);
    }
};

RegisterWindowingSystemInterfaceProxy createWindowingSystemInterfaceProxy;

// declare C entry point for static compilation.
extern "C" void graphicswindow_Carbon(void)
{
    osg::GraphicsContext::setWindowingSystemInterface(new osgViewer::OSXCarbonWindowingSystemInterface());
}

#endif
