// TabDialog.h: interface for the TabDialog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TABDIALOG_H__B61B61AE_0A44_47D8_B2C9_7623B99B65E7__INCLUDED_)
#define AFX_TABDIALOG_H__B61B61AE_0A44_47D8_B2C9_7623B99B65E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct SListHead
{
  LPTSTR strTitle;
  INT32  iWidth;
  INT32  iAlign;
};

class TabDialog : public CDialog
{
public:
	TabDialog(VOID);
  TabDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~TabDialog(VOID);
  
  VOID InitListView(HWND hList, struct SListHead lstLvHdrs[]);
  VOID InitListView(HWND hList, struct SListHead lstLvHdrs[], CONST WORD wCount);

protected:
  BOOL Fill(LPVOID lpParam = NULL);
};

#endif // !defined(AFX_TABDIALOG_H__B61B61AE_0A44_47D8_B2C9_7623B99B65E7__INCLUDED_)
