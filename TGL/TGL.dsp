# Microsoft Developer Studio Project File - Name="TGL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=TGL - Win32 Debug_Memory
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "TGL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TGL.mak" CFG="TGL - Win32 Debug_Memory"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TGL - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TGL - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "TGL - Win32 Debug_Memory" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "TGL - Win32 Release"

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
# ADD CPP /nologo /MT /W3 /GX /O2 /I "C:\Brain\Library\SDL-1.2.11\include" /I "C:\Brain\Library\SDL_image-1.2.5\include" /I "C:\Brain\Library\SDL_mixer-1.2.7\include" /I "C:\Brain\Library\SDL_ttf-2.0.8\include" /I "src" /I "C:\Brain\Library\SDL_gfx\include" /I "C:\Brain\Library\pthreads-w32-2-7-0-release\Pre-built.2\include" /I "C:\Brain\Library\zlib-1.2.3" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "NDEBUG"
# ADD RSC /l 0xc0a /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SDL.lib SDLmain.lib SDL_image.lib SDL_mixer.lib SDL_ttf.lib opengl32.lib glu32.lib SDL_gfx.lib pthreadVSE2.lib zlib1.lib /nologo /subsystem:windows /machine:I386 /libpath:"C:\Brain\Library\zlib-1.2.3\bin" /libpath:"C:\Brain\Library\SDL-1.2.11\lib" /libpath:"C:\Brain\Library\SDL_image-1.2.5\lib" /libpath:"C:\Brain\Library\SDL_mixer-1.2.7\lib" /libpath:"C:\Brain\Library\SDL_ttf-2.0.8\lib" /libpath:"C:\Brain\Library\SDL_gfx\lib" /libpath:"C:\Brain\Library\pthreads-w32-2-7-0-release\Pre-built.2\lib"

!ELSEIF  "$(CFG)" == "TGL - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "C:\Brain\Library\SDL-1.2.11\include" /I "C:\Brain\Library\SDL_image-1.2.5\include" /I "C:\Brain\Library\SDL_mixer-1.2.7\include" /I "C:\Brain\Library\SDL_ttf-2.0.8\include" /I "src" /I "C:\Brain\Library\SDL_gfx\include" /I "C:\Brain\Library\pthreads-w32-2-7-0-release\Pre-built.2\include" /I "C:\Brain\Library\zlib-1.2.3" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 zlib1.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SDL.lib SDLmain.lib SDL_image.lib SDL_mixer.lib SDL_ttf.lib opengl32.lib glu32.lib SDL_gfx.lib pthreadVSE2.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Brain\Library\zlib-1.2.3\bin" /libpath:"C:\Brain\Library\SDL-1.2.11\lib" /libpath:"C:\Brain\Library\SDL_image-1.2.5\lib" /libpath:"C:\Brain\Library\SDL_mixer-1.2.7\lib" /libpath:"C:\Brain\Library\SDL_ttf-2.0.8\lib" /libpath:"C:\Brain\Library\SDL_gfx\lib" /libpath:"C:\Brain\Library\pthreads-w32-2-7-0-release\Pre-built.2\lib"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "TGL - Win32 Debug_Memory"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "TGL___Win32_Debug_Memory"
# PROP BASE Intermediate_Dir "TGL___Win32_Debug_Memory"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "TGL___Win32_Debug_Memory"
# PROP Intermediate_Dir "TGL___Win32_Debug_Memory"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I "C:\Brain\Library\SDL-1.2.11\include" /I "C:\Brain\Library\SDL_image-1.2.5\include" /I "C:\Brain\Library\SDL_mixer-1.2.7\include" /I "C:\Brain\Library\SDL_ttf-2.0.8\include" /I "src" /I "C:\Brain\Library\SDL_gfx\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "src\memory_manager" /I "C:\Brain\Library\SDL-1.2.11\include" /I "C:\Brain\Library\SDL_image-1.2.5\include" /I "C:\Brain\Library\SDL_mixer-1.2.7\include" /I "C:\Brain\Library\SDL_ttf-2.0.8\include" /I "src" /I "C:\Brain\Library\SDL_gfx\include" /I "C:\Brain\Library\pthreads-w32-2-7-0-release\Pre-built.2\include" /I "C:\Brain\Library\zlib-1.2.3" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "KITSCHY_DEBUG_MEMORY" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0xc0a /d "_DEBUG"
# ADD RSC /l 0xc0a /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SDL.lib SDLmain.lib SDL_image.lib SDL_mixer.lib SDL_ttf.lib opengl32.lib glu32.lib SDL_gfx.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Brain\Library\SDL-1.2.11\lib" /libpath:"C:\Brain\Library\SDL_image-1.2.5\lib" /libpath:"C:\Brain\Library\SDL_mixer-1.2.7\lib" /libpath:"C:\Brain\Library\SDL_ttf-2.0.8\lib" /libpath:"C:\Brain\Library\SDL_gfx\lib"
# SUBTRACT BASE LINK32 /profile
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib SDL.lib SDLmain.lib SDL_image.lib SDL_mixer.lib SDL_ttf.lib opengl32.lib glu32.lib SDL_gfx.lib pthreadVSE2.lib zlib1.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"C:\Brain\Library\zlib-1.2.3\bin" /libpath:"C:\Brain\Library\SDL-1.2.11\lib" /libpath:"C:\Brain\Library\SDL_image-1.2.5\lib" /libpath:"C:\Brain\Library\SDL_mixer-1.2.7\lib" /libpath:"C:\Brain\Library\SDL_ttf-2.0.8\lib" /libpath:"C:\Brain\Library\SDL_gfx\lib" /libpath:"C:\Brain\Library\pthreads-w32-2-7-0-release\Pre-built.2\lib"
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "TGL - Win32 Release"
# Name "TGL - Win32 Debug"
# Name "TGL - Win32 Debug_Memory"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\2DCMC.cpp
# End Source File
# Begin Source File

SOURCE=.\src\auxiliar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\collision.cpp
# End Source File
# Begin Source File

SOURCE=.\src\compression.cpp
# End Source File
# Begin Source File

SOURCE=.\src\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\src\memory_manager\debug_memorymanager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\fade_effects.cpp

!IF  "$(CFG)" == "TGL - Win32 Release"

!ELSEIF  "$(CFG)" == "TGL - Win32 Debug"

!ELSEIF  "$(CFG)" == "TGL - Win32 Debug_Memory"

# ADD CPP /I "src/memory_manager"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\geometrics.cpp
# End Source File
# Begin Source File

SOURCE=.\src\GLTile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\GLTManager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\keyboardstate.cpp
# End Source File
# Begin Source File

SOURCE=.\src\LevelPack.cpp
# End Source File
# Begin Source File

SOURCE=.\src\main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\PlayerProfile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\ranrotb.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SDL_glutaux.cpp
# End Source File
# Begin Source File

SOURCE=.\src\SFXManager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\sincos.cpp
# End Source File
# Begin Source File

SOURCE=.\src\sound.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Symbol.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGL.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLapp.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLinterface.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLinterfaceBrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLinterfaceConfirmation.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLinterfacePrint.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLmap.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLreplay.cpp
# End Source File
# Begin Source File

SOURCE=.\src\Vector.cpp
# End Source File
# Begin Source File

SOURCE=.\src\VirtualController.cpp
# End Source File
# Begin Source File

SOURCE=.\src\XMLparser.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\2DCMC.h
# End Source File
# Begin Source File

SOURCE=.\src\auxiliar.h
# End Source File
# Begin Source File

SOURCE=.\src\collision.h
# End Source File
# Begin Source File

SOURCE=.\src\compression.h
# End Source File
# Begin Source File

SOURCE=.\src\debug.h
# End Source File
# Begin Source File

SOURCE=.\src\memory_manager\debug_memorymanager.h
# End Source File
# Begin Source File

SOURCE=.\src\geometrics.h
# End Source File
# Begin Source File

SOURCE=.\src\GLTile.h
# End Source File
# Begin Source File

SOURCE=.\src\GLTManager.h
# End Source File
# Begin Source File

SOURCE=.\src\keyboardstate.h
# End Source File
# Begin Source File

SOURCE=.\src\LevelPack.h
# End Source File
# Begin Source File

SOURCE=.\src\List.h
# End Source File
# Begin Source File

SOURCE=.\src\PlayerProfile.h
# End Source File
# Begin Source File

SOURCE=.\src\randomc.h
# End Source File
# Begin Source File

SOURCE=.\src\SDL_glutaux.h
# End Source File
# Begin Source File

SOURCE=.\src\SFXManager.h
# End Source File
# Begin Source File

SOURCE=.\src\sincos.h
# End Source File
# Begin Source File

SOURCE=.\src\sound.h
# End Source File
# Begin Source File

SOURCE=.\src\Symbol.h
# End Source File
# Begin Source File

SOURCE=.\src\TGL.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLapp.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLinterface.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLmap.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLreplay.h
# End Source File
# Begin Source File

SOURCE=.\src\Vector.h
# End Source File
# Begin Source File

SOURCE=.\src\VirtualController.h
# End Source File
# Begin Source File

SOURCE=.\src\XMLparser.h
# End Source File
# End Group
# Begin Group "objects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\TGLobject_ball.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ball.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ballstand.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ballstand.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_big_tank.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_big_tank.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_big_tank_cannon.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_big_tank_cannon.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_bullet.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_bullet.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_bullet_hmissile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_bullet_hmissile.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_bullet_missile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_bullet_missile.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_button.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_button.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_cannon_down.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_cannon_down.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_cannon_left.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_cannon_left.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_cannon_right.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_cannon_right.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_cannon_up.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_cannon_up.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_directionalcannon_down.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_directionalcannon_down.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_directionalcannon_left.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_directionalcannon_left.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_directionalcannon_right.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_directionalcannon_right.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_directionalcannon_up.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_directionalcannon_up.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_enemy.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_enemy.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_fastcannon_down.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_fastcannon_down.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_fastcannon_left.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_fastcannon_left.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_fastcannon_right.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_fastcannon_right.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_fastcannon_up.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_fastcannon_up.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_fuelrecharge.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_fuelrecharge.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ha_directionalcannon_down.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ha_directionalcannon_down.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ha_directionalcannon_left.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ha_directionalcannon_left.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ha_directionalcannon_right.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ha_directionalcannon_right.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ha_directionalcannon_up.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ha_directionalcannon_up.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_laser_down.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_laser_down.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_laser_horizontal.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_laser_horizontal.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_laser_left.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_laser_left.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_laser_right.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_laser_right.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_laser_up.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_laser_up.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_laser_vertical.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_laser_vertical.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_leftdoor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_leftdoor.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_pipehscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_pipehscreen.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_pipevscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_pipevscreen.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_radar.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_radar.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_redlight.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_redlight.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_rightdoor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_rightdoor.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_spike_left.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_spike_left.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_spike_right.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_spike_right.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_tank.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_tank.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_tank_cannon.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_tank_cannon.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_tank_turret.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_tank_turret.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_techno_computer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_techno_computer.h
# End Source File
# End Group
# Begin Group "ships"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\TGLobject_ship.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_accura.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_accura.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_c_harpoon.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_c_harpoon.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_chain.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_chain.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_dflecter.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_dflecter.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_dodger.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_dodger.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_grabber.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_grabber.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_gravis.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_gravis.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_gyrus.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_gyrus.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_nitroblaster.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_nitroblaster.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_shadowrunner.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_shadowrunner.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_shield.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_shield.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_vipperbeam.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_vipperbeam.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_vpanther.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_vpanther.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_xterminator.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_ship_xterminator.h
# End Source File
# End Group
# Begin Group "effects"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\TGLobject_FX_explosion1.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_FX_explosion1.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_FX_explosion2.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_FX_explosion2.h
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_FX_particle.cpp
# End Source File
# Begin Source File

SOURCE=.\src\TGLobject_FX_particle.h
# End Source File
# End Group
# Begin Group "game states"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\state_configure.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_editor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_game.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_highscores.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_highscores_times.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_intro.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_levelpackbrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_levelpackscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_mainmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_mapeditor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_playerprofile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_postgame.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_pregame.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_replaybrowser.cpp
# End Source File
# Begin Source File

SOURCE=.\src\state_savereplay.cpp
# End Source File
# End Group
# Begin Source File

SOURCE="..\transball-to-do.txt"
# End Source File
# End Target
# End Project
