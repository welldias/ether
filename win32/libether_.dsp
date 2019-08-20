# Microsoft Developer Studio Project File - Name="libether" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libether - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libether.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libether.mak" CFG="libether - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libether - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libether - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libether - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "..\src\lib" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x416 /d "NDEBUG"
# ADD RSC /l 0x416 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "libether - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "..\src\lib" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x416 /d "_DEBUG"
# ADD RSC /l 0x416 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "libether - Win32 Release"
# Name "libether - Win32 Debug"
# Begin Source File

SOURCE=..\src\lib\Ether.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_application.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_application.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_camera.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_camera.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_display.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_display.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_drivers.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_facet.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_facet.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_fig.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_fig.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_light.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_light.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_math.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_math.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_matrix.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_matrix.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_memory.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_object.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_object.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_palette.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_palette.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_plg.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_plg.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_primitives.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_primitives.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_private.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_raster.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_raster.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_render.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_render.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_rep.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_rep.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_shape.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_shape.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_statistics.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_statistics.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_surface.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_surface.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_system.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_system.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_task.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_task.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_timer.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_timer.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_transform.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_vector.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_vector.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_wld.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_wld.h
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_world.c
# End Source File
# Begin Source File

SOURCE=..\src\lib\ether_world.h
# End Source File
# End Target
# End Project
