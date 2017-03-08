// TabDatDir.cpp : implementation file
//

#include "stdafx.h"
#include "PeFile.h"
#include "PeToolM.h"
#include "TabDatDir.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CNT_LIST_COLS 4

static struct SListHead _stLvHdr[CNT_LIST_COLS] =
{
  {TEXT("Type"),           40,  LVCFMT_CENTER}, //[0]
  {TEXT("Name"),           190, LVCFMT_LEFT},   //[1]
  {TEXT("VirtualAddress"), 96,  LVCFMT_CENTER}, //[2]
  {TEXT("Size"),           60,  LVCFMT_CENTER}  //[3]
};

static LPCTSTR strDatDirTypeName[IMAGE_NUMBEROF_DIRECTORY_ENTRIES] =
{
  "Export Directory",              //0
  "Import Directory",              //1
  "Resource Directory",            //2
  "Exception Directory",           //3
  "Security Directory",            //4
  "Base Relocation Table",         //5
  "Debug Directory",               //6
  "Architecture Specific Data",    //7
  "RVA of GP",                     //8
  "TLS Directory",                 //9
  "Load Configuration Directory",  //10
  "Bound Import Directory",        //11
  "Import Address Table",          //12
  "Delay Load Import Descriptors", //13
  "COM Runtime descriptor",        //14
  "UnUsed"                         //15
};

/////////////////////////////////////////////////////////////////////////////
// CTabDatDir dialog


CTabDatDir::CTabDatDir(CWnd* pParent /*=NULL*/)
	: TabDialog(CTabDatDir::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabDatDir)
	//}}AFX_DATA_INIT
}


void CTabDatDir::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabDatDir)
	DDX_Control(pDX, IDC_LIST_DATA_DIRECTORY, m_oLvDatDir);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabDatDir, CDialog)
	//{{AFX_MSG_MAP(CTabDatDir)
  ON_MESSAGE(UDM_FILLFORM, OnFillForm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//Common Member Function
BOOL CTabDatDir::Fill(LPVOID lpParam/* = NULL*/)
{
  int iRow = 0;
  WORD i = 0, wDatDirNum = 0;
  PFORMAT_PE lpFmtPe = NULL;
  PIMAGE_DATA_DIRECTORY lpDatDir = NULL;
  PIMAGE_DATA_DIRECTORY lpData = NULL;
  TCHAR strMsg[TEMP_BUFF_SIZE];

  this->m_oLvDatDir.DeleteAllItems();

  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("读取DataDirectory时,PE结构无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  if((NULL == lpFmtPe->lpDatDir) || (NULL == lpFmtPe->lpOptHdr))
  {
    MessageBox("读取的DataDirectory无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  lpDatDir = lpFmtPe->lpDatDir;
  wDatDirNum = (WORD)(lpFmtPe->lpOptHdr->NumberOfRvaAndSizes);

  for(i = 0; i < wDatDirNum; i++)
  {
    //STEP1: insert new row
    iRow = this->m_oLvDatDir.InsertItem(i, "11");

    //STEP2: set the text for every column
    ////Type:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%02d", i);
    this->m_oLvDatDir.SetItemText(iRow, 0, strMsg);

    ////Name:
    this->m_oLvDatDir.SetItemText(iRow, 1, strDatDirTypeName[i]);

    //get data
    lpData = lpDatDir + i;

    ////VirtualAddress:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", lpData->VirtualAddress);
    this->m_oLvDatDir.SetItemText(iRow, 2, strMsg);

    ////Size:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", lpData->Size);
    this->m_oLvDatDir.SetItemText(iRow, 3, strMsg);
  }
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTabDatDir message handlers

BOOL CTabDatDir::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->InitListView(this->m_oLvDatDir.m_hWnd, _stLvHdr, CNT_LIST_COLS);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabDatDir::OnFillForm(WPARAM wParam, LPARAM lParam)
{
  this->Fill();
}
