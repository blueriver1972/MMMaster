#pragma once


#define MAX_BUFF	1024*100





// CMySQLCMD
extern 	CString  m_cssearchgb;
extern  CString  m_cssearchcd;
class CMySQLCMD : public CWnd
{
	DECLARE_DYNAMIC(CMySQLCMD)

public:
	CMySQLCMD();
	virtual ~CMySQLCMD();

protected:
	DECLARE_MESSAGE_MAP()
public:
	int db_init(CString csAddr, CString csDBName, CString id, CString pw, CString csport);
	int db_close(void);
	int db_query_cmd(CString csquery);

	int msglogdb(CString cscode, CString msg);

	int db_update_1637(LPt1637OutBlock1 p);
	int db_update_1302(LPt1302OutBlock1 p, CString cshcode, CString cshname);
	int db_update_1305(LPt1305OutBlock1 p, CString cshcode, CString cshname);

	int checkweektbl(CString shcode, int nprice);
	int updateDate(CString csindex);
	int checkDate(CString csdate);

	CString getquery(CString cs);

	CString searchingjm(CString csprice, CString csrate);
	int checkmsdate(CString csshcode, int count, CString &csdate, int passdays);
	int getinfomsg(CString csshcode, CString &csinfo);
	int db_getinfo(CString csmode);
	int getavginfo(CString shcode, CString csdatetime, int days, CString &csavgprice, CString &csvol5m, CString &csavgdiff);

	CString getJisuDate(CString csdate, CString csmode);
	CString getJisu(CString csmode);
	CString getMMinfo(CString csmode);
	CString getJisuList(CString csmode);
	CString getMMListinfo(CString csmode);
	int jangoUpdate(CString cscode, CString cshanme, CString csjanqty, CString cspamt, CString csprice, CString csrate);
	int dailyjangoUpdate(CString tot, CString mstot, CString curprofit, CString frofit);

	int UpdateAVGInfo(void);
	int mgAVGjminfo(CString csshcode, CString cshname, CString csavg5price, CString csavg20price,  CString csavg60price, CString csavg5vol, CString csavg20vol,  CString csavg60vol, int mode);

	CString getTMavg(CString shcode);

	CString checkUPcode(CString cscode);
	long int getValuefromT1702(CString shcode);
	long int getValuetypefromT1702(CString shcode, CString cstype);
	CString getJonggb(CString shcode);
	CString getmarkettype(CString cs, int mode);
	CString getMatchID(CString cs, int mode);
	CString getchlevel(CString cscode, CString &csdiff);


	int updatecurinfo(CString shcode);
	int check8407info(CString shcode, CString cstime);
	int getcurprice(CString csshcode, CString &csprice, CString &csdiff);

	int checktablelist(CString csshcode);
	CString chPMstatus(CString shcode, CString &csvalue);
	int checkorgstatus(CString shcode, int mode);
	int checkstartpoint1702(CString shcode, int mode);
	int getNewMSInfo(CString csmode);
	int getMaxSUKEUB(CString csmode);
	int getEnbelob(int mode, int countmode);

	float getmgAVGjminfoRange(CString csshcode, CString cstype, int range, CString &csavg);
	int getmgAVGjminfoOne(CString csshcode, CString cshname, CString &csavg5price, CString &csavg20price,  CString &csavg60price, CString &csavg5vol, CString &csavg20vol,  CString &csavg60vol, int mode);
	int FindJMNew(void);
	int FindAVGJMNew(void);
	int addNewJMinfo(CString csshcode, CString cshname, CString csprice, CString csinfo);
	int addEnvelopjminfo(CString csshcode, CString cshname, CString csprice, CString avg20price, int mode, int countmode);
	int updateNewMSInfo(CString csshcode, CString cshname, CString csprice, int mode);
	int updateMaxSUKEUB(CString csshcode, CString cshname, CString csprice, int mode);
	int updateEnvelopjminfo(CString csshcode, CString cshname, CString csprice, CString csmsmdprice, int mode, int countmode);
	int get_org_forg_T1702(CString shcode, CString  &csorg, CString  &csforg, CString  &cstotal, int days);

	int checkavginfo(CString shcode, int days, int nvol);

	CString CommaFormat(CString val);
	int compreTime(CString sctime , CString tgtime , int gaptime);
	int check_jango_date(CString shcode);
	int checkvolume(CString shcode, int multitime, int days);
	int getListMaxvolume(int multtime, int days); //최근에 거래량이 급증한 종목 list 
	
	int db_initBack(CString csAddr, CString csDBName, CString id, CString pw, CString csport);
	int dbback_close(void);

	int displaylog(CString m_csEvent, CString m_csSubcode,CString m_csRate, CString csdate);
	int orgdisplaylog(CString csmode, CString csordermode);
	int Logcurinfo(CString gcscode);
	int analycur(CString gcscode);
	int dbback_check(void);
};


