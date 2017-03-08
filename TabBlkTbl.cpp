// TabBlkTbl.cpp : implementation file
//

#include "stdafx.h"
#include "PeFile.h"
#include "PeToolM.h"
#include "TabBlkTbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CNT_LIST_COLS 7

static struct SListHead _stLvHdr[CNT_LIST_COLS] =
{
  {TEXT("Name"),             60,  LVCFMT_CENTER}, //[0]
  {TEXT("VirtualAddress"),   96,  LVCFMT_CENTER}, //[1]
  {TEXT("VirtualSize"),      80,  LVCFMT_CENTER}, //[2]
  {TEXT("PointerToRawData"), 110, LVCFMT_CENTER}, //[3]
  {TEXT("SizeOfRawData"),    96,  LVCFMT_CENTER}, //[4]
  {TEXT("Characteristics"),  102, LVCFMT_CENTER}, //[5]
  {TEXT("Description"),      78,  LVCFMT_LEFT}    //[6]
};

/////////////////////////////////////////////////////////////////////////////
// CTabBlkTbl dialog

CTabBlkTbl::CTabBlkTbl(CWnd* pParent /*=NULL*/)
	: TabDialog(CTabBlkTbl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabBlkTbl)
	//}}AFX_DATA_INIT
}


void CTabBlkTbl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabBlkTbl)
	DDX_Control(pDX, IDC_LIST_SECTION_TABLE, m_oLvBlkTbl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabBlkTbl, CDialog)
	//{{AFX_MSG_MAP(CTabBlkTbl)
  ON_MESSAGE(UDM_FILLFORM, OnFillForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//Common Member Function
BOOL CTabBlkTbl::Fill(LPVOID lpParam/* = NULL*/)
{
  int iRow = 0;
  WORD i = 0, wCount = 0;
  PFORMAT_PE lpFmtPe = NULL;
  PIMAGE_SECTION_HEADER aSections = NULL;
  PIMAGE_SECTION_HEADER lpSection = NULL;
  TCHAR strMsg[TEMP_BUFF_SIZE];

  this->m_oLvBlkTbl.DeleteAllItems();

  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("读取SectionTable时,PE结构无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  if((NULL == lpFmtPe->lpBlkTbl) || (NULL == lpFmtPe->lpFilHdr))
  {
    MessageBox("读取的SectionTable无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  aSections = lpFmtPe->lpBlkTbl;
  wCount = (WORD)(lpFmtPe->lpFilHdr->NumberOfSections);

  for(i = 0; i < wCount; i++)
  {
    //get data
    lpSection = aSections + i;

    //STEP1: insert new row
    iRow = this->m_oLvBlkTbl.InsertItem(i, "11");
    
    //STEP2: set the text for every column
    ////Name:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%s", lpSection->Name);
    this->m_oLvBlkTbl.SetItemText(iRow, 0, strMsg);

    ////VirtualAddress:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", lpSection->VirtualAddress);
    this->m_oLvBlkTbl.SetItemText(iRow, 1, strMsg);

    ////VirtualSize:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", lpSection->Misc.VirtualSize);
    this->m_oLvBlkTbl.SetItemText(iRow, 2, strMsg);

    ////PointerToRawData:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", lpSection->PointerToRawData);
    this->m_oLvBlkTbl.SetItemText(iRow, 3, strMsg);

    ////SizeOfRawData:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", lpSection->SizeOfRawData);
    this->m_oLvBlkTbl.SetItemText(iRow, 4, strMsg);

    ////Characteristics:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", lpSection->Characteristics);
    this->m_oLvBlkTbl.SetItemText(iRow, 5, strMsg);

    //Description
    g_lpPeFile->GetSectionProperty(strMsg, sizeof(strMsg), lpSection->Characteristics);
    this->m_oLvBlkTbl.SetItemText(iRow, 6, strMsg);
  }
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTabBlkTbl message handlers

BOOL CTabBlkTbl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->InitListView(this->m_oLvBlkTbl.m_hWnd, _stLvHdr, CNT_LIST_COLS);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabBlkTbl::OnFillForm(WPARAM wParam, LPARAM lParam)
{
  this->Fill();
}
