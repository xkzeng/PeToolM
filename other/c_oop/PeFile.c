#include <stdio.h>
#include <time.h>
#include <string.h>
#include <malloc.h>
#include <windows.h>
#include "PeFile.h"

#define SECTION_MAX_NUMBER 16

static LPCSTR strDataDirType[16] =
{
  "Export Directory",              //0
  "Import Directory",              //1
  "Resource Directory",            //2
  "Exception Directory",           //3
  "Security Directory",            //4
  "Base Relocation Table",         //5
  "Debug Directory",               //6
  "Architecture Specific Data",    //7
  "RVA of GP",                     //8
  "TLS Directory",                 //9
  "Load Configuration Directory",  //10
  "Bound Import Directory",        //11
  "Import Address Table",          //12
  "Delay Load Import Descriptors", //13
  "COM Runtime descriptor",        //14
  "UnUsed"                         //15
};

static struct SPeFile stPeFile;

static TCHAR stc_strDumpFile[FILE_PATH_LENGTH];
static BYTE  stc_bOutputMode = MODE_OUTPUT_SCREEN; //default: print all information onto screen;
static FILE* stc_lpFdFile = stdout;                //default: print all information onto screen;

static char* getMachineString(WORD machine)
{
  char* name = NULL;
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

static char* getSubsystemString(WORD subsystem, WORD magic)
{
  char* name = NULL;
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

static DWORD PE_Rva2Rda(DWORD dwRva)
{
  WORD i = 0, wCount = 0;
  DWORD dwRda = 0, dwBegin = 0, dwEnd = 0;
  PIMAGE_SECTION_HEADER aSections = NULL;
  PIMAGE_SECTION_HEADER lpSection = NULL;
  
  aSections = stPeFile.lpFmtPe->lpBlkTbl;
  wCount = stPeFile.lpFmtPe->lpFilHdr->NumberOfSections;
  
  for(i = 0; i < wCount; i++)
  {
    lpSection = aSections + i;
    dwBegin = lpSection->VirtualAddress;
    dwEnd   = lpSection->VirtualAddress + lpSection->Misc.VirtualSize;
    if(dwRva >= dwBegin && dwRva <= dwEnd)
    {
      dwRda = (dwRva - lpSection->VirtualAddress) + lpSection->PointerToRawData;
      break;
    }
  }
  return dwRda;
}

static DWORD PE_Rda2Rva(DWORD dwRda)
{
  WORD i = 0, wCount = 0;
  DWORD dwRva = 0, dwBegin = 0, dwEnd = 0;
  PIMAGE_SECTION_HEADER aSections = NULL;
  PIMAGE_SECTION_HEADER lpSection = NULL;
  
  aSections = stPeFile.lpFmtPe->lpBlkTbl;
  wCount = stPeFile.lpFmtPe->lpFilHdr->NumberOfSections;
  
  for(i = 0; i < wCount; i++)
  {
    lpSection = aSections + i;
    dwBegin = lpSection->PointerToRawData;
    dwEnd   = lpSection->PointerToRawData + lpSection->SizeOfRawData;
    if(dwRda >= dwBegin && dwRda <= dwEnd)
    {
      dwRva = (dwRda - lpSection->PointerToRawData) + lpSection->VirtualAddress;
      break;
    }
  }
  return dwRva;
}

static BOOL IsDumpedFromMemory(VOID)
{
  BOOL bResult = TRUE;
  PIMAGE_SECTION_HEADER aSections = NULL;
  PIMAGE_SECTION_HEADER lpSection = NULL;
  WORD i = 0, wCount = 0;
  
  //Section Table
  aSections = stPeFile.lpFmtPe->lpBlkTbl;
  wCount = stPeFile.lpFmtPe->lpFilHdr->NumberOfSections;
  
  if(wCount > SECTION_MAX_NUMBER)
  {
    wCount = SECTION_MAX_NUMBER;
  }
  
  bResult = TRUE;
  for(i = 0; i < wCount; i++)
  {
    lpSection = aSections + i;
    if((lpSection->VirtualAddress != lpSection->PointerToRawData) || (lpSection->Misc.VirtualSize != lpSection->SizeOfRawData))
    {
      bResult = FALSE;
      break;
    }
  }
  return bResult;
}

static BOOL PE_Parse(LPDWORD lpdwResult)
{
  BOOL ret = FALSE;
  DWORD dwStart = 0;
  PIMAGE_DOS_HEADER lpDosHdr = NULL;
  PIMAGE_NT_HEADERS lpNtHdr  = NULL;

  if(NULL == stPeFile.lpvBase)
  {
    printf("Invalid Image\n");
    *lpdwResult = 1;
    return FALSE;
  }

  //File Base
  if(NULL == stPeFile.strBase)
  {
    stPeFile.strBase = (LPCSTR)(stPeFile.lpvBase);
  }

  //check DOS Header
  lpDosHdr = (PIMAGE_DOS_HEADER)(stPeFile.strBase);
  if(IMAGE_DOS_SIGNATURE != lpDosHdr->e_magic)
  {
    printf("Unknow Windows File Format\n");
    *lpdwResult = 2;
    return FALSE;
  }

  //check NT Header
  lpNtHdr = (PIMAGE_NT_HEADERS)(stPeFile.strBase + lpDosHdr->e_lfanew);
  if(IMAGE_NT_SIGNATURE != lpNtHdr->Signature)
  {
    printf("Not a NT File\n");
    *lpdwResult = 3;
    return FALSE;
  }

  //DOS Header
  stPeFile.lpFmtPe->lpDosHdr = lpDosHdr;

  //NT Header
  stPeFile.lpFmtPe->lpNtHdr  = lpNtHdr;

  //File Header
  stPeFile.lpFmtPe->lpFilHdr = &(lpNtHdr->FileHeader);

  //Optional Header
  stPeFile.lpFmtPe->lpOptHdr = &(lpNtHdr->OptionalHeader);

  //Data Directory
  stPeFile.lpFmtPe->lpDatDir = stPeFile.lpFmtPe->lpOptHdr->DataDirectory;

  //Section Table
  stPeFile.lpFmtPe->lpBlkTbl = (PIMAGE_SECTION_HEADER)(stPeFile.strBase + stPeFile.lpFmtPe->lpDosHdr->e_lfanew + sizeof(IMAGE_NT_HEADERS));

  //Export Directory
  stPeFile.lpFmtPe->lpExpBlk = stPeFile.lpFmtPe->lpDatDir + IMAGE_DIRECTORY_ENTRY_EXPORT;
  dwStart = stPeFile.Rva2Rda(stPeFile.lpFmtPe->lpExpBlk->VirtualAddress);
  stPeFile.lpFmtPe->lpExpTbl = (PIMAGE_EXPORT_DIRECTORY)(stPeFile.strBase + dwStart);

  //Import Directory
  stPeFile.lpFmtPe->lpImpBlk = stPeFile.lpFmtPe->lpDatDir + IMAGE_DIRECTORY_ENTRY_IMPORT;
  dwStart = stPeFile.Rva2Rda(stPeFile.lpFmtPe->lpImpBlk->VirtualAddress);
  stPeFile.lpFmtPe->lpImpTbl = (PIMAGE_IMPORT_DESCRIPTOR)(stPeFile.strBase + dwStart);

  //Base Relocation Table
  stPeFile.lpFmtPe->lpRlcBlk = stPeFile.lpFmtPe->lpDatDir + IMAGE_DIRECTORY_ENTRY_BASERELOC;
  dwStart = stPeFile.Rva2Rda(stPeFile.lpFmtPe->lpRlcBlk->VirtualAddress);
  stPeFile.lpFmtPe->lpRlcTbl = (PIMAGE_BASE_RELOCATION)(stPeFile.strBase + dwStart);

  //Debug Directory
  stPeFile.lpFmtPe->lpDbgBlk = stPeFile.lpFmtPe->lpDatDir + IMAGE_DIRECTORY_ENTRY_DEBUG;
  dwStart = stPeFile.Rva2Rda(stPeFile.lpFmtPe->lpDbgBlk->VirtualAddress);
  stPeFile.lpFmtPe->lpDbgTbl = (PIMAGE_DEBUG_DIRECTORY)(stPeFile.strBase + dwStart);

  //TLS Directory
  stPeFile.lpFmtPe->lpTlsBlk = stPeFile.lpFmtPe->lpDatDir + IMAGE_DIRECTORY_ENTRY_TLS;
  dwStart = stPeFile.Rva2Rda(stPeFile.lpFmtPe->lpTlsBlk->VirtualAddress);
  stPeFile.lpFmtPe->lpTlsTbl = (PIMAGE_TLS_DIRECTORY)(stPeFile.strBase + dwStart);

  //Import Address Table (IAT)
  stPeFile.lpFmtPe->lpIatBlk = stPeFile.lpFmtPe->lpDatDir + IMAGE_DIRECTORY_ENTRY_IAT;
  dwStart = stPeFile.Rva2Rda(stPeFile.lpFmtPe->lpIatBlk->VirtualAddress);
  stPeFile.lpFmtPe->lpIatTbl = (PIMAGE_THUNK_DATA)(stPeFile.strBase + dwStart);

  *lpdwResult = 0;

  if(IsDumpedFromMemory())
  {
    printf("%s is dumped from memory\n", stPeFile.strFile);
  }
  else
  {
    printf("%s is not dumped from memory\n", stPeFile.strFile);
  }
  return TRUE;
}

static BOOL PE_ShowDosHdr(VOID)
{
  PIMAGE_DOS_HEADER lpHdr = NULL;
  char* tmp = NULL;
  WORD* w = NULL;
  
  //DOS Header
  lpHdr = stPeFile.lpFmtPe->lpDosHdr;
  
  fprintf(stc_lpFdFile, "================DOS HEADER================\n");
  tmp = (char*)&(lpHdr->e_magic);
  fprintf(stc_lpFdFile, " e_magic   : %04X -> %c%c\n", lpHdr->e_magic, tmp[0], tmp[1]);
  fprintf(stc_lpFdFile, " e_cblp    : %04X\n", lpHdr->e_cblp);
  fprintf(stc_lpFdFile, " e_cp      : %04X\n", lpHdr->e_cp);
  fprintf(stc_lpFdFile, " e_crlc    : %04X\n", lpHdr->e_crlc);
  fprintf(stc_lpFdFile, " e_cparhdr : %04X\n", lpHdr->e_cparhdr);
  fprintf(stc_lpFdFile, " e_minalloc: %04X\n", lpHdr->e_minalloc);
  fprintf(stc_lpFdFile, " e_maxalloc: %04X\n", lpHdr->e_maxalloc);
  fprintf(stc_lpFdFile, " e_ss      : %04X\n", lpHdr->e_ss);
  fprintf(stc_lpFdFile, " e_sp      : %04X\n", lpHdr->e_sp);
  fprintf(stc_lpFdFile, " e_csum    : %04X\n", lpHdr->e_csum);
  fprintf(stc_lpFdFile, " e_ip      : %04X\n", lpHdr->e_ip);
  fprintf(stc_lpFdFile, " e_cs      : %04X\n", lpHdr->e_cs);
  fprintf(stc_lpFdFile, " e_lfarlc  : %04X\n", lpHdr->e_lfarlc);
  fprintf(stc_lpFdFile, " e_ovno    : %04X\n", lpHdr->e_ovno);

  w = lpHdr->e_res;
  fprintf(stc_lpFdFile, " e_res     : %04X %04X %04X %04X\n", w[0], w[1], w[2], w[3]);
  fprintf(stc_lpFdFile, " e_oemid   : %04X\n", lpHdr->e_oemid);
  fprintf(stc_lpFdFile, " e_oeminfo : %04X\n", lpHdr->e_oeminfo);

  w = lpHdr->e_res2;
  fprintf(stc_lpFdFile, " e_res2    : %04X %04X %04X %04X %04X %04X %04X %04X %04X %04X\n", w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9]);
  fprintf(stc_lpFdFile, " e_lfanew  : %08X\n\n", lpHdr->e_lfanew);
  return TRUE;
}

static BOOL PE_ShowNtHeader(VOID)
{
  PIMAGE_NT_HEADERS lpHdr = NULL;
  char* tmp = NULL;
  
  //NT Header
  lpHdr = stPeFile.lpFmtPe->lpNtHdr;
  
  fprintf(stc_lpFdFile, "================NT HEADER================\n");
  tmp = (char*)&(lpHdr->Signature);
  fprintf(stc_lpFdFile, " Signature: %08X -> %c%c%d%d\n\n", lpHdr->Signature, tmp[0], tmp[1], tmp[2], tmp[3]);
  stPeFile.ParseFilHdr();
  stPeFile.ParseOptHdr();
  return TRUE;
}

static BOOL PE_ShowFileHeader(VOID)
{
  PIMAGE_FILE_HEADER lpHdr = NULL;
  char strTime[32];
  
  //File Header
  lpHdr = stPeFile.lpFmtPe->lpFilHdr;
  
  fprintf(stc_lpFdFile, "================FILE HEADER================\n");
  fprintf(stc_lpFdFile, " Machine             : %04X -> %s\n", lpHdr->Machine, getMachineString(lpHdr->Machine));
  fprintf(stc_lpFdFile, " NumberOfSections    : %04X\n", lpHdr->NumberOfSections);

  ZeroMemory((PVOID)strTime, (SIZE_T)sizeof(strTime));
  strftime(strTime, sizeof(strTime), "%Y-%m-%d %H:%M:%S", localtime((const time_t*)&(lpHdr->TimeDateStamp)));
  fprintf(stc_lpFdFile, " TimeDateStamp       : %08X -> %s\n", lpHdr->TimeDateStamp, strTime);
  fprintf(stc_lpFdFile, " PointerToSymbolTable: %08X\n", lpHdr->PointerToSymbolTable);
  fprintf(stc_lpFdFile, " NumberOfSymbols     : %08X\n", lpHdr->NumberOfSymbols);
  fprintf(stc_lpFdFile, " SizeOfOptionalHeader: %04X\n", lpHdr->SizeOfOptionalHeader);
  fprintf(stc_lpFdFile, " Characteristics     : %04X\n\n", lpHdr->Characteristics);
  return TRUE;
}

static BOOL PE_ShowOptionHeader(VOID)
{
  PIMAGE_OPTIONAL_HEADER lpHdr = NULL;
  
  //Optional Header
  lpHdr = stPeFile.lpFmtPe->lpOptHdr;
  
  fprintf(stc_lpFdFile, "================OPTIONAL HEADER================\n");
  fprintf(stc_lpFdFile, " Magic                      : %04X\n", lpHdr->Magic);
  fprintf(stc_lpFdFile, " MajorLinkerVersion         : %02X\n", lpHdr->MajorLinkerVersion);
  fprintf(stc_lpFdFile, " MinorLinkerVersion         : %02X\n", lpHdr->MinorLinkerVersion);
  fprintf(stc_lpFdFile, " SizeOfCode                 : %08X\n", lpHdr->SizeOfCode);
  fprintf(stc_lpFdFile, " SizeOfInitializedData      : %08X\n", lpHdr->SizeOfInitializedData);
  fprintf(stc_lpFdFile, " SizeOfInitializedData      : %08X\n", lpHdr->SizeOfUninitializedData);
  fprintf(stc_lpFdFile, " AddressOfEntryPoint        : %08X\n", lpHdr->AddressOfEntryPoint);
  fprintf(stc_lpFdFile, " BaseOfCode                 : %08X\n", lpHdr->BaseOfCode);
  fprintf(stc_lpFdFile, " BaseOfData                 : %08X\n", lpHdr->BaseOfData);
  fprintf(stc_lpFdFile, " ImageBase                  : %08X\n", lpHdr->ImageBase);
  fprintf(stc_lpFdFile, " SectionAlignment           : %08X\n", lpHdr->SectionAlignment);
  fprintf(stc_lpFdFile, " FileAlignment              : %08X\n", lpHdr->FileAlignment);
  fprintf(stc_lpFdFile, " MajorOperatingSystemVersion: %04X\n", lpHdr->MajorOperatingSystemVersion);
  fprintf(stc_lpFdFile, " MajorOperatingSystemVersion: %04X\n", lpHdr->MinorOperatingSystemVersion);
  fprintf(stc_lpFdFile, " MajorImageVersion          : %04X\n", lpHdr->MajorImageVersion);
  fprintf(stc_lpFdFile, " MinorImageVersion          : %04X\n", lpHdr->MinorImageVersion);
  fprintf(stc_lpFdFile, " MajorSubsystemVersion      : %04X\n", lpHdr->MajorSubsystemVersion);
  fprintf(stc_lpFdFile, " MajorSubsystemVersion      : %04X\n", lpHdr->MinorSubsystemVersion);
  fprintf(stc_lpFdFile, " Win32VersionValue          : %08X\n", lpHdr->Win32VersionValue);
  fprintf(stc_lpFdFile, " SizeOfImage                : %08X\n", lpHdr->SizeOfImage);
  fprintf(stc_lpFdFile, " SizeOfHeaders              : %08X\n", lpHdr->SizeOfHeaders);
  fprintf(stc_lpFdFile, " CheckSum                   : %08X\n", lpHdr->CheckSum);
  fprintf(stc_lpFdFile, " Subsystem                  : %04X -> %s\n", lpHdr->Subsystem, getSubsystemString(lpHdr->Subsystem, lpHdr->Magic));
  fprintf(stc_lpFdFile, " DllCharacteristics         : %04X\n", lpHdr->DllCharacteristics);
  fprintf(stc_lpFdFile, " Win32VersionValue          : %08X\n", lpHdr->SizeOfStackReserve);
  fprintf(stc_lpFdFile, " SizeOfStackCommit          : %08X\n", lpHdr->SizeOfStackCommit);
  fprintf(stc_lpFdFile, " SizeOfHeapReserve          : %08X\n", lpHdr->SizeOfHeapReserve);
  fprintf(stc_lpFdFile, " SizeOfHeapCommit           : %08X\n", lpHdr->SizeOfHeapCommit);
  fprintf(stc_lpFdFile, " LoaderFlags                : %08X\n", lpHdr->LoaderFlags);
  fprintf(stc_lpFdFile, " NumberOfRvaAndSizes        : %08X\n\n", lpHdr->NumberOfRvaAndSizes);
  stPeFile.ParseDatDir();
  return TRUE;
}

static BOOL PE_ShowDataDirectory(VOID)
{
  PIMAGE_DATA_DIRECTORY lpDataDir = NULL;
  PIMAGE_DATA_DIRECTORY lpData = NULL;
  DWORD i = 0, dwCount = 0;
  
  //Data Directory
  lpDataDir = stPeFile.lpFmtPe->lpDatDir;

  dwCount = stPeFile.lpFmtPe->lpOptHdr->NumberOfRvaAndSizes;
  
  if(dwCount > IMAGE_NUMBEROF_DIRECTORY_ENTRIES)
  {
    dwCount = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;
  }

  fprintf(stc_lpFdFile, "==========================DATA DIRECTORY==========================\n");
  fprintf(stc_lpFdFile, " %-6s  %-30s %-16s%-12s\n", "Type", "Name", "VirtualAddress", "Size");
  for(i = 0; i < dwCount; i++)
  {
    lpData = lpDataDir + i;
    fprintf(stc_lpFdFile, " %-6d  %-30s %08X        %08X\n", i, strDataDirType[i], lpData->VirtualAddress, lpData->Size);
  }
  fprintf(stc_lpFdFile, "\n");
  return TRUE;
}

static BOOL PE_ShowSectionTable(VOID)
{
  PIMAGE_SECTION_HEADER aSections = NULL;
  PIMAGE_SECTION_HEADER lpSection = NULL;
  WORD i = 0, wCount = 0;
  
  //Section Table
  aSections = stPeFile.lpFmtPe->lpBlkTbl;
  wCount = stPeFile.lpFmtPe->lpFilHdr->NumberOfSections;

  if(wCount > SECTION_MAX_NUMBER)
  {
    wCount = SECTION_MAX_NUMBER;
  }

  fprintf(stc_lpFdFile, "==========================SECTION TABLE======================================================\n");
  fprintf(stc_lpFdFile, " %-5s%-10s%-16s%-13s%-18s%-15s%s\n", "Seq", "Name", "VirtualAddress", "VirtualSize", "PointerToRawData", "SizeOfRawData", "Characteristics");
  for(i = 0; i < wCount; i++)
  {
    lpSection = aSections + i;
    fprintf(stc_lpFdFile, " %-5d%-10s%08X        %08X     %08X          %08X       %08X\n", i, lpSection->Name, lpSection->VirtualAddress, lpSection->Misc.VirtualSize, lpSection->PointerToRawData, lpSection->SizeOfRawData, lpSection->Characteristics);
  }
  fprintf(stc_lpFdFile, "\n");
  return TRUE;
}

static BOOL PE_ShowExportTable(VOID)
{
  PIMAGE_EXPORT_DIRECTORY lpIED = NULL;
  DWORD dwIEDStart = 0, dwRda = 0;
  WORD i = 0, wTmp = 0;
  LPCSTR lpTmp = NULL;
  CHAR strTmp[32];
  LPDWORD adwRvasFuncAddr = NULL;
  LPDWORD adwRvasFuncName = NULL;
  LPWORD  awOrdinals = NULL;
  WORD    wOrdinal = 0;

  fprintf(stc_lpFdFile, "==========================EXPORT TABLE====================================\n");
  if(0 == stPeFile.lpFmtPe->lpExpBlk->Size)
  {
    fprintf(stc_lpFdFile, "This is no Export Directory\n\n");
    return FALSE;
  }

  //start position of the Export Directory
  lpIED = stPeFile.lpFmtPe->lpExpTbl;

  //start address of the dll name
  dwRda = stPeFile.Rva2Rda(lpIED->Name);
  lpTmp = stPeFile.strBase + dwRda;
  ZeroMemory((PVOID)strTmp, sizeof(strTmp));
  for(i = 0; *lpTmp; i++, lpTmp++)
  {
    strTmp[i] = (*lpTmp);
  }
  strTmp[i] = 0;

  fprintf(stc_lpFdFile, " DllFileName          : %08X -> %08X -> %s\n", lpIED->Name, dwRda, strTmp);
  fprintf(stc_lpFdFile, " Characteristics      : %08X\n", lpIED->Characteristics);
  ZeroMemory((PVOID)strTmp, sizeof(strTmp));
  strftime(strTmp, sizeof(strTmp), "%Y-%m-%d %H:%M:%S", localtime((const time_t*)&(lpIED->TimeDateStamp)));
  fprintf(stc_lpFdFile, " TimeDateStamp        : %08X -> %s\n", lpIED->TimeDateStamp, strTmp);
  fprintf(stc_lpFdFile, " MajorVersion         : %04X\n", lpIED->MajorVersion);
  fprintf(stc_lpFdFile, " MinorVersion         : %04X\n", lpIED->MinorVersion);
  fprintf(stc_lpFdFile, " Base                 : %08X\n", lpIED->Base);
  fprintf(stc_lpFdFile, " NumberOfFunctions    : %08X\n", lpIED->NumberOfFunctions);
  fprintf(stc_lpFdFile, " NumberOfNames        : %08X\n", lpIED->NumberOfNames);
  dwRda = stPeFile.Rva2Rda(lpIED->AddressOfFunctions);
  fprintf(stc_lpFdFile, " AddressOfFunctions   : %08X -> %08X\n", lpIED->AddressOfFunctions, dwRda);
  dwRda = stPeFile.Rva2Rda(lpIED->AddressOfNames);
  fprintf(stc_lpFdFile, " AddressOfNames       : %08X -> %08X\n", lpIED->AddressOfNames, dwRda);
  fprintf(stc_lpFdFile, " AddressOfNameOrdinals: %08X\n", lpIED->AddressOfNameOrdinals);
  fprintf(stc_lpFdFile, "\n");

  dwRda = stPeFile.Rva2Rda(lpIED->AddressOfFunctions);
  adwRvasFuncAddr = (LPDWORD)(stPeFile.strBase + dwRda); //RVA Array,so,adwRvasFuncAddr[0] is a RVA;

  dwRda = stPeFile.Rva2Rda(lpIED->AddressOfNames);
  adwRvasFuncName = (LPDWORD)(stPeFile.strBase + dwRda); //RVA Array,so,adwRvasFuncName[0] is a RVA;

  dwRda = stPeFile.Rva2Rda(lpIED->AddressOfNameOrdinals);
  awOrdinals = (LPWORD)(stPeFile.strBase + dwRda);

  fprintf(stc_lpFdFile, " ********EXPORT FUNCTION****************************\n");
  fprintf(stc_lpFdFile, " %-9s%-10s%-10s%s\n", "Ordinal", "RVA", "FOA", "Name");
  for(i = 0; i < lpIED->NumberOfNames; i++)
  {
    dwRda = stPeFile.Rva2Rda(adwRvasFuncName[i]);
    lpTmp = stPeFile.strBase + dwRda;
    wOrdinal = awOrdinals[i];
    dwRda = stPeFile.Rva2Rda(adwRvasFuncAddr[wOrdinal]);
    fprintf(stc_lpFdFile, " %04X    %08X  %08X  %s\n", (wOrdinal + lpIED->Base), adwRvasFuncAddr[wOrdinal], dwRda, lpTmp);
  }
  fprintf(stc_lpFdFile, "\n");
  return TRUE;
}

static BOOL PE_ShowImportTable(VOID)
{
  PIMAGE_IMPORT_DESCRIPTOR lpIID = NULL;
  LPDWORD dwaINT = NULL, dwaIAT = NULL;
  DWORD dwThkINT = 0, dwThkIAT = 0;
  DWORD dwFoa = 0, dwFoaOFT = 0, dwFoaFT = 0;
  PIMAGE_IMPORT_BY_NAME lpByName = NULL;
  WORD i = 0, wOrdinal = 0;
  LPCSTR strName = NULL;

  fprintf(stc_lpFdFile, "==========================IMPORT TABLE============================================================================\n");
  if(0 == stPeFile.lpFmtPe->lpImpBlk->Size)
  {
    fprintf(stc_lpFdFile, "This is no Import Directory\n\n");
    return FALSE;
  }

  //start position of the Import Directory
  lpIID = stPeFile.lpFmtPe->lpImpTbl;

  ////only Import Table, but not include Thunk table;
  fprintf(stc_lpFdFile, " %-22s%-15s%-16s%-12s%-12s%-16s%s\n", "OriginalFirstThunk", "TimeDateStamp", "ForwarderChain", "DllNameRVA", "DllNameFOA", "DllName", "FirstThunk");
  for(; lpIID->Name; lpIID++)
  {
    //start address of the dll name
    dwFoa = stPeFile.Rva2Rda(lpIID->Name);
    strName = stPeFile.strBase + dwFoa;
    dwFoaOFT = stPeFile.Rva2Rda(lpIID->OriginalFirstThunk);
    dwFoaFT  = stPeFile.Rva2Rda(lpIID->FirstThunk);
    fprintf(stc_lpFdFile, " %08X -> %08X  %08X       %08X        %08X    %08X    %-16s%08X -> %08X\n", lpIID->OriginalFirstThunk, dwFoaOFT, lpIID->TimeDateStamp, lpIID->ForwarderChain, lpIID->Name, dwFoa, strName, lpIID->FirstThunk, dwFoaFT);
  }
  fprintf(stc_lpFdFile, "\n");

  ////Thunk table
  lpIID = stPeFile.lpFmtPe->lpImpTbl;
  for(; lpIID->Name; lpIID++)
  {
    //start address of the dll name
    dwFoa = stPeFile.Rva2Rda(lpIID->Name);
    strName = stPeFile.strBase + dwFoa;
    fprintf(stc_lpFdFile, " ******** %s ********************************\n", strName);
    
    fprintf(stc_lpFdFile, " %-10s%-10s%-10s%-6s%-s\n", "RVA", "FOA", "IAT", "ORDI", "FUNCTION");

    //start address of the Original First Thunk table,named Import Name Table(INT);
    dwFoa = stPeFile.Rva2Rda(lpIID->OriginalFirstThunk);
    dwaINT = (LPDWORD)(stPeFile.strBase + dwFoa);

    //start address of the First Thunk table,named Import Address Table(IAT);
    dwFoa = stPeFile.Rva2Rda(lpIID->FirstThunk);
    dwaIAT = (LPDWORD)(stPeFile.strBase + dwFoa);
    
    //Parse the Import Name Table(INT) and Import Address Table(IAT);
    for(i = 0; (dwThkINT = dwaINT[i]) && (dwThkIAT = dwaIAT[i]); i++)
    {
      if(dwThkIAT == dwThkINT) //INT
      {
        if(IMAGE_SNAP_BY_ORDINAL32(dwThkINT))  //Import By Ordinal
        {
          wOrdinal = (WORD)IMAGE_ORDINAL32(dwThkINT);
          fprintf(stc_lpFdFile, " %08X  %08X  %08X  %04X  %s\n", dwThkINT, 0, dwThkIAT, wOrdinal, "by ordinal");
        }
        else                                   //Import By Name
        {
          dwFoa = stPeFile.Rva2Rda(dwThkINT);
          lpByName = (PIMAGE_IMPORT_BY_NAME)(stPeFile.strBase + dwFoa);
          fprintf(stc_lpFdFile, " %08X  %08X  %08X  %04X  %s\n", dwThkINT, dwFoa, dwThkIAT, lpByName->Hint, lpByName->Name);
        }
      }
      else                     //IAT
      {
        if(IMAGE_SNAP_BY_ORDINAL32(dwThkINT))  //Import By Ordinal
        {
          wOrdinal = (WORD)IMAGE_ORDINAL32(dwThkINT);
          fprintf(stc_lpFdFile, " %08X  %08X  %08X  %04X  %s\n", dwThkINT, 0, dwThkIAT, wOrdinal, "by ordinal");
        }
        else                                   //Import By Name
        {
          dwFoa = stPeFile.Rva2Rda(dwThkINT);
          lpByName = (PIMAGE_IMPORT_BY_NAME)(stPeFile.strBase + dwFoa);
          fprintf(stc_lpFdFile, " %08X  %08X  %08X  %04X  %s\n", dwThkINT, dwFoa, dwThkIAT, lpByName->Hint, lpByName->Name);
        }
      }
    }
    fprintf(stc_lpFdFile, "\n");
  }
  return TRUE;
}

static BOOL PE_ShowBaseRelocationTable(VOID)
{
  LPBYTE strBaseReloc = NULL;
  PIMAGE_BASE_RELOCATION lpBaseReloc = NULL;
  LPWORD awTypeOffsets = NULL;
  DWORD dwBytesOfBaseReloc = 0, dwNumberOfBaseReloc = 0;
  DWORD dwRda = 0, dwCodeOffset = 0;
  DWORD dwCodeRVA = 0, dwCodeVA = 0, dwMachineCode = 0;
  WORD i = 0, wTypeOffset = 0;
  LPBYTE strMachineCodes = NULL;
  
  fprintf(stc_lpFdFile, "==========================BASE RELOCATION TABLE====================================\n");
  if(0 == stPeFile.lpFmtPe->lpRlcBlk->Size)
  {
    fprintf(stc_lpFdFile, "This no Base Relocation Table\n\n");
    return FALSE;
  }

  lpBaseReloc = stPeFile.lpFmtPe->lpRlcTbl;
  strBaseReloc = (LPBYTE)lpBaseReloc;

  dwRda = stPeFile.Rva2Rda(lpBaseReloc->VirtualAddress);
  strMachineCodes = (LPBYTE)stPeFile.strBase + dwRda;
  fprintf(stc_lpFdFile, " VirtualAddress   : %08X -> %08X\n", lpBaseReloc->VirtualAddress, dwRda);
  fprintf(stc_lpFdFile, " SizeOfBlock      : %08X\n", lpBaseReloc->SizeOfBlock);
  dwBytesOfBaseReloc = lpBaseReloc->SizeOfBlock - 8;
  fprintf(stc_lpFdFile, " BytesOfBaseReloc : %08X\n", dwBytesOfBaseReloc);
  dwNumberOfBaseReloc = dwBytesOfBaseReloc / 2;
  fprintf(stc_lpFdFile, " NumberOfBaseReloc: %08X -> %u\n", dwNumberOfBaseReloc, dwNumberOfBaseReloc);
  
  fprintf(stc_lpFdFile, " *******************************************************\n");
  //awTypeOffsets = (LPWORD)(stPeFile.strBase + dwStart + 8);
  awTypeOffsets = (LPWORD)(strBaseReloc + 8);
  fprintf(stc_lpFdFile, " %-12s%-12s%-10s%-10s%s\n", "TypeOffset", "CodeOffset", "CodeRVA", "CodeVA", "MachineCode");
  for(i = 0; i < dwNumberOfBaseReloc; i++)
  {
    wTypeOffset = awTypeOffsets[i];
    dwCodeOffset = wTypeOffset & 0x0FFF;
    dwCodeRVA = (dwCodeOffset + lpBaseReloc->VirtualAddress);
    dwCodeVA  = (dwCodeOffset + lpBaseReloc->VirtualAddress + stPeFile.lpFmtPe->lpOptHdr->ImageBase);
    dwMachineCode = *((LPDWORD)(strMachineCodes + dwCodeOffset));
    fprintf(stc_lpFdFile, " %4X        %08X    %08X  %08X  %08X\n", wTypeOffset, dwCodeOffset, dwCodeRVA, dwCodeVA, dwMachineCode);
  }
  fprintf(stc_lpFdFile, "\n");
  return TRUE;
}

static BOOL PE_ShowImportAddressTable(VOID)
{
  PIMAGE_THUNK_DATA aIatTbl = NULL;
  PIMAGE_THUNK_DATA lpThunk = NULL;
  PIMAGE_IMPORT_BY_NAME lpByName = NULL;
  DWORD dwRda = 0;
  WORD i = 0;

  fprintf(stc_lpFdFile, "==========================IMPORT ADDRESS TABLE====================================\n");
  if(0 == stPeFile.lpFmtPe->lpIatBlk->Size)
  {
    fprintf(stc_lpFdFile, "This no Import Address Table\n\n");
    return FALSE;
  }
  
  fprintf(stc_lpFdFile, " %-11s%-11s%-7s%-s\n", "RVA", "FOA", "ORDI", "FUNC");
  
  aIatTbl = stPeFile.lpFmtPe->lpIatTbl;
  for(i = 0; aIatTbl[i].u1.AddressOfData; i++)
  {
    lpThunk = aIatTbl + i;
    //if(IMAGE_SNAP_BY_ORDINAL32(lpThunk->u1.Ordinal))
    if(lpThunk->u1.Ordinal & 0xF0000000)
    {
      fprintf(stc_lpFdFile, " %08X   %08X   %04X   %s\n", lpThunk->u1.AddressOfData, 0, 0, "Function Address");
    }
    else
    {
      dwRda = stPeFile.Rva2Rda(lpThunk->u1.Ordinal);
      lpByName = (PIMAGE_IMPORT_BY_NAME)(stPeFile.strBase + dwRda);
      fprintf(stc_lpFdFile, " %08X   %08X   %04X   %s\n", lpThunk->u1.AddressOfData, dwRda, lpByName->Hint, lpByName->Name);
    }
  }
  fprintf(stc_lpFdFile, "\n");
  return TRUE;
}

static WORD PE_SetFileName(LPCTSTR strFileName)
{
  WORD wLen = 0;
  
  if(NULL == strFileName)
  {
    return 0;
  }

  wLen = strlen(strFileName);
  if(0 == wLen)
  {
    return 0;
  }

  ZeroMemory((PVOID)stPeFile.strFile, (SIZE_T)FILE_PATH_LENGTH);
  CopyMemory((PVOID)stPeFile.strFile, (CONST VOID*)strFileName, (SIZE_T)wLen);
  //wRet = _snprintf((char*)stPeFile.strFile, FILE_PATH_LENGTH, "%s", strFileName);
  return wLen;
}

static BOOL PE_Open(LPCTSTR strFileName, LPDWORD lpdwReason)
{
  if(stPeFile.SetFileName(strFileName) == 0)
  {
    *lpdwReason = 1;
    return FALSE;
  }

  //open file
  stPeFile.lphFile = CreateFile((LPCTSTR)stPeFile.strFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
  if((INVALID_HANDLE_VALUE == stPeFile.lphFile) || (NULL == stPeFile.lphFile))
  {
    *lpdwReason = 2;
    return FALSE;
  }
  *lpdwReason = 0;
  return TRUE;
}

static BOOL PE_OpenEx(LPCTSTR strFileName, LPDWORD lpdwReason)
{
  if(stPeFile.SetFileName(strFileName) == 0)
  {
    *lpdwReason = 1;
    return FALSE;
  }
  
  //open file
  stPeFile.lphFile = CreateFile((LPCTSTR)stPeFile.strFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
  if((INVALID_HANDLE_VALUE == stPeFile.lphFile) || (NULL == stPeFile.lphFile))
  {
    *lpdwReason = 2;
    return FALSE;
  }

  //mapping the file into memory
  stPeFile.lphMapFile = CreateFileMapping(stPeFile.lphFile, NULL, PAGE_READONLY, 0, 0, NULL);
  if(NULL == stPeFile.lphMapFile)
  {
    CloseHandle(stPeFile.lphFile);
    stPeFile.lphFile = NULL;
    *lpdwReason = 3;
    return FALSE;
  }

  //mapping a view of a file into the address space of the calling process
  stPeFile.lpvBase = MapViewOfFile(stPeFile.lphMapFile, FILE_MAP_READ, 0, 0, 0);
  if(NULL == stPeFile.lpvBase)
  {
    CloseHandle(stPeFile.lphMapFile);
    stPeFile.lphMapFile = NULL;
    CloseHandle(stPeFile.lphFile);
    stPeFile.lphFile = NULL;
    *lpdwReason = 4;
    return FALSE;
  }
  
  stPeFile.strBase = (LPCSTR)(stPeFile.lpvBase);

  *lpdwReason = 0;
  return TRUE;
}

static BOOL PE_MapFile(DWORD dwProtect, LPWORD lpdwReason)
{
  if((INVALID_HANDLE_VALUE == stPeFile.lphFile) || (NULL == stPeFile.lphFile))
  {
    *lpdwReason = 1;
    return FALSE;
  }

  //mapping the file into memory
  stPeFile.lphMapFile = CreateFileMapping(stPeFile.lphFile, NULL, PAGE_READONLY, 0, 0, NULL);
  if(NULL == stPeFile.lphMapFile)
  {
    *lpdwReason = 2;
    return FALSE;
  }
  *lpdwReason = 0;
  return TRUE;
}

static BOOL PE_MapFileView(DWORD dwProtect, LPWORD lpdwReason)
{
  if(NULL == stPeFile.lphMapFile)
  {
    *lpdwReason = 1;
    return FALSE;
  }

  //mapping a view of a file into the address space of the calling process
  stPeFile.lpvBase = MapViewOfFile(stPeFile.lphMapFile, FILE_MAP_READ, 0, 0, 0);
  if(NULL == stPeFile.lpvBase)
  {
    *lpdwReason = 2;
    return FALSE;
  }
  
  stPeFile.strBase = (LPCSTR)(stPeFile.lpvBase);
  *lpdwReason = 0;
  return TRUE;
}

static BOOL PE_UnMapFileView(VOID)
{
  BOOL bRet = FALSE;
  if(NULL == stPeFile.lpvBase)
  {
    stPeFile.strBase = NULL;
    return FALSE;
  }

  bRet = UnmapViewOfFile(stPeFile.lpvBase); //release the file view
  if(bRet)
  {
    stPeFile.lpvBase = NULL;
    stPeFile.strBase = NULL;
  }
  return bRet;
}

static BOOL PE_Close(VOID)
{
  BOOL bRet = FALSE;
  
  if(NULL != stPeFile.lpvBase)
  {
    bRet = UnmapViewOfFile(stPeFile.lpvBase); //release the file view
    if(bRet)
    {
      stPeFile.lpvBase = NULL;
      stPeFile.strBase = NULL;
    }
  }
  
  if(NULL != stPeFile.lphMapFile)
  {
    bRet = CloseHandle(stPeFile.lphMapFile);   //close the file mapping object
    if(bRet)
    {
      stPeFile.lphMapFile = NULL;
    }
  }
  
  //close the file
  if((INVALID_HANDLE_VALUE != stPeFile.lphFile) && (NULL != stPeFile.lphFile))
  {
    bRet = CloseHandle(stPeFile.lphFile);
    if(bRet)
    {
      stPeFile.lphFile = NULL;
    }
  }
  return bRet;
};

static BOOL PE_OpenOutput(BYTE bMode)
{
  time_t curTime = 0;
  CHAR strTime[32];

  if(MODE_OUTPUT_FILE == bMode)
  {
    //make the file name for pe dump;
    ZeroMemory((PVOID)stc_strDumpFile, (SIZE_T)FILE_PATH_LENGTH);
    if(strlen(stPeFile.strFile))
    {
      ZeroMemory((PVOID)strTime, (SIZE_T)sizeof(strTime));
      curTime = time(NULL);
      strftime(strTime, sizeof(strTime), "%Y%m%d_%H%M%S", localtime((const time_t*)&curTime));
      _snprintf((char*)stc_strDumpFile, FILE_PATH_LENGTH, "%s_%s.txt", stPeFile.strFile, strTime);
    }
    else
    {
      _snprintf((char*)stc_strDumpFile, FILE_PATH_LENGTH, "%s", "dump.txt");
    }

    //open the dumpfile to save the pe detail;
    stc_lpFdFile = fopen(stc_strDumpFile, "a");
    if(NULL == stc_lpFdFile)
    {
      stc_bOutputMode = MODE_OUTPUT_SCREEN; //default: print all information onto screen;
      stc_lpFdFile = stdout;                //default: print all information onto screen;
      printf("failed to open the dumpfile: %s\n", stc_strDumpFile);
    }
    else
    {
      stc_bOutputMode = MODE_OUTPUT_FILE;   //all information are output into file;
    }
  }
  else
  {
    stc_bOutputMode = MODE_OUTPUT_SCREEN;   //print all information onto screen;
    stc_lpFdFile = stdout;                  //print all information onto screen;
  }
  return TRUE;
}

static BOOL PE_CloseOutput(VOID)
{
  if(MODE_OUTPUT_FILE == stc_bOutputMode)   //if output into file, then, close the dumpfile
  {
    if(stc_lpFdFile)
    {
      fflush(stc_lpFdFile);
      fclose(stc_lpFdFile);
    }
    printf("all information output into the dumpfile: %s\nplease looks at it!!!\n", stc_strDumpFile);
  }
  return TRUE;
}

struct SPeFile* AcquirePeFile(void)
{
  stPeFile.lpFmtPe = NULL;
  stPeFile.lpFmtPe = (struct SPeFormat*)malloc(sizeof(struct SPeFormat));
  if(NULL == stPeFile.lpFmtPe)
  {
    return (struct SPeFile*)NULL;
  }

  stPeFile.SetFileName = PE_SetFileName;
  stPeFile.Open    = PE_Open;
  stPeFile.OpenEx  = PE_OpenEx;
  stPeFile.Close   = PE_Close;
  stPeFile.Rva2Rda = PE_Rva2Rda;
  stPeFile.Rda2Rva = PE_Rda2Rva;

  stPeFile.MapFile       = PE_MapFile;
  stPeFile.MapFileView   = PE_MapFileView;
  stPeFile.UnMapFileView = PE_UnMapFileView;

  stPeFile.OpenOutput  = PE_OpenOutput;
  stPeFile.CloseOutput = PE_CloseOutput;
  
  stPeFile.Parse       = PE_Parse;
  stPeFile.ParseDosHdr = PE_ShowDosHdr;
  stPeFile.ParseNtHdr  = PE_ShowNtHeader;
  stPeFile.ParseFilHdr = PE_ShowFileHeader;
  stPeFile.ParseOptHdr = PE_ShowOptionHeader;
  stPeFile.ParseDatDir = PE_ShowDataDirectory;
  stPeFile.ParseExpTbl = PE_ShowExportTable;
  stPeFile.ParseImpTbl = PE_ShowImportTable;
  stPeFile.ParseBlkTbl = PE_ShowSectionTable;
  stPeFile.ParseRlcTbl = PE_ShowBaseRelocationTable;
  stPeFile.ParseIatTbl = PE_ShowImportAddressTable;

  stc_bOutputMode = MODE_OUTPUT_SCREEN; //default: print all information onto screen;
  stc_lpFdFile = stdout;                //default: print all information onto screen;
  return (&stPeFile);
};

void ReleasePeFile(void)
{
  if(stPeFile.lpFmtPe)
  {
    free(stPeFile.lpFmtPe);
    stPeFile.lpFmtPe = NULL;
  }
};
