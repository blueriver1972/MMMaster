#pragma once

#include "MySQLCMD.h"
#include "afxwin.h"


// CanalyPatten 대화 상자입니다.

class CanalyPatten : public CDialog
{
	DECLARE_DYNAMIC(CanalyPatten)

public:
	CanalyPatten(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CanalyPatten();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int CallView(int mode);
	CListCtrl m_ctrlMS;
	CListCtrl m_ctrlMD;

	CMySQLCMD ctrlDB;

	CString m_hname;
	CString m_shcode;
	CString m_minval;
	CString m_maxval;
	CString m_anyperiod;
	afx_msg void OnLvnColumnclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnColumnclickList3(NMHDR *pNMHDR, LRESULT *pResult);

	int m_nSortColumn;
	bool m_bSortAscending;

	CBrush m_brush;

	int m_Indexof;
	afx_msg void OnNMCustomdrawList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawList3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CString CommaFormat(CString val);
	CStatic m_ctrlMSdata;
	CStatic m_ctrlMDdata;
	CStatic m_ctrlSMSdata;
	int m_nSMSval;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int UpdateAnaly(void);
};
