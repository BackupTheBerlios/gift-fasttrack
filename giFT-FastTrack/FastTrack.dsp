# Microsoft Developer Studio Project File - Name="FastTrack" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=FastTrack - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "FastTrack.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "FastTrack.mak" CFG="FastTrack - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "FastTrack - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "FastTrack - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "FastTrack - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "FastTrack_Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /GX /O2 /I "../win32" /I "../FastTrack/src" /I "../FastTrack/src/crypt" /I "../lib" /I "../src" /I "../OpenFT" /I "../plugin" /I "../Gnutella" /I ".." /D "NDEBUG" /D "_LIB" /D "PLUGIN_FASTTRACK" /D "PLUGIN_OPENFT" /D "PLUGIN_GNUTELLA" /D "HAVE_CONFIG_H" /D "WIN32" /D "_MBCS" /D "_WINDOWS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "FastTrack - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "FastTrack___Win32_Debug"
# PROP BASE Intermediate_Dir "FastTrack___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "FastTrack_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../win32" /I "../FastTrack/src" /I "../FastTrack/src/crypt" /I "../lib" /I "../src" /I "../OpenFT" /I "../plugin" /I "../Gnutella" /I ".." /D "_DEBUG" /D "_LIB" /D "PLUGIN_FASTTRACK" /D "PLUGIN_OPENFT" /D "PLUGIN_GNUTELLA" /D "HAVE_CONFIG_H" /D "WIN32" /D "_MBCS" /D "_WINDOWS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "FastTrack - Win32 Release"
# Name "FastTrack - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\FastTrack\src\crypt\enc_type_1.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\crypt\enc_type_2.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\crypt\enc_type_20.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\crypt\fst_crypt.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_download.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_fasttrack.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_hash.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_http.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_meta.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_node.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_packet.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_search.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_session.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_stats.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_utils.c
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\md5.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\FastTrack\src\crypt\fst_crypt.h
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_download.h
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_fasttrack.h
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_hash.h
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_http.h
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_meta.h
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_node.h
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_packet.h
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_search.h
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_session.h
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_stats.h
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\fst_utils.h
# End Source File
# Begin Source File

SOURCE=..\FastTrack\src\md5.h
# End Source File
# End Group
# End Target
# End Project
