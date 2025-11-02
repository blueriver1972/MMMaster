#pragma once


// CGetList

class CGetList : public CButton
{
	DECLARE_DYNAMIC(CGetList)

public:
	CGetList();
	virtual ~CGetList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);

public:
	int T1825(CString sh_cd, CString gubun);
	int T1826(int index);
	int T8430(void);
	int T0167(void);
	int T1302(CString shcode, int gubun, int cnt);
	int T1602(CString upcode, int cnt);
	int T1305(CString shcode, int gubun, int cnt);
	int T1102(CString cs);
	int T1702(CString shcode);
	int T1442(int index);
	int T1444(int index);
	int T1301(CString cscode, CString csvol, CString csCT, CString csST, CString csET);
	int T1636(int gubun, int ctsidx);
	int T1637(CString cscode, CString cubun1,  CString cubun2);
	int getCurInfo(CString shcode);
	int T1532(CString shcode);


	CMySQLCMD ctrl_db;

};


