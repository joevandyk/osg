# Microsoft Developer Studio Project File - Name="osgPlugin 3ds" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=osgPlugin 3ds - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "lib3ds.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "lib3ds.mak" CFG="osgPlugin 3ds - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "osgPlugin 3ds - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "osgPlugin 3ds - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "osgPlugin 3ds - Win32 Release Static" (based on "Win32 (x86) Static Library")
!MESSAGE "osgPlugin 3ds - Win32 Debug Static" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "osgPlugin 3ds - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../../../lib"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../lib"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../../include" /I "../../../../OpenThreads/include" /I "../../../../Producer/include" /I "../../../../3rdParty/include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 OpenThreadsWin32.lib /nologo /dll /pdb:none /machine:I386 /nodefaultlib:"LIBC" /out:"../../../bin/osgdb_3ds.dll" /libpath:"../../../lib" /libpath:"../../../../OpenThreads/lib/win32" /libpath:"../../../../Producer/lib" /libpath:"../../../../3rdParty/lib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "osgPlugin 3ds - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../../../lib"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /vmg /GR /GX /Zi /Od /I "../../../include" /I "../../../../OpenThreads/include" /I "../../../../Producer/include" /I "../../../../3rdParty/include" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "WIN32" /D "_DEBUG" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 OpenThreadsWin32d.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"LIBC" /out:"../../../bin/osgdb_3dsd.dll" /pdbtype:sept /libpath:"../../../lib" /libpath:"../../../../OpenThreads/lib/win32" /libpath:"../../../../Producer/lib" /libpath:"../../../../3rdParty/lib"
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "osgPlugin 3ds - Win32 Release Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../../../lib"
# PROP BASE Intermediate_Dir "Release_Static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../../lib"
# PROP Intermediate_Dir "Release_Static"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "OSG_LIBRARY_STATIC" /D "OT_LIBRARY_STATIC" /D "PR_LIBRARY_STATIC" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "../../../include" /I "../../../../OpenThreads/include" /I "../../../../Producer/include" /I "../../../../3rdParty/include" /D "WIN32" /D "OSG_LIBRARY_STATIC" /D "OT_LIBRARY_STATIC" /D "PR_LIBRARY_STATIC" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nodefaultlib /nologo /out:"../../../lib/osgdb_3ds_s.lib"
# SUBTRACT LIB32 /nodefaultlib

!ELSEIF  "$(CFG)" == "osgPlugin 3ds - Win32 Debug Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../../../lib"
# PROP BASE Intermediate_Dir "Debug_Static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../../lib"
# PROP Intermediate_Dir "Debug_Static"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "OSG_LIBRARY_STATIC" /D "OT_LIBRARY_STATIC" /D "PR_LIBRARY_STATIC" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /vmg /GR /GX /Z7 /Od /I "../../../include" /I "../../../../OpenThreads/include" /I "../../../../Producer/include" /I "../../../../3rdParty/include" /D "_WINDOWS" /D "OSG_LIBRARY_STATIC" /D "OT_LIBRARY_STATIC" /D "PR_LIBRARY_STATIC" /D "_MBCS" /D "WIN32" /D "_DEBUG" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nodefaultlib /nologo /out:"../../../lib/osgdb_3dsd_s.lib"
# SUBTRACT LIB32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "osgPlugin 3ds - Win32 Release"
# Name "osgPlugin 3ds - Win32 Debug"
# Name "osgPlugin 3ds - Win32 Release Static"
# Name "osgPlugin 3ds - Win32 Debug Static"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\atmosphere.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\background.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Camera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Chunk.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Ease.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\File.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Lib3ds_float.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Light.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Material.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Matrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Mesh.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Node.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Quat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\ReaderWriter3DS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\readwrite.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Shadow.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Tcb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Tracks.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Vector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Viewport.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\atmosphere.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\background.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Camera.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Chunk.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\chunktable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Ease.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\File.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Lib3ds_float.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Light.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Material.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Matrix.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Mesh.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Node.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Quat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\readwrite.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Shadow.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Tcb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Tracks.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Types.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Vector.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Src\osgPlugins\Lib3ds\Viewport.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
