# Microsoft Developer Studio Project File - Name="mapeditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=mapeditor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "mapeditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "mapeditor.mak" CFG="mapeditor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mapeditor - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "mapeditor - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "mapeditor - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "C:\Brain\Library\SDL-1.2.10\include" /I "C:\Brain\Library\SDL_mixer-1.2.6\include" /I "C:\Brain\Library\SDL_image-1.2.4\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SDL.lib SDLmain.lib SDL_mixer.lib SDL_image.lib opengl32.lib glu32.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\Brain\Library\SDL-1.2.10\lib" /libpath:"C:\Brain\Library\SDL_mixer-1.2.6\lib" /libpath:"C:\Brain\Library\SDL_image-1.2.4\lib"

!ELSEIF  "$(CFG)" == "mapeditor - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "C:\Brain\Library\SDL-1.2.11\include" /I "C:\Brain\Library\SDL_mixer-1.2.7\include" /I "C:\Brain\Library\SDL_image-1.2.5\include" /I "C:\Brain\Library\SDL_ttf-2.0.8\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SDL.lib SDLmain.lib SDL_mixer.lib SDL_image.lib opengl32.lib glu32.lib SDL_ttf.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Brain\Library\SDL-1.2.11\lib" /libpath:"C:\Brain\Library\SDL_mixer-1.2.7\lib" /libpath:"C:\Brain\Library\SDL_image-1.2.5\lib" /libpath:"C:\Brain\Library\SDL_ttf-2.0.8\lib"

!ENDIF 

# Begin Target

# Name "mapeditor - Win32 Release"
# Name "mapeditor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\2DCMC.cpp
# End Source File
# Begin Source File

SOURCE=.\auxiliar.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\geometrics.cpp
# End Source File
# Begin Source File

SOURCE=.\GLTile.cpp
# End Source File
# Begin Source File

SOURCE=.\GLTManager.cpp
# End Source File
# Begin Source File

SOURCE=.\keyboardstate.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# Begin Source File

SOURCE=.\ranrotb.cpp
# End Source File
# Begin Source File

SOURCE=.\SDL_glutaux.cpp
# End Source File
# Begin Source File

SOURCE=.\SFXManager.cpp
# End Source File
# Begin Source File

SOURCE=.\sound.cpp
# End Source File
# Begin Source File

SOURCE=.\ST2_importer.cpp
# End Source File
# Begin Source File

SOURCE=.\Symbol.cpp
# End Source File
# Begin Source File

SOURCE=.\TGLmap.cpp
# End Source File
# Begin Source File

SOURCE=.\TGLmapeditor.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\2DCMC.h
# End Source File
# Begin Source File

SOURCE=.\auxiliar.h
# End Source File
# Begin Source File

SOURCE=.\debug.h
# End Source File
# Begin Source File

SOURCE=.\geometrics.h
# End Source File
# Begin Source File

SOURCE=.\GLTile.h
# End Source File
# Begin Source File

SOURCE=.\GLTManager.h
# End Source File
# Begin Source File

SOURCE=.\keyboardstate.h
# End Source File
# Begin Source File

SOURCE=.\List.h
# End Source File
# Begin Source File

SOURCE=.\randomc.h
# End Source File
# Begin Source File

SOURCE=.\SDL_glutaux.h
# End Source File
# Begin Source File

SOURCE=.\SFXManager.h
# End Source File
# Begin Source File

SOURCE=.\sound.h
# End Source File
# Begin Source File

SOURCE=.\Symbol.h
# End Source File
# Begin Source File

SOURCE=.\TGLmap.h
# End Source File
# Begin Source File

SOURCE=.\TGLmapeditor.h
# End Source File
# Begin Source File

SOURCE=.\Vector.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Source File

SOURCE=".\to-do.txt"
# End Source File
# End Target
# End Project
