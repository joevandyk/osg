/*
 *  QTUtils.h
 *  NativeContext
 *
 *  Created by Stephan Huber on Fri Sep 06 2002.
 *  Copyright (c) 2002 digital mind. All rights reserved.
 *
 */

#ifndef QTUTILS_HEADER_
#define QTUTILS_HEADER_

// Quicktime plugin is able to load regular 2D images 
// besides movie streams. 
// It is used as default image loader on __APPLE__
// Uncomment this define to use it as image loader
// on other platforms. 
// #define QT_HANDLE_IMAGES_ALSO

#ifdef __APPLE__
    #include <QuickTime/QuickTime.h>
    #include <Carbon/Carbon.h>
    #define QT_HANDLE_IMAGES_ALSO
#else
    #include <QTML.h>
    #include <Movies.h>
    #include <Quickdraw.h>
    #include <QDOffscreen.h>
    #include <QuicktimeComponents.h>
    #include <FixMath.h>
    #include <CGBitmapContext.h>
    #include <CGImage.h>
    #include <CGColorSpace.h>
    #include <ImageCompression.h>

    extern "C" {
        /** legacy function for Windows */
        inline void GetPortBounds(GWorldPtr gw, Rect* rect) {
            (*rect) = (gw->portRect);
        }
        /** legacy function for Windows */
        inline PixMapHandle GetPortPixMap (CGrafPtr port) {
            return port->portPixMap;
        }
 
    }

#define SetRect MacSetRect
#define OffsetRect MacOffsetRect

#endif


    /**
     * inits Quicktime, if erase = true, the Quicktime-stuff gets cleaned, call it before your app exits
     * @param erase true, if you want to cleanup quicktime-related stuff
     */
    void initQuicktime(bool erase = false);
    
    /** cleans up all quicktime-related stuff */
    void exitQuicktime();
    
    /** constructs an FSSpec out of an path */
     OSStatus MakeFSSpecFromPath(const char* path, FSSpec* spec);
    
    /** opens a movie from a path */
      OSStatus MakeMovieFromPath(const char* path, Movie* movie, short& resref);


#endif
