#include "stdafx.h"
#include "PeFile.h"

CPeFile* CPeFile::m_stcObject = NULL;

CPeFile::CPeFile(void)
{
  this->m_lpFmtPe = NULL;
  this->m_lpFmtPe = new struct SPeFormat;

  ZeroMemory(this->m_lpFmtPe, sizeof(struct SPeFormat));
}

CPeFile::~CPeFile(void)
{
  if(this->m_lpFmtPe)
  {
    delete (this->m_lpFmtPe);
    this->m_lpFmtPe = NULL;
  }
}

CPeFile* CPeFile::Make(void)
{
  if(NULL == m_stcObject)
  {
    m_stcObject = new CPeFile();
  }
  return m_stcObject;
}

VOID CPeFile::Free(void)
{
  if(m_stcObject)
  {
    m_stcObject->Close();
    delete m_stcObject;
    m_stcObject = NULL;
  }
}

DWORD CPeFile::Rva2Foa(DWORD dwRva)
{
  WORD i = 0, wCount = 0;
  DWORD dwFoa = 0, dwBegin = 0, dwEnd = 0;
  PIMAGE_SECTION_HEADER aSections = NULL;
  PIMAGE_SECTION_HEADER lpSection = NULL;

  if((NULL == this->m_lpFmtPe) || (NULL == this->m_lpFmtPe->lpBlkTbl) || (NULL == this->m_lpFmtPe->lpFilHdr))
  {
    return 0;
  }

  aSections = this->m_lpFmtPe->lpBlkTbl;                 //节表基址
  wCount = this->m_lpFmtPe->lpFilHdr->NumberOfSections;  //节的数量

  for(i = 0; i < wCount; i++)
  {
    lpSection = aSections + i;
    dwBegin = lpSection->VirtualAddress;
    dwEnd   = lpSection->VirtualAddress + lpSection->Misc.VirtualSize;
    if(dwRva >= dwBegin && dwRva <= dwEnd)
    {
      dwFoa = (dwRva - lpSection->VirtualAddress) + lpSection->PointerToRawData;
      break;
    }
  }
  return dwFoa;
}

DWORD CPeFile::Foa2Rva(DWORD dwFoa)
{
  WORD i = 0, wCount = 0;
  DWORD dwRva = 0, dwBegin = 0, dwEnd = 0;
  PIMAGE_SECTION_HEADER aSections = NULL;
  PIMAGE_SECTION_HEADER lpSection = NULL;

  if((NULL == this->m_lpFmtPe) || (NULL == this->m_lpFmtPe->lpBlkTbl) || (NULL == this->m_lpFmtPe->lpFilHdr))
  {
    return 0;
  }

  aSections = this->m_lpFmtPe->lpBlkTbl;                 //节表基址
  wCount = this->m_lpFmtPe->lpFilHdr->NumberOfSections;  //节的数量

  for(i = 0; i < wCount; i++)
  {
    lpSection = aSections + i;
    dwBegin = lpSection->PointerToRawData;
    dwEnd   = lpSection->PointerToRawData + lpSection->SizeOfRawData;
    if(dwFoa >= dwBegin && dwFoa <= dwEnd)
    {
      dwRva = (dwFoa - lpSection->PointerToRawData) + lpSection->VirtualAddress;
      break;
    }
  }
  return dwRva;
}

DWORD CPeFile::GetPeFormat(VOID)
{
  return (DWORD)(this->m_lpFmtPe);
}

LPSTR CPeFile::GetMachineString(WORD machine)
{
  LPSTR name = NULL;
  switch(machine)
  {
    case IMAGE_FILE_MACHINE_I386:
    {
      name = "I386";
    }
    break;

    case IMAGE_FILE_MACHINE_IA64:
    {
      name = "IA64";
    }
    break;

    case IMAGE_FILE_MACHINE_ARM:
    {
      name = "ARM";
    }
    break;

    case IMAGE_FILE_MACHINE_THUMB:
    {
      name = "ARM Thumb";
    }
    break;

    case IMAGE_FILE_MACHINE_ALPHA:
    {
      name = "ALPHA";
    }
    break;

    case IMAGE_FILE_MACHINE_ALPHA64:
    {
      name = "ALPHA64";
    }
    break;

    case IMAGE_FILE_MACHINE_POWERPC:
    {
      name = "POWERPC";
    }
    break;

    default:
    {
      name = "Other";
    }
  }
  return name;
}

LPSTR CPeFile::GetSubsystemString(WORD subsystem, WORD magic)
{
  LPSTR name = NULL;
  switch(subsystem)
  {
    case IMAGE_SUBSYSTEM_UNKNOWN:
    {
      name = "Unknown";
    }
    break;

    case IMAGE_SUBSYSTEM_NATIVE:
    {
      name = "No Need Subsystem";
    }
    break;

    case IMAGE_SUBSYSTEM_WINDOWS_GUI:
    {
      if(IMAGE_NT_OPTIONAL_HDR32_MAGIC == magic)
      {
        name = "Win32 GUI";
      }
      else if(IMAGE_NT_OPTIONAL_HDR64_MAGIC == magic)
      {
        name = "Win64 GUI";
      }
      else if(IMAGE_ROM_OPTIONAL_HDR_MAGIC == magic)
      {
        name = "ROM GUI";
      }
      else
      {
        name = "Windows GUI";
      }
    }
    break;

    case IMAGE_SUBSYSTEM_WINDOWS_CUI:
    {
      if(IMAGE_NT_OPTIONAL_HDR32_MAGIC == magic)
      {
        name = "Win32 CUI";
      }
      else if(IMAGE_NT_OPTIONAL_HDR64_MAGIC == magic)
      {
        name = "Win64 CUI";
      }
      else if(IMAGE_ROM_OPTIONAL_HDR_MAGIC == magic)
      {
        name = "ROM CUI";
      }
      else
      {
        name = "Windows CUI";
      }
    }
    break;

    case IMAGE_SUBSYSTEM_OS2_CUI:
    {
      name = "OS2 CUI";
    }
    break;

    case IMAGE_SUBSYSTEM_POSIX_CUI:
    {
      name = "Posix CUI";
    }
    break;

    case IMAGE_SUBSYSTEM_NATIVE_WINDOWS:
    {
      name = "Win9X Driver";
    }
    break;

    case IMAGE_SUBSYSTEM_WINDOWS_CE_GUI:
    {
      name = "Windows CE GUI";
    }
    break;

    default:
    {
      name = "Other";
    }
  }
  return name;
}

DWORD CPeFile::GetSectionProperty(LPSTR strBuf, WORD wBufSize, DWORD dwCharacteristic)
{
  DWORD dwLen = 0;

  ZeroMemory(strBuf, wBufSize);

  if(dwCharacteristic & IMAGE_SCN_MEM_READ)
  {
    strBuf[dwLen++] = 'R';
  }

  if(dwCharacteristic & IMAGE_SCN_MEM_WRITE)
  {
    strBuf[dwLen++] = 'W';
  }

  if(dwCharacteristic & IMAGE_SCN_MEM_EXECUTE)
  {
    strBuf[dwLen++] = 'E';
  }

  if(dwCharacteristic & IMAGE_SCN_MEM_SHARED)
  {
    strBuf[dwLen++] = 'S';
  }

  if(dwCharacteristic & IMAGE_SCN_MEM_DISCARDABLE)
  {
    strBuf[dwLen++] = 'D';
  }

  if(dwCharacteristic & IMAGE_SCN_CNT_CODE)
  {
    strBuf[dwLen++] = 'C';
  }

  if(dwCharacteristic & IMAGE_SCN_CNT_INITIALIZED_DATA)
  {
    strBuf[dwLen++] = 'I';
  }

  if(dwCharacteristic & IMAGE_SCN_CNT_UNINITIALIZED_DATA)
  {
    strBuf[dwLen++] = 'U';
  }
  return dwLen;
}

DWORD CPeFile::Encrpty(LPBYTE lpStart, WORD wLength, CONST BYTE bKey)
{
  LPBYTE pos = NULL, posEnd = NULL;
  BYTE tmp = 0;
  posEnd = lpStart + wLength;
  for(pos = lpStart; pos != posEnd; pos++)
  {
    //(*pos) = (*pos) & bKey;
    tmp = *pos;
    tmp = (tmp >> 4) | (tmp << 4);
    *pos = tmp;
  }
  return 0;
}

DWORD CPeFile::Decrpty(LPBYTE lpStart, WORD wLength, CONST BYTE bKey)
{
  LPBYTE pos = NULL, posEnd = NULL;
  BYTE tmp = 0;
  posEnd = lpStart + wLength;
  for(pos = lpStart; pos != posEnd; pos++)
  {
    //(*pos) = (*pos) & bKey;
    tmp = *pos;
    tmp = (tmp >> 4) | (tmp << 4);
    *pos = tmp;
  }
  return 0;
}

BOOL CPeFile::Parse(LPDWORD lpdwResult)
{
  PIMAGE_DOS_HEADER lpDosHdr = NULL;
  PIMAGE_NT_HEADERS lpNtHdr  = NULL;
  DWORD dwStart = 0;

  if(NULL == this->m_lpFmtPe)
  {
    *lpdwResult = 1;
    return FALSE;
  }

  //check DOS Header
  lpDosHdr = (PIMAGE_DOS_HEADER)(this->m_strBase);
  if(IMAGE_DOS_SIGNATURE != lpDosHdr->e_magic) //Invalid DOS header
  {
    *lpdwResult = 2;
    return FALSE;
  }

  //check NT Header
  lpNtHdr = (PIMAGE_NT_HEADERS)(this->m_strBase + lpDosHdr->e_lfanew);
  if(IMAGE_NT_SIGNATURE != lpNtHdr->Signature)
  {
    *lpdwResult = 3;
    return FALSE;
  }

  //DOS Header
  this->m_lpFmtPe->lpDosHdr = lpDosHdr;

  //NT Header
  this->m_lpFmtPe->lpNtHdr  = lpNtHdr;

  //File Header
  this->m_lpFmtPe->lpFilHdr = &(this->m_lpFmtPe->lpNtHdr->FileHeader);

  //Optional Header
  this->m_lpFmtPe->lpOptHdr = &(this->m_lpFmtPe->lpNtHdr->OptionalHeader);

  //Data Directory
  this->m_lpFmtPe->lpDatDir = this->m_lpFmtPe->lpOptHdr->DataDirectory;

  //Section Table
  this->m_lpFmtPe->lpBlkTbl = (PIMAGE_SECTION_HEADER)(this->m_strBase + this->m_lpFmtPe->lpDosHdr->e_lfanew + sizeof(IMAGE_NT_HEADERS));

  //Export Directory
  this->m_lpFmtPe->lpExpBlk = this->m_lpFmtPe->lpDatDir + IMAGE_DIRECTORY_ENTRY_EXPORT;
  dwStart = this->Rva2Foa(this->m_lpFmtPe->lpExpBlk->VirtualAddress);
  this->m_lpFmtPe->lpExpTbl = (PIMAGE_EXPORT_DIRECTORY)(this->m_strBase + dwStart);

  //Import Directory
  this->m_lpFmtPe->lpImpBlk = this->m_lpFmtPe->lpDatDir + IMAGE_DIRECTORY_ENTRY_IMPORT;
  dwStart = this->Rva2Foa(this->m_lpFmtPe->lpImpBlk->VirtualAddress);
  this->m_lpFmtPe->lpImpTbl = (PIMAGE_IMPORT_DESCRIPTOR)(this->m_strBase + dwStart);

  //Resource Directory
  this->m_lpFmtPe->lpResBlk = this->m_lpFmtPe->lpDatDir + IMAGE_DIRECTORY_ENTRY_RESOURCE;
  dwStart = this->Rva2Foa(this->m_lpFmtPe->lpResBlk->VirtualAddress);
  this->m_lpFmtPe->lpResTbl = (PIMAGE_RESOURCE_DIRECTORY)(this->m_strBase + dwStart);

  //Base Relocation Table
  this->m_lpFmtPe->lpRlcBlk = this->m_lpFmtPe->lpDatDir + IMAGE_DIRECTORY_ENTRY_BASERELOC;
  dwStart = this->Rva2Foa(this->m_lpFmtPe->lpRlcBlk->VirtualAddress);
  this->m_lpFmtPe->lpRlcTbl = (PIMAGE_BASE_RELOCATION)(this->m_strBase + dwStart);

  //Debug Directory
  this->m_lpFmtPe->lpDbgBlk = this->m_lpFmtPe->lpDatDir + IMAGE_DIRECTORY_ENTRY_DEBUG;
  dwStart = this->Rva2Foa(this->m_lpFmtPe->lpDbgBlk->VirtualAddress);
  this->m_lpFmtPe->lpDbgTbl = (PIMAGE_DEBUG_DIRECTORY)(this->m_strBase + dwStart);

  //TLS Directory
  this->m_lpFmtPe->lpTlsBlk = this->m_lpFmtPe->lpDatDir + IMAGE_DIRECTORY_ENTRY_TLS;
  dwStart = this->Rva2Foa(this->m_lpFmtPe->lpTlsBlk->VirtualAddress);
  this->m_lpFmtPe->lpTlsTbl = (PIMAGE_TLS_DIRECTORY)(this->m_strBase + dwStart);

  //Import Address Table (IAT)
  this->m_lpFmtPe->lpIatBlk = this->m_lpFmtPe->lpDatDir + IMAGE_DIRECTORY_ENTRY_IAT;
  dwStart = this->Rva2Foa(this->m_lpFmtPe->lpIatBlk->VirtualAddress);
  this->m_lpFmtPe->lpIatTbl = (PIMAGE_THUNK_DATA)(this->m_strBase + dwStart);

  //OK
  *lpdwResult = (DWORD)(this->m_lpFmtPe);
  return TRUE;
}

BOOL CPeFile::IsValidPeFile(void)
{
  return TRUE;
}

class CPeFile* g_lpPeFile = NULL;
struct SPePage* g_lpPages = NULL;
