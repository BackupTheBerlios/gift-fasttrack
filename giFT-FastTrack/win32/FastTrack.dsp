# Microsoft Developer Studio Project File - Name="FastTrack" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=FastTrack - Win32 Debug
!MESSAGE Dies ist kein g�ltiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und f�hren Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "FastTrack.mak".
!MESSAGE 
!MESSAGE Sie k�nnen beim Ausf�hren von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "FastTrack.mak" CFG="FastTrack - Win32 Debug"
!MESSAGE 
!MESSAGE F�r die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "FastTrack - Win32 Release" (basierend auf  "Win32 (x86) Static Library")
!MESSAGE "FastTrack - Win32 Debug" (basierend auf  "Win32 (x86) Static Library")
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
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "FastTrack - Win32 Debug"

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
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
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
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\crypt\enc_type_1.c
# End Source File
# Begin Source File

SOURCE=..\src\crypt\enc_type_2.c
# End Source File
# Begin Source File

SOURCE=..\src\crypt\enc_type_20.c
# End Source File
# Begin Source File

SOURCE=..\src\crypt\enc_type_80.c
# End Source File
# Begin Source File

SOURCE=..\src\crypt\fst_crypt.c
# End Source File
# Begin Source File

SOURCE=..\src\fst_download.c
# End Source File
# Begin Source File

SOURCE=..\src\fst_fasttrack.c
# End Source File
# Begin Source File

SOURCE=..\src\fst_hash.c
# End Source File
# Begin Source File

SOURCE=..\src\fst_http_client.c
# End Source File
# Begin Source File

SOURCE=..\src\fst_http_header.c
# End Source File
# Begin Source File

SOURCE=..\src\fst_meta.c
# End Source File
# Begin Source File

SOURCE=..\src\fst_node.c
# End Source File
# Begin Source File

SOURCE=..\src\fst_packet.c
# End Source File
# Begin Source File

SOURCE=..\src\fst_search.c
# End Source File
# Begin Source File

SOURCE=..\src\fst_session.c
# End Source File
# Begin Source File

SOURCE=..\src\fst_stats.c
# End Source File
# Begin Source File

SOURCE=..\src\fst_utils.c
# End Source File
# Begin Source File

SOURCE=..\src\md5.c
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\crypt\fst_crypt.h
# End Source File
# Begin Source File

SOURCE=..\src\fst_download.h
# End Source File
# Begin Source File

SOURCE=..\src\fst_fasttrack.h
# End Source File
# Begin Source File

SOURCE=..\src\fst_hash.h
# End Source File
# Begin Source File

SOURCE=..\src\fst_http_client.h
# End Source File
# Begin Source File

SOURCE=..\src\fst_http_header.h
# End Source File
# Begin Source File

SOURCE=..\src\fst_meta.h
# End Source File
# Begin Source File

SOURCE=..\src\fst_node.h
# End Source File
# Begin Source File

SOURCE=..\src\fst_packet.h
# End Source File
# Begin Source File

SOURCE=..\src\fst_search.h
# End Source File
# Begin Source File

SOURCE=..\src\fst_session.h
# End Source File
# Begin Source File

SOURCE=..\src\fst_stats.h
# End Source File
# Begin Source File

SOURCE=..\src\fst_utils.h
# End Source File
# Begin Source File

SOURCE=..\src\md5.h
# End Source File
# End Group
# End Target
# End Project
