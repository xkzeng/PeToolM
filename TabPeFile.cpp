// TabPeFile.cpp : implementation file
//

#include "stdafx.h"
#include <string.h>
#include "PeFile.h"
#include "PeToolM.h"
#include "TabPeFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabPeFile dialog


CTabPeFile::CTabPeFile(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPeFile::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabPeFile)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabPeFile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabPeFile)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabPeFile, CDialog)
	//{{AFX_MSG_MAP(CTabPeFile)
	ON_BN_CLICKED(IDC_BTN_OPEN_FILE, OnBtnOpenFile)
  ON_MESSAGE(WM_PARSECOMMANDLINE, OnParseCommandLine)
  ON_MESSAGE(UDM_CR_UPDATE, OnUpdateCopyRight)
  ON_WM_DROPFILES()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//Common Function
PSYSTEMTIME CTabPeFile::FileTime2SystemTime(PFILETIME lpFileTime)
{
  FILETIME   ft;
  static SYSTEMTIME st;

  FileTimeToLocalFileTime(lpFileTime, &ft);
  FileTimeToSystemTime(&ft, &st);
  return (&st);
}

WORD CTabPeFile::FormatFileTime(LPSTR strBuf, WORD wBufSize, PFILETIME lpFileTime)
{
  WORD wLen = 0;
  FILETIME   ft;
  SYSTEMTIME st;

  FileTimeToLocalFileTime(lpFileTime, &ft);
  FileTimeToSystemTime(&ft, &st);

  ZeroMemory(strBuf, wBufSize);
  wLen = _snprintf(strBuf, wBufSize, "%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
  return wLen;
}

WORD CTabPeFile::FormatFileAttr(LPSTR strBuf, WORD wBufSize, DWORD dwFileAttributes)
{
  WORD wLen = 0, wTotalLen = 0;
  LPSTR pos = NULL;
  //CHAR strTmp[64];

  ZeroMemory(strBuf, wBufSize);

  pos = strBuf;

  if(dwFileAttributes & FILE_ATTRIBUTE_NORMAL)
  {
    //ZeroMemory(strTmp, sizeof(strTmp));
    //wLen = _snprintf(strTmp, sizeof(strTmp), "%s,", "��ͨ");
    wLen = _snprintf(pos + wTotalLen, wBufSize - wTotalLen, "%s,", "��ͨ");
    //strncat(strBuf, strTmp, wLen);
    wTotalLen += wLen;
  }

  if(dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
  {
    //ZeroMemory(strTmp, sizeof(strTmp));
    //wLen = _snprintf(strTmp, sizeof(strTmp), "%s,", "Ŀ¼");
    wLen = _snprintf(pos + wTotalLen, wBufSize - wTotalLen, "%s,", "Ŀ¼");
    //strncat(strBuf, strTmp, wLen);
    wTotalLen += wLen;
  }

  if(dwFileAttributes & FILE_ATTRIBUTE_OFFLINE)
  {
    //ZeroMemory(strTmp, sizeof(strTmp));
    //wLen = _snprintf(strTmp, sizeof(strTmp), "%s,", "����");
    wLen = _snprintf(pos + wTotalLen, wBufSize - wTotalLen, "%s,", "����");
    //strncat(strBuf, strTmp, wLen);
    wTotalLen += wLen;
  }

  if(dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
  {
    //ZeroMemory(strTmp, sizeof(strTmp));
    //wLen = _snprintf(strTmp, sizeof(strTmp), "%s,", "ϵͳ");
    wLen = _snprintf(pos + wTotalLen, wBufSize - wTotalLen, "%s,", "ϵͳ");
    //strncat(strBuf, strTmp, wLen);
    wTotalLen += wLen;
  }

  if(dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)
  {
    //ZeroMemory(strTmp, sizeof(strTmp));
    //wLen = _snprintf(strTmp, sizeof(strTmp), "%s,", "��ʱ");
    wLen = _snprintf(pos + wTotalLen, wBufSize - wTotalLen, "%s,", "��ʱ");
    //strncat(strBuf, strTmp, wLen);
    wTotalLen += wLen;
  }

  if(dwFileAttributes & FILE_ATTRIBUTE_READONLY)
  {
    //ZeroMemory(strTmp, sizeof(strTmp));
    //wLen = _snprintf(strTmp, sizeof(strTmp), "%s,", "ֻ��");
    wLen = _snprintf(pos + wTotalLen, wBufSize - wTotalLen, "%s,", "ֻ��");
    //strncat(strBuf, strTmp, wLen);
    wTotalLen += wLen;
  }
  
  if(dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
  {
    //ZeroMemory(strTmp, sizeof(strTmp));
    //wLen = _snprintf(strTmp, sizeof(strTmp), "%s,", "����");
    wLen = _snprintf(pos + wTotalLen, wBufSize - wTotalLen, "%s,", "����");
    //strncat(strBuf, strTmp, wLen);
    wTotalLen += wLen;
  }

  if(dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)
  {
    //ZeroMemory(strTmp, sizeof(strTmp));
    //wLen = _snprintf(strTmp, sizeof(strTmp), "%s,", "ѹ��");
    wLen = _snprintf(pos + wTotalLen, wBufSize - wTotalLen, "%s,", "ѹ��");
    //strncat(strBuf, strTmp, wLen);
    wTotalLen += wLen;
  }

  if(dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)
  {
    //ZeroMemory(strTmp, sizeof(strTmp));
    //wLen = _snprintf(strTmp, sizeof(strTmp), "%s,", "����");
    wLen = _snprintf(pos + wTotalLen, wBufSize - wTotalLen, "%s,", "����");
    //strncat(strBuf, strTmp, wLen);
    wTotalLen += wLen;
  }

  if(dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE)
  {
    //ZeroMemory(strTmp, sizeof(strTmp));
    //wLen = _snprintf(strTmp, sizeof(strTmp), "%s,", "�鵵");
    wLen = _snprintf(pos + wTotalLen, wBufSize - wTotalLen, "%s", "�鵵");
    //strncat(strBuf, strTmp, wLen);
    wTotalLen += wLen;
  }

  if(0 == wTotalLen)
  {
    //ZeroMemory(strTmp, sizeof(strTmp));
    //wLen = _snprintf(strTmp, sizeof(strTmp), "%s,", "�鵵");
    wLen = _snprintf(pos + wTotalLen, wBufSize - wTotalLen, "%s", "δ֪");
    //strncat(strBuf, strTmp, wLen);
    wTotalLen += wLen;
    return wTotalLen;
  }

  if(',' == strBuf[wTotalLen - 1])
  {
    wTotalLen--;
    strBuf[wTotalLen] = 0;
  }
  return wTotalLen;
}

BOOL CTabPeFile::ParseFeFile(LPTSTR strFileName)
{
  BOOL bRet = FALSE;
  DWORD dwResult = 0;
  WIN32_FILE_ATTRIBUTE_DATA wfad;
  CHAR strMsg[TEMP_BUFF_SIZE];

  if(NULL == g_lpPeFile)
  {
    MessageBox("û�д���PE�ļ������", "������Ϣ", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  if((NULL == strFileName) || (strlen((LPCSTR)strFileName) < 5))
  {
    MessageBox("��ѡ���ļ�", "��ʾ", MB_OK | MB_ICONINFORMATION);
    return FALSE;
  }

  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_PE_FILE), WM_SETTEXT, NULL, (LPARAM)strFileName);

  //Get the attribute of the file
  ZeroMemory(&wfad, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
  bRet = GetFileAttributesEx(strFileName, GetFileExInfoStandard, &wfad);
  if(FALSE == bRet)
  {
    MessageBox("��ȡ�ļ�����ʧ��", "������Ϣ", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  //File Size
  ULONGLONG ullFileSize = (wfad.nFileSizeHigh << (sizeof(DWORD) * 8)) + wfad.nFileSizeLow;
  ZeroMemory(strMsg, TEMP_BUFF_SIZE);
  _snprintf(strMsg, TEMP_BUFF_SIZE, "%I64u bytes", ullFileSize);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_FILE_SIZE), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //Create Time
  this->FormatFileTime(strMsg, TEMP_BUFF_SIZE, &wfad.ftCreationTime);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_CTIME), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //Modified Time
  this->FormatFileTime(strMsg, TEMP_BUFF_SIZE, &wfad.ftLastWriteTime);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_MTIME), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //Access Time
  this->FormatFileTime(strMsg, TEMP_BUFF_SIZE, &wfad.ftLastAccessTime);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_ATIME), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //All Attributes
  this->FormatFileAttr(strMsg, TEMP_BUFF_SIZE, wfad.dwFileAttributes);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_ATTRS), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  g_lpPeFile->Close();
  
  bRet = g_lpPeFile->Open(strFileName, &dwResult, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, PAGE_READWRITE, FILE_MAP_READ | FILE_MAP_WRITE);
  if(bRet == FALSE)
  {
    MessageBox("���ļ�ʧ��", "������Ϣ", MB_OK | MB_ICONERROR);
    return FALSE;
  }
  
  bRet = g_lpPeFile->Parse(&dwResult);
  if(FALSE == bRet)
  {
    MessageBox("�����ļ�ʧ��", "������Ϣ", MB_OK | MB_ICONERROR);
    return FALSE;
  }
  
  //֪ͨ��ǰ���ڶ�ȡ��Ȩ��Ϣ;
  this->PostMessage(UDM_CR_UPDATE, 0, 0);
  
  //֪ͨ������,�ø�����֪ͨ�����Ӵ���,����������;
  this->m_pParentWnd->PostMessage(UDM_FILLFORM, 0, 0);
  
  //֪ͨ������,������������������ı�Ϊ��ǰ�����ļ��������ڽ�����PE�ļ���·����
  this->m_pParentWnd->SendMessage(UDM_SET_TITLE_TEXT, 0, (LPARAM)strFileName);
  return TRUE;
}

VOID CTabPeFile::ShowDefaultCopyRight(VOID)
{
  //SoftName:
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_SOFT_NAME), WM_SETTEXT, NULL, (LPARAM)TEXT("PeToolM"));

  //Version:
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_SOFT_VERSION), WM_SETTEXT, NULL, (LPARAM)TEXT("2015.09.09 -- Now"));

  //Author:
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_SOFT_AUTHOR), WM_SETTEXT, NULL, (LPARAM)TEXT("���ֿ�"));

  //Email:
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_EMAIL), WM_SETTEXT, NULL, (LPARAM)TEXT("zengxiankui@qq.com"));
}

BOOL CTabPeFile::ShowCopyRight(VOID)
{
  LPBYTE strFileBase = NULL;
  WORD i = 0, wCount = 0;
  PFORMAT_PE lpFmtPe = NULL;
  PIMAGE_SECTION_HEADER aSections = NULL;
  PIMAGE_SECTION_HEADER lpSection = NULL;
  LPBYTE strSignBase = NULL, pos = NULL;
  LPBYTE strBackUp = NULL;
  struct SSoftSign* lpSign = NULL;
  struct SString* lpStr = NULL;
  TCHAR strMsg[TEMP_BUFF_SIZE];

  if(NULL == g_lpPeFile)
  {
    this->ShowDefaultCopyRight();
    goto ERROR_READ_COPYRIGHT;
  }
  
  strFileBase = g_lpPeFile->GetBase();
  if(NULL == strFileBase)
  {
    this->ShowDefaultCopyRight();
    goto ERROR_READ_COPYRIGHT;
  }

  //ȡ��PE�ļ�����;
  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    this->ShowDefaultCopyRight();
    goto ERROR_READ_COPYRIGHT;
  }
  
  if((NULL == lpFmtPe->lpBlkTbl) || (NULL == lpFmtPe->lpFilHdr))
  {
    this->ShowDefaultCopyRight();
    goto ERROR_READ_COPYRIGHT;
  }

  //���ҿ�ִ�еĴ����,�����ش���ε���ʼ��ַ(FOA:PointerToRawData);
  aSections = lpFmtPe->lpBlkTbl;
  wCount = (WORD)(lpFmtPe->lpFilHdr->NumberOfSections);
  for(i = 0; i < wCount; i++)
  {
    //get section data
    lpSection = NULL;
    lpSection = aSections + i;
    
    //�ýھ��пɶ�(R)����ִ��(E)����������(C)������,��ýھ��Ǵ����(.text/CODE);
    if((lpSection->Characteristics & IMAGE_SCN_MEM_READ) && (lpSection->Characteristics & IMAGE_SCN_MEM_EXECUTE) && (lpSection->Characteristics & IMAGE_SCN_CNT_CODE))
    {
      break;
    }
  }
  
  if(i == wCount)
  {
    this->ShowDefaultCopyRight();
    goto ERROR_READ_COPYRIGHT;
  }
  
  if(NULL == lpSection)
  {
    this->ShowDefaultCopyRight();
    goto ERROR_READ_COPYRIGHT;
  }
  
  //�ҵ����������Ĵ����֮��,ȡ�øö��ڴ����ļ��е���ʼ��ַ;
  strSignBase = strFileBase + lpSection->PointerToRawData + lpSection->Misc.VirtualSize + SKIP_LENGTH;
  pos = strSignBase;
  
  lpSign = (struct SSoftSign*)pos;
  if(0 == lpSign->bLength)
  {
    this->ShowDefaultCopyRight();
    goto ERROR_READ_COPYRIGHT;
  }

  strBackUp = new BYTE[lpSign->bLength];
  ZeroMemory(strBackUp, lpSign->bLength);
  CopyMemory(strBackUp, pos, lpSign->bLength);
  g_lpPeFile->Decrpty(strBackUp + sizeof(lpSign->bLength), (WORD)(lpSign->bLength - sizeof(lpSign->bLength)), KEY_ENCRPTY);
  pos = strBackUp;

  //Version;
  lpSign = (struct SSoftSign*)pos;
  if((lpSign->wVersion1 == 0) && (lpSign->bVersion2 == 0) && (lpSign->bVersion3 == 0))
  {
    delete[] strBackUp;
    this->ShowDefaultCopyRight();
    goto ERROR_READ_COPYRIGHT;
  }

  ZeroMemory(strMsg, sizeof(strMsg));
  wsprintf(strMsg, "%04d.%02d.%02d", lpSign->wVersion1, lpSign->bVersion2, lpSign->bVersion3);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_SOFT_VERSION), WM_SETTEXT, 0, (LPARAM)strMsg);
  pos += sizeof(struct SSoftSign);
  
  //SoftName:
  lpStr = (struct SString*)pos;
  ZeroMemory(strMsg, sizeof(strMsg));
  CopyMemory(strMsg, lpStr->strBuffer, lpStr->bLength);
  pos += (sizeof(lpStr->bLength) + lpStr->bLength);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_SOFT_NAME), WM_SETTEXT, 0, (LPARAM)strMsg);
  
  //Author:
  lpStr = (struct SString*)pos;
  ZeroMemory(strMsg, sizeof(strMsg));
  CopyMemory(strMsg, lpStr->strBuffer, lpStr->bLength);
  pos += (sizeof(lpStr->bLength) + lpStr->bLength);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_SOFT_AUTHOR), WM_SETTEXT, 0, (LPARAM)strMsg);
  
  //Email:
  lpStr = (struct SString*)pos;
  ZeroMemory(strMsg, sizeof(strMsg));
  CopyMemory(strMsg, lpStr->strBuffer, lpStr->bLength);
  pos += (sizeof(lpStr->bLength) + lpStr->bLength);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_EMAIL), WM_SETTEXT, 0, (LPARAM)strMsg);
  
  delete[] strBackUp;
  return TRUE;
  
ERROR_READ_COPYRIGHT:
  return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CTabPeFile message handlers

void CTabPeFile::OnBtnOpenFile() 
{
	// TODO: Add your control notification handler code here
  BOOL bRet = FALSE;
  OPENFILENAME ofn;
  TCHAR strFileName[FILE_PATH_LEN];

  ZeroMemory((PVOID)strFileName, (SIZE_T)FILE_PATH_LEN);
  ZeroMemory((PVOID)&ofn, (SIZE_T)sizeof(OPENFILENAME));

  ofn.lStructSize = sizeof(OPENFILENAME);
  ofn.lpstrFilter = TEXT("EXE�ļ�(*.exe)\0*.exe\0DLL�ļ�(*.dll)\0*.dll\0OCX�ļ�(*.ocx)\0*.ocx\0COM�ļ�(*.com)\0*.com\0SYS�ļ�(*.sys)\0*.sys\0DRV�ļ�(*.drv)\0*.drv\0");
  ofn.nFilterIndex = 1;
  ofn.lpstrFile = strFileName;
  ofn.nMaxFile = FILE_PATH_LEN;
  ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY | OFN_HIDEREADONLY;

  bRet = GetOpenFileName(&ofn);
  if(FALSE == bRet)
  {
    return;
  }
  
  this->ParseFeFile(strFileName);
  //this->ShowCopyRight();

  /***
  //Get the attribute of the file
  ZeroMemory(&wfad, sizeof(WIN32_FILE_ATTRIBUTE_DATA));
  bRet = GetFileAttributesEx(strFileName, GetFileExInfoStandard, &wfad);
  if(FALSE == bRet)
  {
    MessageBox("��ȡ�ļ�����ʧ��", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }

  //File Size
  ULONGLONG ullFileSize = (wfad.nFileSizeHigh << (sizeof(DWORD) * 8)) + wfad.nFileSizeLow;
  ZeroMemory(strMsg, TEMP_BUFF_SIZE);
  _snprintf(strMsg, TEMP_BUFF_SIZE, "%I64u bytes", ullFileSize);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_FILE_SIZE), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //Create Time
  this->FormatFileTime(strMsg, TEMP_BUFF_SIZE, &wfad.ftCreationTime);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_CTIME), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //Modified Time
  this->FormatFileTime(strMsg, TEMP_BUFF_SIZE, &wfad.ftLastWriteTime);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_MTIME), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //Access Time
  this->FormatFileTime(strMsg, TEMP_BUFF_SIZE, &wfad.ftLastAccessTime);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_ATIME), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //All Attributes
  this->FormatFileAttr(strMsg, TEMP_BUFF_SIZE, wfad.dwFileAttributes);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_ATTRS), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  g_lpPeFile->Close();
  
  bRet = g_lpPeFile->Open(strFileName, &dwResult);
  if(bRet == FALSE)
  {
    MessageBox("���ļ�ʧ��", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }
  
  bRet = g_lpPeFile->Parse(&dwResult);
  if(FALSE == bRet)
  {
    MessageBox("�����ļ�ʧ��", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }
  
  //֪ͨ������,�ø�����֪ͨ�����Ӵ���,����������;
  this->m_pParentWnd->PostMessage(UDM_FILLFORM, 0, 0);
  
  //֪ͨ������,������������������ı�Ϊ��ǰ�����ļ��������ڽ�����PE�ļ���·����
  this->m_pParentWnd->SendMessage(UDM_SET_TITLE_TEXT, 0, (LPARAM)strFileName);
  ***/
}

void CTabPeFile::OnParseCommandLine(WPARAM wParam, LPARAM lParam)
{
  this->ParseFeFile((LPTSTR)lParam);
}

void CTabPeFile::OnUpdateCopyRight(WPARAM wParam, LPARAM lParam)
{
  this->ShowCopyRight();
}

BOOL CTabPeFile::OnInitDialog() 
{
  CDialog::OnInitDialog();
  
  // TODO: Add extra initialization here
  this->DragAcceptFiles();
  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabPeFile::OnDropFiles(HDROP hDropFile)
{
  UINT iFileNum = 0;
  TCHAR strFileName[FILE_PATH_LEN];

  if(NULL == hDropFile)
  {
    return;
  }

  iFileNum = DragQueryFile(hDropFile, 0xFFFFFFFF, NULL, 0);
  if(iFileNum < 1)
  {
    goto EXIT_OnDropFiles;
    return;
  }

  ZeroMemory(strFileName, sizeof(strFileName));
  iFileNum = DragQueryFile(hDropFile, 0, strFileName, sizeof(strFileName));
  if(iFileNum <= 0)
  {
    goto EXIT_OnDropFiles;
    return;
  }

  this->ParseFeFile(strFileName);

EXIT_OnDropFiles:
  DragFinish(hDropFile);

  CDialog::OnDropFiles(hDropFile);
}