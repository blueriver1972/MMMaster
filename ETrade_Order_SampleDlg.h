// ETrade_Order_SampleDlg.h : header file
//

#if !defined(AFX_ETRADE_ORDER_SAMPLEDLG_H__2BBA0CFB_402B_4932_989B_2E1A0C652542__INCLUDED_)
#define AFX_ETRADE_ORDER_SAMPLEDLG_H__2BBA0CFB_402B_4932_989B_2E1A0C652542__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MySQLCMD.h"
#include "S_Login.h"
#include "S_Order_Debt.h"
#include "GetList.h"
#include "S_Order_Debt.h"
#include "ConfigManager.h"
#include "afxwin.h"
#include "afxcmn.h"


/////////////////////////////////////////////////////////////////////////////
// CETrade_Order_SampleDlg dialog

class CETrade_Order_SampleDlg : public CDialog
{
// Construction
public:
	CETrade_Order_SampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CETrade_Order_SampleDlg)

	enum { IDD = IDD_ETRADE_ORDER_SAMPLE_DIALOG1 };

	CS_Order_Debt	m_ctrl_CS_Order;
	CS_Login	m_ctrl_CS_Login;
	CGetList m_ctrlGL;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CETrade_Order_SampleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CETrade_Order_SampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();


	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStop();
protected:
	afx_msg	LRESULT		OnXMReceiveData		( WPARAM wParam, LPARAM lParam );
	afx_msg	LRESULT		OnXMTimeoutData		( WPARAM wParam, LPARAM lParam );
public:

	CMySQLCMD ctrlDB;
	CS_Order_Debt ctrlOrder;
	CConfigManager m_ConfigManager;  // 설정 파일 관리자
	afx_msg void OnBnStart();


	CString  m_csCount;
	CString  m_csCntPw;
	CString  m_csID;
	CString  m_csIdPw;
	CString  m_csPubVerify;
	CString  m_csShortCode;
	CString CalPrice(CString csshcode, CString price, double per);
	CString Cal(CString price, double per);
	CString CommaFormat(CString val);
	void CallTRBuy(int mode);
	void CallSell_org(int mode);
	int AddLog(CString csmsg, int mode);

	HBRUSH m_brsBase;

	afx_msg void OnBnClickedButtonConnect();

	CListCtrl m_ctrlCount;
	CListBox m_ctrlLog;
	afx_msg void OnBnClickedButtonMd1();
	afx_msg void OnBnClickedButtonMd2();
	afx_msg void OnBnClickedButtonMs1();
	afx_msg void OnBnClickedButtonMs2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_ctrlMSMD;
	afx_msg void OnBnClickedButtonCn1();
	afx_msg void OnBnClickedButtonCn2();
	afx_msg void OnBnClickedButtonMi1();
	afx_msg void OnBnClickedButtonMi2();
	afx_msg void OnNMDblclkList5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();

	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	
	afx_msg void OnBnClickedButton10();
	CString m_csSTtime;
	CString m_csOPtime;
	afx_msg void OnNMRClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_csJMInfo;


	afx_msg void OnNMClickList5(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList5(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedButtonEditSave();

	void CallXML(void);
	void CallDayXML(void);
	CListBox m_ctrlLog1;
	CListBox m_ctrlLog2;
	afx_msg void OnBnClickedCheckAuto();
	CString m_csPrice;
	afx_msg void OnBnClickedButton11();
	void CallSell(int mode);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ETRADE_ORDER_SAMPLEDLG_H__2BBA0CFB_402B_4932_989B_2E1A0C652542__INCLUDED_)
