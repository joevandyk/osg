# Microsoft Developer Studio Project File - Name="Core osgDB" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102
# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Core osgDB - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "osgDB.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "osgDB.mak" CFG="Core osgDB - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Core osgDB - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Core osgDB - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Core osgDB - Win32 Release Static" (based on "Win32 (x86) Static Library")
!MESSAGE "Core osgDB - Win32 Debug Static" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Core osgDB - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../../lib"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../bin/$(PlatformName)"
# PROP Intermediate_Dir "$(PlatformName)/$(ConfigurationName)"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "../../include" /I "../../../OpenThreads/include" /I "../../../3rdParty/include" /D "NDEBUG" /D "_MBCS" /D "_USRDLL" /D "OSGDB_LIBRARY" /D "WIN32" /D "_WINDOWS" /D "_CRT_SECURE_NO_DEPRECATE" /YX /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 OpenThreadsWin32.lib opengl32.lib glu32.lib /nologo /dll /debug /machine:I386 /opt:ref /opt:icf /out:"$(OutDir)/osgDB.dll" /implib:"../../lib/$(PlatformName)/osgDB.lib" /libpath:"../../lib/$(PlatformName)" /libpath:"../../../OpenThreads/lib/$(PlatformName)" /libpath:"../../../3rdParty/lib/$(PlatformName)" /libpath:"../../../3rdParty/lib"

!ELSEIF  "$(CFG)" == "Core osgDB - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../bin/$(PlatformName)"
# PROP Intermediate_Dir "$(PlatformName)/$(ConfigurationName)"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /vmg /GR /GX /Zi /Od /I "../../include" /I "../../../OpenThreads/include" /I "../../../3rdParty/include" /D "OSGDB_LIBRARY" /D "_WINDOWS" /D "WIN32" /D "_DEBUG" /D "_CRT_SECURE_NO_DEPRECATE" /YX /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 OpenThreadsWin32d.lib opengl32.lib glu32.lib /nologo /dll /debug /machine:I386 /out:"$(OutDir)/osgDBd.dll" /pdbtype:sept /implib:"../../lib/$(PlatformName)/osgDBd.lib" /libpath:"../../lib/$(PlatformName)" /libpath:"../../../OpenThreads/lib/$(PlatformName)" /libpath:"../../../3rdParty/lib/$(PlatformName)" /libpath:"../../../3rdParty/lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Core osgDB - Win32 Release Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../../lib"
# PROP BASE Intermediate_Dir "Release_Static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../../lib/$(PlatformName)"
# PROP Intermediate_Dir "$(PlatformName)/$(ConfigurationName)_Static"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "OSG_LIBRARY_STATIC" /D "OT_LIBRARY_STATIC" /D "PR_LIBRARY_STATIC" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "../../include" /I "../../../OpenThreads/include" /I "../../../3rdParty/include" /D "NDEBUG" /D "_MBCS" /D "OSG_LIBRARY_STATIC" /D "OT_LIBRARY_STATIC" /D "PR_LIBRARY_STATIC" /D "WIN32" /D "_WINDOWS" /D "_CRT_SECURE_NO_DEPRECATE" /YX /FD /Zm200 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nodefaultlib /nologo /out:"$(OutDir)/osgDB_s.lib"

!ELSEIF  "$(CFG)" == "Core osgDB - Win32 Debug Static"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_Static"
# PROP BASE Intermediate_Dir "Debug_Static"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../../lib/$(PlatformName)"
# PROP Intermediate_Dir "$(PlatformName)/$(ConfigurationName)_Static"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
F90=df.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Z7 /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "OSG_LIBRARY_STATIC" /D "OT_LIBRARY_STATIC" /D "PR_LIBRARY_STATIC" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /vmg /GR /GX /Z7 /Od /I "../../include" /I "../../../OpenThreads/include" /I "../../../3rdParty/include" /D "OSG_LIBRARY_STATIC" /D "OT_LIBRARY_STATIC" /D "PR_LIBRARY_STATIC" /D "_WINDOWS" /D "WIN32" /D "_DEBUG" /D "_CRT_SECURE_NO_DEPRECATE" /YX /FD /GZ /Zm200 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nodefaultlib /nologo /out:"$(OutDir)/osgDBd_s.lib"
# SUBTRACT LIB32 /pdb:none

!ENDIF 

# Begin Target

# Name "Core osgDB - Win32 Release"
# Name "Core osgDB - Win32 Debug"
# Name "Core osgDB - Win32 Release Static"
# Name "Core osgDB - Win32 Debug Static"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\src\osgDB\Archive.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgDB\DatabasePager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\DotOsgWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\DynamicLibrary.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\Field.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\FieldReader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\FieldReaderIterator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\FileNameUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\FileUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\ImageOptions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\Input.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\Output.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\ReaderWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\ReadFile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\Registry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\src\osgDB\SharedStateManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\Version.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Src\osgDB\WriteFile.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter ";h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\Include\osgDB\Archive
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\DatabasePager
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\DotOsgWrapper
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\DynamicLibrary
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\Export
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\Field
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\FieldReader
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\FieldReaderIterator
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\FileNameUtils
# End Source File
# Begin Source File

SOURCE=..\..\include\osgDB\FileUtils
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\ImageOptions
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\Input
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\Output
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\ParameterOutput
# End Source File
# Begin Source File

SOURCE=..\..\include\osgDB\ReaderWriter
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\ReadFile
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\Registry
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\SharedStateManager
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\Version
# End Source File
# Begin Source File

SOURCE=..\..\Include\osgDB\WriteFile
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
