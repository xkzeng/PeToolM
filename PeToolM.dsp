# Microsoft Developer Studio Project File - Name="PeToolM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=PeToolM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PeToolM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PeToolM.mak" CFG="PeToolM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PeToolM - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "PeToolM - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PeToolM - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Shlwapi.lib /nologo /subsystem:windows /incremental:yes /machine:I386
# SUBTRACT LINK32 /pdb:none /nodefaultlib
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=cscript /nologo cmd\CopyByTime.js Release\PeToolM.exe R
# End Special Build Tool

!ELSEIF  "$(CFG)" == "PeToolM - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Shlwapi.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /incremental:no
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=cscript /nologo cmd\CopyByTime.vbs Debug\PeToolM.exe D
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "PeToolM - Win32 Release"
# Name "PeToolM - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\MapFile.cpp
# End Source File
# Begin Source File

SOURCE=.\TabBlkTbl.cpp
# End Source File
# Begin Source File

SOURCE=.\TabDatDir.cpp
# End Source File
# Begin Source File

SOURCE=.\TabDosHdr.cpp
# End Source File
# Begin Source File

SOURCE=.\TabExpTbl.cpp
# End Source File
# Begin Source File

SOURCE=.\TabFilHdr.cpp
# End Source File
# Begin Source File

SOURCE=.\TabImpTbl.cpp
# End Source File
# Begin Source File

SOURCE=.\TabOptHdr.cpp
# End Source File
# Begin Source File

SOURCE=.\TabPeFile.cpp
# End Source File
# Begin Source File

SOURCE=.\TabResTbl.cpp
# End Source File
# Begin Source File

SOURCE=.\TabRlcTbl.cpp
# End Source File
# Begin Source File

SOURCE=.\TabUsrOpr.cpp
# End Source File
# Begin Source File

SOURCE=.\TabDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PeFile.cpp
# End Source File
# Begin Source File

SOURCE=.\PeToolM.cpp
# End Source File
# Begin Source File

SOURCE=.\PeToolM.rc
# End Source File
# Begin Source File

SOURCE=.\PeToolDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\MapFile.h
# End Source File
# Begin Source File

SOURCE=.\TabBlkTbl.h
# End Source File
# Begin Source File

SOURCE=.\TabDatDir.h
# End Source File
# Begin Source File

SOURCE=.\TabDosHdr.h
# End Source File
# Begin Source File

SOURCE=.\TabExpTbl.h
# End Source File
# Begin Source File

SOURCE=.\TabFilHdr.h
# End Source File
# Begin Source File

SOURCE=.\TabImpTbl.h
# End Source File
# Begin Source File

SOURCE=.\TabOptHdr.h
# End Source File
# Begin Source File

SOURCE=.\TabPeFile.h
# End Source File
# Begin Source File

SOURCE=.\TabResTbl.h
# End Source File
# Begin Source File

SOURCE=.\TabRlcTbl.h
# End Source File
# Begin Source File

SOURCE=.\TabUsrOpr.h
# End Source File
# Begin Source File

SOURCE=.\TabDialog.h
# End Source File
# Begin Source File

SOURCE=.\PeFile.h
# End Source File
# Begin Source File

SOURCE=.\PeToolM.h
# End Source File
# Begin Source File

SOURCE=.\PeToolDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\PeToolM.ico
# End Source File
# Begin Source File

SOURCE=.\res\PeToolM.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
