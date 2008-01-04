# Locate QuickTime
# This module defines
# QUICKTIME_LIBRARY
# QUICKTIME_FOUND, if false, do not try to link to gdal 
# QUICKTIME_INCLUDE_DIR, where to find the headers
#
# $QUICKTIME_DIR is an environment variable that would
# correspond to the ./configure --prefix=$QUICKTIME_DIR
#
# Created by Eric Wing. 

# QuickTime on OS X looks different than QuickTime for Windows,
# so I am going to case the two.

IF(APPLE)
  FIND_PATH(QUICKTIME_INCLUDE_DIR QuickTime/QuickTime.h)
  FIND_LIBRARY(QUICKTIME_LIBRARY QuickTime)
ELSE(APPLE)
  FIND_PATH(QUICKTIME_INCLUDE_DIR QuickTime.h
    $ENV{QUICKTIME_DIR}/include
    $ENV{QUICKTIME_DIR}
    NO_DEFAULT_PATH
  )
  FIND_PATH(QUICKTIME_INCLUDE_DIR QuickTime.h
    PATHS ${CMAKE_PREFIX_PATH} # Unofficial: We are proposing this.
    NO_DEFAULT_PATH
    PATH_SUFFIXES include
  )
  FIND_PATH(QUICKTIME_INCLUDE_DIR QuickTime.h)

  FIND_LIBRARY(QUICKTIME_LIBRARY QuickTime
    $ENV{QUICKTIME_DIR}/lib
    $ENV{QUICKTIME_DIR}
    NO_DEFAULT_PATH
  )
  FIND_LIBRARY(QUICKTIME_LIBRARY QuickTime
    PATHS ${CMAKE_PREFIX_PATH} # Unofficial: We are proposing this.
    NO_DEFAULT_PATH
    PATH_SUFFIXES lib64 lib
  )
  FIND_LIBRARY(QUICKTIME_LIBRARY QuickTime)
ENDIF(APPLE)

SET(QUICKTIME_FOUND "NO")
IF(QUICKTIME_LIBRARY AND QUICKTIME_INCLUDE_DIR)
  SET(QUICKTIME_FOUND "YES")
ENDIF(QUICKTIME_LIBRARY AND QUICKTIME_INCLUDE_DIR)


