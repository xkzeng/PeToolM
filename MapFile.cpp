#include "stdafx.h"
#include "MapFile.h"
#include <string.h>

CMapFile::CMapFile(void)
{
  ZeroMemory((PVOID)(this->m_strFileName), (SIZE_T)FILE_PATH_LENGTH);
  this->m_hFile = NULL;
  this->m_hMappingFile = NULL;
  this->m_lpvBase = NULL;
  this->m_strBase = NULL;
}

CMapFile::~CMapFile(void)
{
  ZeroMemory((PVOID)(this->m_strFileName), (SIZE_T)FILE_PATH_LENGTH);
  this->m_hFile = NULL;
  this->m_hMappingFile = NULL;
  this->m_lpvBase = NULL;
  this->m_strBase = NULL;
}

WORD CMapFile::SetFileName(LPCTSTR strFileName)
{
  WORD wLen = 0;

  if((NULL == strFileName) || (strlen(strFileName) == 0))
  {
    return 0;
  }
  
  wLen = _snprintf(this->m_strFileName, FILE_PATH_LENGTH, "%s", strFileName);
  return wLen;
}

LPCTSTR CMapFile::GetFileName(void)
{
  return (this->m_strFileName);
}

BOOL CMapFile::Open(LPCTSTR strFileName, DWORD dwDesiredAccess/* = GENERIC_READ*/, DWORD dwShareMode/* = FILE_SHARE_READ*/)
{
  if(this->SetFileName(strFileName) == 0)
  {
    return FALSE;
  }
  
  //Open File
  this->m_hFile = CreateFile(this->m_strFileName, dwDesiredAccess, dwShareMode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
  if((INVALID_HANDLE_VALUE == this->m_hFile) || (NULL == this->m_hFile))
  {
    return FALSE;
  }
  return TRUE;
}

BOOL CMapFile::MapFile(DWORD dwMapProtect, LPCSTR lpObjectName/* = NULL*/)
{
  if((INVALID_HANDLE_VALUE == this->m_hFile) || (NULL == this->m_hFile))
  {
    return FALSE;
  }
  
  //Mapping the file into memory
  this->m_hMappingFile = CreateFileMapping(this->m_hFile, NULL, dwMapProtect, 0, 0, NULL);
  if(NULL == this->m_hMappingFile)
  {
    return FALSE;
  }
  return TRUE;
}

BOOL CMapFile::MapFileView(DWORD dwDesiredViewAccess, SIZE_T dwNumberOfBytesToMap/* = 0*/)
{
  if(NULL == this->m_hMappingFile)
  {
    return FALSE;
  }

  //Mapping the view of the file into the address space of the calling process;
  this->m_lpvBase = MapViewOfFile(this->m_hMappingFile, dwDesiredViewAccess, 0, 0, dwNumberOfBytesToMap);
  if(NULL == this->m_lpvBase)
  {
    return FALSE;
  }
  this->m_strBase = (LPBYTE)(this->m_lpvBase);
  return TRUE;
}

BOOL CMapFile::UnMapFileView(void)
{
  BOOL bResult = FALSE;

  if(NULL == this->m_lpvBase)
  {
    return FALSE;
  }

  bResult = UnmapViewOfFile(this->m_lpvBase);
  if(FALSE == bResult)
  {
    return FALSE;
  }

  this->m_lpvBase = NULL;
  this->m_strBase = NULL;
  return TRUE;
}

BOOL CMapFile::Open(LPCTSTR strFileName, LPDWORD lpResult, DWORD dwDesiredAccess/* = GENERIC_READ*/, DWORD dwShareMode/* = FILE_SHARE_READ*/, DWORD dwMapProtect/* = PAGE_READONLY*/, DWORD dwDesiredViewAccess/* = FILE_MAP_READ*/)
{
  if(this->SetFileName(strFileName) == 0)
  {
    *lpResult = 1;
    return FALSE;
  }

  //Open File
  this->m_hFile = CreateFile(this->m_strFileName, dwDesiredAccess, dwShareMode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_ARCHIVE, NULL);
  if((INVALID_HANDLE_VALUE == this->m_hFile) || (NULL == this->m_hFile))
  {
    *lpResult = 2;
    return FALSE;
  }

  //Mapping the file into memory
  this->m_hMappingFile = CreateFileMapping(this->m_hFile, NULL, dwMapProtect, 0, 0, NULL);
  if(NULL == this->m_hMappingFile)
  {
    CloseHandle(this->m_hFile);
    this->m_hFile = NULL;
    *lpResult = 3;
    return FALSE;
  }

  //Mapping the view of the file into the address space of the calling process;
  this->m_lpvBase = MapViewOfFile(this->m_hMappingFile, dwDesiredViewAccess, 0, 0, 0);
  if(NULL == this->m_lpvBase)
  {
    CloseHandle(this->m_hMappingFile);
    this->m_hMappingFile = NULL;
    CloseHandle(this->m_hFile);
    this->m_hFile = NULL;
    *lpResult = 4;
    return FALSE;
  }

  this->m_strBase = (LPBYTE)(this->m_lpvBase);
  
  *lpResult = 0;
  return TRUE;
}

VOID CMapFile::Close(void)
{
  //Unmap all mapped views of the file-mapping object
  if(NULL != this->m_lpvBase)
  {
    UnmapViewOfFile(this->m_lpvBase);
    this->m_lpvBase = NULL;
    this->m_strBase = NULL;
  }

  //Close the file-mapping object handle
  if(NULL != this->m_hMappingFile)
  {
    CloseHandle(this->m_hMappingFile);
    this->m_hMappingFile = NULL;
  }

  //Close the object handle of the file
  if((INVALID_HANDLE_VALUE != this->m_hFile) && (NULL != this->m_hFile))
  {
    CloseHandle(this->m_hFile);
    this->m_hFile = NULL;
  }
}
