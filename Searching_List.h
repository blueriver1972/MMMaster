#pragma once
#include "afxcmn.h"


// CSearching_List 대화 상자입니다.



class CSearching_List : public CDialog
{
	DECLARE_DYNAMIC(CSearching_List)

public:
	CSearching_List(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSearching_List();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctrlList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_price;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int CallChView(int mode);
	CString CommaFormat(CString val);

};
