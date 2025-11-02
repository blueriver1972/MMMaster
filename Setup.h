

#pragma once
#include "MySQLCMD.h"

// CSetup 대화 상자입니다.

class CSetup : public CDialog
{
	DECLARE_DYNAMIC(CSetup)

public:
	CSetup(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetup();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CMySQLCMD ctrlDB;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CString m_csCount;
	CString m_csCntPw;
	CString m_csID;
	CString m_csIdPw;
	CString m_csPubVerify;
	CString m_csShortCode;
	CString m_csMoney;
	CString m_csSetp;
	virtual BOOL OnInitDialog();

	afx_msg void OnEnChangeEditJmDiff();
};
