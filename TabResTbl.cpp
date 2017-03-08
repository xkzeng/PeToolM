// TabResTbl.cpp : implementation file
//

#include "stdafx.h"
#include "PeFile.h"
#include "PeToolM.h"
#include "TabResTbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BYTES_OF_PER_LINE 60
#define EXTRA_BYTES       128
#define NODE_PARAM_VALUE  0xFFFFFFF0
#define NODE_PARAM_EMPTY  0x00000000

/////////////////////////////////////////////////////////////////////////////
// CTabResTbl dialog
CTabResTbl::CTabResTbl(CWnd* pParent /*=NULL*/)
	: CDialog(CTabResTbl::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabResTbl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
  this->m_strResTbl = NULL;
}


void CTabResTbl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabResTbl)
	DDX_Control(pDX, IDC_TREE_RESOURCE_TABLE, m_oTvResTbl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabResTbl, CDialog)
	//{{AFX_MSG_MAP(CTabResTbl)
  ON_MESSAGE(UDM_FILLFORM, OnFillForm)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_RESOURCE_TABLE, OnSelchangedTreeResourceTable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
//Common Member Function
VOID CTabResTbl::CreateTreeView(HTREEITEM hParent, WORD wDepth, LPSTR strLvlStr, WORD wCount)
{
  WORD i = 0;
  HTREEITEM hNode = NULL;
  TCHAR strCurLvlStr[TEMP_BUFF_SIZE];
  TCHAR strNodeText[TEMP_BUFF_SIZE];
  
  for(i = 1; i <= wCount; i++)
  {
    ZeroMemory(strCurLvlStr, sizeof(strCurLvlStr));
    wsprintf(strCurLvlStr, "%s%d", strLvlStr, i);
    ZeroMemory(strNodeText, sizeof(strNodeText));
    if(3 <= wDepth) //�ܹ�wDepth��,��,���ĸ߶���wDepth��;����Ѿ��ﵽ������wDepth,���wDepth����Ҷ�ӽڵ�;
    {
      wsprintf(strNodeText, "Left%s", strCurLvlStr);
      hNode = this->m_oTvResTbl.InsertItem(TEXT(strNodeText), hParent, TVI_LAST); //create a sub node
      if(hNode)
      {
        this->m_oTvResTbl.SetItemData(hNode, (DWORD)NODE_PARAM_EMPTY);
      }
    }
    else
    {
      wsprintf(strNodeText, "%s%s", ((1 == wDepth) ? "Root" : "Sub"), strCurLvlStr);
      hNode = this->m_oTvResTbl.InsertItem(TEXT(strNodeText), hParent, TVI_LAST); //create a sub node
      if(hNode)
      {
        this->m_oTvResTbl.SetItemData(hNode, (DWORD)NODE_PARAM_VALUE);
      }

      this->CreateTreeView(hNode, (WORD)(wDepth + 1), strCurLvlStr, wCount);
    }
  }
}

VOID CTabResTbl::InitTreeView(VOID)
{
  DWORD dwStyle = 0;
  
  //set Style
  dwStyle = ::GetWindowLong(this->m_oTvResTbl.m_hWnd, GWL_STYLE);
  dwStyle |= TVS_HASBUTTONS;
  dwStyle |= TVS_HASLINES;
  dwStyle |= TVS_LINESATROOT;
  ::SetWindowLong(this->m_oTvResTbl.m_hWnd, GWL_STYLE, dwStyle);
  
  //insert the table header
  this->CreateTreeView(TVI_ROOT, 1, "", 4);
}

//��Դ����ID������ͷ�ļ�WinUser.h��;
LPCTSTR CTabResTbl::Type2Name(WORD wTypeId)
{
  static LPCTSTR strResType = NULL;
  switch(wTypeId)
  {
  case RT_CURSOR: //Hardware-dependent cursor resource
    strResType = "Cursor";
    break;

  case RT_BITMAP: //Bitmap resource
    strResType = "BitMap";
    break;

  case RT_ICON: //Hardware-dependent icon resource
    strResType = "Icon";
    break;

  case RT_MENU: //Menu resource
    strResType = "Menu";
    break;

  case RT_DIALOG: //Dialog box
    strResType = "Dialog";
    break;

  case RT_STRING: //String-table entry
    strResType = "String";
    break;

  case RT_FONTDIR: //Font directory resource
    strResType = "FontDir";
    break;

  case RT_FONT: //Font resource
    strResType = "Font";
    break;

  case RT_ACCELERATOR: //Accelerator table
    strResType = "Accelerator";
    break;

  case RT_RCDATA: //Application-defined resource (raw data)
    strResType = "RCdata";
    break;

  case RT_MESSAGETABLE: //Message-table entry
    strResType = "MessageTable";
    break;

  case RT_GROUP_CURSOR: //Hardware-independent cursor resource
    strResType = "GroupCursor";
    break;

  case RT_GROUP_ICON: //Hardware-independent icon resource
    strResType = "GroupIcon";
    break;

  case RT_VERSION: //Version resource
    strResType = "Version";
    break;

  case RT_DLGINCLUDE:
    strResType = "DlgInclude";
    break;

  case RT_PLUGPLAY: //Plug and Play resource
    strResType = "PlugPlay";
    break;

  case RT_VXD: //VxD
    strResType = "VxD";
    break;

  case RT_ANICURSOR: //Animated cursor
    strResType = "AniCursor";
    break;

  case RT_ANIICON: //Animated icon
    strResType = "AniIcon";
    break;

  case RT_HTML: //HTML
    strResType = "Html";
    break;

  default:
    strResType = "UserDefined";
  }
  return strResType;
}

BOOL CTabResTbl::ParseResourceDirectory(PIMAGE_RESOURCE_DIRECTORY lpResDir, HTREEITEM hParent, WORD wDepth)
{
  PIMAGE_RESOURCE_DIRECTORY lpResSubDir = NULL;
  PIMAGE_RESOURCE_DIRECTORY_ENTRY apResDirEntry = NULL;
  PIMAGE_RESOURCE_DIRECTORY_ENTRY lpResDirEntry = NULL;
  PIMAGE_RESOURCE_DIR_STRING_U lpResName = NULL;
  PIMAGE_RESOURCE_DATA_ENTRY lpResDataEntry = NULL;
  HTREEITEM hCurNode = NULL, hLeaf = NULL;
  WORD wLoop = 0, wNumberOfDirEntries = 0;
  LPCTSTR strResType = NULL;
  TCHAR strNodeText[TEMP_BUFF_SIZE];

  //the number of IMAGE_RESOURCE_DIRECTORY_ENTRY
  wNumberOfDirEntries = lpResDir->NumberOfNamedEntries + lpResDir->NumberOfIdEntries;
  apResDirEntry = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)((LPBYTE)lpResDir + sizeof(IMAGE_RESOURCE_DIRECTORY));
  for(wLoop = 0; wLoop < wNumberOfDirEntries; wLoop++)
  {
    lpResDirEntry = apResDirEntry + wLoop;
    
    //Name
    ZeroMemory(strNodeText, sizeof(strNodeText));
    //if(lpResDirEntry->Name & 0x80000000) //bit[31] = 1, Name is a String point to IMAGE_RESOURCE_DIR_STRING_U;
    if(lpResDirEntry->NameIsString)        //this way is OK,too: lpResDirEntry->NameIsString <==> (lpResDirEntry->Name & 0x80000000)
    {
      //lpResName = (PIMAGE_RESOURCE_DIR_STRING_U)(this->m_strResTbl + (lpResDirEntry->Name & (~0x80000000)));
      lpResName = (PIMAGE_RESOURCE_DIR_STRING_U)(this->m_strResTbl + lpResDirEntry->NameOffset); //this way is OK,too: lpResDirEntry->NameOffset <==> (lpResDirEntry->Name & (~0x80000000))
      wsprintf(strNodeText, "%s", lpResName->NameString);
    }
    else                            //bit[31] = 0, Name is Id;
    {
      if(0 == wDepth)
      {
        strResType = this->Type2Name(lpResDirEntry->Id);
        wsprintf(strNodeText, "%s", strResType);
      }
      else
      {
        wsprintf(strNodeText, "%d", lpResDirEntry->Id);
      }
    }

    hCurNode = this->m_oTvResTbl.InsertItem(TEXT(strNodeText), hParent, TVI_LAST); //create a sub node
    if(hCurNode)
    {
      this->m_oTvResTbl.SetItemData(hCurNode, (DWORD)NODE_PARAM_VALUE);
    }
    
    //OffsetToData
    //if(lpResDirEntry->OffsetToData & 0x80000000) //bit[31] = 1, bit[30:0] is a pointer that point to the address of the next level directory(IMAGE_RESOURCE_DIRECTORY);
    if(lpResDirEntry->DataIsDirectory)             //this way is OK,too: lpResDirEntry->DataIsDirectory <==> (lpResDirEntry->OffsetToData & 0x80000000)
    {
      //lpNextResDir = (PIMAGE_RESOURCE_DIRECTORY)(this->m_strResTbl + (lpResDirEntry->OffsetToData & (~0x80000000)));
      lpResSubDir = (PIMAGE_RESOURCE_DIRECTORY)(this->m_strResTbl + lpResDirEntry->OffsetToDirectory); //this way is OK,too: lpResDirEntry->OffsetToDirectory <==> (lpResDirEntry->OffsetToData & (~0x80000000))
      this->ParseResourceDirectory(lpResSubDir, hCurNode, wDepth + 1); //�ݹ����,������һ��Ŀ¼;
    }
    else                                  //bit[31] = 0, bit[30:0] is a pointer that point to IMAGE_RESOURCE_DATA_ENTRY
    {
      lpResDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)(this->m_strResTbl + lpResDirEntry->OffsetToData);
      ZeroMemory(strNodeText, sizeof(strNodeText));
      wsprintf(strNodeText, "%X - %X", lpResDataEntry->OffsetToData, lpResDataEntry->Size);
      hLeaf = this->m_oTvResTbl.InsertItem(TEXT(strNodeText), hCurNode, TVI_LAST); //Leaf Node

      //��Դ���ݿ����ʼ��ַ���Ա����ڵ�ǰҶ�ӽڵ�hLeaf�л�ǰҶ�ӽڵ�hLeaf�ĸ��ڵ�hCurNode����;��ʵ���Ǳ����ڵ�ǰҶ�ӽڵ�hLeaf��;
      if(hLeaf)
      {
        this->m_oTvResTbl.SetItemData(hLeaf, (DWORD)lpResDataEntry);
      }
      //break; //exit the current function
    }
  }
  return TRUE;
}

BOOL CTabResTbl::Fill(LPVOID lpParam/* = NULL*/)
{
  LPBYTE strFileBase = NULL;
  PFORMAT_PE lpFmtPe = NULL;
  LPBYTE strResTbl = NULL;
  PIMAGE_RESOURCE_DIRECTORY lpResRootDir = NULL;
  TCHAR strMsg[TEMP_BUFF_SIZE];
  
  this->m_oTvResTbl.DeleteAllItems();
  
  strFileBase = g_lpPeFile->GetBase();
  if(NULL == strFileBase)
  {
    MessageBox("��ȡ��Դ��ʱ,�ļ���ַ��Ч", "������Ϣ", MB_OK | MB_ICONERROR);
    return FALSE;
  }
  
  lpFmtPe = (PFORMAT_PE)(g_lpPeFile->GetPeFormat());
  if(NULL == lpFmtPe)
  {
    MessageBox("��ȡ��Դ��ʱ,PE�ṹ��Ч", "������Ϣ", MB_OK | MB_ICONERROR);
    return FALSE;
  }
  
  if((NULL == lpFmtPe->lpResTbl) || (NULL == lpFmtPe->lpResBlk))
  {
    MessageBox("��ȡ����Դ����Ч", "������Ϣ", MB_OK | MB_ICONERROR);
    return FALSE;
  }
  
  if(0 == lpFmtPe->lpResBlk->Size)
  {
    //insert one item;
    ZeroMemory(strMsg, sizeof(strMsg));
    wsprintf(strMsg, "%s", NO_PE_DATA);
    HTREEITEM hRoot = this->m_oTvResTbl.InsertItem((LPCTSTR)strMsg, NULL, NULL);
    if(hRoot)
    {
      this->m_oTvResTbl.SetItemData(hRoot, (DWORD)NODE_PARAM_EMPTY);
    }
    return FALSE;
  }
  
  lpResRootDir = lpFmtPe->lpResTbl;
  this->m_strResTbl = (LPBYTE)lpResRootDir;

  this->ParseResourceDirectory(lpResRootDir, TVI_ROOT, 0);
  return TRUE;
}

BOOL CTabResTbl::ReadResourceData(PIMAGE_RESOURCE_DATA_ENTRY lpResDataEntry)
{
  LPBYTE strFileBase = NULL, strResDataBase = NULL;
  DWORD dwNumberOfLines = 0, dwTotalBytes = 0;
  LPBYTE pos = NULL, posHead = NULL, posTail = NULL;
  DWORD dwFoa = 0, i = 0, j = 0;
  int iLen = 0, k = -1;
  LPTSTR strData = NULL;

  if(NULL == lpResDataEntry)
  {
    MessageBox("��ȡ��Դ����ʱ,���ݲ�����Ч", "������Ϣ", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  strFileBase = g_lpPeFile->GetBase();
  if(NULL == strFileBase)
  {
    MessageBox("��ȡ��Դ����ʱ,�ļ���ַ��Ч", "������Ϣ", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  dwNumberOfLines = (lpResDataEntry->Size / 16);
  if(lpResDataEntry->Size % 16)
  {
    dwNumberOfLines += 1; //������;
  }

  //���ֽ���;
  dwTotalBytes = (BYTES_OF_PER_LINE * dwNumberOfLines) + EXTRA_BYTES;
  strData = NULL;
  strData = new TCHAR[dwTotalBytes];
  if(NULL == strData)
  {
    MessageBox("��ȡ��Դ����ʱ,�����ڴ�ʧ��", "������Ϣ", MB_OK | MB_ICONERROR);
    return FALSE;
  }

  dwFoa = g_lpPeFile->Rva2Foa(lpResDataEntry->OffsetToData);
  strResDataBase = strFileBase + dwFoa;
  posHead = strResDataBase;
  posTail = posHead + lpResDataEntry->Size;

  ZeroMemory(strData, dwTotalBytes);

  //format the header;
  //i = _snprintf(strData, dwTotalBytes, "%-10s", "FOA");
  i = wsprintf(strData, "%-10s", "FOA");
  for(j = 0; j < 16; j++)
  {
    //iLen = _snprintf(strData + i, dwTotalBytes - i, ((j == 0xF) ? "%02X\r\n" : "%02X "), j);
    iLen = wsprintf(strData + i, ((j == 0xF) ? "%02X\r\n" : "%02X "), j);
    i += iLen;
  }

  //iLen = _snprintf(strData + i, dwTotalBytes - i, "%s", "----------------------------------------------------------\r\n");
  iLen = wsprintf(strData + i, "%s", "----------------------------------------------------------\r\n");
  i += iLen;

  //format the resource data;
  j = 0;
  for(pos = posHead; pos != posTail; pos++)
  {
    //format one line;
    //FOA:
    if(0 == j) //new line
    {
      k++;
      //iLen = _snprintf(strData + i, dwTotalBytes - i, "%08X: ", dwFoa + 16*k);
      iLen = wsprintf(strData + i, "%08X: ", dwFoa + 16*k);
      i += iLen;
    }

    //byte:
    //iLen = _snprintf(strData + i, dwTotalBytes - i, ((j == 0xF) ? "%02X\r\n" : "%02X "), (*pos));
    iLen = wsprintf(strData + i, ((j == 0xF) ? "%02X\r\n" : "%02X "), (*pos));
    i += iLen;
    
    j++;
    if(16 == j)
    {
      j = 0;
    }
  }
  
  if(' ' == strData[i - 1])
  {
    strData[i - 1] = 0;
  }
  
  SetDlgItemText(IDC_EDT_RES_TBL_DATA, strData);

  if(strData)
  {
    delete[] strData;
  }
  return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTabResTbl message handlers

BOOL CTabResTbl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->InitTreeView();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabResTbl::OnFillForm(WPARAM wParam, LPARAM lParam)
{
  this->Fill();
}

void CTabResTbl::OnSelchangedTreeResourceTable(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = reinterpret_cast<NM_TREEVIEW*>(pNMHDR);
	// TODO: Add your control notification handler code here
  HTREEITEM hLeaf = NULL;
  BOOL bHasSubNode = FALSE;
  DWORD dwData = 0;
  PIMAGE_RESOURCE_DATA_ENTRY lpResDataEntry = NULL;
  
  //hLeaf = this->m_oTvResTbl.GetSelectedItem();
  hLeaf = pNMTreeView->itemNew.hItem;
  if(NULL == hLeaf)
  {
    return;
  }

  //�����ѡ��Ľڵ�û���ӽڵ�(��,�ýڵ���Ҷ�ӽڵ�),���ȡҶ�ӽڵ��ϱ������Դ���ݵ���ʼ��ַ;
  bHasSubNode = this->m_oTvResTbl.ItemHasChildren(hLeaf);
  if(TRUE == bHasSubNode)
  {
    return; //ѡ��Ĳ���Ҷ�ӽڵ�;
  }
  
  dwData = this->m_oTvResTbl.GetItemData(hLeaf);
  if((NODE_PARAM_VALUE == dwData) || (NODE_PARAM_EMPTY == dwData))
  {
    return; //Ҷ�ӽڵ��ϱ������Դ������ʼ��ַ��Ч;
  }

  lpResDataEntry = NULL;
  lpResDataEntry = (PIMAGE_RESOURCE_DATA_ENTRY)dwData;
  this->ReadResourceData(lpResDataEntry);
  
	*pResult = 0;
}
