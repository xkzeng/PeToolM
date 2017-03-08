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
      ����: ��һ��PE�ļ�;
      ����: strFileName: PE�ļ���·����;
            lpResult: ����ļ��򿪹����еĴ�����;
            dwDesiredAccess: ��Ҫʹ�õķ���ģʽ,��ѡֵ��GENERIC_READ/GENERIC_WRITE,Ĭ��ΪGENERIC_READ;
            dwShareMode: ��Ҫʹ�õĹ���ģʽ,��ѡֵ��FILE_SHARE_READ/FILE_SHARE_WRITE,Ĭ��ΪFILE_SHARE_READ;
            dwMapProtect: ����CreateFileMapping()ʱʹ�õ�ӳ�䱣��ģʽ,��ѡֵ��PAGE_READONLY/PAGE_READWRITE/PAGE_WRITECOPY,Ĭ��ΪPAGE_READONLY;
            dwDesiredViewAccess: ����MapViewOfFile()ʱ��Ҫʹ�õ�ӳ����ͼ����ģʽ,��ѡֵ��FILE_MAP_READ/FILE_MAP_WRITE/FILE_MAP_ALL_ACCESS,Ĭ��ΪFILE_MAP_READ;
      ���: ����ļ��򿪳ɹ�,�򷵻�TRUE,lpResultָ����ڴ��ֵΪ0;����,����FALSE,lpResultָ����ڴ��ֵΪ������;
    ***/
    BOOL Open(LPCTSTR strFileName, LPDWORD lpResult, DWORD dwDesiredAccess = GENERIC_READ, DWORD dwShareMode = FILE_SHARE_READ, DWORD dwMapProtect = PAGE_READONLY, DWORD dwDesiredViewAccess = FILE_MAP_READ);
    VOID Close(void);
    LPBYTE GetBase(void) { return (this->m_strBase); };

    /***
      ����: ��һ��PE�ļ�;
      ����: strFileName: PE�ļ���·����;
            dwDesiredAccess: ��Ҫʹ�õķ���ģʽ,��ѡֵ��GENERIC_READ/GENERIC_WRITE,Ĭ��ΪGENERIC_READ;
            DWORD dwShareMode: ��Ҫʹ�õĹ���ģʽ,��ѡֵ��FILE_SHARE_READ/FILE_SHARE_WRITE,Ĭ��ΪFILE_SHARE_READ;
      ���: ����ļ��򿪳ɹ�,�򷵻�TRUE;����,����FALSE;
    ***/
    BOOL Open(LPCTSTR strFileName, DWORD dwDesiredAccess = GENERIC_READ, DWORD dwShareMode = FILE_SHARE_READ);

    /***
      ����: �����ļ����ڴ�ӳ�����;
      ����: dwMapProtect: ����CreateFileMapping()ʱʹ�õ�ӳ�䱣��ģʽ,��ѡֵ��PAGE_READONLY/PAGE_READWRITE/PAGE_WRITECOPY,Ĭ��ΪPAGE_READONLY;
            lpObjectName: ӳ��������ַ���,Ĭ��ΪNULL;
      ���: ��������ɹ�,�򷵻�TRUE;����,����FALSE;
    ***/
    BOOL MapFile(DWORD dwMapProtect, LPCSTR lpObjectName = NULL);

    /***
      ����: ���ļ���������ͼ�ķ�ʽӳ�䵽�ļ�ӳ�������;
      ����: dwDesiredViewAccess: ����MapViewOfFile()ʱ��Ҫʹ�õ�ӳ����ͼ����ģʽ,��ѡֵ��FILE_MAP_READ/FILE_MAP_WRITE/FILE_MAP_ALL_ACCESS,Ĭ��ΪFILE_MAP_READ;
            dwNumberOfBytesToMap: ӳ����ֽ���,Ĭ��Ϊ0,��ʾȫ�����ݶ�ӳ�䵽�ڴ���;
      ���: ���ӳ��ɹ�,�򷵻�TRUE;����,����FALSE;
    ***/
    BOOL MapFileView(DWORD dwDesiredViewAccess, SIZE_T dwNumberOfBytesToMap = 0);

    /***
      ����: ���ļ�����ӳ����ͼ���ڴ�ӳ������г���;
      ����: void
      ���: ���ӳ����ͼ�����ɹ�,�򷵻�TRUE;����,����FALSE;
    ***/
    BOOL UnMapFileView(void);
};

#endif
