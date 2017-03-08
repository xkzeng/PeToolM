// TabUsrOpr.cpp : implementation file
//

#include "stdafx.h"
#include <Shlwapi.h.>
#include "PeFile.h"
#include "PeToolM.h"
#include "TabUsrOpr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabUsrOpr dialog

CTabUsrOpr::CTabUsrOpr(CWnd* pParent /*=NULL*/)
	: CDialog(CTabUsrOpr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabUsrOpr)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabUsrOpr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabUsrOpr)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabUsrOpr, CDialog)
	//{{AFX_MSG_MAP(CTabUsrOpr)
  ON_MESSAGE(UDM_FILLFORM, OnFillForm)
	ON_BN_CLICKED(IDC_CHECK_ADDRESS_CONVERT, OnCheckAddressConvert)
	ON_BN_CLICKED(IDC_BTN_ADDRESS_CONVERT, OnBtnAddressConvert)
	ON_BN_CLICKED(IDC_BTN_SEARCH_IMPORT, OnBtnSearchImport)
	ON_BN_CLICKED(IDC_BTN_SEARCH_EXPORT, OnBtnSearchExport)
	ON_BN_CLICKED(IDC_BTN_SOFT_SIGN, OnBtnSoftSign)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//Common Member Function
VOID CTabUsrOpr::UpdateCheckBox(VOID)
{
  int iState = -1;
  
  iState = ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_CHECK_ADDRESS_CONVERT), BM_GETCHECK, 0, 0);
  if(BST_UNCHECKED == iState)
  {
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_SRC), WM_SETTEXT, 0, (LPARAM)"RVA");
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_DST), WM_SETTEXT, 0, (LPARAM)"FOA");
  }
  else if(BST_CHECKED == iState)
  {
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_SRC), WM_SETTEXT, 0, (LPARAM)"FOA");
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_STATIC_DST), WM_SETTEXT, 0, (LPARAM)"RVA");
  }
}

VOID CTabUsrOpr::EnableSignControl(BOOL bEnable/* = TRUE*/)
{
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER1), EM_SETREADONLY, (WPARAM)(!bEnable), 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER2), EM_SETREADONLY, (WPARAM)(!bEnable), 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER3), EM_SETREADONLY, (WPARAM)(!bEnable), 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_SOFT_NAME), EM_SETREADONLY, (WPARAM)(!bEnable), 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_AUTHOR), EM_SETREADONLY, (WPARAM)(!bEnable), 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_EMAIL), EM_SETREADONLY, (WPARAM)(!bEnable), 0);
  ::EnableWindow(::GetDlgItem(this->m_hWnd, IDC_BTN_SOFT_SIGN), bEnable);
  /*
  if(bEnable)
  {
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER1), EM_SETREADONLY, (WPARAM)FALSE, 0);
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER2), EM_SETREADONLY, (WPARAM)FALSE, 0);
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER3), EM_SETREADONLY, (WPARAM)FALSE, 0);
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_SOFT_NAME), EM_SETREADONLY, (WPARAM)FALSE, 0);
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_AUTHOR), EM_SETREADONLY, (WPARAM)FALSE, 0);
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_EMAIL), EM_SETREADONLY, (WPARAM)FALSE, 0);
    ::EnableWindow(::GetDlgItem(this->m_hWnd, IDC_BTN_SOFT_SIGN), TRUE);
  }
  else
  {
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER1), EM_SETREADONLY, (WPARAM)TRUE, 0);
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER2), EM_SETREADONLY, (WPARAM)TRUE, 0);
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER3), EM_SETREADONLY, (WPARAM)TRUE, 0);
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_SOFT_NAME), EM_SETREADONLY, (WPARAM)TRUE, 0);
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_AUTHOR), EM_SETREADONLY, (WPARAM)TRUE, 0);
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_EMAIL), EM_SETREADONLY, (WPARAM)TRUE, 0);
    ::EnableWindow(::GetDlgItem(this->m_hWnd, IDC_BTN_SOFT_SIGN), FALSE);
  }
  */
}

BOOL CTabUsrOpr::UseDefaultSign(VOID)
{
  SYSTEMTIME stSysTime;
  TCHAR strMsg[TEMP_BUFF_SIZE];

  //SoftName:
  //::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_SOFT_NAME), WM_SETTEXT, 0, (LPARAM)(AfxGetAppName()));
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_SOFT_NAME), WM_SETTEXT, 0, (LPARAM)(TEXT("PeToolM")));

  //Author:
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_AUTHOR), WM_SETTEXT, 0, (LPARAM)(TEXT("���ֿ�")));

  //Version
  ZeroMemory(&stSysTime, sizeof(SYSTEMTIME));
  GetSystemTime(&stSysTime);

  //Version1:
  ZeroMemory(strMsg, sizeof(strMsg));
  wsprintf(strMsg, "%04d", stSysTime.wYear);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER1), WM_SETTEXT, 0, (LPARAM)strMsg);
  
  //Version2:
  ZeroMemory(strMsg, sizeof(strMsg));
  wsprintf(strMsg, "%02d", stSysTime.wMonth);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER2), WM_SETTEXT, 0, (LPARAM)strMsg);
  
  //Version3:
  ZeroMemory(strMsg, sizeof(strMsg));
  wsprintf(strMsg, "%02d", stSysTime.wDay);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER3), WM_SETTEXT, 0, (LPARAM)strMsg);

  //Email:
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_EMAIL), WM_SETTEXT, 0, (LPARAM)(TEXT("zengxiankui@qq.com")));
  return TRUE;
}

BOOL CTabUsrOpr::Fill(LPVOID lpParam/* = NULL*/)
{
  LPBYTE strFileBase = NULL;
  WORD i = 0, wCount = 0;
  PFORMAT_PE lpFmtPe = NULL;
  PIMAGE_SECTION_HEADER aSections = NULL;
  PIMAGE_SECTION_HEADER lpSection = NULL;
  LPBYTE strSignBase = NULL, strBackUp = NULL, pos = NULL;
  struct SSoftSign* lpSign = NULL;
  struct SString* lpStr = NULL;
  TCHAR strMsg[TEMP_BUFF_SIZE];

  if(NULL == g_lpPeFile)
  {
    MessageBox("��ȡǩ��ʱ,̽�⵽PE�ļ������Ѿ�ʧЧ", "������Ϣ", MB_OK | MB_ICONERROR);
    this->UseDefaultSign();
    return FALSE;
  }
  
  strFileBase = g_lpPeFile->GetBase();
  if(NULL == strFileBase)
  {
    MessageBox("��ȡǩ��ʱ,̽�⵽�ļ���ַ��Ч", "������Ϣ", MB_OK | MB_ICONERROR);
    this->UseDefaultSign();
    return FALSE;
  }

  //ȡ��PE�ļ�����;
  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("��ȡǩ��ʱ,�Ҳ���PE�ṹ", "������Ϣ", MB_OK | MB_ICONERROR);
    this->UseDefaultSign();
    return FALSE;
  }
  
  if((NULL == lpFmtPe->lpBlkTbl) || (NULL == lpFmtPe->lpFilHdr))
  {
    MessageBox("��ȡǩ��ʱ,�ڱ���Ч", "������Ϣ", MB_OK | MB_ICONERROR);
    this->UseDefaultSign();
    return FALSE;
  }

  //Enable the sign control
  this->EnableSignControl(TRUE);
  /*
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER1), EM_SETREADONLY, (WPARAM)FALSE, 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER2), EM_SETREADONLY, (WPARAM)FALSE, 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER3), EM_SETREADONLY, (WPARAM)FALSE, 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_SOFT_NAME), EM_SETREADONLY, (WPARAM)FALSE, 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_AUTHOR), EM_SETREADONLY, (WPARAM)FALSE, 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_EMAIL), EM_SETREADONLY, (WPARAM)FALSE, 0);
  EnableWindow(::GetDlgItem(this->m_hWnd, IDC_BTN_SOFT_SIGN), TRUE);
  */

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
    MessageBox("��ȡǩ����Ϣʱ,�Ҳ������ǩ����Ϣ�ĵط�", "������Ϣ", MB_OK | MB_ICONWARNING);
    this->UseDefaultSign();
    return FALSE;
  }
  
  if(NULL == lpSection)
  {
    MessageBox("��ȡǩ��ʱ,�ҵ���λ������Ч��", "������Ϣ", MB_OK | MB_ICONWARNING);
    this->UseDefaultSign();
    return FALSE;
  }
  
  //�ҵ����������Ĵ����֮��,ȡ�øö��ڴ����ļ��е���ʼ��ַ;
  strSignBase = strFileBase + lpSection->PointerToRawData + lpSection->Misc.VirtualSize + SKIP_LENGTH;
  pos = strSignBase;
  
  lpSign = (struct SSoftSign*)pos;
  if(0 == lpSign->bLength)
  {
    MessageBox("û���ҵ�ǩ����Ϣ,��PE�ļ��п���û��ǩ��", "����", MB_OK | MB_ICONWARNING);
    this->UseDefaultSign();
    return FALSE;
  }

  strBackUp = NULL;
  strBackUp = new BYTE[lpSign->bLength];
  if(NULL == strBackUp)
  {
    MessageBox("�������ǩ��ʱ,�����ڴ�ʧ��", "������Ϣ", MB_OK | MB_ICONERROR);
    this->UseDefaultSign();
    return FALSE;
  }

  ZeroMemory(strBackUp, lpSign->bLength);
  CopyMemory(strBackUp, pos, lpSign->bLength);
  g_lpPeFile->Decrpty(strBackUp + sizeof(lpSign->bLength), (WORD)(lpSign->bLength - sizeof(lpSign->bLength)), KEY_ENCRPTY);
  pos = strBackUp;

  //Version;
  lpSign = (struct SSoftSign*)pos;
  //if((lpSign->wVersion1 == 0) && (lpSign->wVersion2 == 0) && (lpSign->wVersion3 == 0))
  //{
  //  delete[] strBackUp;
  //  MessageBox("û���ҵ�ǩ����Ϣ,��PE�ļ��п���û��ǩ��", "����", MB_OK | MB_ICONWARNING);
  //  this->UseDefaultSign();
  //  return FALSE;
  //}

  //Version1:
  ZeroMemory(strMsg, sizeof(strMsg));
  wsprintf(strMsg, "%04d", lpSign->wVersion1);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER1), WM_SETTEXT, 0, (LPARAM)strMsg);

  //Version2:
  ZeroMemory(strMsg, sizeof(strMsg));
  wsprintf(strMsg, "%02d", lpSign->bVersion2);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER2), WM_SETTEXT, 0, (LPARAM)strMsg);

  //Version3:
  ZeroMemory(strMsg, sizeof(strMsg));
  wsprintf(strMsg, "%02d", lpSign->bVersion3);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER3), WM_SETTEXT, 0, (LPARAM)strMsg);
  
  pos += sizeof(struct SSoftSign);
  
  //SoftName:
  lpStr = (struct SString*)pos;
  ZeroMemory(strMsg, sizeof(strMsg));
  CopyMemory(strMsg, lpStr->strBuffer, lpStr->bLength);
  pos += (sizeof(lpStr->bLength) + lpStr->bLength);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_SOFT_NAME), WM_SETTEXT, 0, (LPARAM)strMsg);
  
  //Author:
  lpStr = (struct SString*)pos;
  ZeroMemory(strMsg, sizeof(strMsg));
  CopyMemory(strMsg, lpStr->strBuffer, lpStr->bLength);
  pos += (sizeof(lpStr->bLength) + lpStr->bLength);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_AUTHOR), WM_SETTEXT, 0, (LPARAM)strMsg);
  
  //Email:
  lpStr = (struct SString*)pos;
  ZeroMemory(strMsg, sizeof(strMsg));
  CopyMemory(strMsg, lpStr->strBuffer, lpStr->bLength);
  pos += (sizeof(lpStr->bLength) + lpStr->bLength);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_EMAIL), WM_SETTEXT, 0, (LPARAM)strMsg);

  //Disable the sign control
  this->EnableSignControl(FALSE);
  delete[] strBackUp;
  /*
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER1), EM_SETREADONLY, (WPARAM)TRUE, 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER2), EM_SETREADONLY, (WPARAM)TRUE, 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER3), EM_SETREADONLY, (WPARAM)TRUE, 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_SOFT_NAME), EM_SETREADONLY, (WPARAM)TRUE, 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_AUTHOR), EM_SETREADONLY, (WPARAM)TRUE, 0);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_EMAIL), EM_SETREADONLY, (WPARAM)TRUE, 0);
  EnableWindow(::GetDlgItem(this->m_hWnd, IDC_BTN_SOFT_SIGN), FALSE);
  */
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTabUsrOpr message handlers

BOOL CTabUsrOpr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_CHECK_ADDRESS_CONVERT), BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
  this->UpdateCheckBox();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabUsrOpr::OnFillForm(WPARAM wParam, LPARAM lParam)
{
  this->Fill();
}

void CTabUsrOpr::OnCheckAddressConvert() 
{
	// TODO: Add your control notification handler code here
	this->UpdateCheckBox();
}

void CTabUsrOpr::OnBtnAddressConvert() 
{
	// TODO: Add your control notification handler code here
  LRESULT lResult = 0;
  int iState = -1;
  TCHAR strMsg[TEMP_BUFF_SIZE];
  DWORD dwAddr = 0;

  if(NULL == g_lpPeFile)
  {
    MessageBox("PE�ļ�������Ч", "ERROR", MB_OK|MB_ICONERROR);
    return;
  }
  
  ZeroMemory(strMsg, sizeof(strMsg));
  lResult = ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_ADDR_SRC), WM_GETTEXT, (WPARAM)sizeof(strMsg), (LPARAM)strMsg);
  if(8 != lResult)
  {
    MessageBox("Դ��ַ������8���ַ�����ʮ��������", "Prompt", MB_OK|MB_ICONINFORMATION);
    return;
  }
  
  sscanf(strMsg, "%08X", &dwAddr);
  
  iState = ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_CHECK_ADDRESS_CONVERT), BM_GETCHECK, 0, 0);
  if(BST_UNCHECKED == iState)     //RVA to FOA
  {
    dwAddr = g_lpPeFile->Rva2Foa(dwAddr);
  }
  else if(BST_CHECKED == iState)  //FOA to RVA
  {
    dwAddr = g_lpPeFile->Foa2Rva(dwAddr);
  }
  else
  {
    dwAddr = 0;
  }

  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", dwAddr);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_ADDR_DST), WM_SETTEXT, NULL, (LPARAM)strMsg);
}

void CTabUsrOpr::OnBtnSearchImport() 
{
	// TODO: Add your control notification handler code here
	LPBYTE strFileBase = NULL;
  PFORMAT_PE lpFmtPe = NULL;
  PIMAGE_IMPORT_DESCRIPTOR lpIID = NULL;
  LPDWORD dwaINT = NULL, dwaIAT = NULL;
  DWORD dwRva = 0, dwFoa = 0;
  DWORD dwThkINT = 0, dwThkIAT = 0;
  PIMAGE_IMPORT_BY_NAME lpByName = NULL;
  WORD i = 0, wOrdinal = 0;
  LPCSTR strDllName = NULL;
  LRESULT lResult = 0;
  BOOL bFound = FALSE;
  CHAR strKey[TEMP_BUFF_SIZE];
  CHAR strMsg[TEMP_BUFF_SIZE+128];

  lResult = ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SRCH_IMP_KEY), WM_GETTEXTLENGTH, 0, 0);
  if(((sizeof(strKey) - 1) < lResult) || (0 >= lResult))
  {
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "û�����ݲ�������,�����������Ϣ̫��,�����Ϣ������%d", (sizeof(strKey) - 1));
    MessageBox(strMsg, "��ʾ", MB_OK | MB_ICONERROR);
    return;
  }

  ZeroMemory(strKey, sizeof(strKey));
  lResult = ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SRCH_IMP_KEY), WM_GETTEXT, (WPARAM)sizeof(strKey), (LPARAM)strKey);

  if(NULL == g_lpPeFile)
  {
    MessageBox("PE�ļ�����û�б���", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }

  strFileBase = g_lpPeFile->GetBase();
  if(NULL == strFileBase)
  {
    MessageBox("��ѯʱ̽�⵽�ļ���ַ��Ч", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }

  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("��ѯʱ̽�⵽�򿪵��ļ���Ч,����û��PE�ļ�����", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }
  
  if((NULL == lpFmtPe->lpImpBlk) || (NULL == lpFmtPe->lpImpTbl))
  {
    MessageBox("��ѯʱ̽�⵽�򿪵�PE�ļ���ʽ��Ч", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }

  if(0 == lpFmtPe->lpImpBlk->Size)
  {
    MessageBox("��ѯʱ̽�⵽��PE�ļ�û�е����", "������Ϣ", MB_OK | MB_ICONWARNING);
    return;
  }

  lpIID = lpFmtPe->lpImpTbl;

  //begin to search
  bFound = FALSE;
  for(; lpIID->Name; lpIID++)
  {
    //get dll name
    dwFoa = g_lpPeFile->Rva2Foa(lpIID->Name);
    strDllName = (LPCSTR)(strFileBase + dwFoa);
    
    //start address of the Original First Thunk table,named Import Name Table(INT);
    dwFoa = g_lpPeFile->Rva2Foa(lpIID->OriginalFirstThunk);
    dwaINT = (LPDWORD)(strFileBase + dwFoa);
    
    //start address of the First Thunk table,named Import Address Table(IAT);
    dwFoa = g_lpPeFile->Rva2Foa(lpIID->FirstThunk);
    dwaIAT = (LPDWORD)(strFileBase + dwFoa);
    
    //Iterate the Import Name Table(INT) and Import Address Table(IAT);
    for(i = 0; (dwThkINT = dwaINT[i]) && (dwThkIAT = dwaIAT[i]); i++)
    {
      //Import By Ordinal,no operation
      if(IMAGE_SNAP_BY_ORDINAL32(dwThkINT))
      {
        wOrdinal = (WORD)IMAGE_ORDINAL32(dwThkINT);
        continue;
      }

      //Import By Name
      dwFoa = g_lpPeFile->Rva2Foa(dwThkINT);
      lpByName = (PIMAGE_IMPORT_BY_NAME)(strFileBase + dwFoa);
      if((0 == StrCmpNI((LPCTSTR)(lpByName->Name), (LPCTSTR)strKey, lResult)) || StrStrI((LPCTSTR)(lpByName->Name), (LPCTSTR)strKey))
      {
        bFound = TRUE; //Found the function
        ZeroMemory(strMsg, sizeof(strMsg));
        _snprintf(strMsg, sizeof(strMsg), "%s: %s\r\n%-4s: %s\r\n%-4s: %08X\r\n%-4s: %08X\r\n%-4s: %08X\r\n%-4s: %04X",
                                          "Func", lpByName->Name,
                                          "DLL", strDllName,
                                          "RVA", dwThkINT,
                                          "FOA", dwFoa,
                                          "IAT", dwThkIAT,
                                          "Hint", lpByName->Hint);
        ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SRCH_IMP_RESULT), WM_SETTEXT, NULL, (LPARAM)strMsg);
        break;
      }
    }
    
    if(TRUE == bFound)
    {
      break;
    }
  }
  
  if(FALSE == bFound)
  {
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "'%s' is not found!!!", strKey);
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SRCH_IMP_RESULT), WM_SETTEXT, NULL, (LPARAM)strMsg);
  }
}

void CTabUsrOpr::OnBtnSearchExport() 
{
	// TODO: Add your control notification handler code here
  LPBYTE strFileBase = NULL;
  PFORMAT_PE lpFmtPe = NULL;
  PIMAGE_EXPORT_DIRECTORY lpIED = NULL;
  LPDWORD adwRvaAddrs = NULL;
  LPDWORD adwRvaNames = NULL;
  LPWORD  awOrdinals = NULL;
  WORD i = 0, wOrdinal = 0;
  DWORD dwFoa = 0, dwAddr = 0;
  LRESULT lResult = 0;
  BOOL bFound = FALSE;
  LPCSTR strFuncName = NULL;
  CHAR strKey[TEMP_BUFF_SIZE];
  CHAR strMsg[TEMP_BUFF_SIZE+128];
  
  lResult = ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SRCH_EXP_KEY), WM_GETTEXTLENGTH, 0, 0);
  if(((sizeof(strKey) - 1) < lResult) || (0 >= lResult))
  {
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "û�����ݲ�������,�����������Ϣ̫��,�����Ϣ������%d", (sizeof(strKey) - 1));
    MessageBox(strMsg, "��ʾ", MB_OK | MB_ICONERROR);
    return;
  }
  
  ZeroMemory(strKey, sizeof(strKey));
  lResult = ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SRCH_EXP_KEY), WM_GETTEXT, (WPARAM)sizeof(strKey), (LPARAM)strKey);
  
  if(NULL == g_lpPeFile)
  {
    MessageBox("PE�ļ�����û�б���", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }
  
  strFileBase = g_lpPeFile->GetBase();
  if(NULL == strFileBase)
  {
    MessageBox("��ѯʱ̽�⵽�ļ���ַ��Ч", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }
  
  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("��ѯʱ̽�⵽�򿪵��ļ���Ч,����û��PE�ļ�����", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }
  
  if((NULL == lpFmtPe->lpExpBlk) || (NULL == lpFmtPe->lpExpTbl))
  {
    MessageBox("��ѯʱ̽�⵽�򿪵�PE�ļ���ʽ��Ч", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }
  
  if(0 == lpFmtPe->lpExpBlk->Size)
  {
    MessageBox("��ѯʱ̽�⵽��PE�ļ�û�е�����", "������Ϣ", MB_OK | MB_ICONWARNING);
    return;
  }
  
  lpIED = lpFmtPe->lpExpTbl;
  
  //������ַ��
  dwFoa = g_lpPeFile->Rva2Foa(lpIED->AddressOfFunctions);
  adwRvaAddrs = (LPDWORD)(strFileBase + dwFoa); //RVA Array,so,adwRvaAddrs[i] is a RVA;
  
  //�������Ʊ�
  dwFoa = g_lpPeFile->Rva2Foa(lpIED->AddressOfNames);
  adwRvaNames = (LPDWORD)(strFileBase + dwFoa); //RVA Array,so,adwRvaNames[i] is a RVA;
  
  //����������
  dwFoa = g_lpPeFile->Rva2Foa(lpIED->AddressOfNameOrdinals);
  awOrdinals = (LPWORD)(strFileBase + dwFoa);
  
  //begin to search
  bFound = FALSE;
  for(i = 0; i < lpIED->NumberOfNames; i++)
  {
    //Name:
    dwFoa = g_lpPeFile->Rva2Foa(adwRvaNames[i]);
    strFuncName = (LPCSTR)strFileBase + dwFoa;
    if((0 == StrCmpNI((LPCTSTR)strFuncName, (LPCTSTR)strKey, lResult)) || StrStrI((LPCTSTR)strFuncName, (LPCTSTR)strKey))
    {
      bFound = TRUE; //Found the function

      //Ordinal:
      wOrdinal = awOrdinals[i];
      
      //Address:
      dwAddr = adwRvaAddrs[wOrdinal];

      ZeroMemory(strMsg, sizeof(strMsg));
      _snprintf(strMsg, sizeof(strMsg), "%s: %s\r\n%-4s: %04X\r\n%-4s: %04X\r\n%-4s: %08X",
                                        "Func", strFuncName,
                                        "Ordi", wOrdinal + lpIED->Base,
                                        "Hint", wOrdinal,
                                        "Addr", dwAddr);
      ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SRCH_EXP_RESULT), WM_SETTEXT, NULL, (LPARAM)strMsg);
      break;
    }
  }

  if(FALSE == bFound)
  {
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "'%s' is not found!!!", strKey);
    ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SRCH_EXP_RESULT), WM_SETTEXT, NULL, (LPARAM)strMsg);
  }
}

void CTabUsrOpr::OnBtnSoftSign() 
{
	// TODO: Add your control notification handler code here
  LPBYTE strFileBase = NULL;
  WORD i = 0, wCount = 0;
  PFORMAT_PE lpFmtPe = NULL;
  PIMAGE_SECTION_HEADER aSections = NULL;
  PIMAGE_SECTION_HEADER lpSection = NULL;
  LPBYTE strSignBase = NULL, pos = NULL;
	struct SSoftSign stSign;
  struct SSoftSign* lpSign = NULL;
  struct SString stSoftName;
  struct SString stAuthor;
  struct SString stEmail;
  struct SString* lpStr = NULL;
  DWORD dwSignLength = 0;
  LRESULT lResult = 0;
  TCHAR strBuf[TEMP_BUFF_SIZE];

  if(NULL == g_lpPeFile)
  {
    MessageBox("ǩ��ʱ,̽��PE�ļ������Ѿ�ʧЧ", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }
  
  strFileBase = g_lpPeFile->GetBase();
  if(NULL == strFileBase)
  {
    MessageBox("ǩ��ʱ,̽�⵽�ļ���ַ��Ч", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }

  ZeroMemory(&stSign, sizeof(struct SSoftSign));
  ZeroMemory(&stSoftName, sizeof(struct SString));
  ZeroMemory(&stAuthor, sizeof(struct SString));
  ZeroMemory(&stEmail, sizeof(struct SString));

  //Version1:
  ZeroMemory(strBuf, sizeof(strBuf));
  lResult = ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER1), WM_GETTEXT, (WPARAM)sizeof(strBuf), (LPARAM)strBuf);
  stSign.wVersion1 = StrToInt(strBuf);

  //Version2:
  ZeroMemory(strBuf, sizeof(strBuf));
  lResult = ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER2), WM_GETTEXT, (WPARAM)sizeof(strBuf), (LPARAM)strBuf);
  stSign.bVersion2 = StrToInt(strBuf);

  //Version3:
  ZeroMemory(strBuf, sizeof(strBuf));
  lResult = ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_VER3), WM_GETTEXT, (WPARAM)sizeof(strBuf), (LPARAM)strBuf);
  stSign.bVersion3 = StrToInt(strBuf);

  //SoftName:
  lResult = ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_SOFT_NAME), WM_GETTEXT, (WPARAM)sizeof(stSoftName.strBuffer), (LPARAM)(stSoftName.strBuffer));
  stSoftName.bLength = (BYTE)lResult;

  //Author:
  lResult = ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_AUTHOR), WM_GETTEXT, (WPARAM)sizeof(stAuthor.strBuffer), (LPARAM)(stAuthor.strBuffer));
  stAuthor.bLength = (BYTE)lResult;

  //Email:
  lResult = ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_SIGN_EMAIL), WM_GETTEXT, (WPARAM)sizeof(stEmail.strBuffer), (LPARAM)(stEmail.strBuffer));
  stEmail.bLength = (BYTE)lResult;

  //����ǩ�����ܳ���dwSignLength:
  dwSignLength = sizeof(struct SSoftSign);
  dwSignLength += (sizeof(stSoftName.bLength) + stSoftName.bLength); //SoftName�ĳ���,�����ַ�����ֹ��'\';
  dwSignLength += (sizeof(stAuthor.bLength) + stAuthor.bLength);     //Author�ĳ���,�����ַ�����ֹ��'\';
  dwSignLength += (sizeof(stEmail.bLength) + stEmail.bLength);       //EMail�ĳ���,�����ַ�����ֹ��'\';
  dwSignLength += SKIP_LENGTH; //��������β��SKIP_LENGTH���ֽڵ�λ�ô���ʼ���ǩ����Ϣ;
  
  //ȡ��PE�ļ�����;
  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("ǩ��ʱ,�Ҳ���PE�ṹ", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }
  
  if((NULL == lpFmtPe->lpBlkTbl) || (NULL == lpFmtPe->lpFilHdr))
  {
    MessageBox("ǩ��ʱ,�ڱ���Ч", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }
  
  //����ǩ�����ܳ���,���ҿ�ִ�еĴ����,�����ش���ε���ʼ��ַ(FOA:PointerToRawData);
  aSections = lpFmtPe->lpBlkTbl;
  wCount = (WORD)(lpFmtPe->lpFilHdr->NumberOfSections);
  for(i = 0; i < wCount; i++)
  {
    //get section data
    lpSection = NULL;
    lpSection = aSections + i;
    
    //�ýھ��пɶ�(R)����ִ��(E)����������(C)������,��ýھ��Ǵ����(.text/CODE);����ýں����ʣ��ռ䳤�ȴ���ǩ����Ϣ�ܳ���,��ý��ʺϴ��ǩ����Ϣ;
    if((lpSection->Characteristics & IMAGE_SCN_MEM_READ) && (lpSection->Characteristics & IMAGE_SCN_MEM_EXECUTE) && (lpSection->Characteristics & IMAGE_SCN_CNT_CODE) && ((lpSection->SizeOfRawData - lpSection->Misc.VirtualSize) > dwSignLength))
    {
      break;
    }
  }

  if(i == wCount)
  {
    MessageBox("ǩ����Ϣ����̫��,�Ҳ������ʵĵط����ǩ����Ϣ", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }

  if(NULL == lpSection)
  {
    MessageBox("ǩ��ʱ,�ҵ���λ������Ч��", "������Ϣ", MB_OK | MB_ICONERROR);
    return;
  }
  
  //Enable the sign control
  this->EnableSignControl(TRUE);

  //�ҵ����������Ĵ����֮��,ȡ�øö��ڴ����ļ��е���ʼ��ַ;
  strSignBase = strFileBase + lpSection->PointerToRawData + lpSection->Misc.VirtualSize + SKIP_LENGTH;
  pos = strSignBase;
  
  //д��ǩ����Ϣ;
  //Version;
  lpSign = (struct SSoftSign*)pos;
  lpSign->bLength = (BYTE)(dwSignLength - SKIP_LENGTH);
  lpSign->wVersion1 = stSign.wVersion1;
  lpSign->bVersion2 = stSign.bVersion2;
  lpSign->bVersion3 = stSign.bVersion3;
  pos += sizeof(struct SSoftSign);

  //SoftName:
  lpStr = (struct SString*)pos;
  lpStr->bLength = stSoftName.bLength;
  CopyMemory(lpStr->strBuffer, stSoftName.strBuffer, stSoftName.bLength);
  //lpSign->dwSoftName = pos - strFileBase - lpSection->PointerToRawData;
  pos += (sizeof(stSoftName.bLength) + stSoftName.bLength);

  //Author:
  lpStr = (struct SString*)pos;
  lpStr->bLength = stAuthor.bLength;
  CopyMemory(lpStr->strBuffer, stAuthor.strBuffer, stAuthor.bLength);
  //lpSign->dwAuthor = pos - strFileBase - lpSection->PointerToRawData;
  pos += (sizeof(stAuthor.bLength) + stAuthor.bLength);

  //Email:
  lpStr = (struct SString*)pos;
  lpStr->bLength = stEmail.bLength;
  CopyMemory(lpStr->strBuffer, stEmail.strBuffer, stEmail.bLength);
  //lpSign->dwEmail = pos - strFileBase - lpSection->PointerToRawData;
  pos += (sizeof(stEmail.bLength) + stEmail.bLength);
  
  //Encrpty Sign
  g_lpPeFile->Encrpty(strSignBase + sizeof(lpSign->bLength), (WORD)(lpSign->bLength - sizeof(lpSign->bLength)), KEY_ENCRPTY);
  
  //Disable the sign control
  this->EnableSignControl(FALSE);
  //EnableWindow(::GetDlgItem(this->m_hWnd, IDC_BTN_SOFT_SIGN), FALSE);

  //֪ͨ������,����֪ͨ��1��ҳ����°�Ȩ��Ϣ;
  this->m_pParentWnd->SendMessage(UDM_CR_UPDATE, 0, 0);

  MessageBox("Signed OK", "Tip", MB_OK | MB_ICONINFORMATION);
}
