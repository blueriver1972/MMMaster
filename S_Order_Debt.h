#if !defined(AFX_S_ORDER_DEBT_H__AEC6C1C4_EEAC_4800_B605_DC3DA0DF8EDB__INCLUDED_)
#define AFX_S_ORDER_DEBT_H__AEC6C1C4_EEAC_4800_B605_DC3DA0DF8EDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// S_Order_Debt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CS_Order_Debt window


class CS_Order_Debt : public CButton
{
// Construction
public:

	// 윈도우 핸들로 메시지 뿌림
	bool StockBuy(CString strcode,CString strOrdQty, CString strOrdPrc, CString mode, CString msmode);
	bool StockSecondBuy(CString strcode,CString strOrdQty, CString strOrdPrc, CString mode, CString msmode);
	
	bool StockSell(CString strCode, CString strOrdQty, CString strOrdPrc, CString mode, CString mdmode, CString csLoanDt);
	bool StockModify(CString strOrgordNo, CString strCode, CString strOrdQty, CString strOrdPrc, CString mode);
	bool StockCancel(CString strOrgordNo, CString strCode, CString strOrdQty);
	bool StockCheck(CString strAcntNo, CString strPwd, CString strtemp);
	bool OrderCheck(CString strAcntNo, CString strPwd, CString strmode);

	bool CheckVal(void);
	bool CheckOrder(CString csnextvla);
	bool CheckCount(void);
	bool CSPAT00600(
		CString strAcntNo
		,CString strInptPwd      
		,CString strIsuNo        
		,CString strOrdQty       
		,CString strOrdPrc       
		,CString strBnsTpCode    
		,CString strOrdprcPtnCode
		,CString strMgntrnCode   
		,CString strLoanDt       
		,CString strOrdCndiTpCode
		);

	bool CSPAT00700(CString strOrgordNo, CString strAcntNo, CString strInptPwd ,CString strIsuNo ,CString strOrdQty ,CString strOrdPrc       
		,CString strOrdprcPtnCode , CString strOrdCndiTpCode);
	bool CSPAT00800(CString strOrgordNo, CString strAcntNo, CString strInptPwd ,CString strIsuNo ,CString strOrdQty);
	bool T0424(	CString strAcntNo, CString strInptPwd, CString strexpcode, CString strchegb, CString strmedosu, CString strsortgb, CString strcts ); 
	bool T0425(	CString strAcntNo, CString strInptPwd, CString strexpcode, CString strchegb, CString strmedosu, CString strsortgb, CString strcts ); 
	bool CSPAQ12300(CString strAcntNo, CString strInptPwd ,CString strtemp);
	bool CSPAQ13700(CString strAcntNo, CString strInptPwd ,CString strmode);
	bool CSPAQ12200(CString strAcntNo, CString strInptPwd);


	CS_Order_Debt();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CS_Order_Debt)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CS_Order_Debt();

	// Generated message map functions
protected:
	//{{AFX_MSG(CS_Order_Debt)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	//afx_msg	LRESULT		OnXMReceiveData		( WPARAM wParam, LPARAM lParam );
	//afx_msg	LRESULT		OnXMTimeoutData		( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_S_ORDER_DEBT_H__AEC6C1C4_EEAC_4800_B605_DC3DA0DF8EDB__INCLUDED_)
