// TabImpTbl.cpp : implementation file
//

#include "stdafx.h"
#include "PeFile.h"
#include "PeToolM.h"
#include "TabImpTbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//The header of Import Table List View
static struct SListHead _stLvHdrImpTbl[] =
{
  {TEXT("OriginalFirstThunk"), 120, LVCFMT_CENTER}, //[0]
  {TEXT("TimeDateStamp"),      90,  LVCFMT_CENTER}, //[1]
  {TEXT("ForwarderChain"),     96,  LVCFMT_CENTER}, //[2]
  {TEXT("NameRVA"),            60,  LVCFMT_CENTER}, //[3]
  {TEXT("NameFOA"),            60,  LVCFMT_CENTER}, //[4]
  {TEXT("Name"),               90,  LVCFMT_LEFT},   //[5]
  {TEXT("FirstThunk"),         75,  LVCFMT_CENTER}, //[6]
  {NULL, 0, 0}                                      //[7]
};

//The header of Import Table Detail List View
static struct SListHead _stLvHdrImpDtl[] =
{
  {TEXT("RVA"),      60,  LVCFMT_CENTER}, //[0]
  {TEXT("FOA"),      60,  LVCFMT_CENTER}, //[1]
  {TEXT("IAT"),      60,  LVCFMT_CENTER}, //[2]
  {TEXT("Hint"),     60,  LVCFMT_CENTER}, //[3]
  {TEXT("Function"), 463, LVCFMT_LEFT},   //[4]
  {NULL, 0, 0}                            //[5]
};

/////////////////////////////////////////////////////////////////////////////
// CTabImpTbl dialog

CTabImpTbl::CTabImpTbl(CWnd* pParent /*=NULL*/)
	: TabDialog(CTabImpTbl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabImpTbl)
	//}}AFX_DATA_INIT
  this->m_iRowSelectedLast = -10000;
  this->m_bImpTblIsEmpty = FALSE;
}


void CTabImpTbl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabImpTbl)
	DDX_Control(pDX, IDC_LIST_IMPORT_DETAIL, m_oLvImpDtl);
	DDX_Control(pDX, IDC_LIST_IMPORT_TABLE, m_oLvImpTbl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabImpTbl, CDialog)
	//{{AFX_MSG_MAP(CTabImpTbl)
  ON_MESSAGE(UDM_FILLFORM, OnFillForm)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_IMPORT_TABLE, OnItemchangedListImportTable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//Common Member Function
BOOL CTabImpTbl::Fill(LPVOID lpParam/* = NULL*/)
{
  int iRow = 0;
  WORD i = 0;
  DWORD dwFoa = 0;
  LPBYTE strFileBase = NULL;
  PFORMAT_PE lpFmtPe = NULL;
  PIMAGE_IMPORT_DESCRIPTOR lpIID = NULL;
  TCHAR strMsg[TEMP_BUFF_SIZE];

  this->m_oLvImpTbl.DeleteAllItems();
  this->m_oLvImpDtl.DeleteAllItems();

  strFileBase = g_lpPeFile->GetBase();
  if(NULL == strFileBase)
  {
    MessageBox("读取ImportTable时,文件基址无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("读取ImportTable时,PE结构无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  if((NULL == lpFmtPe->lpImpTbl) || (NULL == lpFmtPe->lpImpBlk))
  {
    MessageBox("读取的ImportTable无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  if(0 == lpFmtPe->lpImpBlk->Size)
  {
    //insert one row;
    iRow = this->m_oLvImpTbl.InsertItem(1, "11");

    //fill the first column;
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%s", NO_PE_DATA);
    this->m_oLvImpTbl.SetItemText(iRow, 0, strMsg);
    this->m_bImpTblIsEmpty = TRUE;
    return FALSE;
  }

  lpIID = lpFmtPe->lpImpTbl;

  for(i = 0; lpIID->Name; lpIID++, i++)
  {
    //STEP1: insert new row
    iRow = this->m_oLvImpTbl.InsertItem(i, "11");

    //STEP2: set the text for every column
    ////OriginalFirstThunk:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", lpIID->OriginalFirstThunk);
    this->m_oLvImpTbl.SetItemText(iRow, 0, strMsg);

    ////TimeDateStamp:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", lpIID->TimeDateStamp);
    this->m_oLvImpTbl.SetItemText(iRow, 1, strMsg);

    ////ForwarderChain:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", lpIID->ForwarderChain);
    this->m_oLvImpTbl.SetItemText(iRow, 2, strMsg);

    ////NameRVA:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", lpIID->Name);
    this->m_oLvImpTbl.SetItemText(iRow, 3, strMsg);

    ////NameFOA:
    dwFoa = g_lpPeFile->Rva2Foa(lpIID->Name);
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", dwFoa);
    this->m_oLvImpTbl.SetItemText(iRow, 4, strMsg);

    ////DllName String:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%s", strFileBase + dwFoa);
    this->m_oLvImpTbl.SetItemText(iRow, 5, strMsg);

    ////FirstThunk:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", lpIID->FirstThunk);
    this->m_oLvImpTbl.SetItemText(iRow, 6, strMsg);
  }
  this->m_bImpTblIsEmpty = FALSE;
  return TRUE;
}

BOOL CTabImpTbl::FillDetail(LPNM_LISTVIEW lpLv)
{
  LPBYTE strFileBase = NULL;
  PFORMAT_PE lpFmtPe = NULL;
  DWORD dwRva = 0, dwFoa = 0;
  LPDWORD dwaINT = NULL, dwaIAT = NULL;
  DWORD dwThkINT = 0, dwThkIAT = 0;
  PIMAGE_IMPORT_BY_NAME lpByName = NULL;
  WORD i = 0, wOrdinal = 0;
  int iRow = 0, iLen = 0;
  TCHAR strMsg[TEMP_BUFF_SIZE];
  
  this->m_oLvImpDtl.DeleteAllItems();
  
  strFileBase = g_lpPeFile->GetBase();
  if(NULL == strFileBase)
  {
    MessageBox("读取ImportTable详情时,文件基址无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }
  
  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("读取ImportTable详情时,PE结构无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }
  
  if((NULL == lpFmtPe->lpImpTbl) || (NULL == lpFmtPe->lpImpBlk))
  {
    MessageBox("读取的ImportTable详情无效", "错误信息", MB_OK | MB_ICONERROR);
    return FALSE;
  }
  
  //读取导入表视图第0列OriginalFirstThunk的值,并定位Original First Thunk Table,用于填充Import Name Table(INT)列表视图;
  ZeroMemory(strMsg, sizeof(strMsg));
  iLen = this->m_oLvImpTbl.GetItemText(lpLv->iItem, 0, strMsg, sizeof(strMsg));
  iLen = sscanf(strMsg, "%08X", &dwRva);
  dwFoa = g_lpPeFile->Rva2Foa(dwRva);
  dwaINT = (LPDWORD)(strFileBase + dwFoa);
  
  //读取导入表视图第6列FirstThunk的值,并定位First Thunk Table,用于填充Import Address Table(IAT)列表视图;
  ZeroMemory(strMsg, sizeof(strMsg));
  iLen = this->m_oLvImpTbl.GetItemText(lpLv->iItem, 6, strMsg, sizeof(strMsg));
  iLen = sscanf(strMsg, "%08X", &dwRva);
  dwFoa = g_lpPeFile->Rva2Foa(dwRva);
  dwaIAT = (LPDWORD)(strFileBase + dwFoa);
  
  //Iterate the Import Name Table(INT) and Import Address Table(IAT),then,Fill the Import Name Table List View and Import Address Table List View;
  for(i = 0; (dwThkINT = dwaINT[i]) && (dwThkIAT = dwaIAT[i]); i++)
  {
    //STEP1: insert new row
    iRow = this->m_oLvImpDtl.InsertItem(i, "11");
    
    //STEP2: set the text for every column
    ////RVA:
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%08X", dwThkINT);
    this->m_oLvImpDtl.SetItemText(iRow, 0, strMsg);
    
    if(IMAGE_SNAP_BY_ORDINAL32(dwThkINT)) //Import By Ordinal
    {
      wOrdinal = (WORD)IMAGE_ORDINAL32(dwThkINT);
      
      ////FOA:
      ZeroMemory(strMsg, sizeof(strMsg));
      wsprintf(strMsg, "%08X", 0);
      this->m_oLvImpDtl.SetItemText(iRow, 1, strMsg);
      
      ////IAT:
      ZeroMemory(strMsg, sizeof(strMsg));
      wsprintf(strMsg, "%08X", dwThkIAT);
      this->m_oLvImpDtl.SetItemText(iRow, 2, strMsg);
      
      ////Hint:
      ZeroMemory(strMsg, sizeof(strMsg));
      wsprintf(strMsg, "%04X", wOrdinal);
      this->m_oLvImpDtl.SetItemText(iRow, 3, strMsg);
      
      ////Function:
      ZeroMemory(strMsg, sizeof(strMsg));
      wsprintf(strMsg, "%s", "by ordinal");
      this->m_oLvImpDtl.SetItemText(iRow, 4, strMsg);
    }
    else                                  //Import By Name
    {
      ////FOA:
      dwFoa = g_lpPeFile->Rva2Foa(dwThkINT);
      ZeroMemory(strMsg, sizeof(strMsg));
      wsprintf(strMsg, "%08X", dwFoa);
      this->m_oLvImpDtl.SetItemText(iRow, 1, strMsg);
      
      ////IAT:
      ZeroMemory(strMsg, sizeof(strMsg));
      wsprintf(strMsg, "%08X", dwThkIAT);
      this->m_oLvImpDtl.SetItemText(iRow, 2, strMsg);
      
      ////Hint:
      lpByName = (PIMAGE_IMPORT_BY_NAME)(strFileBase + dwFoa);
      ZeroMemory(strMsg, sizeof(strMsg));
      wsprintf(strMsg, "%04X", lpByName->Hint);
      this->m_oLvImpDtl.SetItemText(iRow, 3, strMsg);
      
      ////Function:
      ZeroMemory(strMsg, sizeof(strMsg));
      wsprintf(strMsg, "%s", lpByName->Name);
      this->m_oLvImpDtl.SetItemText(iRow, 4, strMsg);
    }

    /*
    if(dwThkIAT == dwThkINT) //INT
    {
      if(IMAGE_SNAP_BY_ORDINAL32(dwThkINT)) //Import By Ordinal
      {
        wOrdinal = (WORD)IMAGE_ORDINAL32(dwThkINT);
        
        ////FOA:
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%08X", 0);
        this->m_oLvImpDtl.SetItemText(iRow, 1, strMsg);
        
        ////IAT:
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%08X", dwThkIAT);
        this->m_oLvImpDtl.SetItemText(iRow, 2, strMsg);
        
        ////Hint:
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%04X", wOrdinal);
        this->m_oLvImpDtl.SetItemText(iRow, 3, strMsg);
        
        ////Function:
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%s", "by ordinal");
        this->m_oLvImpDtl.SetItemText(iRow, 4, strMsg);
      }
      else                                  //Import By Name
      {
        ////FOA:
        dwFoa = g_lpPeFile->Rva2Foa(dwThkINT);
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%08X", dwFoa);
        this->m_oLvImpDtl.SetItemText(iRow, 1, strMsg);
        
        ////IAT:
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%08X", dwThkIAT);
        this->m_oLvImpDtl.SetItemText(iRow, 2, strMsg);
        
        ////Hint:
        lpByName = (PIMAGE_IMPORT_BY_NAME)(strFileBase + dwFoa);
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%04X", lpByName->Hint);
        this->m_oLvImpDtl.SetItemText(iRow, 3, strMsg);
        
        ////Function:
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%s", lpByName->Name);
        this->m_oLvImpDtl.SetItemText(iRow, 4, strMsg);
      }
    }
    else                     //IAT
    {
      if(IMAGE_SNAP_BY_ORDINAL32(dwThkINT)) //Import By Ordinal
      {
        wOrdinal = (WORD)IMAGE_ORDINAL32(dwThkINT);
        
        ////FOA:
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%08X", 0);
        this->m_oLvImpDtl.SetItemText(iRow, 1, strMsg);
        
        ////IAT:
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%08X", dwThkIAT);
        this->m_oLvImpDtl.SetItemText(iRow, 2, strMsg);
        
        ////Hint:
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%04X", wOrdinal);
        this->m_oLvImpDtl.SetItemText(iRow, 3, strMsg);
        
        ////Function:
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%s", "by ordinal");
        this->m_oLvImpDtl.SetItemText(iRow, 4, strMsg);
      }
      else                                  //Import By Name
      {
        ////FOA:
        dwFoa = g_lpPeFile->Rva2Foa(dwThkINT);
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%08X", dwFoa);
        this->m_oLvImpDtl.SetItemText(iRow, 1, strMsg);
        
        ////IAT:
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%08X", dwThkIAT);
        this->m_oLvImpDtl.SetItemText(iRow, 2, strMsg);
        
        ////Hint:
        lpByName = (PIMAGE_IMPORT_BY_NAME)(strFileBase + dwFoa);
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%04X", lpByName->Hint);
        this->m_oLvImpDtl.SetItemText(iRow, 3, strMsg);
        
        ////Function:
        ZeroMemory(strMsg, sizeof(strMsg));
        wsprintf(strMsg, "%s", lpByName->Name);
        this->m_oLvImpDtl.SetItemText(iRow, 4, strMsg);
      }
    }
    */
  }
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTabImpTbl message handlers

BOOL CTabImpTbl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->InitListView(this->m_oLvImpTbl.m_hWnd, _stLvHdrImpTbl);
  this->InitListView(this->m_oLvImpDtl.m_hWnd, _stLvHdrImpDtl);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabImpTbl::OnFillForm(WPARAM wParam, LPARAM lParam)
{
  this->m_iRowSelectedLast = -10000;
  this->Fill();
}

void CTabImpTbl::OnItemchangedListImportTable(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

  if(TRUE == this->m_bImpTblIsEmpty)
  {
    return;
  }

  if(pNMListView->uChanged & LVIF_STATE)  //状态发生改变;
  {
    //新状态为:有焦点,切选中;
    if((pNMListView->uNewState & LVIS_FOCUSED) && (pNMListView->uNewState & LVIS_SELECTED))
    {
      if((-1 != pNMListView->iItem) && (pNMListView->iItem != this->m_iRowSelectedLast))
      {
        this->FillDetail(pNMListView);
        this->m_iRowSelectedLast = pNMListView->iItem;
      }
    }
  }

	*pResult = 0;
}
