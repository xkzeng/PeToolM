#pragma once

#ifndef __PE_FILE_H__
#define __PE_FILE_H__
#include <windows.h>
#include "MapFile.h"

#define PAGE_TITLE_LEN 32
#define TEMP_BUFF_SIZE 32
#define FILE_PATH_LEN  256
#define SIGN_BUF_LEN   32
#define SKIP_LENGTH    1
#define KEY_ENCRPTY    0xA
#define NO_PE_DATA     "None"

#pragma pack(1)
struct SSoftSign
{
  BYTE bLength;   //记录所有签名相关的信息总长度;[0,255]
  WORD wVersion1; //[0,65535]
  BYTE bVersion2; //[0,255]
  BYTE bVersion3; //[0,255]
  //DWORD dwSoftName;
  //DWORD dwAuthor;
  //DWORD dwEmail;
};
#pragma pack()

struct SString
{
  BYTE  bLength;
  TCHAR strBuffer[SIGN_BUF_LEN];
};

struct SPePage
{
  UINT     uiIDD;
  CDialog* lpTabDlg;
  LPCTSTR  strTitle;
};

struct SPeFormat
{
  PIMAGE_DOS_HEADER         lpDosHdr; //DOS Header
  PIMAGE_NT_HEADERS         lpNtHdr;  //NT Header
  PIMAGE_FILE_HEADER        lpFilHdr; //File Header
  PIMAGE_OPTIONAL_HEADER    lpOptHdr; //Optional Header
  PIMAGE_DATA_DIRECTORY     lpDatDir; //Data Directory
  PIMAGE_SECTION_HEADER     lpBlkTbl; //Section Table

  PIMAGE_DATA_DIRECTORY     lpExpBlk; //Export Block(VirtualAddress && Size)
  PIMAGE_EXPORT_DIRECTORY   lpExpTbl; //Export Directory(FOA)

  PIMAGE_DATA_DIRECTORY     lpImpBlk; //Import Block(VirtualAddress && Size)
  PIMAGE_IMPORT_DESCRIPTOR  lpImpTbl; //Import Directory(FOA)

  PIMAGE_DATA_DIRECTORY     lpResBlk; //Resource Directory(VirtualAddress && Size)
  PIMAGE_RESOURCE_DIRECTORY lpResTbl; //Resource Directory(FOA)

  PIMAGE_DATA_DIRECTORY     lpRlcBlk; //Base Relocation Block(VirtualAddress && Size)
  PIMAGE_BASE_RELOCATION    lpRlcTbl; //Base Relocation Table(FOA)

  PIMAGE_DATA_DIRECTORY     lpDbgBlk; //Debug Block(VirtualAddress && Size)
  PIMAGE_DEBUG_DIRECTORY    lpDbgTbl; //Debug Directory(FOA)

  PIMAGE_DATA_DIRECTORY     lpTlsBlk; //TLS Block(VirtualAddress && Size)
  PIMAGE_TLS_DIRECTORY      lpTlsTbl; //TLS Directory(FOA)

  PIMAGE_DATA_DIRECTORY     lpIatBlk; //Import Address Table Block(VirtualAddress && Size)
  PIMAGE_THUNK_DATA         lpIatTbl; //Import Address Table(FOA)
};

typedef struct SPeFormat* PFORMAT_PE;

class CPeFile: public CMapFile
{
  private:
    static CPeFile* m_stcObject;
    PFORMAT_PE      m_lpFmtPe;

  public:
    CPeFile(void);
    virtual ~CPeFile(void);

    static CPeFile* Make(void);
    static VOID     Free(void);

    DWORD Rva2Foa(DWORD dwRva);
    DWORD Foa2Rva(DWORD dwFoa);
    DWORD GetPeFormat(VOID);
    LPSTR GetMachineString(WORD machine);
    LPSTR GetSubsystemString(WORD subsystem, WORD magic);
    DWORD GetSectionProperty(LPSTR strBuf, WORD wBufSize, DWORD dwCharacteristic);
    DWORD Encrpty(LPBYTE lpStart, WORD wLength, CONST BYTE bKey);
    DWORD Decrpty(LPBYTE lpStart, WORD wLength, CONST BYTE bKey);

    /***
     return:
       TRUE  -> (struct SPeFormat*)(*lpdwResult),lpdwResult是一个指向struct SPeFormat结构的指针;
       FALSE -> (*lpResult) is reason for failed;
    ***/
    BOOL Parse(LPDWORD lpdwResult);
    BOOL IsValidPeFile(void);
};

extern class CPeFile* g_lpPeFile;
extern struct SPePage* g_lpPages;

#endif
