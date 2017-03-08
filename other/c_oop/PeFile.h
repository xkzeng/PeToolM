#pragma once

#ifndef __PEFILE_H__
#define __PEFILE_H__
#include <stdio.h>
#include <windows.h>

#define FILE_PATH_LENGTH 256
#define MODE_OUTPUT_SCREEN 0 //数据输出到屏幕上;
#define MODE_OUTPUT_FILE   1 //数据输出到文件中;

struct SPeFormat
{
  PIMAGE_DOS_HEADER        lpDosHdr;  //DOS Header
  PIMAGE_NT_HEADERS        lpNtHdr;   //NT Header
  PIMAGE_FILE_HEADER       lpFilHdr;  //File Header
  PIMAGE_OPTIONAL_HEADER   lpOptHdr;  //Optional Header
  PIMAGE_DATA_DIRECTORY    lpDatDir;  //Data Directory
  PIMAGE_SECTION_HEADER    lpBlkTbl;  //Section Table

  PIMAGE_DATA_DIRECTORY    lpExpBlk;  //Export Block(VirtualAddress && Size)
  PIMAGE_EXPORT_DIRECTORY  lpExpTbl;  //Export Directory(RDA)

  PIMAGE_DATA_DIRECTORY    lpImpBlk;  //Import Block(VirtualAddress && Size)
  PIMAGE_IMPORT_DESCRIPTOR lpImpTbl;  //Import Directory(RDA)

  PIMAGE_DATA_DIRECTORY    lpRlcBlk;  //Base Relocation Block(VirtualAddress && Size)
  PIMAGE_BASE_RELOCATION   lpRlcTbl;  //Base Relocation Table(RDA)

  PIMAGE_DATA_DIRECTORY    lpDbgBlk;  //Debug Block(VirtualAddress && Size)
  PIMAGE_DEBUG_DIRECTORY   lpDbgTbl;  //Debug Directory(RDA)

  PIMAGE_DATA_DIRECTORY    lpTlsBlk;  //TLS Block(VirtualAddress && Size)
  PIMAGE_TLS_DIRECTORY     lpTlsTbl;  //TLS Directory(RDA)

  PIMAGE_DATA_DIRECTORY    lpIatBlk;  //Import Address Table Block(VirtualAddress && Size)
  PIMAGE_THUNK_DATA        lpIatTbl;  //Import Address Table(RDA)
};

struct SPeFile
{
  TCHAR  strFile[FILE_PATH_LENGTH];
  HANDLE lphFile;
  HANDLE lphMapFile;
  LPVOID lpvBase;
  LPCSTR strBase;
  struct SPeFormat* lpFmtPe;

  WORD   (*SetFileName)(LPCTSTR strFileName);
  LPTSTR (*GetFileName)(VOID);
  BOOL   (*Open)(LPCTSTR strFileName, LPDWORD lpdwReason);
  BOOL   (*OpenEx)(LPCTSTR strFileName, LPDWORD lpdwReason);
  BOOL   (*MapFile)(DWORD dwProtect, LPWORD lpdwReason);
  BOOL   (*MapFileView)(DWORD dwProtect, LPWORD lpdwReason);
  BOOL   (*UnMapFileView)(VOID);
  BOOL   (*Close)(VOID);

  BOOL   (*OpenOutput)(BYTE bMode);
  BOOL   (*CloseOutput)(VOID);
  BOOL   (*Parse)(LPDWORD lpdwResult);
  DWORD  (*Rva2Rda)(DWORD dwRva);
  DWORD  (*Rda2Rva)(DWORD dwRda);
  BOOL   (*ParseDosHdr)(VOID);
  BOOL   (*ParseNtHdr)(VOID);
  BOOL   (*ParseFilHdr)(VOID);
  BOOL   (*ParseOptHdr)(VOID);
  BOOL   (*ParseDatDir)(VOID);
  BOOL   (*ParseBlkTbl)(VOID);
  BOOL   (*ParseExpTbl)(VOID);
  BOOL   (*ParseImpTbl)(VOID);
  BOOL   (*ParseRlcTbl)(VOID);
  BOOL   (*ParseIatTbl)(VOID);
};

struct SPeFile* AcquirePeFile(void);
void ReleasePeFile(void);

#endif
