// TabRlcTbl.cpp : implementation file
//

#include "stdafx.h"
#include "PeFile.h"
#include "PeToolM.h"
#include "TabRlcTbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CNT_LIST_COLS 5

static struct SListHead _stLvHdr[CNT_LIST_COLS] =
{
  {TEXT("TypeOffset"),  80, LVCFMT_CENTER}, //[0]
  {TEXT("CodeOffset"),  80, LVCFMT_CENTER}, //[1]
  {TEXT("CodeRVA"),     60, LVCFMT_CENTER}, //[2]
  {TEXT("CodeVA"),      96, LVCFMT_CENTER}, //[3]
  {TEXT("MachineCode"), 80, LVCFMT_CENTER}  //[4]
};
  
/////////////////////////////////////////////////////////////////////////////
// CTabRlcTbl dialog

CTabRlcTbl::CTabRlcTbl(CWnd* pParent /*=NULL*/)
	: TabDialog(CTabRlcTbl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabRlcTbl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTabRlcTbl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabRlcTbl)
	DDX_Control(pDX, IDC_LIST_BASE_RELOCATION, m_oLvRlcTbl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabRlcTbl, CDialog)
	//{{AFX_MSG_MAP(CTabRlcTbl)
  ON_MESSAGE(UDM_FILLFORM, OnFillForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//Common Member Function
BOOL CTabRlcTbl::Fill(LPVOID lpParam/* = NULL*/)
{
  int iRow = 0;
  WORD i = 0;
  LPBYTE strFileBase = NULL;
  LPBYTE strRlcBase = NULL;
  PFORMAT_PE lpFmtPe = NULL;
  PIMAGE_BASE_RELOCATION lpRlcTbl = NULL;
  LPWORD awTypeOffsets = NULL;
  LPBYTE strMachineCodes = NULL;
  DWORD dwFoa = 0, dwBytesOfRlc = 0, dwNumberOfRlc = 0;
  WORD wTypeOffset = 0, wCodeOffset = 0;
  DWORD dwCodeRVA = 0, dwCodeVA = 0, dwMachineCode = 0;
  TCHAR strMsg[TEMP_BUFF_SIZE];

  this->m_oLvRlcTbl.DeleteAllItems();

  strFileBase = g_lpPeFile->GetBase();
  if(NULL == strFileBase)
  {
    MessageBox("获取重定位表时,文件基址无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("读取重定位表时,PE结构无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  if((NULL == lpFmtPe->lpRlcTbl) || (NULL == lpFmtPe->lpRlcBlk) || (NULL == lpFmtPe->lpOptHdr))
  {
    MessageBox("读取的重定位表无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  if(0 == lpFmtPe->lpRlcBlk->Size)
  {
    //insert one row;
    iRow = this->m_oLvRlcTbl.InsertItem(1, "11");
    
    //fill the first column;
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%s", NO_PE_DATA);
    this->m_oLvRlcTbl.SetItemText(iRow, 0, strMsg);
    return FALSE;
  }

  lpRlcTbl = lpFmtPe->lpRlcTbl;
  
  /*
  if((8 >= lpRlcTbl->SizeOfBlock) || (0!= (lpRlcTbl->SizeOfBlock % 2)))
  {
    //insert one row;
    iRow = this->m_oLvRlcTbl.InsertItem(1, "11");
    
    //fill the first column;
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%s", "exception occurred");
    this->m_oLvRlcTbl.SetItemText(iRow, 0, strMsg);
    return FALSE;
  }
  */

  strRlcBase = (LPBYTE)lpRlcTbl;

  //VirtualAddress:
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpRlcTbl->VirtualAddress);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_RLC_TBL_VIRTUALADDRESS), WM_SETTEXT, NULL, (LPARAM)strMsg);
  
  //FOA:
  dwFoa = g_lpPeFile->Rva2Foa(lpRlcTbl->VirtualAddress);
  strMachineCodes = (LPBYTE)(strFileBase + dwFoa);
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", dwFoa);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_RLC_TBL_FOA), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //SizeOfBlock:
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", lpRlcTbl->SizeOfBlock);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_RLC_TBL_SIZEOFBLOCK), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //BytesOfReloc:
  dwBytesOfRlc = lpRlcTbl->SizeOfBlock - 8;
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", dwBytesOfRlc);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_RLC_TBL_BYTESOFRELOC), WM_SETTEXT, NULL, (LPARAM)strMsg);

  //NumberOfReloc:
  dwNumberOfRlc = dwBytesOfRlc / 2;
  ZeroMemory(strMsg, sizeof(strMsg));
  _snprintf(strMsg, sizeof(strMsg), " %08X", dwNumberOfRlc);
  ::SendMessage(::GetDlgItem(this->m_hWnd, IDC_EDT_RLC_TBL_NUMBEROFRELOC), WM_SETTEXT, NULL, (LPARAM)strMsg);

  awTypeOffsets = (LPWORD)(strRlcBase + 8);
  for(i = 0; i < dwNumberOfRlc; i++)
  {
    wTypeOffset   = awTypeOffsets[i];
    wCodeOffset   = wTypeOffset & 0x0FFF;                        //需要进行重定位的数据在其所属块中的偏移地址;
    dwCodeRVA     = wCodeOffset + lpRlcTbl->VirtualAddress;      //需要进行重定位的数据在其所属块中的偏移地址的RVA;
    dwCodeVA      = dwCodeRVA + lpFmtPe->lpOptHdr->ImageBase;    //需要进行重定位的数据在其所属块中的偏移地址的VA;
    dwMachineCode = *((LPDWORD)(strMachineCodes + wCodeOffset)); //直接寻址指令中,需要进行重定位的地址数据(存在于指令码中);

    //STEP1: insert new row
    iRow = this->m_oLvRlcTbl.InsertItem(i, "11");
    
    //STEP2: set the text for every column
    ////TypeOffset:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%04X", wTypeOffset);
    this->m_oLvRlcTbl.SetItemText(iRow, 0, strMsg);

    ////CodeOffset:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%04X", wCodeOffset);
    this->m_oLvRlcTbl.SetItemText(iRow, 1, strMsg);

    ////CodeRVA:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", dwCodeRVA);
    this->m_oLvRlcTbl.SetItemText(iRow, 2, strMsg);

    ////CodeVA:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", dwCodeVA);
    this->m_oLvRlcTbl.SetItemText(iRow, 3, strMsg);

    //MachineCode:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", dwMachineCode);
    this->m_oLvRlcTbl.SetItemText(iRow, 4, strMsg);
  }
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTabRlcTbl message handlers

BOOL CTabRlcTbl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->InitListView(this->m_oLvRlcTbl.m_hWnd, _stLvHdr, CNT_LIST_COLS);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabRlcTbl::OnFillForm(WPARAM wParam, LPARAM lParam)
{
  this->Fill();
}
