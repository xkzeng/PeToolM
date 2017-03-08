#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <winnt.h>

static char* DataDirTyes[16] =
{
  "Export Table",     //0
  "Import Table",     //1
  "Resource Table",   //2
  "Exception",        //3
  "Security",         //4
  "Relocation Table", //5
  "Debug Info",       //6
  "Copy Right Info",  //7
  "Global PTR",       //8
  "TLS Table",        //9
  "Load Config",      //10
  "Bound Import",     //11
  "IAT",              //12
  "Delay Import",     //13
  "Com Descriptor",   //14
  "UnUsed"            //15
};

void showDosHeader(PIMAGE_DOS_HEADER lpHeader)
{
  unsigned char* tmp = NULL;
  printf("====================DOS HEADER====================\n");
  tmp = (unsigned char*)&(lpHeader->e_magic);
  printf("e_magic : %04Xh %c%c\n", lpHeader->e_magic, tmp[0], tmp[1]);
  printf("e_lfanew: %08Xh\n", lpHeader->e_lfanew);
}

void showFileHeader(PIMAGE_FILE_HEADER lpHeader)
{
  char strTime[32];
  printf("====================FILE HEADER====================\n");
  //tmp = (unsigned char*)&(lpHeader->e_magic);
  printf("Machine             : %04X\n", lpHeader->Machine);
  printf("NumberOfSections    : %04Xh\n", lpHeader->NumberOfSections);

  memset(strTime, 0, sizeof(strTime));
  strftime(strTime, sizeof(strTime), "%Y-%m-%d %H:%M:%S", localtime((time_t*)&(lpHeader->TimeDateStamp)));
  printf("TimeDateStamp       : %08Xh --> %s\n", lpHeader->TimeDateStamp, strTime);
  printf("PointerToSymbolTable: %08Xh\n", lpHeader->PointerToSymbolTable);
  printf("NumberOfSymbols     : %08Xh\n", lpHeader->NumberOfSymbols);
  printf("SizeOfOptionalHeader: %04Xh\n", lpHeader->SizeOfOptionalHeader);
  printf("Characteristics     : %08Xh\n", lpHeader->Characteristics);
}

void showDataDirectory(IMAGE_DATA_DIRECTORY DataDirectory[], int count)
{
  int i = 0;
  PIMAGE_DATA_DIRECTORY lpItem = NULL;
  
  if(count > IMAGE_NUMBEROF_DIRECTORY_ENTRIES)
  {
    printf("Warning: the number of data directories is too larg!!! the largest is %d, you entered %d\n", IMAGE_NUMBEROF_DIRECTORY_ENTRIES, count);
    count = IMAGE_NUMBEROF_DIRECTORY_ENTRIES;
  }

  printf("====================DATA DIRECTORIES====================\n");
  printf("%-6s  %-16s  %-16s  %-10s\n", "Index", "Name", "VirtualAddress", "Size");
  for(i = 0; i < count; i++)
  {
    lpItem = &DataDirectory[i];
    printf("%-6d  %-16s  %08Xh         %08Xh\n", i, DataDirTyes[i], lpItem->VirtualAddress, lpItem->Size);
  }
}

void showOptionHeader32(PIMAGE_OPTIONAL_HEADER32 lpHeader)
{
  //unsigned char* tmp = NULL;
  printf("====================OPTIONAL HEADER32====================\n");
  printf("Magic              : %04Xh\n", lpHeader->Magic);
  printf("AddressOfEntryPoint: %08Xh\n", lpHeader->AddressOfEntryPoint);
  printf("ImageBase          : %08Xh\n", lpHeader->ImageBase);
  printf("SectionAlignment   : %08Xh\n", lpHeader->SectionAlignment);
  printf("FileAlignment      : %08Xh\n", lpHeader->FileAlignment);
  printf("BaseOfCode         : %08Xh\n", lpHeader->BaseOfCode);
  printf("BaseOfData         : %08Xh\n", lpHeader->BaseOfData);
  printf("NumberOfRvaAndSizes: %08Xh\n", lpHeader->NumberOfRvaAndSizes);
  showDataDirectory(lpHeader->DataDirectory, lpHeader->NumberOfRvaAndSizes);
}

void showNtHeader(PIMAGE_NT_HEADERS lpHeader)
{
  unsigned char* tmp = NULL;
  printf("====================NT HEADER====================\n");
  tmp = (unsigned char*)&(lpHeader->Signature);
  printf("Signature: %08Xh %c%c%d%d\n", lpHeader->Signature, tmp[0],tmp[1],tmp[2],tmp[3]);
  showFileHeader(&(lpHeader->FileHeader));
  showOptionHeader32(&(lpHeader->OptionalHeader));
}

void showSectionTables(IMAGE_SECTION_HEADER sections[], WORD wCount)
{
  WORD i = 0;
  PIMAGE_SECTION_HEADER lpSection = NULL;
  
  printf("====================SECTION TABLE====================\n");
  //printf("%-10s %-18s %-16s %-16s %-14s %-16\n", "Name", "PointerToRawData", "SizeOfRawData", "VirtualAddress", "VirtualSize", "Characteristics");
  printf("Name      PointerToRawData  SizeOfRawData   VirtualAddress  VirtualSize   Characteristics\n");
  for(i = 0; i < wCount; i++)
  {
    lpSection = &sections[i];
    printf("%-10s%08Xh         %08Xh       %08Xh       %08Xh     %08Xh\n", lpSection->Name, lpSection->PointerToRawData, lpSection->SizeOfRawData,
      lpSection->VirtualAddress, lpSection->Misc.VirtualSize, lpSection->Characteristics);
  }
}

DWORD RVA2RDA(IMAGE_SECTION_HEADER sections[], WORD wCount, DWORD dwRva)
{
  DWORD dwRdaRet = 0, dwBegin = 0, dwEnd = 0;
  WORD  i = 0;
  PIMAGE_SECTION_HEADER lpSection = NULL;

  for(i = 0; i < wCount; i++)
  {
    lpSection = &sections[i];
    dwBegin = lpSection->VirtualAddress;
    dwEnd   = lpSection->VirtualAddress + lpSection->Misc.VirtualSize;
    if(dwRva >= dwBegin && dwRva <= dwEnd)
    {
      dwRdaRet = (dwRva - dwBegin) + lpSection->PointerToRawData;
      break;
    }
  }
  return dwRdaRet;
}

DWORD RDA2RVA(IMAGE_SECTION_HEADER sections[], WORD wCount, DWORD dwRda)
{
  DWORD dwRvaRet = 0, dwBegin = 0, dwEnd = 0;
  WORD  i = 0;
  PIMAGE_SECTION_HEADER lpSection = NULL;
  
  for(i = 0; i < wCount; i++)
  {
    lpSection = &sections[i];
    dwBegin = lpSection->PointerToRawData;
    dwEnd   = lpSection->PointerToRawData + lpSection->SizeOfRawData;
    if(dwRda >= dwBegin && dwRda <= dwEnd)
    {
      dwRvaRet = (dwRda - dwBegin) + lpSection->VirtualAddress;
      break;
    }
  }
  return dwRvaRet;
}

int main(int argc, char** argv)
{
  int ret = 0, bytes = 0;
  unsigned char* tmp = NULL;
  char* file = NULL;
  struct _stat stat;
  FILE* fd = NULL;
  IMAGE_DOS_HEADER stHeaderDos;
  PIMAGE_DOS_HEADER lpHeaderDos = NULL;
  IMAGE_NT_HEADERS32 stHeaderNT;
  PIMAGE_NT_HEADERS32 lpHeaderNT = NULL;
  IMAGE_SECTION_HEADER arrSections[16];
  WORD wSectionCount = 0, wDllCount = 0, i = 0, wOrdinal = 0;
  IMAGE_IMPORT_DESCRIPTOR stIIDs[10];
  IMAGE_BASE_RELOCATION stBaseReloc;
  DWORD dwStart = 0, dwPos1 = 0, dwPos2 = 0;
  char strTmp[128], strTmp1[32];
  IMAGE_THUNK_DATA32 stThunk;
  PIMAGE_IMPORT_BY_NAME lpImportByName = NULL;
  DWORD dwCountBaseReloc = 0, dwRda = 0, dwCodeAddr = 0, dwCodeIdx = 0;
  WORD awTypeOffset[512], wTypeOffset = 0;
  unsigned char strCode[0x3000];

  printf("stImportByName = %d\n", sizeof(IMAGE_IMPORT_BY_NAME));

  if(argc < 2)
  {
    printf("usage: %s <pe_file_path>\n", argv[0]);
    return -1;
  }

  file = argv[1];
  bytes = sizeof(struct _stat);
  memset(&stat, 0, bytes);
  ret = _stat(file, &stat);
  if(ret != 0)
  {
    printf("get state of file %s failed[%d]:%s\n", argv[0], ret, strerror(errno));
    return -2;
  }
  printf("file size: %d\n", stat.st_size);

  fd = fopen(file, "rb");
  if(NULL == fd)
  {
    printf("failed to open file %s:%s\n", file, strerror(errno));
    return -3;
  }

  //read dos header;
  bytes = sizeof(IMAGE_DOS_HEADER);
  lpHeaderDos = &stHeaderDos;
  memset(lpHeaderDos, 0, bytes);
  ret = fread((void*)lpHeaderDos, bytes, 1, fd);
  printf("read bytes = %d, size = %d\n", ret, bytes);
  showDosHeader(lpHeaderDos);

  //read pe header;
  ret = fseek(fd, lpHeaderDos->e_lfanew - bytes, SEEK_CUR);
  if(0 != ret)
  {
    fclose(fd);
    printf("failed to seek1 file %s:%s\n", file, strerror(errno));
    return -4;
  }

  bytes = sizeof(IMAGE_NT_HEADERS32);
  lpHeaderNT = &stHeaderNT;
  memset(lpHeaderNT, 0, bytes);
  ret = fread((void*)lpHeaderNT, bytes, 1, fd);
  showNtHeader(lpHeaderNT);

  //read section table
  bytes = sizeof(IMAGE_SECTION_HEADER);
  wSectionCount = lpHeaderNT->FileHeader.NumberOfSections;
  memset(arrSections, 0, sizeof(arrSections));
  ret = fread(arrSections, bytes, wSectionCount, fd);
  printf("read %d sections, per size = %d\n", ret, bytes);
  showSectionTables(arrSections, wSectionCount);

  bytes = 0x2034;
  printf("%08Xh ---> %08Xh\n", bytes, RVA2RDA(arrSections, wSectionCount, bytes));

  bytes = 0x0634;
  printf("%08Xh ---> %08Xh\n", bytes, RDA2RVA(arrSections, wSectionCount, bytes));

  //read Import Table
  wDllCount = 0;
  dwStart = RVA2RDA(arrSections, wSectionCount, lpHeaderNT->OptionalHeader.DataDirectory[1].VirtualAddress);
  printf("start address of the import table = %08Xh\n", dwStart);
  ret = fseek(fd, dwStart, SEEK_SET);
  if(0 != ret)
  {
    fclose(fd);
    printf("failed to seek2 file %s:%s\n", file, strerror(errno));
    return -5;
  }
  
  bytes =  sizeof(IMAGE_IMPORT_DESCRIPTOR);
  memset((void*)stIIDs, 0, sizeof(stIIDs));
  while(TRUE)
  {
    
    ret = fread((void*)(&stIIDs[wDllCount]), bytes, 1, fd);
    //if(0 != ret)
    //{
    //  printf("failed to read import table %s:%s\n", file, strerror(errno));
    //  break;
    //}
    
    if(0 == stIIDs[wDllCount].Name)
    {
      break;
    }
    printf("DLL %08Xh, %08Xh\n", stIIDs[wDllCount].Name, stIIDs[wDllCount].OriginalFirstThunk);
    wDllCount++;
  }
  
  printf("wDllCount = %d\n", wDllCount);
  printf("====================IMPORT TABLE====================\n");
  lpImportByName = (PIMAGE_IMPORT_BY_NAME)strTmp1;
  for(i = 0; i < wDllCount; i++)
  {
    memset((void*)strTmp, 0, sizeof(strTmp));
    dwPos1 = RVA2RDA(arrSections, wSectionCount, stIIDs[i].Name);
    fseek(fd, dwPos1, SEEK_SET);
    fread(strTmp, 16, 1, fd);
    //printf("DLL %08Xh, %08Xh, %s, %08Xh, %08Xh\n", stIIDs[i].Name, dwPos1, strTmp, stIIDs[i].OriginalFirstThunk, RVA2RDA(arrSections, wSectionCount, stIIDs[i].OriginalFirstThunk));
    printf("DLL %s:\n", strTmp);
    dwPos1 = RVA2RDA(arrSections, wSectionCount, stIIDs[i].OriginalFirstThunk);
    //fseek(fd, dwPos1, SEEK_SET);
    printf("%-10s %-10s %-6s %-20s\n", "RVA", "RDA", "HINT", "FUNCTION");
    while(TRUE)
    {
      fseek(fd, dwPos1, SEEK_SET);
      ret = fread(&stThunk, sizeof(IMAGE_THUNK_DATA32), 1, fd);
      if(0 == stThunk.u1.AddressOfData)
      {
        break;
      }
      dwPos1 = dwPos1 + sizeof(IMAGE_THUNK_DATA32);
      dwPos2 = RVA2RDA(arrSections, wSectionCount, stThunk.u1.Ordinal);
      fseek(fd, dwPos2, SEEK_SET);
      memset((void*)strTmp1, 0, sizeof(strTmp1));
      ret = fread(strTmp1, sizeof(strTmp1), 1, fd);
      if(IMAGE_SNAP_BY_ORDINAL32(stThunk.u1.Ordinal))
      {
        wOrdinal = (WORD)IMAGE_ORDINAL32(stThunk.u1.Ordinal);
        printf("%08Xh  %08Xh  %04Xh  %s\n", stThunk.u1.AddressOfData, dwPos2, wOrdinal, "By Ordinal");
      }
      else
      {
        printf("%08Xh  %08Xh  %04Xh  %s\n", stThunk.u1.AddressOfData, dwPos2, lpImportByName->Hint, lpImportByName->Name);
      }
    }
    printf("\n");
  }

  //read .text
  dwStart = RVA2RDA(arrSections, wSectionCount, 0x1000);
  fseek(fd, dwStart, SEEK_SET);
  printf("start address of code = %08Xh\n", dwStart);

  bytes = sizeof(strCode);
  memset(strCode, 0, bytes);
  ret = fread((void*)strCode, bytes, 1, fd);
  printf("read Code = %d\n", ret);

  //Base relocation table
  dwStart = RVA2RDA(arrSections, wSectionCount, lpHeaderNT->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);
  printf("start address of the base relocation table = %08Xh\n", dwStart);
  fseek(fd, dwStart, SEEK_SET);

  bytes = sizeof(IMAGE_BASE_RELOCATION);
  memset((void*)&stBaseReloc, 0, bytes);
  ret = fread(&stBaseReloc, bytes, 1, fd);

  printf("============BASE RELOCATION TABLE==============\n");
  dwRda = RVA2RDA(arrSections, wSectionCount, stBaseReloc.VirtualAddress);
  printf(" VirtualAddress    : %08Xh -> %08Xh\n", stBaseReloc.VirtualAddress, dwRda);
  dwCountBaseReloc = stBaseReloc.SizeOfBlock - 8;
  printf(" SizeOfBlock       : %08Xh\n", stBaseReloc.SizeOfBlock);
  printf(" BytesOfBaseRelocs : %08Xh\n", dwCountBaseReloc);
  dwCountBaseReloc /= 2;
  printf(" NumberOfBaseRelocs: %08Xh\n", dwCountBaseReloc);

  memset((void*)awTypeOffset, 0, sizeof(awTypeOffset));
  ret = fread(awTypeOffset, sizeof(2), dwCountBaseReloc, fd);
  printf("read CountBaseReloc = %X (%d)\n", ret, ret);
  
  fseek(fd, dwRda, SEEK_SET);

  bytes = sizeof(strCode);
  memset(strCode, 0, bytes);
  ret = fread((void*)strCode, bytes, 1, fd);

  if(dwCountBaseReloc > 10)
  {
    dwCountBaseReloc = 10;
  }
  
  printf(" *******************************************\n");
  printf(" %-12s%-11s%s\n", "TypeOffset", "Address", "InsAddress");
  for(i = 0; i < dwCountBaseReloc; i++)
  {
    wTypeOffset = awTypeOffset[i];
    dwCodeIdx = (wTypeOffset & 0x0FFF);
    if(dwCodeIdx < 0x3000)
    {
      dwCodeAddr = *((LPDWORD)(strCode + dwCodeIdx));
      printf(" %8X    %08X   %08X\n", wTypeOffset, (dwCodeIdx + lpHeaderNT->OptionalHeader.ImageBase + stBaseReloc.VirtualAddress), dwCodeAddr);
    }
    else
    {
      dwCodeAddr = (DWORD)(strCode + dwCodeIdx);
      printf(" %8X    %08X   %s\n", wTypeOffset, (dwCodeIdx + lpHeaderNT->OptionalHeader.ImageBase + stBaseReloc.VirtualAddress), "*****");
    }
  }

  if(NULL != fd)
  {
    fclose(fd);
  }
  return 0;
}





















