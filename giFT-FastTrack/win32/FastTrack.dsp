# Microsoft Developer Studio Project File - Name="FastTrack" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=FastTrack - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "FastTrack.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "FastTrack.mak" CFG="FastTrack - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "FastTrack - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "FastTrack - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FASTTRACK_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FASTTRACK_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

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
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FASTTRACK_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\\" /I "..\crypt" /I "..\..\lib" /I "..\..\src" /I "..\..\plugin" /I "..\..\win32" /D "HAVE_CONFIG_H" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "FASTTRACK_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 libgiFT.lib ws2_32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "FastTrack - Win32 Release"
# Name "FastTrack - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\fst_download.c
# End Source File
# Begin Source File

SOURCE=..\fst_fasttrack.c
# End Source File
# Begin Source File

SOURCE=..\fst_hash.c
# End Source File
# Begin Source File

SOURCE=..\fst_http.c
# End Source File
# Begin Source File

SOURCE=..\fst_meta.c
# End Source File
# Begin Source File

SOURCE=..\fst_node.c
# End Source File
# Begin Source File

SOURCE=..\fst_packet.c
# End Source File
# Begin Source File

SOURCE=..\fst_search.c
# End Source File
# Begin Source File

SOURCE=..\fst_session.c
# End Source File
# Begin Source File

SOURCE=..\fst_stats.c
# End Source File
# Begin Source File

SOURCE=..\fst_utils.c
# End Source File
# Begin Source File

SOURCE=..\md5.c
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\fst_download.h
# End Source File
# Begin Source File

SOURCE=..\fst_fasttrack.h
# End Source File
# Begin Source File

SOURCE=..\fst_hash.h
# End Source File
# Begin Source File

SOURCE=..\fst_http.h
# End Source File
# Begin Source File

SOURCE=..\fst_meta.h
# End Source File
# Begin Source File

SOURCE=..\fst_node.h
# End Source File
# Begin Source File

SOURCE=..\fst_packet.h
# End Source File
# Begin Source File

SOURCE=..\fst_search.h
# End Source File
# Begin Source File

SOURCE=..\fst_session.h
# End Source File
# Begin Source File

SOURCE=..\fst_stats.h
# End Source File
# Begin Source File

SOURCE=..\fst_utils.h
# End Source File
# Begin Source File

SOURCE=..\md5.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "crypt"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\crypt\enc_type_1.c
# End Source File
# Begin Source File

SOURCE=..\crypt\enc_type_2.c
# End Source File
# Begin Source File

SOURCE=..\crypt\enc_type_20.c
# End Source File
# Begin Source File

SOURCE=..\crypt\fst_crypt.c
# End Source File
# Begin Source File

SOURCE=..\crypt\fst_crypt.h
# End Source File
# End Group
# Begin Group "misc"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\CHANGES
# End Source File
# Begin Source File

SOURCE=..\Makefile
# End Source File
# Begin Source File

SOURCE=..\README
# End Source File
# Begin Source File

SOURCE=.\README.win32
# End Source File
# Begin Source File

SOURCE=..\TODO
# End Source File
# End Group
# End Target
# End Project
