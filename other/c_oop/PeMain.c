#include <stdio.h>
#include <windows.h>
#include "PeFile.h"

int main(int argc, char** argv)
{
  BOOL bRet = FALSE;
  struct SPeFile* lpFilePe = NULL;
  char* strFilePath = NULL;
  DWORD dwReason = 0;

  if(argc < 2)
  {
    printf("usage: %s <file_path>\n", argv[0]);
    return -1;
  }

  strFilePath = argv[1];
  printf("\nparsing file %s\n", strFilePath);

  lpFilePe = AcquirePeFile();
  if(NULL == lpFilePe)
  {
    printf("ERROR: invalid object\n");
    return -2;
  }

  if(NULL == lpFilePe->OpenEx)
  {
    printf("ERROR: OpenEx = NULL\n");
    return -3;
  }

  bRet = lpFilePe->OpenEx(strFilePath, &dwReason);
  if(FALSE == bRet)
  {
    printf("ERROR: failed to open file %s, reason = %lu\n", strFilePath, dwReason);
    return -4;
  }

  if(lpFilePe->OpenOutput)
  {
    lpFilePe->OpenOutput(MODE_OUTPUT_FILE);
  }

  printf("open file %s ok\n", strFilePath);
  bRet = lpFilePe->Parse(&dwReason);
  if(FALSE == bRet)
  {
    printf("ERROR: failed to parse pe file %s, reason = %lu\n", strFilePath, dwReason);
    goto do_end;
  }
  printf("parse pe file %s ok\n", strFilePath);

  if(lpFilePe->ParseDosHdr)
  {
    lpFilePe->ParseDosHdr();
  }

  if(lpFilePe->ParseNtHdr)
  {
    lpFilePe->ParseNtHdr();
  }

  if(lpFilePe->ParseBlkTbl)
  {
    lpFilePe->ParseBlkTbl();
  }

  if(lpFilePe->ParseExpTbl)
  {
    lpFilePe->ParseExpTbl();
  }

  if(lpFilePe->ParseImpTbl)
  {
    lpFilePe->ParseImpTbl();
  }

  if(lpFilePe->ParseRlcTbl)
  {
    lpFilePe->ParseRlcTbl();
  }

  if(lpFilePe->ParseIatTbl)
  {
    lpFilePe->ParseIatTbl();
  }

do_end:
  bRet = lpFilePe->Close();

  if(lpFilePe->CloseOutput)
  {
    lpFilePe->CloseOutput();
  }

  ReleasePeFile();
  return 0;
}
