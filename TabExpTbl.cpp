// TabExpTbl.cpp : implementation file
//

#include "stdafx.h"
#include "PeFile.h"
#include "PeToolM.h"
#include "TabExpTbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CNT_LIST_COLS 4

static struct SListHead _stLvHdr[CNT_LIST_COLS] =
{
  {TEXT("Ordinal"),  60, LVCFMT_CENTER}, //[0]
  {TEXT("Hint"),     60, LVCFMT_CENTER}, //[1]
  {TEXT("Function"), 96, LVCFMT_LEFT},   //[2]
  {TEXT("Address"),  60, LVCFMT_CENTER}  //[3]
};

/////////////////////////////////////////////////////////////////////////////
// CTabExpTbl dialog

CTabExpTbl::CTabExpTbl(CWnd* pParent /*=NULL*/)
	: TabDialog(CTabExpTbl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabExpTbl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabExpTbl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabExpTbl)
	DDX_Control(pDX, IDC_LIST_EXPORT_TABLE, m_oLvExpTbl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabExpTbl, CDialog)
	//{{AFX_MSG_MAP(CTabExpTbl)
  ON_MESSAGE(UDM_FILLFORM, OnFillForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//Common Member Function
BOOL CTabExpTbl::Fill(LPVOID lpParam/* = NULL*/)
{
  LPBYTE strFileBase = NULL;
  PFORMAT_PE lpFmtPe = NULL;
  PIMAGE_EXPORT_DIRECTORY lpIED = NULL;
  LPDWORD adwRvaAddrs = NULL;
  LPDWORD adwRvaNames = NULL;
  LPWORD awOrdinals = NULL;
  WORD i = 0, wOrdinal = 0;
  DWORD dwFoa = 0, dwAddr = 0;
  LPCSTR strName = NULL;
  int iRow = 0;
  TCHAR strMsg[TEMP_BUFF_SIZE];

  this->m_oLvExpTbl.DeleteAllItems();

  strFileBase = g_lpPeFile->GetBase();
  if(NULL == strFileBase)
  {
    MessageBox("读取ExportTable时,文件基址无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("读取ExportTable时,PE结构无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  if((NULL == lpFmtPe->lpExpTbl) || (NULL == lpFmtPe->lpExpBlk))
  {
    MessageBox("读取的ExportTable无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  if(0 == lpFmtPe->lpExpBlk->Size)
  {
    //insert one row;
    iRow = this->m_oLvExpTbl.InsertItem(1, "11");
    
    //fill the first column;
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%s", NO_PE_DATA);
    this->m_oLvExpTbl.SetItemText(iRow, 0, strMsg);
    return FALSE;
  }

  lpIED = lpFmtPe->lpExpTbl;

  //Characteristics:
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpIED->Characteristics);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_EXP_TBL_CHARACTERISTICS), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //TimeDateStamp:
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpIED->TimeDateStamp);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_EXP_TBL_TIMEDATESTAMP), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //MajorVersion:
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpIED->MajorVersion);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_EXP_TBL_MAJORVERSION), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //MinorVersion:
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %04X", lpIED->MinorVersion);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_EXP_TBL_MINORVERSION), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //Name:
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpIED->Name);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_EXP_TBL_NAME), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //Base:
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpIED->Base);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_EXP_TBL_BASE), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //NumberOfFunctions:
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpIED->NumberOfFunctions);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_EXP_TBL_NUMBEROFFUNCTIONS), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //NumberOfNames:
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpIED->NumberOfNames);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_EXP_TBL_NUMBEROFNAMES), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //AddressOfFunctions:
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpIED->AddressOfFunctions);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_EXP_TBL_ADDRESSOFFUNCTIONS), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //AddressOfNames:
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpIED->AddressOfNames);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_EXP_TBL_ADDRESSOFNAMES), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //AddressOfNameOrdinals:
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpIED->AddressOfNameOrdinals);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_EXP_TBL_ADDRESSOFNAMEORDINALS), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //DllName:
  dwFoa = g_lpPeFile->Rva2Foa(lpIED->Name);
  strName = (LPCSTR)strFileBase + dwFoa;
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %s", strName);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_EXP_TBL_DLLNAME), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //MkTime:
  ZeroMemory(strMsg, sizeof(strMsg));
  strftime(strMsg, sizeof(strMsg), " %Y-%m-%d %H:%M:%S ", localtime((const time_t*)&(lpIED->TimeDateStamp)));
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_EXP_TBL_MKTIME), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //函数地址表
  dwFoa = g_lpPeFile->Rva2Foa(lpIED->AddressOfFunctions);
  adwRvaAddrs = (LPDWORD)(strFileBase + dwFoa); //RVA Array,so,adwRvaAddrs[i] is a RVA;

  //函数名称表
  dwFoa = g_lpPeFile->Rva2Foa(lpIED->AddressOfNames);
  adwRvaNames = (LPDWORD)(strFileBase + dwFoa); //RVA Array,so,adwRvaNames[i] is a RVA;

  //函数序数表
  dwFoa = g_lpPeFile->Rva2Foa(lpIED->AddressOfNameOrdinals);
  awOrdinals = (LPWORD)(strFileBase + dwFoa);
  
  //导出函数列表视图
  for(i = 0; i < lpIED->NumberOfNames; i++)
  {
    //Name:
    dwFoa = g_lpPeFile->Rva2Foa(adwRvaNames[i]);
    strName = (LPCSTR)strFileBase + dwFoa;

    //Ordinal:
    wOrdinal = awOrdinals[i];

    //Address:
    dwAddr = adwRvaAddrs[wOrdinal];

    //STEP1: insert new row
    iRow = this->m_oLvExpTbl.InsertItem(i, "11");
    
    //STEP2: set the text for every column
    ////Oridinal:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%04X", wOrdinal + lpIED->Base);
    this->m_oLvExpTbl.SetItemText(iRow, 0, strMsg);

    ////Hint:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%04X", wOrdinal);
    this->m_oLvExpTbl.SetItemText(iRow, 1, strMsg);

    ////Function:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%s", strName);
    this->m_oLvExpTbl.SetItemText(iRow, 2, strMsg);

    ////Address:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", dwAddr);
    this->m_oLvExpTbl.SetItemText(iRow, 3, strMsg);
  }
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTabExpTbl message handlers

BOOL CTabExpTbl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->InitListView(this->m_oLvExpTbl.m_hWnd, _stLvHdr, CNT_LIST_COLS);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabExpTbl::OnFillForm(WPARAM wParam, LPARAM lParam)
{
  this->Fill();
}
