#include <stdio.h>
#include <time.h>
#include <string.h>
#include <windows.h>

#define SECTION_MAX_NUMBER 16

static char* strDataDirType[16] =
{
  "Export Table", //0
  "Import Table", //1
  "Resource Table", //2
  "Exception Table", //3
  "Security Table", //4
  "Base Relocation Table", //5
  "Debug Information Table", //6
  "Architecture Specific Data", //7
  "RVA of GP", //8
  "TLS Data", //9
  "Load Configuration Directory", //10
  "Bound Import Directory", //11
  "Import Address Table", //12
  "Delay Load Import Descriptors", //13
  "COM Runtime descriptor", //14
  "UnUsed" //15
};

char* getMachineString(WORD machine)
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

char* getSubsystemString(WORD subsystem)
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
      name = "Windows GUI";
    }
    break;

    case IMAGE_SUBSYSTEM_WINDOWS_CUI:
    {
      name = "Windows CUI";
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

void showDosHeader(PIMAGE_DOS_HEADER lpHeader)
{
  char* tmp = NULL;
  WORD* w = NULL;
  printf("================DOS HEADER================\n");
  tmp = (char*)&(lpHeader->e_magic);
  printf("e_magic   : %04Xh -> %c%c\n", lpHeader->e_magic, tmp[0], tmp[1]);
  printf("e_cblp    : %04Xh\n", lpHeader->e_cblp);
  printf("e_cp      : %04Xh\n", lpHeader->e_cp);
  printf("e_crlc    : %04Xh\n", lpHeader->e_crlc);
  printf("e_cparhdr : %04Xh\n", lpHeader->e_cparhdr);
  printf("e_minalloc: %04Xh\n", lpHeader->e_minalloc);
  printf("e_maxalloc: %04Xh\n", lpHeader->e_maxalloc);
  printf("e_ss      : %04Xh\n", lpHeader->e_ss);
  printf("e_sp      : %04Xh\n", lpHeader->e_sp);
  printf("e_csum    : %04Xh\n", lpHeader->e_csum);
  printf("e_ip      : %04Xh\n", lpHeader->e_ip);
  printf("e_cs      : %04Xh\n", lpHeader->e_cs);
  printf("e_lfarlc  : %04Xh\n", lpHeader->e_lfarlc);
  printf("e_ovno    : %04Xh\n", lpHeader->e_ovno);

  w = lpHeader->e_res;
  printf("e_res     : %04X %04X %04X %04Xh\n", w[0], w[1], w[2], w[3]);
  printf("e_oemid   : %04Xh\n", lpHeader->e_oemid);
  printf("e_oeminfo : %04Xh\n", lpHeader->e_oeminfo);

  w = lpHeader->e_res2;
  printf("e_res2    : %04X %04X %04X %04X %04X %04X %04X %04X %04X %04Xh\n", w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9]);
  printf("e_lfanew  : %08Xh\n\n", lpHeader->e_lfanew);
}

void showFileHeader(PIMAGE_FILE_HEADER lpHeader)
{
  char strTime[32];
  printf("================FILE HEADER================\n");
  printf("Machine             : %04Xh -> %s\n", lpHeader->Machine, getMachineString(lpHeader->Machine));
  printf("NumberOfSections    : %04Xh\n", lpHeader->NumberOfSections);

  ZeroMemory((PVOID)strTime, (SIZE_T)sizeof(strTime));
  strftime(strTime, sizeof(strTime), "%Y-%m-%d %H:%M:%S", localtime((const time_t*)&(lpHeader->TimeDateStamp)));
  printf("TimeDateStamp       : %08Xh -> %s\n", lpHeader->TimeDateStamp, strTime);
  printf("PointerToSymbolTable: %08Xh\n", lpHeader->PointerToSymbolTable);
  printf("NumberOfSymbols     : %08Xh\n", lpHeader->NumberOfSymbols);
  printf("SizeOfOptionalHeader: %04Xh\n", lpHeader->SizeOfOptionalHeader);
  printf("Characteristics     : %04Xh\n\n", lpHeader->Characteristics);
}

void showDataDirectory(IMAGE_DATA_DIRECTORY datas[], DWORD count)
{
  DWORD i = 0;
  PIMAGE_DATA_DIRECTORY lpData = NULL;

  if(count > IMAGE_NUMBEROF_DIRECTORY_ENTRIES)
  {
    count = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;
  }

  printf("==========================DATA DIRECTORY==========================\n");
  printf("%6s  %-30s %-16s %-12s\n", "Type", "Name", "VirtualAddress", "Size");
  for(i = 0; i < count; i++)
  {
    lpData = &datas[i];
    //lpData = datas + i;
    printf("%6d  %-30s %08Xh        %08Xh\n", i, strDataDirType[i], lpData->VirtualAddress, lpData->Size);
  }
  printf("\n");
}

void showOptionHeader32(PIMAGE_OPTIONAL_HEADER32 lpHeader)
{
  printf("================OPTIONAL HEADER================\n");
  printf("Magic                      : %04Xh\n", lpHeader->Magic);
  printf("MajorLinkerVersion         : %02Xh\n", lpHeader->MajorLinkerVersion);
  printf("MinorLinkerVersion         : %02Xh\n", lpHeader->MinorLinkerVersion);
  printf("SizeOfCode                 : %08Xh\n", lpHeader->SizeOfCode);
  printf("SizeOfInitializedData      : %08Xh\n", lpHeader->SizeOfInitializedData);
  printf("SizeOfInitializedData      : %08Xh\n", lpHeader->SizeOfUninitializedData);
  printf("AddressOfEntryPoint        : %08Xh\n", lpHeader->AddressOfEntryPoint);
  printf("BaseOfCode                 : %08Xh\n", lpHeader->BaseOfCode);
  printf("BaseOfData                 : %08Xh\n", lpHeader->BaseOfData);
  printf("ImageBase                  : %08Xh\n", lpHeader->ImageBase);
  printf("SectionAlignment           : %08Xh\n", lpHeader->SectionAlignment);
  printf("FileAlignment              : %08Xh\n", lpHeader->FileAlignment);
  printf("MajorOperatingSystemVersion: %04Xh\n", lpHeader->MajorOperatingSystemVersion);
  printf("MajorOperatingSystemVersion: %04Xh\n", lpHeader->MinorOperatingSystemVersion);
  printf("MajorImageVersion          : %04Xh\n", lpHeader->MajorImageVersion);
  printf("MinorImageVersion          : %04Xh\n", lpHeader->MinorImageVersion);
  printf("MajorSubsystemVersion      : %04Xh\n", lpHeader->MajorSubsystemVersion);
  printf("MajorSubsystemVersion      : %04Xh\n", lpHeader->MinorSubsystemVersion);
  printf("Win32VersionValue          : %08Xh\n", lpHeader->Win32VersionValue);
  printf("SizeOfImage                : %08Xh\n", lpHeader->SizeOfImage);
  printf("SizeOfHeaders              : %08Xh\n", lpHeader->SizeOfHeaders);
  printf("CheckSum                   : %08Xh\n", lpHeader->CheckSum);
  printf("Subsystem                  : %04Xh -> %s\n", lpHeader->Subsystem, getSubsystemString(lpHeader->Subsystem));
  printf("DllCharacteristics         : %04Xh\n", lpHeader->DllCharacteristics);
  printf("Win32VersionValue          : %08Xh\n", lpHeader->SizeOfStackReserve);
  printf("SizeOfStackCommit          : %08Xh\n", lpHeader->SizeOfStackCommit);
  printf("SizeOfHeapReserve          : %08Xh\n", lpHeader->SizeOfHeapReserve);
  printf("SizeOfHeapCommit           : %08Xh\n", lpHeader->SizeOfHeapCommit);
  printf("LoaderFlags                : %08Xh\n", lpHeader->LoaderFlags);
  printf("NumberOfRvaAndSizes        : %08Xh\n\n", lpHeader->NumberOfRvaAndSizes);
  showDataDirectory(lpHeader->DataDirectory, lpHeader->NumberOfRvaAndSizes);
}

void showNtHeader(PIMAGE_NT_HEADERS32 lpHeader)
{
  char* tmp = NULL;
  printf("================NT HEADER================\n");
  tmp = (char*)&(lpHeader->Signature);
  printf("Signature: %08Xh -> %c%c%d%d\n\n", lpHeader->Signature, tmp[0], tmp[1], tmp[2], tmp[3]);
  showFileHeader(&(lpHeader->FileHeader));
  showOptionHeader32(&(lpHeader->OptionalHeader));
}

void showSectionTable(IMAGE_SECTION_HEADER sections[], DWORD count)
{
  DWORD i = 0;
  PIMAGE_SECTION_HEADER lpSection = NULL;

  if(count > SECTION_MAX_NUMBER)
  {
    count = SECTION_MAX_NUMBER;
  }
  printf("==========================SECTION TABLE==============================================================\n");
  printf("%4s  %-10s %-16s %-14s %-18s %-16s %-16s\n", "Seq", "Name", "VirtualAddress", "VirtualSize", "PointerToRawData", "SizeOfRawData", "Characteristics");
  for(i = 0; i < count; i++)
  {
    lpSection = sections + i;
    printf("%4d  %-10s %08Xh        %08Xh      %08Xh          %08Xh        %08Xh\n", i, lpSection->Name, lpSection->VirtualAddress, lpSection->Misc.VirtualSize, lpSection->PointerToRawData, lpSection->SizeOfRawData, lpSection->Characteristics);
  }
}

int main(int argc, char** argv)
{
  BOOL ret = FALSE;
  char* strFilePath = NULL;
  HANDLE hFile = NULL;
  IMAGE_DOS_HEADER stHeaderDos;
  PIMAGE_DOS_HEADER lpHeaderDos = NULL;
  IMAGE_NT_HEADERS stHeaderNt;
  PIMAGE_NT_HEADERS lpHeaderNt = NULL;
  IMAGE_SECTION_HEADER stSectionTable[SECTION_MAX_NUMBER];
  DWORD dwBytes = 0, dwReaded = 0;

  if(argc < 2)
  {
    printf("usage: %s <file_path>\n", argv[0]);
    return -1;
  }

  strFilePath = argv[1];

  //open file
  hFile = CreateFile((LPCTSTR)strFilePath,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
  if(INVALID_HANDLE_VALUE == hFile)
  {
    printf("failed to open file %s\n", strFilePath);
    return -2;
  }

  //read DOS Header
  dwBytes = sizeof(IMAGE_DOS_HEADER);
  lpHeaderDos = &stHeaderDos;
  ZeroMemory((PVOID)lpHeaderDos, (SIZE_T)dwBytes);
  ret = ReadFile(hFile, (LPVOID)lpHeaderDos, dwBytes, &dwReaded, NULL);
  if(FALSE == ret)
  {
    CloseHandle(hFile);
    printf("failed to read dos-header\n");
    return -3;
  }

  if(IMAGE_DOS_SIGNATURE != lpHeaderDos->e_magic)
  {
    CloseHandle(hFile);
    printf("Unknow File Format\n");
    return -4;
  }

  showDosHeader(lpHeaderDos);

  //read NT Header
  SetFilePointer(hFile, lpHeaderDos->e_lfanew - dwBytes, NULL, FILE_CURRENT);
  dwBytes = sizeof(IMAGE_NT_HEADERS);
  lpHeaderNt = &stHeaderNt;
  ZeroMemory((PVOID)lpHeaderNt, dwBytes);
  ret = ReadFile(hFile, (LPVOID)lpHeaderNt, dwBytes, &dwReaded, NULL);
  if(FALSE == ret)
  {
    CloseHandle(hFile);
    printf("failed to read nt-header\n");
    return -5;
  }

  if(IMAGE_NT_SIGNATURE != lpHeaderNt->Signature)
  {
    CloseHandle(hFile);
    printf("Not a NT File\n");
    return -6;
  }

  showNtHeader(lpHeaderNt);

  //read Section Table
  dwBytes = sizeof(IMAGE_SECTION_HEADER);
  ZeroMemory((PVOID)stSectionTable, sizeof(stSectionTable));
  dwBytes *= lpHeaderNt->FileHeader.NumberOfSections; 
  ret = ReadFile(hFile, (LPVOID)stSectionTable, dwBytes, &dwReaded, NULL);
  if(FALSE == ret)
  {
    CloseHandle(hFile);
    printf("failed to read section table\n");
    return -7;
  }

  showSectionTable(stSectionTable, lpHeaderNt->FileHeader.NumberOfSections);
  
  //close the file
  if((NULL != hFile) && (INVALID_HANDLE_VALUE != hFile))
  {
    CloseHandle(hFile);
  }
  return 0;
}