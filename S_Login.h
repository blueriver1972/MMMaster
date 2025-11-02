#if !defined(AFX_S_LOGIN_H__80D14894_0365_4B24_9676_A4762AAC0629__INCLUDED_)
#define AFX_S_LOGIN_H__80D14894_0365_4B24_9676_A4762AAC0629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// S_Login.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CS_Login window

class CS_Login : public CButton
{
// Construction
public:
	CS_Login();

	bool ConnectServer();
	bool Login(CString csid, CString cspw, CString cscerti);


// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CS_Login)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CS_Login();

	// Generated message map functions
protected:
	//{{AFX_MSG(CS_Login)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnLogin	( WPARAM wParam, LPARAM lParam );

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_S_LOGIN_H__80D14894_0365_4B24_9676_A4762AAC0629__INCLUDED_)
