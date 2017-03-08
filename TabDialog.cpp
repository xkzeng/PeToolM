// TabDialog.cpp: implementation of the TabDialog class.
//

#include "stdafx.h"
#include "TabDialog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
TabDialog::TabDialog(VOID)
{
}

TabDialog::~TabDialog(VOID)
{
}

TabDialog::TabDialog(UINT nIDTemplate, CWnd* pParentWnd/* = NULL*/):CDialog(nIDTemplate, pParentWnd)
{
}

BOOL TabDialog::Fill(LPVOID lpParam/* = NULL*/)
{
  return TRUE;
}

VOID TabDialog::InitListView(HWND hList, struct SListHead lstLvHdrs[])
{
  LONG  lStyle = 0;
  DWORD dwStyle = 0;
  LVCOLUMN stColumn;
  WORD i = 0;
  
  //set Style
  lStyle = GetWindowLong(hList, GWL_STYLE);
  lStyle &= ~LVS_TYPEMASK;
  lStyle |= LVS_REPORT;
  SetWindowLong(hList, GWL_STYLE, lStyle);
  
  //set Extended Style
  dwStyle = ::SendMessage(hList, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
  dwStyle |= LVS_EX_FULLROWSELECT;
  dwStyle |= LVS_EX_GRIDLINES;
  ::SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
  
  //insert the table header
  ZeroMemory(&stColumn, sizeof(LVCOLUMN));
  stColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
  stColumn.iSubItem = 0;
  
  for(i = 0; lstLvHdrs[i].strTitle && lstLvHdrs[i].iWidth; i++)
  {
    //column i:
    stColumn.fmt     = lstLvHdrs[i].iAlign;
    stColumn.cx      = lstLvHdrs[i].iWidth;
    stColumn.pszText = lstLvHdrs[i].strTitle;
    ::SendMessage(hList, LVM_INSERTCOLUMN, (WPARAM)i, (LPARAM)&stColumn);
  }
}

VOID TabDialog::InitListView(HWND hList, struct SListHead lstLvHdrs[], CONST WORD wCount)
{
  LONG  lStyle = 0;
  DWORD dwStyle = 0;
  struct SListHead* lpLstHdr = NULL;
  LVCOLUMN stColumn;
  WORD i = 0;
  
  //set Style
  lStyle = GetWindowLong(hList, GWL_STYLE);
  lStyle &= ~LVS_TYPEMASK;
  lStyle |= LVS_REPORT;
  SetWindowLong(hList, GWL_STYLE, lStyle);
  
  //set Extended Style
  dwStyle = ::SendMessage(hList, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, 0);
  dwStyle |= LVS_EX_FULLROWSELECT;
  dwStyle |= LVS_EX_GRIDLINES;
  ::SendMessage(hList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);
  
  //insert the table header
  ZeroMemory(&stColumn, sizeof(LVCOLUMN));
  stColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_FMT;
  stColumn.iSubItem = 0;
  
  for(i = 0; i < wCount; i++)
  {
    //column i:
    lpLstHdr = lstLvHdrs + i;
    stColumn.fmt     = lpLstHdr->iAlign;
    stColumn.cx      = lpLstHdr->iWidth;
    stColumn.pszText = lpLstHdr->strTitle;
    ::SendMessage(hList, LVM_INSERTCOLUMN, (WPARAM)i, (LPARAM)&stColumn);
  }
}
