#pragma once

#ifndef __MAP_FILE_H__
#define __MAP_FILE_H__

class CMapFile
{
  protected:
    enum
    {
      FILE_PATH_LENGTH = 256
    };
    TCHAR  m_strFileName[FILE_PATH_LENGTH];
    HANDLE m_hFile;
    HANDLE m_hMappingFile;
    LPVOID m_lpvBase;
    LPBYTE m_strBase;

  public:
    CMapFile(void);
    virtual ~CMapFile(void);

    WORD SetFileName(LPCTSTR strFileName);
    LPCTSTR GetFileName(void);

    /***
      功能: 打开一个PE文件;
      参数: strFileName: PE文件的路径名;
            lpResult: 存放文件打开过程中的错误码;
            dwDesiredAccess: 需要使用的访问模式,可选值有GENERIC_READ/GENERIC_WRITE,默认为GENERIC_READ;
            dwShareMode: 需要使用的共享模式,可选值有FILE_SHARE_READ/FILE_SHARE_WRITE,默认为FILE_SHARE_READ;
            dwMapProtect: 调用CreateFileMapping()时使用的映射保护模式,可选值有PAGE_READONLY/PAGE_READWRITE/PAGE_WRITECOPY,默认为PAGE_READONLY;
            dwDesiredViewAccess: 调用MapViewOfFile()时需要使用的映射视图访问模式,可选值有FILE_MAP_READ/FILE_MAP_WRITE/FILE_MAP_ALL_ACCESS,默认为FILE_MAP_READ;
      结果: 如果文件打开成功,则返回TRUE,lpResult指向的内存的值为0;否则,返回FALSE,lpResult指向的内存的值为错误码;
    ***/
    BOOL Open(LPCTSTR strFileName, LPDWORD lpResult, DWORD dwDesiredAccess = GENERIC_READ, DWORD dwShareMode = FILE_SHARE_READ, DWORD dwMapProtect = PAGE_READONLY, DWORD dwDesiredViewAccess = FILE_MAP_READ);
    VOID Close(void);
    LPBYTE GetBase(void) { return (this->m_strBase); };

    /***
      功能: 打开一个PE文件;
      参数: strFileName: PE文件的路径名;
            dwDesiredAccess: 需要使用的访问模式,可选值有GENERIC_READ/GENERIC_WRITE,默认为GENERIC_READ;
            DWORD dwShareMode: 需要使用的共享模式,可选值有FILE_SHARE_READ/FILE_SHARE_WRITE,默认为FILE_SHARE_READ;
      结果: 如果文件打开成功,则返回TRUE;否则,返回FALSE;
    ***/
    BOOL Open(LPCTSTR strFileName, DWORD dwDesiredAccess = GENERIC_READ, DWORD dwShareMode = FILE_SHARE_READ);

    /***
      功能: 创建文件的内存映射对象;
      参数: dwMapProtect: 调用CreateFileMapping()时使用的映射保护模式,可选值有PAGE_READONLY/PAGE_READWRITE/PAGE_WRITECOPY,默认为PAGE_READONLY;
            lpObjectName: 映射对象名字符串,默认为NULL;
      结果: 如果创建成功,则返回TRUE;否则,返回FALSE;
    ***/
    BOOL MapFile(DWORD dwMapProtect, LPCSTR lpObjectName = NULL);

    /***
      功能: 把文件内容以视图的方式映射到文件映射对象中;
      参数: dwDesiredViewAccess: 调用MapViewOfFile()时需要使用的映射视图访问模式,可选值有FILE_MAP_READ/FILE_MAP_WRITE/FILE_MAP_ALL_ACCESS,默认为FILE_MAP_READ;
            dwNumberOfBytesToMap: 映射的字节数,默认为0,表示全部内容都映射到内存中;
      结果: 如果映射成功,则返回TRUE;否则,返回FALSE;
    ***/
    BOOL MapFileView(DWORD dwDesiredViewAccess, SIZE_T dwNumberOfBytesToMap = 0);

    /***
      功能: 把文件内容映射视图从内存映射对象中撤销;
      参数: void
      结果: 如果映射视图撤销成功,则返回TRUE;否则,返回FALSE;
    ***/
    BOOL UnMapFileView(void);
};

#endif
