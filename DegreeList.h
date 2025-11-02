#pragma once
#include "afxcmn.h"


// CDegreeList 대화 상자입니다.

class CDegreeList : public CDialog
{
	DECLARE_DYNAMIC(CDegreeList)

public:
	CDegreeList(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDegreeList();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DEGREE_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ctrlDegreeList;
	int CallDegrView(int mode);
	afx_msg void OnBnClickedOk();
};
