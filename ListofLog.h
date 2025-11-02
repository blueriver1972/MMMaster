#pragma once
#include "afxcmn.h"
#include "MySQLCMD.h"


// CListofLog 대화 상자입니다.

class CListofLog : public CDialog
{
	DECLARE_DYNAMIC(CListofLog)

public:
	CListofLog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CListofLog();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CListCtrl m_ctrlChart;
	int CallChView(int mode);
		
	CMySQLCMD ctrlDB;
	int m_gMode;

	CString CommaFormat(CString val);
	CString m_shcode;
	CString m_hname;
	afx_msg void OnBnClickedOk();
};
