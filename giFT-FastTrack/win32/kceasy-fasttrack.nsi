;------------------------------------------------------------------------------------------
; This file creates an installer for gift-fasttrack for use with KCeasy. It assumes
; gift-fasttrack was build using the environment provided by giFT.
;------------------------------------------------------------------------------------------

; Compile time options
;------------------------------------------------------------------------------------------

!define VERSION "0.8.8"

!define OUTFILE "kceasy-fasttrack-${VERSION}.exe"

!include "MUI.nsh"

# we are supposed to be in /win32-dist
!ifndef BUILD_ROOT
!define BUILD_ROOT ".."
!endif

!ifndef DIST_DIR
!define DIST_DIR      "${BUILD_ROOT}\win32-dist"
!endif

!ifndef TMP_DIR
!define TMP_DIR       "${DIST_DIR}\tmp"
!endif


; Installer options
;------------------------------------------------------------------------------------------

Name "FastTrack plugin for KCeasy"
Caption "KCeasy FastTrack plugin ${VERSION} Setup"
OutFile ${OUTFILE}

InstallDir "$PROGRAMFILES\KCeasy"
InstallDirRegKey HKEY_LOCAL_MACHINE "Software\KCeasy" "InstallPath"

SetCompressor lzma
SetOverwrite on
ShowInstDetails hide

InstType "Default"

; Modern UI config
;------------------------------------------------------------------------------------------

!define MUI_COMPONENTSPAGE_SMALLDESC
!define MUI_ABORTWARNING
!define MUI_FINISHPAGE_NOREBOOTSUPPORT

!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_LANGUAGE "English"


; Utils
;------------------------------------------------------------------------------------------

; StrStr
; input, top of stack = string to search for
;        top of stack-1 = string to search in
; output, top of stack (replaces with the portion of the string remaining)
; modifies no other variables.
;
; Usage:
;   Push "this is a long ass string"
;   Push "ass"
;   Call StrStr
;   Pop $R0
;  ($R0 at this point is "ass string")

Function StrStr
  Exch $R1 ; st=haystack,old$R1, $R1=needle
  Exch    ; st=old$R1,haystack
  Exch $R2 ; st=old$R1,old$R2, $R2=haystack
  Push $R3
  Push $R4
  Push $R5
  StrLen $R3 $R1
  StrCpy $R4 0
  ; $R1=needle
  ; $R2=haystack
  ; $R3=len(needle)
  ; $R4=cnt
  ; $R5=tmp
  loop:
    StrCpy $R5 $R2 $R3 $R4
    StrCmp $R5 $R1 done
    StrCmp $R5 "" done
    IntOp $R4 $R4 + 1
    Goto loop
  done:
  StrCpy $R1 $R2 "" $R4
  Pop $R5
  Pop $R4
  Pop $R3
  Pop $R2
  Exch $R1
FunctionEnd

; Installer Sections
;------------------------------------------------------------------------------------------

Section "gift-fasttrack ${VERSION}" SEC_FASTTRACK
	SectionIn 1 RO

	SetOutPath $INSTDIR\giFT\plugins
	File FastTrack.dll

	SetOutPath $INSTDIR\giFT\conf\FastTrack
	File ${TMP_DIR}\FastTrack.conf
	File ${DIST_DIR}\data\FastTrack\nodes
	File ${DIST_DIR}\data\FastTrack\banlist

	; update giftd.conf
	ReadINIStr $R1 $INSTDIR\giFT\conf\giftd.conf "main" "plugins"
	IfErrors ConfError

	; add ":FastTrack" to plugin line if not present
	Push $R1
	Push "FastTrack"
	Call StrStr
	Pop $R0
	StrCmp $R0 "" 0 ConfError
	WriteINIStr $INSTDIR\giFT\conf\giftd.conf "main" "plugins" "$R1:FastTrack"

ConfError:

	; get installer filename in $R0
	System::Call 'kernel32::GetModuleFileNameA(i 0, t .R0, i 1024) i r1'
	; copy the installer into 'My Shared folder' if not already there
	IfFileExists "$INSTDIR\My Shared Folder\${OUTFILE}" InstallerAlreadyShared
	CopyFiles /SILENT /FILESONLY $R0 "$INSTDIR\My Shared Folder\${OUTFILE}" 2480
InstallerAlreadyShared:


SectionEnd

