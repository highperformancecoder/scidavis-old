;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  File                 : scidavisScriptInstall.nsi
;  Project              : SciDAVis
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Copyright            : (C) 2008 by Mauricio Troviano
;  Email (use @ for *)  : m.troviano*gmail.com
;  Description          : Script for the nullsoft scriptable installation system
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                                                         ;
;  This program is free software; you can redistribute it and/or modify   ;
;  it under the terms of the GNU General Public License as published by   ;
;  the Free Software Foundation; either version 2 of the License, or      ;
;  (at your option) any later version.                                    ;
;                                                                         ;
;  This program is distributed in the hope that it will be useful,        ;
;  but WITHOUT ANY WARRANTY; without even the implied warranty of         ;
;  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          ;
;  GNU General Public License for more details.                           ;
;                                                                         ;
;   You should have received a copy of the GNU General Public License     ;
;   along with this program; if not, write to the Free Software           ;
;   Foundation, Inc., 51 Franklin Street, Fifth Floor,                    ;
;   Boston, MA  02110-1301  USA                                           ;
;                                                                         ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "SciDaVis"
!define PRODUCT_VERSION "0.1.3"
!define PRODUCT_WEB_SITE "http://scidavis.sourceforge.net/"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\scidavis.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"
!define PYTHON_URL "http://www.python.org/ftp/python/2.5.2/python-2.5.2.msi"
!define PYTHON_INSTALLER "python-2.5.2.msi"
!define PYQT_URL "http://www.riverbankcomputing.com/static/Downloads/PyQt4/PyQt-Py2.5-gpl-4.3.3-2.exe"
!define PYQT_INSTALLER "PyQt-Py2.5-gpl-4.3.3-2.exe"

SetCompressor /SOLID lzma

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "scidavis.ico"
;!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"
!define MUI_UNICON "scidavis.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "gpl.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "SciDaVis"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\scidavis.exe"
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\README.txt"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "scidavis-${PRODUCT_VERSION}-setup.exe"
InstallDir "$PROGRAMFILES\SciDaVis"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Section "Select components" SEC01
  SetOutPath "$INSTDIR"
  CreateDirectory $INSTDIR\plugins
  SetOverwrite try

  File "CHANGES.txt"
  File "gpl.txt"
  File "mingwm10.dll"
  File "Qt3Support4.dll"
  File "QtAssistantClient4.dll"
  File "QtCore4.dll"
  File "QtGui4.dll"
  File "QtNetwork4.dll"
  File "QtOpenGL4.dll"
  File "QtSql4.dll"
  File "QtSvg4.dll"
  File "QtXml4.dll"
  File "qwtplot3d.dll"
  File "README.txt"
  File "scidavis.ico"
  File "scidavis.exe"
  File "scidavisrc.py"
  File "scidavisrc.pyc"
  File "scidavisUtil.py"
  SetOutPath "$INSTDIR\plugins"
  File "plugins\fitRational0.dll"
  File "plugins\fitRational1.dll"
; Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\SciDaVis.lnk" "$INSTDIR\scidavis.exe"
  CreateShortCut "$DESKTOP\SciDaVis.lnk" "$INSTDIR\scidavis.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -AdditionalIcons
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\scidavis.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\scidavis.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\scidavisUtil.py"
  Delete "$INSTDIR\scidavisrc.pyc"
  Delete "$INSTDIR\scidavisrc.py"
  Delete "$INSTDIR\scidavis.exe"
  Delete "$INSTDIR\scidavis.ico"
  Delete "$INSTDIR\README.txt"
  Delete "$INSTDIR\qwtplot3d.dll"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\QtSvg4.dll"
  Delete "$INSTDIR\QtSql4.dll"
  Delete "$INSTDIR\QtOpenGL4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtAssistantClient4.dll"
  Delete "$INSTDIR\Qt3Support4.dll"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\gpl.txt"
  Delete "$INSTDIR\CHANGES.txt"
  Delete "$INSTDIR\plugins\fitRational1.dll"
  Delete "$INSTDIR\plugins\fitRational0.dll"

  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk"
  Delete "$DESKTOP\SciDaVis.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\SciDaVis.lnk"

  RMDir "$SMPROGRAMS\$ICONS_GROUP"
  RMDir "$INSTDIR\plugins"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd

; Ask whether to install Python and PyQt
Section -Requirements
  SetOutPath $INSTDIR
  ReadRegStr $3 HKLM "SOFTWARE\Python\PythonCore\2.5\InstallPath" ""
  StrCmp $3 "" 0 pythonInstFound
  StrCpy $4 "No Python 2.5 found on your system. Install Python 2.5 from the internet?"
  Goto skip1
  pythonInstFound:
  StrCpy $4 "Python 2.5 found on your system. Install Python 2.5 from the internet nevertheless?"
  skip1:
  MessageBox MB_YESNO $4 /SD IDYES IDNO endInstPython
        StrCpy $2 "$TEMP\${PYTHON_INSTALLER}"
        nsisdl::download /TIMEOUT=30000 ${PYTHON_URL} $2
        Pop $R0 ;Get the return value
                StrCmp $R0 "success" +3
                MessageBox MB_OK "Download failed: $R0"
                Quit
	ExecWait '"msiexec" /i "$2"'
        Delete $2
    Goto endInstPython
  endInstPython:

;;; Install PyQt and SIP provided with the installer.
;;; The PyQt_SIP.exe install was build as follows:
;;; - compile and install PyQt and SIP in some arbitrary dir
;;; - pack contents if this dir
;;; - use NSIS to make an installer out of the ZIP
  ReadRegStr $2 HKLM "SOFTWARE\Python\PythonCore\2.5\InstallPath" ""
  StrCmp $2 "" 0 pythonFound
    MessageBox MB_OK "Could not find Python 2.5 installation (required). Aborting."
    Quit
  pythonFound:
    MessageBox MB_YESNO "Python found in $2. Install PyQt and SIP provided with SciDAVis now? (Recommended. PyQt binaries from Riverbank are reported to be incompatible with SciDAVis.)" /SD IDYES IDNO endExtraInst
    SetOutPath "$2"
    File "PyQt_SIP.exe"
    ExecWait "$2\PyQt_SIP.exe"
    Delete "$2\PyQt_SIP.exe"

;;; Downloading and Installing PyQt works, but SciDAVis crashed using this binary.
;  MessageBox MB_YESNO "Install PyQt (requires active internet connection)?" /SD IDYES IDNO endInstPyQt
;        StrCpy $2 "$TEMP\${PYQT_INSTALLER}"
;        nsisdl::download /TIMEOUT=30000 ${PYQT_URL} $2
;        Pop $R0 ;Get the return value
;                StrCmp $R0 "success" +3
;                MessageBox MB_OK "Download failed: $R0"
;                Quit
;	ExecWait $2
;        Delete $2
;  endInstPyQt:
  endExtraInst:
SectionEnd

