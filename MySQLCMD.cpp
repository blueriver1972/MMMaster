// MySQLCMD.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ETrade_Order_Sample.h"
//#include "ETrade_Order_SampleDlg.h"

#include "MySQLCMD.h"
//#include <mysql/mysql.h>
#include <mysql.h>

#include <mysqld_error.h>


#pragma comment(lib, "libmysql.lib")

MYSQL		*myData;
MYSQL_RES	*myRes, *myRes1;
MYSQL_ROW	myRow, myRow1;

MYSQL		*myDataBack;
MYSQL_RES	*myResBack;
MYSQL_ROW	myRowBack;

CString gcsTableName;

CString  m_cssearchgb;
CString  m_cssearchcd;



struct LogInfo{
	int lognum;								//read number.
	int prenum;
	QData logitem[2000];				//������� �����ϱ� ���� ����ü �迭
};

QData LogCurInfo;

extern MAJMINF mgMa;
extern ALLJISUINFO mgJiSu;
extern T130LIST t1301list;

extern SearchList DBList;
extern MAList    maDBJM;

extern CString gcsStartTime, gcsOpenTime;
extern CString gcsEndTime, gcsCloseTime;

extern T1511LIST   t1511list;
extern T1511INFO   t1511info;
extern T1602LIST   t1602list;
extern T1602INFO   t1602info;

extern ETF_T1602INFO etf_t1602info;
extern MGJMINFO		 etf_mgjminfo;
extern T1637LIST t1637info;

extern CString csJisuKOPI, csJisuKSDQ;
extern CString gscCount, gscCountpw, gcsID, gcsPw, gcsVPw, gcsTotval, gcsMsrate, gcsMdlevel, gcsSunik, gcsMAXSunik, gcsSonjel, gcsMAXSonjel;
extern CString gcsLogIndex;
extern CString gcsStartTime, gcsEndTime;

extern int  gMIN_BAR;
extern int gSetAll, gSetIndex;
extern int glastmode;

JMAVGINFOList jmavginfo5List, jmavginfo20List, jmavginfo60List;

OUTList OUTData;

SearchList IVAnal;

LogInfo loglist;

extern T1511LIST listitme;

#define MAX_QLIST_NUM 8
// CMySQLCMD

IMPLEMENT_DYNAMIC(CMySQLCMD, CWnd)

CString csMsg;
CMySQLCMD::CMySQLCMD()
{

}

CMySQLCMD::~CMySQLCMD()
{
}


BEGIN_MESSAGE_MAP(CMySQLCMD, CWnd)
END_MESSAGE_MAP()


// CMySQLCMD �޽��� ó�����Դϴ�.

int CMySQLCMD::db_init(CString csAddr, CString csDBName, CString csID, CString csPW, CString csport)
{
	CString csquery, csMsg;
	int rc = 0;

	myData = mysql_init((MYSQL *)0);
	if(!myData){
		OutputDebugString("failed to init MYSQL!.");
		return 1;
	}

	if(mysql_real_connect(myData, csAddr, csID, csPW, NULL, atoi(csport), NULL, 0) == NULL){
		csMsg.Format("MySQL ���� ����: %s\n", mysql_error(myData));
		OutputDebugString(csMsg);
        mysql_close(myData);
        exit(1);
		//myData->reconnect = 0;
		return 3;
	}
	else{
		myData->reconnect= 1;

		csquery.Format("CREATE DATABASE %s", csDBName.GetBuffer(csDBName.GetLength()));
		mysql_query(myData, csquery);
		mysql_commit(myData);

		if(mysql_select_db(myData, csDBName.GetBuffer(csDBName.GetLength()) ) < 0){	
			csMsg.Format(_T("Can't select cfg database [%s] !.\n"), csDBName);
			OutputDebugString(csMsg);
			mysql_close(myData);
			myData->reconnect = 0;
			return 2;
		}
		csquery.Format("CREATE TABLE date_jango_tbl(codemode char(4), shcode char(6), hname char(40), cur_date char(30), janqty char(12),  pamt char(12),  price char(12), rate char(6),  primary key(codemode, shcode, cur_date) )  ENGINE = MyISAM");
		mysql_query(myData, csquery);
		mysql_commit(myData);
		csquery.Format("CREATE TABLE daily_jango_%s_tbl(cur_date char(30), total_assets char(20),  purchase_amount char(20),  valuation_gain char(20), realized_profit char(20),  rate char(6), rev char(10), primary key(cur_date) ) ENGINE = MyISAM",  gcsLogIndex);
		rc =mysql_query(myData, csquery);


		csquery.Format("CREATE TABLE envelop_jm_tbl(shcode char(6), hname char(40), reg_date char(10), cur_datetime char(20),  price char(12), 20avgprice char(12), ms_datetime char(20),  msprice char(12), md_datetime char(20),  mdprice char(12), rate char(6), mode char(6),  countmode char(6), primary key(shcode, cur_datetime, price, mode, countmode) ) ENGINE = MyISAM");
		rc = mysql_query(myData, csquery);
		mysql_commit(myData);

		csquery.Format("CREATE TABLE Order_JM_tbl(shcode char(6), hname char(40), reg_date char(10), cur_datetime char(20),  price char(12), msprice char(12), mdprice char(12), info char(20), primary key(shcode, reg_date, info) ) ENGINE = MyISAM");
		rc = mysql_query(myData, csquery);
		mysql_commit(myData);

		csquery.Format("CREATE TABLE logmsg_tbl(shcode char(6), cur_date char(30),  msg varchar(300), primary key(shcode, cur_date) )");
		rc = mysql_query(myData, csquery);
		if(rc){
			csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
			OutputDebugString(csMsg);
			mysql_commit(myData);
		}

		csquery.Format("CREATE TABLE avg_5_20_60_jm_tbl(shcode char(6), hname char(40), cur_date char(20), avg5price char(12), avg20price char(12), avg60price char(12), avg5vol char(12), avg20vol char(12), avg60vol char(12), revinfo char(20), primary key(shcode) ) ENGINE = MyISAM");
		rc = mysql_query(myData, csquery);
		mysql_commit(myData);

		csquery.Format("CREATE TABLE avg_5_20_60_jmlist_tbl(shcode char(6), hname char(40), cur_date char(20),  avg5price char(12), avg20price char(12), avg60price char(12), avg5vol char(12), avg20vol char(12), avg60vol char(12), revinfo char(20), primary key(shcode, cur_date) ) ENGINE = MyISAM");
		rc = mysql_query(myData, csquery);
		mysql_commit(myData);

		csquery.Format("CREATE TABLE t1637_tbl(shcode char(6), cur_date char(20),  time char(12), price char(12), sign char(4), changevalue char(12), diff char(12), svalue char(18), svolume char(18), primary key(shcode, cur_date, time) ) ENGINE = MyISAM");
		rc = mysql_query(myData, csquery);
		//if(rc){
		//	csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		//	OutputDebugString(csMsg);
		//	mysql_commit(myData);
		//}

		mysql_commit(myData);
		csquery.Format("CREATE TABLE cfg_tbl(idindex char(4) , countid char(30), countpw VARBINARY(64), loginid char(30) , loginpw VARBINARY(64), verifypw VARBINARY(64) , msvalue char(12), msrate char(6), mdlevel char(6), sunik char(6), maxsunik char(6), sonjel char(6), maxsonjel char(6),  allcount char(6), primary key(idindex) ) ENGINE = MyISAM");
		mysql_query(myData, csquery);
		mysql_commit(myData);


//ü�� �˻� list
		csquery.Format("CREATE TABLE dbList_tbl(shcode char(6) primary key , cur_date datetime, hname char(40) , mode1 char(5), mode2 char(5), price char(8))");
		mysql_query(myData, csquery);

		//if(rc){
		//	csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		//	OutputDebugString(csMsg);
		//	mysql_commit(myData);
		//}

	}
	
	return 0;
}

int CMySQLCMD::db_close(void)
{

	mysql_close(myData);
	myData->reconnect = 0;
	return 0;
}



// CMySQLCMD �޽��� ó�����Դϴ�.
int CMySQLCMD::db_initBack(CString csAddr, CString csDBName, CString csID, CString csPW, CString csport)
{
	CString csquery;
	int rc = 0;

	myDataBack = mysql_init((MYSQL *)0);
	if(!myDataBack){
		OutputDebugString("failed to init MYSQL!.");
		return 1;
	}

	if(csAddr.Compare("stockserver.iptime.org") == 0)
		csport = "3309";

	if(mysql_real_connect(myDataBack, csAddr, csID, csPW, NULL, atoi(csport), NULL, 0)){
		myDataBack->reconnect= 1;

		csquery.Format("CREATE DATABASE %s", csDBName.GetBuffer(csDBName.GetLength()));
		mysql_query(myDataBack, csquery);

		if(mysql_select_db(myDataBack, csDBName.GetBuffer(csDBName.GetLength()) ) < 0){	
			csMsg.Format(_T("Can't select cfg database [%s] !.\n"), csDBName);
			OutputDebugString(csMsg);
			mysql_close(myDataBack);
			myDataBack->reconnect = 0;
			return 2;
		}
		
		csquery.Format("CREATE TABLE analyList_tbl(shcode char(6), hname char(30), cur_date char(30), curprice char(12), diff char(6), sms char(12), smsval char(12), sinc char(9), fsms char(12), fsmsval char(12), totvol char(15),  totval char(12), rate char(5), twentyflag char(4), newprice char(4), psms char(12), primary key(shcode)  )");
		mysql_query(myDataBack, csquery);

	}
	else{
		mysql_close(myDataBack);
		myDataBack->reconnect = 0;
		return 3;
	}
	return 0;
}

int CMySQLCMD::dbback_close(void)
{

	mysql_close(myDataBack);
	myDataBack->reconnect = 0;
	return 0;
}

int CMySQLCMD::dbback_check(void)
{
	return myDataBack->reconnect;
}

int CMySQLCMD::orgdisplaylog(CString csmode, CString csordermode)
{
	int i = 0, n = 0, index = 0, rc;
	CString cs, cscurdate, csQuery;
	unsigned long *lengths, len = 0;
	char temp[300];
	COleDateTime t;
	COleDateTimeSpan ts;

	if(myDataBack->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();
	cscurdate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	if(myDataBack->reconnect != 1)
		return 0;


	csQuery.Format("select shcode, hname, cur_date, msgcode, subcode, eventprice, curprice, sms, fsms, rate, mstype, msg from  ordlog_tbl where msgcode = '%s' and logdate = '%s' order by cur_date desc limit 2", csmode, cscurdate);

	mysql_set_character_set(myDataBack, "euckr");


	rc = mysql_query(myDataBack, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myDataBack));
		OutputDebugString(csMsg);
		return 0;
	}

	myResBack = mysql_store_result(myDataBack);
	if(myResBack == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myDataBack));
		OutputDebugString(csMsg);
		return 0;
	}


	mgMa.mginfo.jmcnt = 0;
	index = 0;
	while(myRowBack = mysql_fetch_row(myResBack)){

		lengths = mysql_fetch_lengths(myResBack);
		i = 0;

		mgMa.mginfo.jminfo[index].level1 = csordermode;

		//shcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		cs.Format("%s", temp);
		loglist.logitem[index].shcode = cs;

		cs.Format("%s", temp);
		mgMa.mginfo.jminfo[index].shcode = cs;
		t1637info.shcodelist[index] = cs;

		i++;
		//hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		cs.Format("%s", temp);
		loglist.logitem[index].hname = cs;
		mgMa.mginfo.jminfo[index].hname = cs;

		i++;
		//cur_date
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		cs.Format("%s", temp);
		loglist.logitem[index].cur_date = cs;


		i++;
		//msgcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		cs.Format("%s", temp);
		loglist.logitem[index].sign = cs;

		i++;
		//subcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		cs.Format("%s", temp);
		loglist.logitem[index].signcnt = cs;

		i++;
		//event price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		cs.Format("%s", temp);
		loglist.logitem[index].curprice = cs;
		mgMa.mginfo.jminfo[index].price = cs;

		i++;
		//cur price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		cs.Format("%s", temp);
		loglist.logitem[index].volume = cs;

		i++;
		//sms
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		cs.Format("%s", temp);
		loglist.logitem[index].sms = cs;

		i++;
		//fsms
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		cs.Format("%s", temp);
		loglist.logitem[index].fsms = cs;

		i++;
		//rate
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		cs.Format("%s", temp);
		loglist.logitem[index].rate = cs;

		i++;
		//msg
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		cs.Format("%s", temp);
		loglist.logitem[index].mstype = cs;

		i++;
		//msg
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		cs.Format("%s", temp);
		loglist.logitem[index].msg = cs;


		index++;
		if(index >= 100)
			break;
	}
	loglist.lognum = index;
	t1637info.count = mgMa.mginfo.jmcnt = index;
	mysql_free_result(myResBack);
	mysql_commit(myDataBack);

	return i;
}

int CMySQLCMD::displaylog(CString csmode, CString cssub, CString csrate, CString csdate)
{
	int i = 0, index = 0, rc;
	CString cs, csposdate, csQuery;
	unsigned long *lengths, len = 0;
	char temp[300];
	COleDateTime t;
	COleDateTimeSpan ts;

	if(myDataBack->reconnect != 1)
		return 0;


	t = COleDateTime::GetCurrentTime();
	cs.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());


	if(csmode.Compare("0") == 0){

		if((cssub.Compare("9") == 0)){
			csQuery.Format("select shcode, hname, cur_date, msgcode, subcode, eventprice, curprice, sms, fsms, rate, mstype, msg from  ordlog_tbl where logdate >= '%s' order by cur_date desc", cs);
		}
		else if((cssub.Compare("0") == 0)){
			csQuery.Format("select shcode, hname, cur_date, msgcode, subcode, eventprice, curprice, sms, fsms, rate, mstype, msg from  ordlog_tbl where subcode = '%s' and logdate >= '%s' order by cur_date desc", cssub, cs);
		}
		else{
			if(csrate.Compare("0") == 0){
							csQuery.Format("select shcode, hname, cur_date, msgcode, subcode, eventprice, curprice, sms, fsms, rate, mstype, msg from  ordlog_tbl where subcode = '%s' and logdate >= '%s' order by cur_date desc", cssub, cs);
			}
			else{
				csQuery.Format("select shcode, hname, cur_date, msgcode, subcode, eventprice, curprice, sms, fsms, rate, mstype, msg from  ordlog_tbl where subcode = '%s' and rate >= %s and logdate >= '%s' order by cur_date desc", cssub, csrate, cs);
			}
		}
	}
	else if(csmode.Compare("3333") == 0){
		t = COleDateTime::GetCurrentTime();
		cs.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
		csQuery.Format("select shcode, hname, cur_date, msgcode, subcode, eventprice, curprice, sms, fsms, rate, mstype, msg from  ordlog_tbl where msgcode = '%s' and logdate >= '%s' order by cur_date desc", csmode, cs);
	}
	else if(csmode.Compare("1972") == 0){
		t = COleDateTime::GetCurrentTime();
		cs.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
		csQuery.Format("SELECT  distinct  shcode, hname, reg_date, cur_datetime, price FROM etrade_db.envelop_jm_tbl where reg_date = '%s' order by cur_datetime desc", csdate);
	}
	else{
		if((cssub.Compare("9") == 0) ){
			csQuery.Format("select shcode, hname, cur_date, msgcode, subcode, eventprice, curprice, sms, fsms, rate, mstype, msg from  ordlog_tbl where msgcode = '%s' and logdate >= '%s' order by cur_date desc", csmode, cs);

		}
		else if((cssub.Compare("0") == 0)){
			csQuery.Format("select shcode, hname, cur_date, msgcode, subcode, eventprice, curprice, sms, fsms, rate, mstype, msg from  ordlog_tbl where msgcode = '%s' and subcode = '%s' and logdate >= '%s' order by cur_date desc", csmode,  cssub, cs);
		}
		else{
			if(csrate.Compare("0") == 0){
				csQuery.Format("select shcode, hname, cur_date, msgcode, subcode, eventprice, curprice, sms, fsms, rate, mstype, msg from  ordlog_tbl where msgcode = '%s' and subcode = '%s' and logdate >= '%s' order by cur_date desc", csmode, cssub, cs);
			}
			else{
				csQuery.Format("select shcode, hname, cur_date, msgcode, subcode, eventprice, curprice, sms, fsms, rate, mstype, msg from  ordlog_tbl where msgcode = '%s' and subcode = '%s' and rate >= %s and logdate >= '%s' order by cur_date desc", csmode, cssub, csrate, cs);
			}
		}
	}


	if(csmode.Compare("1972") == 0){
		if(myDataBack->reconnect != 1)
			return 0;

		mysql_set_character_set(myDataBack, "euckr");

		rc = mysql_query(myDataBack, csQuery);
		if(rc){

			csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myDataBack));
			OutputDebugString(csMsg);
			return 0;
		}

		myResBack = mysql_store_result(myDataBack);
		if(myResBack == NULL){
			csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myDataBack));
			OutputDebugString(csMsg);
			return 0;
		}
		rc = 0;

		while(myRowBack = mysql_fetch_row(myResBack)){
			lengths = mysql_fetch_lengths(myResBack);

			i = 0;
			//shcode
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRowBack[i], lengths[i]);
			cs.Format("%s", temp);
			loglist.logitem[index].shcode = cs;

			i++;
			//hname
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRowBack[i], lengths[i]);
			cs.Format("%s", temp);
			loglist.logitem[index].hname = cs;


			i++;
			//reg_date
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRowBack[i], lengths[i]);
			cs.Format("%s", temp);
			loglist.logitem[index].signcnt = cs;

			i++;
			//cur_datetime
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRowBack[i], lengths[i]);
			cs.Format("%s", temp);
			loglist.logitem[index].cur_date = cs;



			index++;
			if(index >= 20)
				break;
		}
		mysql_free_result(myResBack);
		mysql_commit(myDataBack);
		loglist.lognum = index;

	}
	else{


		mysql_set_character_set(myDataBack, "euckr");

		rc = mysql_query(myDataBack, csQuery);
		if(rc){

			csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myDataBack));
			OutputDebugString(csMsg);
			return 0;
		}

		myResBack = mysql_store_result(myDataBack);
		if(myResBack == NULL){
			csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myDataBack));
			OutputDebugString(csMsg);
			return 0;
		}


		while(myRowBack = mysql_fetch_row(myResBack)){

			lengths = mysql_fetch_lengths(myResBack);
			i = 0;

			//shcode
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRowBack[i], lengths[i]);
			cs.Format("%s", temp);
			loglist.logitem[index].shcode = cs;

			i++;
			//hname
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRowBack[i], lengths[i]);
			cs.Format("%s", temp);
			loglist.logitem[index].hname = cs;

			i++;
			//cur_date
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRowBack[i], lengths[i]);
			cs.Format("%s", temp);
			loglist.logitem[index].cur_date = cs;

			i++;
			//msgcode
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRowBack[i], lengths[i]);
			cs.Format("%s", temp);
			loglist.logitem[index].sign = cs;



			index++;
			if(index >= 2000)
				break;
		}
		loglist.lognum = index;
		mysql_free_result(myResBack);
		mysql_commit(myDataBack);
	}
	return i;
}

int CMySQLCMD::analycur(CString gcscode)
{
	int i = 0, j = 0, rc;
	CString cs, cssign, csprice, csval, csQuery, csMsg, csCurDate, cstime;
	unsigned long *lengths, len = 0;
	char temp[100];
	__int64 msval = 0, mdval = 0, val = 0;

	if(myDataBack->reconnect != 1)
		return 0;


	csQuery.Format("select shcode, sms, fsms, totvol, rate from  analyList_tbl where shcode = '%s'", gcscode);

	mysql_set_character_set(myDataBack, "euckr");

	rc = mysql_query(myDataBack, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myDataBack));
		OutputDebugString(csMsg);
		return 0;
	}

	myResBack = mysql_store_result(myDataBack);
	if(myResBack == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myDataBack));
		OutputDebugString(csMsg);
		return 0;
	}

	while(myRowBack = mysql_fetch_row(myResBack)){
		lengths = mysql_fetch_lengths(myResBack);

		i = 0;

		//shcode
		i++;

		//���
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		LogCurInfo.sms = temp;
		i++;

		//�ܸż�
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		LogCurInfo.fsms = temp;
		i++;

		//�Ѹż�
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		LogCurInfo.volume = temp;
		i++;

		//����
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRowBack[i], lengths[i]);
		LogCurInfo.rate = temp;
		i++;

	}

	mysql_free_result(myResBack);
	mysql_commit(myDataBack);
	return i;
}



int CMySQLCMD::db_query_cmd(CString csquery)
{

	int rc;
	CString csMsg;

	if(myData->reconnect != 1)
		return 0;

	rc = mysql_query(myData, csquery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	return 1;
}

int CMySQLCMD::msglogdb(CString cscode, CString msg)
{

	int i = 0, rc;
	CString csQuery, csMsg;
	CString csCurDate, logdate, cs;
	COleDateTime t;
	COleDateTimeSpan ts;

	if(myData->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();

	csCurDate.Format(_T("%d%02d%02d %02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	csQuery.Format("INSERT INTO logmsg_tbl(shcode, cur_date, msg)  VALUES ('%s', '%s', '%s')",  cscode, csCurDate, msg);

	mysql_set_character_set(myData, "euckr");
	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);		
		return 0;
	}
	mysql_commit(myData);

	return 1;

}



CString CMySQLCMD::getquery(CString csquery)
{
	int i = 0, rc, index = 0;
	CString cs, csCurDate, csQuery, csMsg;
	unsigned long *lengths, len = 0;
	char temp[10];

	if(myData->reconnect != 1)
		return cs;


	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csquery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}
	
	i = 0;
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		if(lengths[0]){
			i = 0;
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			cs = temp;
		}
	}
	mysql_free_result(myRes);
	mysql_commit(myData);
	return cs;
}


extern int passList[];


int CMySQLCMD::checkDate(CString csdate)
{
	int i = 0, rc, index = 0;
	CString cs, csDate, csQuery, csMsg;
	unsigned long *lengths, len = 0;
	char temp[20];
	COleDateTime t;

	if(myData->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();
	csDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay()); 

	csQuery.Format("select passday from  passday_tbl  where cur_date = '%s'", csdate);
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);

	if(rc){
		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		if(lengths[0]){
			i = 0;
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
		}
	}
	mysql_free_result(myRes);
	mysql_commit(myData);
	return atoi(temp);

}
int CMySQLCMD::checkavginfo(CString shcode, int days, int nvol)
{

	int i = 0, rev  = 1, rc;
	CString cs, csQuery, csMsg;
	unsigned long *lengths, len = 0;


	COleDateTime t;
	COleDateTimeSpan ts;	

	if(myData->reconnect != 1)
		return 0;

	csQuery.Format("select *, volume from (select * from etrade_db.jm_day_tbl where (shcode) in (SELECT shcode FROM   etrade_db.jm_day_tbl where shcode='%s' ) order by cur_datetime desc limit %d)t where t.volume > %d",  shcode, days, nvol);
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;
		rev = 0;

	}
	mysql_free_result(myRes);
	mysql_commit(myData);

	return rev;

}

int CMySQLCMD::checkvolume(CString shcode, int multitime, int days) //�ֱٿ� �ŷ����� ���� ū��
{

	int i = 0, rev  = 0, rc;
	CString cs, csQuery, csMsg;
	unsigned long *lengths, len = 0;

	COleDateTime t;
	COleDateTimeSpan ts;	

	if(myData->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();
	csQuery.Format("SELECT * FROM etrade_db.jminfo_tbl t1 WHERE t1.shcode = '%s' AND t1.volume IN ( SELECT CAST(volume AS SIGNED) FROM (SELECT * FROM etrade_db.jminfo_tbl \
        WHERE shcode = '%s' AND cur_date < CURDATE() ORDER BY cur_date DESC LIMIT %d ) t2 GROUP BY t2.shcode, t2.volume   HAVING MAX(CAST(volume AS SIGNED) *%d) > ( SELECT CAST(volume AS SIGNED) \
        FROM etrade_db.jminfo_tbl t3 WHERE t3.shcode = '%s' AND t3.cur_date = CURDATE() ) ) AND t1.cur_date < (  SELECT MAX(cur_date) FROM etrade_db.jminfo_tbl \
		WHERE shcode = '%s' AND cur_date < CURDATE() ) ORDER BY t1.cur_date DESC", shcode, shcode, days, multitime, shcode, shcode);
	
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}


	int ncount = 0;
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		ncount++;

	}
	mysql_free_result(myRes);
	mysql_commit(myData);

	return ncount;

}


int CMySQLCMD::getListMaxvolume(int multtime, int days) //�ֱٿ� �ŷ����� ������ ���� list  
{

	int i = 0, rev  = 0, rc;
	CString cs, csQuery, csMsg;
	unsigned long *lengths, len = 0;
	char temp[100];

	COleDateTime t;
	COleDateTimeSpan ts;	

	if(myData->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();

	csQuery.Format("SELECT t1.* FROM etrade_db.jminfo_tbl t1 WHERE t1.cur_date = CURDATE()  AND t1.volume > %d * ( SELECT MAX(CAST(t2.volume AS SIGNED)) \
					FROM etrade_db.jminfo_tbl t2 WHERE t2.shcode = t1.shcode AND t2.cur_date < CURDATE() ORDER BY t2.cur_date DESC LIMIT %d )", multtime, days);
	
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}


	int ncount = 0;
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		if(lengths[0]){
			i = 0;
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
		}
		ncount++;

	}
	mysql_free_result(myRes);
	mysql_commit(myData);

	return ncount;

}



int CMySQLCMD::updateDate(CString csindex)
{
	int i = 0, rc, index = 0;
	CString cs, csDate, csQuery, csMsg;
	//unsigned long *lengths, len = 0;
	//char temp[20];
	COleDateTime t;

	if(myData->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();
	csDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay()); 


	csQuery.Format("update passday_tbl set cur_date = '%s' where passday = '%s'",  csDate, csindex);

	mysql_set_character_set(myData, "euckr");
	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);		
		return 0;
	}
	mysql_commit(myData);
	return 0;

}


int CMySQLCMD::mgAVGjminfo(CString csshcode, CString cshname, CString csavg5price, CString csavg20price,  CString csavg60price, CString csavg5vol, CString csavg20vol,  CString csavg60vol, int mode)
{

	int i = 0, rc;
	CString csCurDate, csQuery, csMsg;

	COleDateTime t = COleDateTime::GetCurrentTime();

	if(myData->reconnect != 1)
		return 0;

	csCurDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	if(mode == 0){
		csQuery.Format("replace INTO etrade_db.avg_5_20_60_jm_tbl(shcode, hname, cur_date, avg5price, avg20price, avg60price, avg5vol, avg20vol, avg60vol) VALUES ('%s','%s', '%s','%s', '%s', '%s','%s','%s', '%s')", 					
			csshcode, cshname,  csCurDate, csavg5price, csavg20price, csavg60price,  csavg5vol, csavg20vol, csavg60vol);

	}
	else if(mode == 1){
		csQuery.Format("update etrade_db.avg_5_20_60_jm_tbl set avg20price = '%s',avg20vol = '%s' where shcode = '%s' and cur_date = '%s'",  csavg20price,  csavg20vol, csshcode, csCurDate);
	}
	else{
		csQuery.Format("update etrade_db.avg_5_20_60_jm_tbl set avg60price = '%s',avg60vol = '%s' where shcode = '%s' and cur_date = '%s'",  csavg60price,  csavg60vol, csshcode, csCurDate);
	}

	mysql_set_character_set(myData, "euckr");
	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		//return 0;
	}
	mysql_commit(myData);


//make list for avg

	if(mode == 0){
		csQuery.Format("replace INTO etrade_db.avg_5_20_60_jmlist_tbl(shcode, hname, cur_date, avg5price, avg20price, avg60price, avg5vol, avg20vol, avg60vol) VALUES ('%s','%s', '%s','%s', '%s', '%s','%s','%s', '%s')", 					
			csshcode, cshname,  csCurDate, csavg5price, csavg20price, csavg60price,  csavg5vol, csavg20vol, csavg60vol);

	}
	else if(mode == 1){
		csQuery.Format("update etrade_db.avg_5_20_60_jmlist_tbl set avg20price = '%s',avg20vol = '%s' where shcode = '%s' and cur_date = '%s'",  csavg20price,  csavg20vol, csshcode, csCurDate);
	}
	else{
		csQuery.Format("update etrade_db.avg_5_20_60_jmlist_tbl set avg60price = '%s',avg60vol = '%s' where shcode = '%s' and cur_date = '%s'",  csavg60price,  csavg60vol, csshcode, csCurDate);
	}

	mysql_set_character_set(myData, "euckr");
	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	mysql_commit(myData);

	return 1;

}

int CMySQLCMD::get_org_forg_T1702(CString shcode, CString  &csorg, CString  &csforg, CString  &cstotal, int days)
{

	int i = 0, rc, index = 0;
	CString csQuery, csMsg;
	CString csorgtmp, csforgtmp, cstottmp, csorgtmp1, csforgtmp1, cstottmp1;

	unsigned long *lengths, len = 0;
	char temp[32];
	float val = 0, sumval = 0;

	if(myData->reconnect != 1)
		return 0;


//0
	csQuery.Format("SELECT AVG(org), AVG(forg),  AVG(org) +AVG(forg) FROM ( SELECT org, forg FROM etrade_db.1702list_tbl WHERE shcode = '%s' ORDER BY cur_date DESC  LIMIT 1 ) AS subquery", shcode, days);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	val = 0;
	
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csorg.Format("%s", temp);

		i++;
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csforg.Format("%s", temp);

		i++;
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cstottmp.Format("%s", temp);

		if( atof(csorgtmp) > 0 ){
			val =   (float)atof(csorg)/100;		
			index = 3;
		}
		if( atof(csforgtmp) > 0 ){
			val +=   (float)atof(csforg)/100;	
			index += 3;
		}
		
	}

	mysql_free_result(myRes);
	mysql_commit(myData);

//1
	csQuery.Format("SELECT AVG(org), AVG(forg),  AVG(org) +AVG(forg) FROM ( SELECT org, forg FROM etrade_db.1702list_tbl WHERE shcode = '%s' ORDER BY cur_date DESC  LIMIT %d ) AS subquery", shcode, days);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csorgtmp.Format("%s", temp);

		i++;
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csforgtmp.Format("%s", temp);

		i++;
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cstottmp.Format("%s", temp);

		if( atof(csorgtmp) > 0 ){
			val +=   (float)atof(csorgtmp)/100;	
			index += 1;

		}
		else if(atof(csorgtmp) == 0){
			index += 0;
		}
		else{
			index += 2;
		}

		if( atof(csforgtmp) > 0 ){
			val +=   (float)atof(csforgtmp)/100;
			index += 1;
		}
		else if(atof(csforgtmp) == 0){
			index += 0;
		}
		else{
			index += 2;
		}	
	}

	mysql_free_result(myRes);
	mysql_commit(myData);
//2
	csQuery.Format("SELECT AVG(org), AVG(forg),  AVG(org) +AVG(forg) FROM ( SELECT org, forg FROM etrade_db.1702list_tbl WHERE shcode = '%s' ORDER BY cur_date DESC  LIMIT %d ) AS subquery", shcode, days *2);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}


	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csorgtmp1.Format("%s", temp);

		i++;
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csforgtmp1.Format("%s", temp);

		i++;
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cstottmp1.Format("%s", temp);

		if( atof(csorgtmp1) > 0 ){
			val +=   (float)atof(csorgtmp1)/100;	
			index += 1;
		}
		else if(atof(csorgtmp1) == 0){
			index += 0;
		}
		else{
			index += 2;
		}

		if( atof(csforgtmp1) > 0 ){
			val +=   (float)atof(csforgtmp1)/100;	
			index += 1;
		}
		else if(atof(csforgtmp1) == 0){
			index += 0;
		}
		else{
			index += 2;
		}
	}

	mysql_free_result(myRes);
	mysql_commit(myData);
	return index;

}


int CMySQLCMD::getavginfo(CString shcode, CString csdatetime, int days, CString &csavgprice, CString &csvol5m, CString &csavgdiff)
{

	int i = 0, rev  = 0, rc;
	CString cs, csQuery, csMsg;
	unsigned long *lengths, len = 0;
	char temp[100];

	COleDateTime t;
	COleDateTimeSpan ts;	

	if(myData->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();

	csQuery.Format("select avg(price),  avg(vol5m), avg(diff), count(vol5m) as cnt from  etrade_db.jm_5m_vollist_tbl where  shcode ='%s' and cur_datetime < '%s' order by cur_datetime limit %d", shcode, csdatetime, days);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}


	int ncount = 0;
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		//avg price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csavgprice.Format("%s", temp);
		i++;

		//avg vol5m
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csvol5m.Format("%s", temp);
		i++;

		//avg diff
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csavgdiff.Format("%s", temp);
		i++;

		//count
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		ncount = atoi(temp);
		i++;


	}
	mysql_free_result(myRes);
	mysql_commit(myData);

	return ncount;

}

int CMySQLCMD::UpdateAVGInfo(void)
{
	int i = 0,  j = 0, index = 0, rc;
	CString cs, csshcode, csQuery;
	unsigned long *lengths, len = 0;
	char temp[60];
	CString csMsg;

	if(myData->reconnect != 1)
		return 0;

	csQuery.Format("delete from etrade_db.avg_5_20_60_jm_tbl");
	db_query_cmd(csQuery);

	csQuery.Format("select T1.shcode, T2.hname, avg(t1.price), avg(t1.volume) from etrade_db.jminfo_tbl as t2, ( SELECT etrade_db.jminfo_tbl.shcode, etrade_db.jminfo_tbl.cur_date, etrade_db.jminfo_tbl.price, etrade_db.jminfo_tbl.volume \
		FROM etrade_db.jminfo_tbl LEFT JOIN etrade_db.jminfo_tbl AS lesser ON etrade_db.jminfo_tbl.shcode = lesser.shcode AND etrade_db.jminfo_tbl.cur_date < lesser.cur_date \
		GROUP BY etrade_db.jminfo_tbl.shcode, etrade_db.jminfo_tbl.cur_date HAVING COUNT(lesser.cur_date) < 5 ORDER BY  etrade_db.jminfo_tbl.cur_date DESC) as T1 where T1.shcode = T2.shcode  group by T1.shcode order by t1.shcode");


	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	index = 0;	
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		//shcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		jmavginfo5List.jminfo[index].shcode = cs;
		i++;

		//hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		jmavginfo5List.jminfo[index].hname = cs;
		i++;

		//price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		jmavginfo5List.jminfo[index].avginfo.price.Format("%d", atoi(cs));
		i++;

		//volume
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		jmavginfo5List.jminfo[index].avginfo.volume.Format("%d", atoi(cs));
		i++;

		index++;
		if(index >= 3000)
			break;
	}
	jmavginfo5List.listnum = index;
	mysql_free_result(myRes);
	mysql_commit(myData);

	for(i = 0 ; i < jmavginfo5List.listnum; i++){
		mgAVGjminfo(jmavginfo5List.jminfo[i].shcode, jmavginfo5List.jminfo[i].hname, jmavginfo5List.jminfo[i].avginfo.price, "",  "", jmavginfo5List.jminfo[i].avginfo.volume, "",  "", 0);
	}


	csQuery.Format("select T1.shcode, T2.hname, avg(t1.price), avg(t1.volume) from etrade_db.jminfo_tbl as t2, ( SELECT etrade_db.jminfo_tbl.shcode, etrade_db.jminfo_tbl.cur_date, etrade_db.jminfo_tbl.price, etrade_db.jminfo_tbl.volume \
		FROM etrade_db.jminfo_tbl LEFT JOIN etrade_db.jminfo_tbl AS lesser ON etrade_db.jminfo_tbl.shcode = lesser.shcode AND etrade_db.jminfo_tbl.cur_date < lesser.cur_date \
		GROUP BY etrade_db.jminfo_tbl.shcode, etrade_db.jminfo_tbl.cur_date HAVING COUNT(lesser.cur_date) < 20 ORDER BY  etrade_db.jminfo_tbl.cur_date DESC) as T1 where T1.shcode = T2.shcode  group by T1.shcode order by t1.shcode");


	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	index = 0;
	
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		//shcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csshcode.Format("%s", temp);
		jmavginfo20List.jminfo[index].shcode = csshcode;
		i++;

		//hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		jmavginfo20List.jminfo[index].hname = cs;
		i++;

		//price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		jmavginfo20List.jminfo[index].avginfo.price.Format("%d", atoi(cs));
		i++;

		//volume
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		jmavginfo20List.jminfo[index].avginfo.volume.Format("%d", atoi(cs));
		i++;

		if(index++ >= 3000)
			break;
	}
	jmavginfo20List.listnum = index;
	mysql_free_result(myRes);
	mysql_commit(myData);

	for(i = 0 ; i < jmavginfo20List.listnum; i++){
		mgAVGjminfo(jmavginfo20List.jminfo[i].shcode, jmavginfo20List.jminfo[i].hname, "",  jmavginfo20List.jminfo[i].avginfo.price, "", "",  jmavginfo20List.jminfo[i].avginfo.volume, "", 1);
	}
	
	csQuery.Format("select T1.shcode, T2.hname, avg(t1.price), avg(t1.volume) from etrade_db.jminfo_tbl as t2, ( SELECT etrade_db.jminfo_tbl.shcode, etrade_db.jminfo_tbl.cur_date, etrade_db.jminfo_tbl.price, etrade_db.jminfo_tbl.volume \
		FROM etrade_db.jminfo_tbl LEFT JOIN etrade_db.jminfo_tbl AS lesser ON etrade_db.jminfo_tbl.shcode = lesser.shcode AND etrade_db.jminfo_tbl.cur_date < lesser.cur_date \
		GROUP BY etrade_db.jminfo_tbl.shcode, etrade_db.jminfo_tbl.cur_date HAVING COUNT(lesser.cur_date) < 60 ORDER BY  etrade_db.jminfo_tbl.cur_date DESC) as T1 where T1.shcode = T2.shcode  group by T1.shcode order by t1.shcode");


	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	index = 0;
	
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		//shcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csshcode.Format("%s", temp);
		jmavginfo60List.jminfo[index].shcode = csshcode;
		i++;

		//hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		jmavginfo60List.jminfo[index].hname = cs;
		i++;

		//price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		jmavginfo60List.jminfo[index].avginfo.price.Format("%d", atoi(cs));
		i++;

		//volume
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		jmavginfo60List.jminfo[index].avginfo.volume.Format("%d", atoi(cs));
		i++;

		if(index++ >= 3000)
			break;
	}
	jmavginfo60List.listnum = index;
	mysql_free_result(myRes);
	mysql_commit(myData);

	for(i = 0 ; i < jmavginfo60List.listnum; i++){
		mgAVGjminfo(jmavginfo60List.jminfo[i].shcode, jmavginfo60List.jminfo[i].hname, "", "",  jmavginfo60List.jminfo[i].avginfo.price, "", "",  jmavginfo60List.jminfo[i].avginfo.volume, 2);
	}

	return index;
}





int CMySQLCMD::FindAVGJMNew(void)
{
	int i = 0, j = 0, n = 0, index = 0, mindex = 0, rc;
	CString cs, csmsg, csQuery, csDate, cstime, csshcode, cshname, cstotal, cs5vol, cs20vol, cs5volrate, cs20volrate, cs5price, cs20price, cs5prirate, cs20prirate;
	unsigned long *lengths, len = 0;
	char temp[60];

	CString csMsg, cstemp[MAX_JONGMOK], cschdeg;
	COleDateTime t;
	COleDateTimeSpan ts;

	if(myData->reconnect != 1)
		return 0;

	//UpdateAVGInfo();

	t = COleDateTime::GetCurrentTime();
	csDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay()); 
	cstime.Format(_T("%02d%02d"), t.GetHour(), t.GetMinute());


	mysql_set_character_set(myData, "euckr");
	tjmList.listnum = 0;
	index = 0;
 


	if(atoi(cstime) <= 910){
		csQuery.Format("select t3.shcode, t3.hname, t3.price,  t3.diff, t3.volume, t3.avg5price, t3.avg5vol, t3.avg20price, t3.avg20vol, t3.avg60price, t3.avg60vol, t3.jnilvolume, t3.openprice, t3.hiprice, t3.lowprice, t3.low52date, t3.hi52date, t3.per, t3.total, t3.salert, t3.opert, t3.netrt, t3.bfnetinfome, t3.bfnetinfome2, t3.vol \
					   from etrade_db.jminfo_tbl, (SELECT t1.shcode, t1.hname, t2.avg5price, t2.avg5vol, t2.avg20price, t2.avg20vol, t2.avg60price, t2.avg60vol, t2.cur_date, t1.diff, t1.price, t1.volume, t1.jnilvolume, t1.openprice, t1.hiprice, t1.lowprice, t1.vol,\
					   t1.low52date, t1.hi52date, t1.per, t1.total, t1.salert, t1.opert, t1.netrt, t1.bfnetinfome, t1.bfnetinfome2 FROM etrade_db.avg_5_20_60_jm_tbl as t2 inner join etrade_db.jminfo_tbl as t1 on cast(t1.volume as signed) > (cast(t2.avg5vol as signed) * 0.2) and t1.volume > 10000 and t1.vol > 130 and cast(t1.lowprice as signed) <= t2.avg20price and (t2.avg5price * 1.02 > (t2.avg20price) )  and (t2.avg5price < (t2.avg20price*1.05) ) and cast(t1.price as signed) <= (t2.avg20price * 1.05) and cast(t1.price as signed) <= (t2.avg60price * 1.1)  and  ( cast(t1.total as signed) >= 500 ) and t1.diff < 10 and t2.shcode = t1.shcode  and t1.cur_date >= t2.cur_date and t1.cur_date >= '%s' ) as t3 where etrade_db.jminfo_tbl.shcode = t3.shcode Group by t3.shcode", csDate);

	}
	else if(atoi(cstime) <= 930){
		csQuery.Format("select t3.shcode, t3.hname, t3.price,  t3.diff, t3.volume, t3.avg5price, t3.avg5vol, t3.avg20price, t3.avg20vol, t3.avg60price, t3.avg60vol, t3.jnilvolume, t3.openprice, t3.hiprice, t3.lowprice, t3.low52date, t3.hi52date, t3.per, t3.total, t3.salert, t3.opert, t3.netrt, t3.bfnetinfome, t3.bfnetinfome2, t3.vol \
					   from etrade_db.jminfo_tbl, (SELECT t1.shcode, t1.hname, t2.avg5price, t2.avg5vol, t2.avg20price, t2.avg20vol, t2.avg60price, t2.avg60vol, t2.cur_date, t1.diff, t1.price, t1.volume, t1.jnilvolume, t1.openprice, t1.hiprice, t1.lowprice, t1.vol,\
					   t1.low52date, t1.hi52date, t1.per, t1.total, t1.salert, t1.opert, t1.netrt, t1.bfnetinfome, t1.bfnetinfome2 FROM etrade_db.avg_5_20_60_jm_tbl as t2 inner join etrade_db.jminfo_tbl as t1 on cast(t1.volume as signed) > (cast(t2.avg5vol as signed) * 0.3) and t1.volume > 10000 and t1.vol > 130 and cast(t1.lowprice as signed) <= t2.avg20price and (t2.avg5price * 1.02 > (t2.avg20price) )  and (t2.avg5price < (t2.avg20price*1.05) ) and cast(t1.price as signed) <= (t2.avg20price * 1.05) and cast(t1.price as signed) <= (t2.avg60price * 1.1) and  ( cast(t1.total as signed) >= 500 ) and t1.diff < 10 and t2.shcode = t1.shcode  and t1.cur_date >= t2.cur_date and t1.cur_date >= '%s' ) as t3 where etrade_db.jminfo_tbl.shcode = t3.shcode Group by t3.shcode", csDate);



	}
	else if(atoi(cstime) <= 1000){
		csQuery.Format("select t3.shcode, t3.hname, t3.price,  t3.diff, t3.volume, t3.avg5price, t3.avg5vol, t3.avg20price, t3.avg20vol, t3.avg60price, t3.avg60vol, t3.jnilvolume, t3.openprice, t3.hiprice, t3.lowprice, t3.low52date, t3.hi52date, t3.per, t3.total, t3.salert, t3.opert, t3.netrt, t3.bfnetinfome, t3.bfnetinfome2, t3.vol \
					   from etrade_db.jminfo_tbl, (SELECT t1.shcode, t1.hname, t2.avg5price, t2.avg5vol, t2.avg20price, t2.avg20vol, t2.avg60price, t2.avg60vol, t2.cur_date, t1.diff, t1.price, t1.volume, t1.jnilvolume, t1.openprice, t1.hiprice, t1.lowprice, t1.vol,\
					   t1.low52date, t1.hi52date, t1.per, t1.total, t1.salert, t1.opert, t1.netrt, t1.bfnetinfome, t1.bfnetinfome2 FROM etrade_db.avg_5_20_60_jm_tbl as t2 inner join etrade_db.jminfo_tbl as t1 on cast(t1.volume as signed) > (cast(t2.avg5vol as signed) * 0.7) and t1.volume > 10000 and t1.vol > 130 and cast(t1.lowprice as signed) <= t2.avg20price and (t2.avg5price * 1.02 > (t2.avg20price) )  and (t2.avg5price < (t2.avg20price*1.05) ) and cast(t1.price as signed) <= (t2.avg20price * 1.05) and cast(t1.price as signed) <= (t2.avg60price * 1.1) and  ( cast(t1.total as signed) >= 500 ) and t1.diff < 10 and t2.shcode = t1.shcode  and t1.cur_date >= t2.cur_date and t1.cur_date >= '%s' ) as t3 where etrade_db.jminfo_tbl.shcode = t3.shcode Group by t3.shcode", csDate);



	}
	else if(atoi(cstime) <= 1100){
		csQuery.Format("select t3.shcode, t3.hname, t3.price,  t3.diff, t3.volume, t3.avg5price, t3.avg5vol, t3.avg20price, t3.avg20vol, t3.avg60price, t3.avg60vol, t3.jnilvolume, t3.openprice, t3.hiprice, t3.lowprice, t3.low52date, t3.hi52date, t3.per, t3.total, t3.salert, t3.opert, t3.netrt, t3.bfnetinfome, t3.bfnetinfome2, t3.vol \
					   from etrade_db.jminfo_tbl, (SELECT t1.shcode, t1.hname, t2.avg5price, t2.avg5vol, t2.avg20price, t2.avg20vol, t2.avg60price, t2.avg60vol, t2.cur_date, t1.diff, t1.price, t1.volume, t1.jnilvolume, t1.openprice, t1.hiprice, t1.lowprice, t1.vol,\
					   t1.low52date, t1.hi52date, t1.per, t1.total, t1.salert, t1.opert, t1.netrt, t1.bfnetinfome, t1.bfnetinfome2 FROM etrade_db.avg_5_20_60_jm_tbl as t2 inner join etrade_db.jminfo_tbl as t1 on cast(t1.volume as signed) > (cast(t2.avg5vol as signed) * 1.2) and t1.volume > 10000 and t1.vol > 120 and cast(t1.lowprice as signed) <= t2.avg20price and (t2.avg5price * 1.02 > (t2.avg20price) )  and (t2.avg5price < (t2.avg20price*1.05) ) and cast(t1.price as signed) <= (t2.avg20price * 1.05) and cast(t1.price as signed) <= (t2.avg60price * 1.1) and  ( cast(t1.total as signed) >= 500 ) and t1.diff < 10 and t2.shcode = t1.shcode  and t1.cur_date >= t2.cur_date and t1.cur_date >= '%s' ) as t3 where etrade_db.jminfo_tbl.shcode = t3.shcode Group by t3.shcode", csDate);



	}
	else{
		csQuery.Format("select t3.shcode, t3.hname, t3.price,  t3.diff, t3.volume, t3.avg5price, t3.avg5vol, t3.avg20price, t3.avg20vol, t3.avg60price, t3.avg60vol, t3.jnilvolume, t3.openprice, t3.hiprice, t3.lowprice, t3.low52date, t3.hi52date, t3.per, t3.total, t3.salert, t3.opert, t3.netrt, t3.bfnetinfome, t3.bfnetinfome2, t3.vol \
					   from etrade_db.jminfo_tbl, (SELECT t1.shcode, t1.hname, t2.avg5price, t2.avg5vol, t2.avg20price, t2.avg20vol, t2.avg60price, t2.avg60vol, t2.cur_date, t1.diff, t1.price, t1.volume, t1.jnilvolume, t1.openprice, t1.hiprice, t1.lowprice, t1.vol,\
					   t1.low52date, t1.hi52date, t1.per, t1.total, t1.salert, t1.opert, t1.netrt, t1.bfnetinfome, t1.bfnetinfome2 FROM etrade_db.avg_5_20_60_jm_tbl as t2 inner join etrade_db.jminfo_tbl as t1 on cast(t1.volume as signed) > (cast(t2.avg5vol as signed) * 1.5) and t1.volume > 10000 and t1.vol > 110 and cast(t1.lowprice as signed) <= t2.avg20price and (t2.avg5price * 1.02 > (t2.avg20price) )  and (t2.avg5price < (t2.avg20price*1.05) ) and cast(t1.price as signed) <= (t2.avg20price * 1.05) and cast(t1.price as signed) <= (t2.avg60price * 1.1) and  ( cast(t1.total as signed) >= 500 ) and t1.diff < 10 and t2.shcode = t1.shcode  and t1.cur_date >= t2.cur_date and t1.cur_date >= '%s' ) as t3 where etrade_db.jminfo_tbl.shcode = t3.shcode Group by t3.shcode", csDate);


	}
	mysql_set_character_set(myData, "euckr");


	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}


	index = 0;

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		csmsg.Empty();
		//shcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].shcode = cs;
		i++;
		csmsg = cs;

		//hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].hname = cs;
		i++;
		csmsg += cs;

		//price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].price = cs;
		i++;
		csmsg += cs;

		//diff
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].diff = cs;
		i++;
		csmsg += cs;

		//volume
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].volume = cs;
		i++;
		csmsg += cs;

		//5price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].avg5price = cs;
		i++;
		csmsg += cs;
		//5volume
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].avg5vol = cs;
		i++;
		csmsg += cs;

		//20price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].avg20price = cs;
		i++;
		csmsg += cs;

		//20volume
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].avg20vol = cs;
		i++;
		csmsg += cs;
		//60price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].avg60price = cs;
		i++;
		csmsg += cs;

		//60volume
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].avg60vol = cs;
		i++;
		csmsg += cs;

		//jvolume
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].jvolume = cs;
		i++;
		csmsg += cs;

		//openprice
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].openprice = cs;
		i++;
		csmsg += cs;

		//hiprice
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].hiprice = cs;
		i++;
		csmsg += cs;

		//lowprice
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].lowprice = cs;
		i++;
		csmsg += cs;

		//low52date
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].date52low = cs;
		i++;
		csmsg += cs;

		//hi52date
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].date52hi = cs;
		i++;
		csmsg += cs;

		//per
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].per = cs;
		i++;
		csmsg += cs;

		//total
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		cstotal = cs;
		tjmList.jminfo[index].totval = cs;
		i++;
		csmsg += cs;

		//salert
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].salert = cs;
		i++;
		csmsg += cs;

		//opert
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].opert = cs;
		i++;
		csmsg += cs;

		//netrt
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].netrt = cs;
		i++;
		csmsg += cs;

		//preval
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].preval = cs;
		i++;
		csmsg += cs;

		//ppreval
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].ppreval = cs;
		i++;
		csmsg += cs;

		//chdegree
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].chdegree = cs;
		i++;
		csmsg += cs;

		csmsg += "\n";
		//OutputDebugString(csmsg);

		index++;
		if(index >= 3000)
			break;
	}

	mysql_free_result(myRes);
	mysql_commit(myData);

	tjmList.listnum = index;

	for(i = 0; i < tjmList.listnum; i++){
		addNewJMinfo(tjmList.jminfo[i].shcode, tjmList.jminfo[i].hname, tjmList.jminfo[i].price, "MONTH");
	}
	return index;
}

int CMySQLCMD::FindJMNew(void)
{
	int i = 0, j = 0, n = 0, index = 0, mindex = 0, rc;
	CString cs, csmsg, csQuery, csDate, cstime, csshcode, cshname, cstotal, cs5vol, cs20vol, cs5volrate, cs20volrate, cs5price, cs20price, cs5prirate, cs20prirate;
	unsigned long *lengths, len = 0;
	char temp[60];

	CString csMsg, cstemp[MAX_JONGMOK], cschdeg;
	COleDateTime t;
	COleDateTimeSpan ts;

	if(myData->reconnect != 1)
		return 0;

	//UpdateAVGInfo();

	t = COleDateTime::GetCurrentTime();
	csDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay()); 
	cstime.Format(_T("%02d%02d"), t.GetHour(), t.GetMinute());


	mysql_set_character_set(myData, "euckr");
	tjmList.listnum = 0;
	index = 0;
		mysql_set_character_set(myData, "euckr");
	if(atoi(cstime) <= 910){
		csQuery.Format("select t3.shcode, t3.hname, t3.price,  t3.diff, t3.volume, t3.avg5price, t3.avg5vol, t3.avg20price, t3.avg20vol, t3.avg60price, t3.avg60vol, t3.jnilvolume, t3.openprice, t3.hiprice, t3.lowprice, t3.low52date, t3.hi52date, t3.per, t3.total, t3.salert, t3.opert, t3.netrt, t3.bfnetinfome, t3.bfnetinfome2, t3.vol \
					   from etrade_db.jminfo_tbl, (SELECT t2.shcode, t2.hname, t2.avg5price, t2.avg5vol, t2.avg20price, t2.avg20vol, t2.avg60price, t2.avg60vol, t2.cur_date, t1.diff, t1.price, t1.volume, t1.jnilvolume, t1.openprice, t1.hiprice, t1.lowprice, t1.vol,\
					   t1.low52date, t1.hi52date, t1.per, t1.total, t1.salert, t1.opert, t1.netrt, t1.bfnetinfome, t1.bfnetinfome2 FROM etrade_db.avg_5_20_60_jm_tbl as t2 inner join etrade_db.jminfo_tbl as t1 on cast(t1.volume as signed) > (t2.avg20vol * 0.3) and cast(t1.lowprice as signed) <= t2.avg5price  and cast(t1.price as signed) < (t2.avg5price * 1.05) and cast(t1.price as signed) < (t2.avg20price * 1.05) and (t2.avg5price * 1.05 > (t2.avg20price) )  and (t2.avg5price < (t2.avg20price*1.05) ) and cast(t1.price as signed) <= (t2.avg60price * 1.15)  and t1.volume > 3000 and t1.vol > 170 and  ( cast(t1.total as signed) >= 500 ) and t1.diff < 10 and t2.shcode = t1.shcode  and t1.cur_date >= t2.cur_date and t1.cur_date >= '%s' ) as t3 where etrade_db.jminfo_tbl.shcode = t3.shcode Group by t3.shcode", csDate);

	}
	else if(atoi(cstime) <= 920){
		csQuery.Format("select t3.shcode, t3.hname, t3.price,  t3.diff, t3.volume, t3.avg5price, t3.avg5vol, t3.avg20price, t3.avg20vol, t3.avg60price, t3.avg60vol, t3.jnilvolume, t3.openprice, t3.hiprice, t3.lowprice, t3.low52date, t3.hi52date, t3.per, t3.total, t3.salert, t3.opert, t3.netrt, t3.bfnetinfome, t3.bfnetinfome2, t3.vol \
					   from etrade_db.jminfo_tbl, (SELECT t2.shcode, t2.hname, t2.avg5price, t2.avg5vol, t2.avg20price, t2.avg20vol, t2.avg60price, t2.avg60vol, t2.cur_date, t1.diff, t1.price, t1.volume, t1.jnilvolume, t1.openprice, t1.hiprice, t1.lowprice, t1.vol,\
					   t1.low52date, t1.hi52date, t1.per, t1.total, t1.salert, t1.opert, t1.netrt, t1.bfnetinfome, t1.bfnetinfome2 FROM etrade_db.avg_5_20_60_jm_tbl as t2 inner join etrade_db.jminfo_tbl as t1 on cast(t1.volume as signed) > (t2.avg20vol * 0.5) and cast(t1.lowprice as signed) <= t2.avg5price  and cast(t1.price as signed) < (t2.avg5price * 1.05) and cast(t1.price as signed) < (t2.avg20price * 1.05) and (t2.avg5price * 1.05 > (t2.avg20price) )  and (t2.avg5price < (t2.avg20price*1.05) ) and cast(t1.price as signed) <= (t2.avg60price * 1.15)  and t1.volume > 5000 and t1.vol > 170 and  ( cast(t1.total as signed) >= 500 ) and t1.diff < 10 and t2.shcode = t1.shcode  and t1.cur_date >= t2.cur_date and t1.cur_date >= '%s' ) as t3 where etrade_db.jminfo_tbl.shcode = t3.shcode Group by t3.shcode", csDate);
	}
	else if(atoi(cstime) <= 930){
		csQuery.Format("select t3.shcode, t3.hname, t3.price,  t3.diff, t3.volume, t3.avg5price, t3.avg5vol, t3.avg20price, t3.avg20vol, t3.avg60price, t3.avg60vol, t3.jnilvolume, t3.openprice, t3.hiprice, t3.lowprice, t3.low52date, t3.hi52date, t3.per, t3.total, t3.salert, t3.opert, t3.netrt, t3.bfnetinfome, t3.bfnetinfome2, t3.vol \
					   from etrade_db.jminfo_tbl, (SELECT t2.shcode, t2.hname, t2.avg5price, t2.avg5vol, t2.avg20price, t2.avg20vol, t2.avg60price, t2.avg60vol, t2.cur_date, t1.diff, t1.price, t1.volume, t1.jnilvolume, t1.openprice, t1.hiprice, t1.lowprice, t1.vol,\
					   t1.low52date, t1.hi52date, t1.per, t1.total, t1.salert, t1.opert, t1.netrt, t1.bfnetinfome, t1.bfnetinfome2 FROM etrade_db.avg_5_20_60_jm_tbl as t2 inner join etrade_db.jminfo_tbl as t1 on cast(t1.volume as signed) > (t2.avg20vol * 0.7) and cast(t1.lowprice as signed) <= t2.avg5price  and cast(t1.price as signed) < (t2.avg5price * 1.05) and cast(t1.price as signed) < (t2.avg20price * 1.05) and (t2.avg5price * 1.05 > (t2.avg20price) )  and (t2.avg5price < (t2.avg20price*1.05) ) and cast(t1.price as signed) <= (t2.avg60price * 1.15)  and t1.volume > 10000 and t1.vol > 170 and  ( cast(t1.total as signed) >= 500 ) and t1.diff < 10 and t2.shcode = t1.shcode  and t1.cur_date >= t2.cur_date and t1.cur_date >= '%s' ) as t3 where etrade_db.jminfo_tbl.shcode = t3.shcode Group by t3.shcode", csDate);
	}
	else if(atoi(cstime) <= 1000){
		csQuery.Format("select t3.shcode, t3.hname, t3.price,  t3.diff, t3.volume, t3.avg5price, t3.avg5vol, t3.avg20price, t3.avg20vol, t3.avg60price, t3.avg60vol, t3.jnilvolume, t3.openprice, t3.hiprice, t3.lowprice, t3.low52date, t3.hi52date, t3.per, t3.total, t3.salert, t3.opert, t3.netrt, t3.bfnetinfome, t3.bfnetinfome2, t3.vol \
					   from etrade_db.jminfo_tbl, (SELECT t2.shcode, t2.hname, t2.avg5price, t2.avg5vol, t2.avg20price, t2.avg20vol, t2.avg60price, t2.avg60vol, t2.cur_date, t1.diff, t1.price, t1.volume, t1.jnilvolume, t1.openprice, t1.hiprice, t1.lowprice, t1.vol,\
					   t1.low52date, t1.hi52date, t1.per, t1.total, t1.salert, t1.opert, t1.netrt, t1.bfnetinfome, t1.bfnetinfome2 FROM etrade_db.avg_5_20_60_jm_tbl as t2 inner join etrade_db.jminfo_tbl as t1 on cast(t1.volume as signed) > (t2.avg20vol * 1.2) and cast(t1.lowprice as signed) <= t2.avg5price  and cast(t1.price as signed) < (t2.avg5price * 1.05) and cast(t1.price as signed) < (t2.avg20price * 1.05) and (t2.avg5price * 1.05 > (t2.avg20price) )  and (t2.avg5price < (t2.avg20price*1.05) ) and cast(t1.price as signed) <= (t2.avg60price * 1.15)  and t1.volume > 30000 and t1.vol > 150 and  ( cast(t1.total as signed) >= 500 ) and t1.diff < 10 and t2.shcode = t1.shcode  and t1.cur_date >= t2.cur_date and t1.cur_date >= '%s' ) as t3 where etrade_db.jminfo_tbl.shcode = t3.shcode Group by t3.shcode", csDate);
	}
	else if(atoi(cstime) <= 1030){
		csQuery.Format("select t3.shcode, t3.hname, t3.price,  t3.diff, t3.volume, t3.avg5price, t3.avg5vol, t3.avg20price, t3.avg20vol, t3.avg60price, t3.avg60vol, t3.jnilvolume, t3.openprice, t3.hiprice, t3.lowprice, t3.low52date, t3.hi52date, t3.per, t3.total, t3.salert, t3.opert, t3.netrt, t3.bfnetinfome, t3.bfnetinfome2, t3.vol \
					   from etrade_db.jminfo_tbl, (SELECT t2.shcode, t2.hname, t2.avg5price, t2.avg5vol, t2.avg20price, t2.avg20vol, t2.avg60price, t2.avg60vol, t2.cur_date, t1.diff, t1.price, t1.volume, t1.jnilvolume, t1.openprice, t1.hiprice, t1.lowprice, t1.vol,\
					   t1.low52date, t1.hi52date, t1.per, t1.total, t1.salert, t1.opert, t1.netrt, t1.bfnetinfome, t1.bfnetinfome2 FROM etrade_db.avg_5_20_60_jm_tbl as t2 inner join etrade_db.jminfo_tbl as t1 on cast(t1.volume as signed) > (t2.avg20vol * 1.5) and cast(t1.lowprice as signed) <= t2.avg5price  and cast(t1.price as signed) < (t2.avg5price * 1.05) and cast(t1.price as signed) < (t2.avg20price * 1.05) and (t2.avg5price * 1.05 > (t2.avg20price) )  and (t2.avg5price < (t2.avg20price*1.05) ) and cast(t1.price as signed) <= (t2.avg60price * 1.15)  and t1.volume > 50000 and t1.vol > 130 and  ( cast(t1.total as signed) >= 500 ) and t1.diff < 10 and t2.shcode = t1.shcode  and t1.cur_date >= t2.cur_date and t1.cur_date >= '%s' ) as t3 where etrade_db.jminfo_tbl.shcode = t3.shcode Group by t3.shcode", csDate);
	}
	else{
		csQuery.Format("select t3.shcode, t3.hname, t3.price,  t3.diff, t3.volume, t3.avg5price, t3.avg5vol, t3.avg20price, t3.avg20vol, t3.avg60price, t3.avg60vol, t3.jnilvolume, t3.openprice, t3.hiprice, t3.lowprice, t3.low52date, t3.hi52date, t3.per, t3.total, t3.salert, t3.opert, t3.netrt, t3.bfnetinfome, t3.bfnetinfome2, t3.vol \
					   from etrade_db.jminfo_tbl, (SELECT t2.shcode, t2.hname, t2.avg5price, t2.avg5vol, t2.avg20price, t2.avg20vol, t2.avg60price, t2.avg60vol, t2.cur_date, t1.diff, t1.price, t1.volume, t1.jnilvolume, t1.openprice, t1.hiprice, t1.lowprice, t1.vol,\
					   t1.low52date, t1.hi52date, t1.per, t1.total, t1.salert, t1.opert, t1.netrt, t1.bfnetinfome, t1.bfnetinfome2 FROM etrade_db.avg_5_20_60_jm_tbl as t2 inner join etrade_db.jminfo_tbl as t1 on cast(t1.volume as signed) > (t2.avg20vol * 1.7) and cast(t1.lowprice as signed) <= t2.avg5price  and cast(t1.price as signed) < (t2.avg5price * 1.05) and cast(t1.price as signed) < (t2.avg20price * 1.05) and (t2.avg5price * 1.05 > (t2.avg20price) )  and (t2.avg5price < (t2.avg20price*1.05) ) and cast(t1.price as signed) <= (t2.avg60price * 1.15)  and t1.volume > 70000 and t1.vol > 110 and  ( cast(t1.total as signed) >= 500 ) and t1.diff < 10 and t2.shcode = t1.shcode  and t1.cur_date >= t2.cur_date and t1.cur_date >= '%s' ) as t3 where etrade_db.jminfo_tbl.shcode = t3.shcode Group by t3.shcode", csDate);
	}

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}


	index = 0;

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		csmsg.Empty();
		//shcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].shcode = cs;
		i++;
		csmsg = cs;

		//hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].hname = cs;
		i++;
		csmsg += cs;

		//price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].price = cs;
		i++;
		csmsg += cs;

		//diff
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].diff = cs;
		if(atof(cs) > 10)
			continue;
		i++;
		csmsg += cs;

		//volume
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].volume = cs;
		i++;
		csmsg += cs;

		//5price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].avg5price = cs;
		i++;
		csmsg += cs;
		//5volume
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].avg5vol = cs;
		i++;
		csmsg += cs;

		//20price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].avg20price = cs;
		i++;
		csmsg += cs;

		//20volume
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].avg20vol = cs;
		i++;
		csmsg += cs;
		//60price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].avg60price = cs;
		i++;
		csmsg += cs;

		//60volume
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].avg60vol = cs;
		i++;
		csmsg += cs;

		//jvolume
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].jvolume = cs;
		i++;
		csmsg += cs;

		//openprice
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].openprice = cs;
		i++;
		csmsg += cs;

		//hiprice
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].hiprice = cs;
		i++;
		csmsg += cs;

		//lowprice
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].lowprice = cs;
		i++;
		csmsg += cs;

		//low52date
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].date52low = cs;
		i++;
		csmsg += cs;

		//hi52date
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].date52hi = cs;
		i++;
		csmsg += cs;

		//per
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].per = cs;
		i++;
		csmsg += cs;

		//total
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		cstotal = cs;
		tjmList.jminfo[index].totval = cs;
		i++;
		csmsg += cs;

		//salert
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].salert = cs;
		i++;
		csmsg += cs;

		//opert
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].opert = cs;
		i++;
		csmsg += cs;

		//netrt
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].netrt = cs;
		i++;
		csmsg += cs;

		//preval
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].preval = cs;
		i++;
		csmsg += cs;

		//ppreval
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].ppreval = cs;
		i++;
		csmsg += cs;

		//chdegree
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		tjmList.jminfo[index].chdegree = cs;
		i++;
		csmsg += cs;

		csmsg += "\n";
		//OutputDebugString(csmsg);

		index++;
		if(index >= 3000)
			break;
	}

	mysql_free_result(myRes);
	mysql_commit(myData);

	tjmList.listnum = index;

	for(i = 0; i < tjmList.listnum; i++){
		addNewJMinfo(tjmList.jminfo[i].shcode, tjmList.jminfo[i].hname, tjmList.jminfo[i].price, "DAILY");
	}
	return index;
}

int CMySQLCMD::addNewJMinfo(CString csshcode, CString cshname, CString csregprice, CString csinfo)
{

	int i = 0, rc;
	CString csCurDate, csQuery, csMsg, csRegDate;

	COleDateTime t = COleDateTime::GetCurrentTime();

	if(myData->reconnect != 1)
		return 0;

	csCurDate.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	csRegDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	csQuery.Format("insert INTO etrade_db.Order_JM_tbl(shcode, hname, reg_date, cur_datetime, price, msprice, mdprice, info) VALUES ('%s','%s', '%s','%s', '%s', '%s','%s','%s')", 					
			csshcode, cshname, csRegDate,  csCurDate, csregprice, "",  "", csinfo);


	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		mysql_commit(myData);
		return 0;
	}

	mysql_commit(myData);
	return 1;

}

int CMySQLCMD::checkorgstatus(CString shcode, int mode)
{

	int i = 0, index = 0, rc, value = 1;
	CString csQuery, csMsg;
	CString csCurDate;
	COleDateTime t;
	COleDateTimeSpan ts;
	char temp[30];
	unsigned long *lengths;
	CString cs, precs;
	double totval = 0;

	cs.Empty();
	if(myData->reconnect != 1)
		return 0;


	if(mode){
		csQuery.Format("select org from etrade_db.1702list_tbl where shcode = '%s'  order by cur_date desc limit 3 ", shcode);
	}
	else{
		csQuery.Format("select org from etrade_db.1702list_tbl where shcode = '%s'  order by cur_date desc limit 3 ", shcode);
	}

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	while(myRow = mysql_fetch_row(myRes)){
		i = 0;
		lengths = mysql_fetch_lengths(myRes);
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);

		 
		if(index == 0){
			if(atol(temp) < -100){
				value = 0;
			}
		}
		totval += atol(temp);

		index++;
	}
	mysql_free_result(myRes);
	mysql_commit(myData);
    if(totval < 0)
		return 0;

	return value;
}


int CMySQLCMD::checkstartpoint1702(CString shcode, int mode)
{

	int i = 0, index = 0, rc, value = 1;
	CString csQuery, csMsg;
	CString csCurDate;
	COleDateTime t;
	COleDateTimeSpan ts;
	char temp[30];
	unsigned long *lengths;
	CString cs, precs;

	cs.Empty();
	if(myData->reconnect != 1)
		return 0;


	if(mode){
		csQuery.Format("select org from etrade_db.1702list_tbl where shcode = '%s'  order by cur_date desc limit 5 ", shcode);
	}
	else{
		csQuery.Format("select org from etrade_db.1702list_tbl where shcode = '%s'  order by cur_date desc limit 3 ", shcode);
	}

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	while(myRow = mysql_fetch_row(myRes)){
		i = 0;
		lengths = mysql_fetch_lengths(myRes);
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);

		 
		if(index == 0){
			if(atol(temp) < 300){
				value = 0;
			}
		}
		else{
			if(atol(temp) > 100){
				value = 0;
			}
		}
		index++;
	}
	mysql_free_result(myRes);
	mysql_commit(myData);

	return value;
}


int CMySQLCMD::updateMaxSUKEUB(CString csshcode, CString cshname, CString csprice, int mode)
{
	unsigned long *lengths, len = 0;
	char temp[100];

	int i = 0, rc;
	CString csCurDate, csRegtime, csQuery, csMsg;

	COleDateTime t = COleDateTime::GetCurrentTime();

	if(myData->reconnect != 1)
		return 0;


	csQuery.Format("select logdate from  etrade_db.ordlog_tbl  order by logdate  desc  limit 1");

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		//shcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csRegtime.Format("%s", temp);

	}
	mysql_free_result(myRes);
	mysql_commit(myData);

	//csRegtime.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	csCurDate.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	if(mode){//�ż�
		csQuery.Format("update etrade_db.ordlog_tbl set curprice = '%s',  msgcode = '4' where shcode = '%s' and logdate = '%s' and msgcode = '3333'",  csprice,   csshcode, csRegtime);
	}
	else{//�ŵ�
		csQuery.Format("update etrade_db.ordlog_tbl set curprice = '%s',  msgcode = '8' where shcode = '%s'  and msgcode = '4'",  csprice,  csshcode);

	}

	mysql_set_character_set(myData, "euckr");
	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		//return 0;
	}

	mysql_commit(myData);

	return 1;

}

int CMySQLCMD::getEnbelob(int mode, int countmode)
{

	int i = 0, index = 0, rc;
	CString csQuery, csMsg;
	CString csCurDate, csmsdate, csdate, cs;

	COleDateTimeSpan ts;
	unsigned long *lengths, len = 0;
	char temp[100];
	COleDateTime t = COleDateTime::GetCurrentTime();

	if(myData->reconnect != 1)
		return 0;

	csdate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	csCurDate.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	int h = t.GetHour();
	int m = t.GetMinute();
	ts.SetDateTimeSpan(0, 0, 10, 0);
	t.operator-=(ts);

	csmsdate.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
#if 0
	int mtime = (int)((((t.GetHour() - 9) * 12) + t.GetMinute()/5 ) * 1);
	if(mtime > 9)
		mtime  = 9;
	if(mtime < 5)
		mtime = 5;

	if(countmode){//count 1
		csQuery.Format("select t1.shcode, t1.hname, t2.price, t1.avg20price, t2.cur_date from etrade_db.avg_5_20_60_jm_tbl t1, etrade_db.jminfo_tbl t2 where  cast(t1.avg20price as signed) * 1.25 > cast(t2.price as signed) and cast(t2.jnilvolume as signed) * %d < t2.volume and cast(t1.avg5vol as signed) * %d < t2.volume  and cast(t1.avg5price as signed) * 1.25 > cast(t2.price as signed) and cast(t2.openprice as signed) <= cast(t2.price as signed) and cast(t2.total as signed) > 700 and cast(t2.jnilvolume as signed) > 50000  and cast(t2.diff as signed) > 0 and cast(t2.diff as signed) < 25  and t1.shcode = t2.shcode and t2.cur_date = '%s' order by shcode", mtime, mtime, csCurDate);
		//csQuery.Format("select t1.shcode, t1.hname, t2.price, t1.avg20price, t2.cur_date from etrade_db.avg_5_20_60_jm_tbl t1, etrade_db.jminfo_tbl t2 where  cast(t1.avg5vol as signed) * 2 < t2.volume and cast(t2.total as signed) > 500 and cast(t2.jnilvolume as signed) > 50000 and cast(t1.avg20price as signed)*0.90 < cast(t2.price as signed) and cast(t1.avg20price as signed) * 1.10 > cast(t2.price as signed) and  cast(t1.avg5price as signed) > cast(t1.avg20price as signed)  and t1.shcode = t2.shcode and t2.cur_date = '%s' order by shcode", csCurDate);
	}
	else{//count 0
		csQuery.Format("select t1.shcode, t1.hname, t2.price, t1.avg20price, t2.cur_date from etrade_db.avg_5_20_60_jm_tbl t1, etrade_db.jminfo_tbl t2 where cast(t1.avg5vol as signed) * %d < t2.volume  and cast(t1.avg20vol as signed) *  %d < t2.volume  and cast(t2.jnilvolume as signed) * %d < t2.volume and cast(t2.openprice as signed) <= cast(t2.price as signed) and cast(t2.diff as signed) > 0 and cast(t2.diff as signed) < 25 and t2.janginfo = 'KOSPI'and cast(t2.total as signed) > 1000 and cast(t2.jnilvolume as signed) > 50000 and cast(t1.avg20price as signed) * 0.70 < cast(t2.price as signed) and cast(t1.avg5price as signed) * 1.25 > cast(t2.price as signed)  and cast(t1.avg20price as signed) * 1.25 > cast(t2.price as signed)  and t1.shcode = t2.shcode and t2.cur_date = '%s' order by shcode", mtime, mtime, mtime, csCurDate);

	}
	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	mgMa.mginfo.jmcnt = 0;
	index = 0;
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		//shcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		mgMa.mginfo.jminfo[index].shcode = cs;
		t1637info.shcodelist[index] = cs;

		i++;

		//hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		mgMa.mginfo.jminfo[index].hname = cs;		
		i++;

		//ms price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));
		mgMa.mginfo.jminfo[index].price = cs;
		i++;

		//20avg * 10% price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);		
		cs.Format("%0.2f", atof(temp));
		mgMa.mginfo.jminfo[index].msprice = cs;
		i++;

		//date
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		mgMa.mginfo.jminfo[index].mdprice = cs;
		i++;

		index++;
	}
	t1637info.count = mgMa.mginfo.jmcnt = index;
	mysql_free_result(myRes);
	mysql_commit(myData);

	for(i = 0; i < mgMa.mginfo.jmcnt; i++){
		addEnvelopjminfo(mgMa.mginfo.jminfo[i].shcode, mgMa.mginfo.jminfo[i].hname, mgMa.mginfo.jminfo[i].price, mgMa.mginfo.jminfo[i].msprice, mode, countmode);

	}

#endif


	if(h  >= 15 && m > 25){
		csQuery.Format("select shcode, hname, price, 20avgprice, cur_datetime, countmode from etrade_db.envelop_jm_tbl where reg_date = '%s' and mode = %d and msprice = '' order by price desc", csdate,  countmode);
		glastmode = 1;
	}
	else{
		csQuery.Format("select shcode, hname, price, 20avgprice, cur_datetime, countmode from etrade_db.envelop_jm_tbl where cur_datetime <= '%s' and  cur_datetime >= '%s' and mode = %d and msprice = '' order by cur_datetime desc limit 6",  csCurDate, csmsdate,  countmode);

	}
	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	mgMa.mginfo.jmcnt = 0;
	index = 0;
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		//shcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		mgMa.mginfo.jminfo[index].shcode = cs;
		t1637info.shcodelist[index] = cs;

		i++;

		//hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		mgMa.mginfo.jminfo[index].hname = cs;		
		i++;

		//ms price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));
		mgMa.mginfo.jminfo[index].price = cs;
		i++;

		//20avg * 10% price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);		
		cs.Format("%0.2f", atof(temp));
		mgMa.mginfo.jminfo[index].msprice = cs;
		i++;

		//date
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		mgMa.mginfo.jminfo[index].mdprice = cs;
		i++;

		//countmode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		mgMa.mginfo.jminfo[index].level1 = cs;
		i++;


		index++;
	}
	t1637info.count = mgMa.mginfo.jmcnt = index;
	mysql_free_result(myRes);
	mysql_commit(myData);

	return index;

}

int CMySQLCMD::addEnvelopjminfo(CString csshcode, CString cshname, CString csprice, CString avg20price, int mode, int countmode)
{

	int i = 0, rc;
	CString csCurDate, csQuery, csMsg, csRegDate;

	COleDateTime t = COleDateTime::GetCurrentTime();

	if(myData->reconnect != 1) 
		return 0;

	csCurDate.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	csRegDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	csQuery.Format("insert INTO etrade_db.envelop_jm_tbl(shcode, hname, reg_date, cur_datetime, price, 20avgprice, ms_datetime, msprice, md_datetime, mdprice, rate, mode, countmode) VALUES ('%s','%s', '%s','%s', '%s', '%s','%s','%s', '%s', '%s','%s', '%d', '%d')", 					
			csshcode, cshname, csRegDate,  csCurDate, csprice, "",  "", "", "", "", "", mode, countmode);

	
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		mysql_commit(myData);
		return 0;
	}

	mysql_commit(myData);
	return 1;

}
int CMySQLCMD::checkmsdate(CString csshcode, int countmode, CString &csdate, int passdays)
{
	unsigned long *lengths, len = 0;
	char temp[100];
	int i = 0, rc = 0;
	CString csCurDate, csRegtime, csQuery, csMsg, csmsprice, csrate;

	COleDateTimeSpan ts;


	ts.SetDateTimeSpan(passdays, 0, 0, 0);

	COleDateTime t = COleDateTime::GetCurrentTime();	

	t.operator-=(ts);

	csCurDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	if(myData->reconnect != 1)
		return 0;

	csQuery.Format("select ms_datetime from  etrade_db.envelop_jm_tbl where shcode = '%s' and mode = %d and msprice is not null and msprice != '' and mdprice = '' and reg_date <= '%s' order by reg_date  desc", csshcode, countmode, csCurDate);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		//msprice
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csdate.Format("%s", temp);
		rc = 1;
	}
	mysql_free_result(myRes);
	mysql_commit(myData);


	return rc;

}



int CMySQLCMD::updateEnvelopjminfo(CString csshcode, CString cshname, CString csprice, CString csmsmdprice, int mode, int countmode)
{
	unsigned long *lengths, len = 0;
	char temp[100];
	int i = 0, rc;
	CString csCurDate, csRegtime, csQuery, csMsg, csmsprice, csrate;

	COleDateTime t = COleDateTime::GetCurrentTime();
	COleDateTimeSpan ts;

	if(myData->reconnect != 1)
		return 0;

	if(!mode){//�ŵ��� �ż���
		csQuery.Format("select msprice from  etrade_db.envelop_jm_tbl where shcode = '%s' and mode = '%d' order by reg_date  desc  limit 1", csshcode, countmode);

		mysql_set_character_set(myData, "euckr");

		rc = mysql_query(myData, csQuery);
		if(rc){
			csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
			OutputDebugString(csMsg);
			return 0;
		}
		myRes = mysql_store_result(myData);
		if(myRes == NULL){
			csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
			OutputDebugString(csMsg);
			return 0;
		}

		while(myRow = mysql_fetch_row(myRes)){

			lengths = mysql_fetch_lengths(myRes);
			i = 0;

			//msprice
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			csmsprice.Format("%s", temp);

		}
		mysql_free_result(myRes);
		mysql_commit(myData);
	}

	csCurDate.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	ts.SetDateTimeSpan(5, 0, 0, 0);
	t.operator-=(ts);

	csRegtime.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	if(mode){//�ż�
		csQuery.Format("update etrade_db.envelop_jm_tbl set msprice = '%s', ms_datetime = '%s' where shcode = '%s' and reg_date >= '%s' and mode = '%d' and (msprice = '' || msprice is null)",  csmsmdprice,  csCurDate, csshcode, csRegtime, countmode);
	}
	else{//�ŵ�
		if(atoi(csmsprice) <= 0)
			csmsprice = csmsmdprice; 
		csrate.Format("%0.2f", (float)((float)(atoi(csmsmdprice)  - atoi(csmsprice)) * 100/(float)atoi(csmsprice)) - 0.3);
		csQuery.Format("update etrade_db.envelop_jm_tbl set mdprice = '%s', md_datetime = '%s', rate = '%s' where shcode = '%s' and  mode = '%d'  and msprice is not null and (mdprice = '' || mdprice is null)",  csmsmdprice, csCurDate, csrate,  csshcode, countmode);

	}

	mysql_set_character_set(myData, "euckr");
	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		//return 0;
	}
	mysql_commit(myData);



	return 1;

}


int CMySQLCMD::getMaxSUKEUB(CString csmode)
{

	int i = 0, index = 0, rc;
	CString csQuery, csMsg;
	CString csCurDate, csmsdate, csdate, cs;
	COleDateTime t;
	COleDateTimeSpan ts;
	unsigned long *lengths, len = 0;
	char temp[100];

	if(myData->reconnect != 1)
		return 0;


	csQuery.Format("select logdate from  etrade_db.ordlog_tbl  order by logdate  desc  limit 1");

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	index = 0;
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		//shcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csmsdate.Format("%s", temp);
		index++;
	}
	mysql_free_result(myRes);
	mysql_commit(myData);

	if(index){
		if(csmode.Compare("NEW") == 0){
			csQuery.Format("select shcode, hname, eventprice, sms, fsms from  etrade_db.ordlog_tbl where logdate = '%s' and cast(sms as signed) > 0 and cast(fsms as signed) >= 0 and cast(rate as signed) > 0  and msgcode = '3333' order by cast(sms as signed) desc", csmsdate);
		}
		else{
			if(atoi(gcsLogIndex) == 1){	
				csQuery.Format("select shcode, hname, eventprice, sms, fsms from  etrade_db.ordlog_tbl where logdate = '%s' and cast(sms as signed) > 0 and cast(rate as signed) > 0 and msgcode = '3333' order by cast(sms as signed) desc", csmsdate);
			}
			else{
				csQuery.Format("select shcode, hname, eventprice, sms, fsms from  etrade_db.ordlog_tbl where logdate = '%s' order by cast(sms as signed) desc", csmsdate);
			}
		}

		rc = mysql_query(myData, csQuery);
		if(rc){
			csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
			OutputDebugString(csMsg);
			return 0;
		}
		myRes = mysql_store_result(myData);
		if(myRes == NULL){
			csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
			OutputDebugString(csMsg);
			return 0;
		}

		mgMa.mginfo.jmcnt = 0;
		index = 0;
		while(myRow = mysql_fetch_row(myRes)){

			lengths = mysql_fetch_lengths(myRes);
			i = 0;

			//shcode
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			cs.Format("%s", temp);
			mgMa.mginfo.jminfo[index].shcode = cs;
			t1637info.shcodelist[index] = cs;

			i++;

			//hname
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			cs.Format("%s", temp);
			mgMa.mginfo.jminfo[index].hname = cs;		
			i++;

			//reg price
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			cs.Format("%s", temp);
			mgMa.mginfo.jminfo[index].price = cs;
			i++;

			//ms price
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			cs.Format("%s", temp);
			mgMa.mginfo.jminfo[index].msprice = cs;
			i++;

			//md price
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			cs.Format("%s", temp);
			mgMa.mginfo.jminfo[index].mdprice = cs;
			i++;

			index++;
		}
		t1637info.count = mgMa.mginfo.jmcnt = index;
		mysql_free_result(myRes);
		mysql_commit(myData);
	}
	return index;

}

int CMySQLCMD::updateNewMSInfo(CString csshcode, CString cshname, CString csprice, int mode)
{

	int i = 0, rc;
	CString csCurDate, csRegtime, csQuery, csMsg;

	COleDateTime t = COleDateTime::GetCurrentTime();

	if(myData->reconnect != 1)
		return 0;


	csRegtime.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	csCurDate.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	if(mode){//�ż�
		csQuery.Format("update etrade_db.Order_JM_tbl set msprice = '%s', mdprice = '%s' where shcode = '%s' and reg_date = '%s' and msprice = ''",  csprice,  csCurDate , csshcode, csRegtime);
	}
	else{//�ŵ�
		csQuery.Format("update etrade_db.Order_JM_tbl set mdprice = '%s', msprice = '%s' where shcode = '%s' and msprice is not null",  csprice,  csCurDate , csshcode);

	}

	mysql_set_character_set(myData, "euckr");
	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		//return 0;
	}

	mysql_commit(myData);

	return 1;

}

int CMySQLCMD::getNewMSInfo(CString csmode)
{

	int i = 0, index = 0, rc;
	CString csQuery, csMsg;
	CString csCurDate, csmsdate, csdate, cs;
	COleDateTime t;
	COleDateTimeSpan ts;
	unsigned long *lengths, len = 0;
	char temp[100];

	if(myData->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();

	csdate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	ts.SetDateTimeSpan(0, 0, 30, 0) ;

	t.operator -= (ts);
	csmsdate.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	csQuery.Format("select shcode, hname, price, msprice, mdprice from  etrade_db.Order_JM_tbl where reg_date >= '%s' and cur_datetime >= '%s' and msprice = ''  group by shcode order by cur_datetime  desc limit 1", csdate, csmsdate);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	mgMa.mginfo.jmcnt = 0;
	index = 0;
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		//shcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		mgMa.mginfo.jminfo[index].shcode = cs;
		t1637info.shcodelist[index] = cs;

		i++;

		//hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		mgMa.mginfo.jminfo[index].hname = cs;		
		i++;

		//reg price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		mgMa.mginfo.jminfo[index].price = cs;
		i++;

		//ms price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		mgMa.mginfo.jminfo[index].msprice = cs;
		i++;

		//md price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		mgMa.mginfo.jminfo[index].mdprice = cs;
		i++;

		index++;
	}
	t1637info.count = mgMa.mginfo.jmcnt = index;
	mysql_free_result(myRes);
	mysql_commit(myData);
	return index;

}

int CMySQLCMD::db_update_1637(LPt1637OutBlock1 p)
{

	int i = 0, rc, val = 0;
	CString cs, csQuery, csMsg, csCurDate;
	CString csvalue, csvolume;

	char temp[10][30];

	COleDateTime t;
	COleDateTimeSpan ts;

	t = COleDateTime::GetCurrentTime();

	if(myData->reconnect != 1)
		return 0;

	memset(temp, 0, sizeof(temp));

	memcpy(temp[i++], p->shcode, sizeof(p->shcode));
	cs.Format("%s0", temp[0]+1);
	memcpy(temp[i++], p->date, sizeof(p->date));

	memcpy(temp[i++], p->time, sizeof(p->time));
	memcpy(temp[i++], p->price, sizeof(p->price));
	memcpy(temp[i++], p->sign, sizeof(p->sign));

	memcpy(temp[i++], p->change, sizeof(p->change));
	memcpy(temp[i++], p->diff, sizeof(p->diff));
	memcpy(temp[i++], p->svalue, sizeof(p->svalue));
	csvalue.Format("%ld", atol(temp[7]));
	memcpy(temp[i++], p->svolume, sizeof(p->svolume));
	csvolume.Format("%ld", atol(temp[8]));

	csCurDate.Format(_T("%d%02d%02d_%s"), t.GetYear(), t.GetMonth(), t.GetDay(), temp[0]);
	csQuery.Format("REPLACE INTO t1637_tbl(shcode,  cur_date, time, price, sign, changevalue, diff, svalue, svolume)   VALUES ( '%s', '%s', '%s', '%s' ,'%s', '%s', '%s', '%s', '%s')", 					
					 cs, temp[1], temp[2], temp[3], temp[4], temp[5], temp[6], csvalue, csvolume);		

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	mysql_commit(myData);
	return 1;

}


float CMySQLCMD::getmgAVGjminfoRange(CString shcode, CString csitem, int range, CString &csavg)
{

	int i = 0, index = 0, rc;
	CString csQuery, csMsg;
	CString csCurDate;
	COleDateTime t;
	COleDateTimeSpan ts;
	char temp[30];
	unsigned long *lengths;
	CString cs, csfirst, cslast;

	cs.Empty();
	if(myData->reconnect != 1)
		return 0.0;

	csQuery.Format("select %s from etrade_db.avg_5_20_60_jmlist_tbl  where shcode = '%s' order by cur_date desc limit %d", csitem, shcode, range);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0.0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0.0;
	}

	while(myRow = mysql_fetch_row(myRes)){
		i = 0;
		lengths = mysql_fetch_lengths(myRes);
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		if(index == 0){
			csfirst = cs;
			csavg = cs;
		}
		index++;
		cslast = cs;
	}
	mysql_free_result(myRes);
	mysql_commit(myData);
	if((index > 0) && (atoi(csfirst) > 0 && atoi(cslast) > 0)){
		float rate = (float)( atoi(csfirst) - atoi(cslast) ) * 100 / atoi(cslast) ;
		return rate;
	}
	else{
		return -1.0;
	}

}
int CMySQLCMD::checktablelist(CString csshcode)
{
	unsigned long *lengths;
	CString csQuery, csMsg;
	int rc = 0;

	if(myData->reconnect != 1)
		return 0;


	csQuery.Format("select * from etrade_db.analyList_tbl where shcode = '%s' limit 1", csshcode);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	int rev = 0;
	while(myRow = mysql_fetch_row(myRes)){
		lengths = mysql_fetch_lengths(myRes);
		rev = 1;
	}
	mysql_free_result(myRes);
	mysql_commit(myData);

	return rev;

}

int CMySQLCMD::getmgAVGjminfoOne(CString shcode, CString csname, CString &cs1,  CString &cs2,  CString &cs3,  CString &cs4,  CString &cs5, CString &cs6, int mode)
{

	int i = 0, index = 0, rc;
	CString csQuery, csMsg;
	CString csCurDate;
	COleDateTime t;
	COleDateTimeSpan ts;
	char temp[30];
	unsigned long *lengths;
	CString cs;

	cs.Empty();
	if(myData->reconnect != 1)
		return 0;

	csQuery.Format("select avg5price, avg20price, avg60price, avg5vol, avg20vol,  avg60vol from etrade_db.avg_5_20_60_jm_tbl  where shcode = '%s' ", shcode);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	while(myRow = mysql_fetch_row(myRes)){
		i = 0;
		lengths = mysql_fetch_lengths(myRes);
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs1.Format("%s", temp);
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs2.Format("%s", temp);
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs3.Format("%s", temp);
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs4.Format("%s", temp);
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs5.Format("%s", temp);
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs6.Format("%s", temp);
		i++;

	}
	mysql_free_result(myRes);
	mysql_commit(myData);
	return 1;

}


int CMySQLCMD::check8407info(CString shcode, CString cstime)
{

	int i = 0, index = 0, rc = 0;
	CString csQuery, csMsg;
	char temp[20];
	unsigned long *lengths;
	CString cs, cstemp, cscheltime, csdate;
	COleDateTime t = COleDateTime::GetCurrentTime();

	cs.Empty();
	if(myData->reconnect != 1)
		return 0;

	csdate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	//cstime.Format(_T("%02d%02d%02d"), t.GetHour(), t.GetMinute(), t.GetSecond());

	csQuery.Format("SELECT price, diff, volume, chdegree, openprice, hiprice, lowprice FROM etrade_db.8407_tbl_list  where shcode = '%s' and cur_date = '%s' order by cur_time desc limit 1", shcode, csdate);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	index = 0;
	etf_mgjminfo.remaxvolume = 0;

	while(myRow = mysql_fetch_row(myRes)){
		i = 0;
		lengths = mysql_fetch_lengths(myRes);

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.csprice.Format("%d", atoi(temp));//price

		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.csrate.Format("%s", temp);//diff
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.csvol.Format("%s", temp);//volume
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.csdegree.Format("%d", atoi(temp));//chdegree
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.csstprice.Format("%d", atoi(temp));//open
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.cshiprice.Format("%s", temp);//hi
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.cslowprice.Format("%d", atoi(temp));//low
		i++;

		index++;


	}
	mysql_free_result(myRes);
	mysql_commit(myData);

	return index;
}


int CMySQLCMD::updatecurinfo(CString shcode)
{

	int i = 0, index = 0, rc = 0;
	CString csQuery, csMsg;
	char temp[20];
	unsigned long *lengths;
	CString cs, cstemp, cscheltime, csdate;
	COleDateTime t = COleDateTime::GetCurrentTime();

	cs.Empty();
	if(myData->reconnect != 1)
		return 0;

	csdate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	csQuery.Format("SELECT price, diff, volume, vol, openprice, hiprice, lowprice, uplmprice, dnlmprice FROM etrade_db.jminfo_tbl  where shcode = '%s' and cur_date >= '%s'", shcode, csdate);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	index = 0;
	etf_mgjminfo.remaxvolume = 0;
	etf_mgjminfo.csuplmprice.Empty();
	etf_mgjminfo.csprice.Empty();

	while(myRow = mysql_fetch_row(myRes)){
		i = 0;
		lengths = mysql_fetch_lengths(myRes);

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.csprice.Format("%d", atoi(temp));//price
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.csrate.Format("%s", temp);//diff
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.csvol.Format("%s", temp);//volume
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.csdegree.Format("%d", atoi(temp));//chdegree
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.csstprice.Format("%d", atoi(temp));//open
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.cshiprice.Format("%s", temp);//hi
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.cslowprice.Format("%d", atoi(temp));//low
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.csuplmprice.Format("%s", temp);//max hi
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		etf_mgjminfo.csdnlmprice.Format("%d", atoi(temp));//max low
		i++;
		index++;

	}
	mysql_free_result(myRes);
	mysql_commit(myData);

	return index;
}


int CMySQLCMD::db_getinfo(CString csmode)
{
	int i = 0, rc = 0;
	CString cs, csQuery, csMsg;
	unsigned long *lengths, len = 0;
	char temp[100];

	if(myData->reconnect != 1)
		return 0;

	mysql_set_character_set(myData, "euckr");

	// 계좌 정보 조회: cfg_tbl 테이블에서 idindex로 계좌 정보 조회
	// 암호화된 비밀번호는 AES_DECRYPT로 복호화
	csQuery.Format("select countid, AES_DECRYPT(countpw, '12345678'), loginid, AES_DECRYPT(loginpw, '23456781') , AES_DECRYPT(verifypw, '34567812'),\
				   msvalue, msrate, mdlevel, sunik, maxsunik, sonjel, maxsonjel, allcount from  cfg_tbl where  idindex = '%s'", csmode);
	rc = mysql_query(myData, csQuery);
	if(rc){
		// 에러 로깅: 계좌 정보 조회 실패
		csMsg.Format(_T("[ERROR] %s - Query Failed: idindex=%s, Error: %s\n"), __FUNCTION__, csmode, mysql_error(myData));
		OutputDebugString(csMsg);
		TRACE(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		// 에러 로깅: 결과셋 가져오기 실패
		csMsg.Format(_T("[ERROR] %s - Store Result Failed: idindex=%s, Error: %s\n"), __FUNCTION__, csmode, mysql_error(myData));
		OutputDebugString(csMsg);
		TRACE(csMsg);
		return 0;
	}

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		if(lengths[0]){
			i = 0;

			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			gscCount.Format("%s", temp);
			i++;

			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			gscCountpw.Format("%s", temp);
			i++;

			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			gcsID.Format("%s", temp);
			i++;

			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			gcsPw.Format("%s", temp);
			i++;

			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			gcsVPw.Format("%s", temp);
			i++;

			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			gcsTotval.Format("%s", temp);
			i++;

			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			gcsMsrate.Format("%s", temp);
			i++;

			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			gcsMdlevel.Format("%s", temp);
			i++;

			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			gcsSunik.Format("%s", temp);
			i++;

			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			gcsMAXSunik.Format("%s", temp);
			i++;

			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			gcsSonjel.Format("%s", temp);
			i++;

			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			gcsMAXSonjel.Format("%s", temp);
			i++;

			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			gSetAll= atoi(temp);
			i++;
			rc = 1;

		}
	}
	mysql_free_result(myRes);
	mysql_commit(myData);
	return rc;
}
int CMySQLCMD::checkweektbl(CString shcode, int nprice)
{

	int i = 0, rev  = 0, rc;
	CString cs, csQuery, csMsg;
	unsigned long *lengths, len = 0;

	COleDateTime t;
	COleDateTimeSpan ts;	
	int value = 0;

	if(myData->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();


	csQuery.Format("select *, volume from (select * from etrade_db.jm_week_tbl where (shcode) in (SELECT shcode FROM   etrade_db.jm_week_tbl where shcode='%s' ) order by cur_datetime desc limit 1)t where t.openprice < %d ",  shcode, nprice);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;
		value = 1;


	}
	mysql_free_result(myRes);
	mysql_commit(myData);

	return value;

}

int preMinute = 0;

CString CMySQLCMD::getJisuDate(CString csdate, CString csmode)
{
	int i = 0, j = 0, rc, val = 0;
	double dval = 0;
	CString cs, csjisju, csval, csQuery, csMsg, csCurDate, cstime;
	unsigned long *lengths, len = 0;
	char temp[100];

	COleDateTime t;

	if(myData->reconnect != 1)
		return cs;

	if(myData->reconnect != 1)
		return cs;

	t = COleDateTime::GetCurrentTime();



	csQuery.Format("select pricejisu, diffjisu from  1511_tbl where cur_date like '%s %%' and gubun = '%s' order by cur_date desc limit 1", csdate, csmode);
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);

		i = 0;

		//��������
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", CommaFormat(temp));
		t1511info.cspricejisu = cs;
		csjisju += cs;
		i++;
		if(csmode.Compare("1") == 0){
			mgJiSu.kospi.precurval = mgJiSu.kospi.curval;
			mgJiSu.kospi.curval = cs;
		}
		else if(csmode.Compare("2") == 0){
			mgJiSu.kosdak.precurval = mgJiSu.kosdak.curval;
			mgJiSu.kosdak.curval = cs;
		}

		//diffjisu
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("(%d.%02d)", val/100, val%100);
		}
		else{
			cs.Format("(-%d.%02d)", (val/100 * -1), (val%100 * -1));
		}

		t1511info.csdiffjisu = cs;
		csjisju += cs;
		i++;

		if(val >= 0){
			cs.Format("%d.%02d", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d", (val/100 * -1), (val%100 * -1));
		}
		if(csmode.Compare("1") == 0){
			if(preMinute != t.GetMinute()){
				mgJiSu.kospi.precurrate = mgJiSu.kospi.currate;
			}
			mgJiSu.kospi.currate = cs;
		}
		else if(csmode.Compare("2") == 0){
			if(preMinute != t.GetMinute()){
				mgJiSu.kosdak.precurrate = mgJiSu.kosdak.currate;
				preMinute = t.GetMinute();
			}
			mgJiSu.kosdak.currate = cs;
		}
		csjisju += "\n";

	}

	mysql_free_result(myRes);
	mysql_commit(myData);
	return csjisju;
}


CString CMySQLCMD::getJisu(CString csmode)
{
	int i = 0, j = 0, rc, val = 0;
	double dval = 0;
	CString cs, csjisju, csval, csQuery, csMsg, csCurDate, cstime;
	unsigned long *lengths, len = 0;
	char temp[100];

	COleDateTime t;
	COleDateTimeSpan ts;

	if(myData->reconnect != 1)
		return cs;

	ts.SetDateTimeSpan(0, 0, 2, 0);

	t.operator -=(ts);

	t = COleDateTime::GetCurrentTime();

	cstime.Format(_T("%d-%02d-%02d 09:00:00"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	if(myData->reconnect != 1)
		return cs;

	csQuery.Format("select * from  1511_tbl where cur_date >= '%s' and gubun = '%s' order by cur_date desc limit 1", cstime, csmode);
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);

		i = 1;
		//hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s:", temp);	
		t1511info.cshname = cs;
		csjisju += cs;
		i++;

		//cur_date
		i++;

		//��������
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", CommaFormat(temp));
		t1511info.cspricejisu = cs;
		csjisju += cs;
		i++;
		if(csmode.Compare("1") == 0){
			mgJiSu.kospi.precurval = mgJiSu.kospi.curval;
			mgJiSu.kospi.curval = cs;
		}
		else if(csmode.Compare("2") == 0){
			mgJiSu.kosdak.precurval = mgJiSu.kosdak.curval;
			mgJiSu.kosdak.curval = cs;
		}

		//diffjisu
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("(%d.%02d)", val/100, val%100);
		}
		else{
			cs.Format("(-%d.%02d)", (val/100 * -1), (val%100 * -1));
		}

		t1511info.csdiffjisu = cs;
		csjisju += cs;
		i++;

		if(val >= 0){
			cs.Format("%d.%02d", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d", (val/100 * -1), (val%100 * -1));
		}
		if(csmode.Compare("1") == 0){
			mgJiSu.kospi.precurrate = mgJiSu.kospi.currate;
			mgJiSu.kospi.currate = cs;
		}
		else if(csmode.Compare("2") == 0){
			mgJiSu.kosdak.precurrate = mgJiSu.kosdak.currate;
			mgJiSu.kosdak.currate = cs;
		}

		//����
		i++;

		//��������
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("-����(%d.%02d)", val/100, val%100);
		}
		else{
			cs.Format("-����(-%d.%02d)", (val/100 * -1), (val%100 * -1));
		}
		t1511info.csopendiff = cs;
		csjisju += cs;
		i++;

		if(val >= 0){
			cs.Format("%d.%02d", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d", (val/100 * -1), (val%100 * -1));
		}
		if(csmode.Compare("1") == 0){
			mgJiSu.kospi.openrate = cs;
		}
		else if(csmode.Compare("2") == 0){
			mgJiSu.kosdak.openrate = cs;
		}

		//��������
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format(":����(%d.%02d::", val/100, val%100);
		}
		else{
			cs.Format(":����(-%d.%02d::", (val/100 * -1), (val%100 * -1));
		}
		t1511info.cshighdiff = cs;
		csjisju += cs;
		i++;

		if(val >= 0){
			cs.Format("%d.%02d", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d", (val/100 * -1), (val%100 * -1));
		}
		if(csmode.Compare("1") == 0){
			mgJiSu.kospi.hirate = cs;
		}
		else if(csmode.Compare("2") == 0){
			mgJiSu.kosdak.hirate = cs;
		}

		//�����ð�
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s)", temp);
		t1511info.cshightime = cs;
		csjisju += cs;
		i++;


		//��������
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format(":����(%d.%02d::", val/100, val%100);
		}
		else{
			cs.Format(":����(-%d.%02d::", (val/100 * -1), (val%100 * -1));
		}
		t1511info.cslowdiff = cs;
		csjisju += cs;
		i++;

		if(val >= 0){
			cs.Format("%d.%02d", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d", (val/100 * -1), (val%100 * -1));
		}
		if(csmode.Compare("1") == 0){
			mgJiSu.kospi.lowrate = cs;
		}
		else if(csmode.Compare("2") == 0){
			mgJiSu.kosdak.lowrate = cs;
		}

		//�����ð�
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s)", temp);
		t1511info.cslowtime = cs;
		csjisju += cs;
		i++;

		csjisju += "\n\t";

		//1 hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("(%s:", temp);
		t1511info.csfirstjname = cs;
		csjisju += cs;
		i++;


		//diffjisu
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("%d.%02d)", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d)", (val/100 * -1), (val%100 * -1));
		}
		t1511info.csfirdiff = cs;
		csjisju += cs;
		i++;

		//2 hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("(%s:", temp);	
		t1511info.cssecondjname = cs;
		csjisju += cs;
		i++;


		//diffjisu
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("%d.%02d)", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d)", (val/100 * -1), (val%100 * -1));
		}
		t1511info.cssecdiff = cs;
		csjisju += cs;
		i++;


		//3 hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("(%s:", temp);	
		t1511info.csthirdjname = cs;
		csjisju += cs;
		i++;


		//diffjisu
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("%d.%02d)", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d)", (val/100 * -1), (val%100 * -1));
		}
		t1511info.csthrdiff = cs;
		csjisju += cs;
		i++;


		//4 hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("(%s:", temp);	
		t1511info.csfourthjname = cs;
		csjisju += cs;
		i++;


		//diffjisu
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("%d.%02d)", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d)", (val/100 * -1), (val%100 * -1));
		}
		t1511info.csfordiff = cs;
		csjisju += cs;
		i++;


		//���
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("-(%d:", atoi(temp));
		t1511info.cshighjo = cs;
		csjisju += cs;
		i++;

		//����
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d:",  atoi(temp));
		t1511info.csupjo = cs;
		csjisju += cs;
		i++;


		//�϶�
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d:", atoi(temp));
		t1511info.cslowjo = cs;
		csjisju += cs;
		i++;

		//����
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d)", atoi(temp));	
		t1511info.csdownjo = cs;
		csjisju += cs;
		i++;

		csjisju += "\n";

	}

	mysql_free_result(myRes);
	mysql_commit(myData);
	return csjisju;
}

CString CMySQLCMD::getMMinfo(CString csmode)
{
	int i = 0, j = 0, rc, val = 0;
	double dval = 0;
	CString cs, csjisju, csval, csQuery, csMsg, csCurDate, cstime;
	unsigned long *lengths, len = 0;
	char temp[100];

	COleDateTime t;
	COleDateTimeSpan ts;

	if(myData->reconnect != 1)
		return cs;

	ts.SetDateTimeSpan(0, 0, 2, 0);

	t.operator -=(ts);

	t = COleDateTime::GetCurrentTime();
	cstime.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	cstime.Format(_T("%d-%02d-%02d 09:00:00"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	if(myData->reconnect != 1)
		return cs;

	csQuery.Format("select * from  1602_tbl where cur_date >= '%s' and upmode = '%s' order by cstime desc limit 1", cstime, csmode);
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);

		i = 1;

		//upmode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);	
		if(cs.Compare("001") == 0)
			cs = t1602info.csupmode = "�ڽ���:";
		else
			cs = t1602info.csupmode = "�ڽ���:";

		csjisju += cs;
		i++;

		//cstime
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s:", temp);
		t1602info.cscstime = cs;
		csjisju += cs;
		i++;


		//sv_00
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("���:%d,", atoi(temp));
		t1602info.cssv_00 = cs;
		csjisju += cs;
		i++;

		//sv_01
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("����:%d,", atoi(temp));
		t1602info.cssv_01 = cs;
		csjisju += cs;
		i++;

		//sv_02
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("����:%d,", atoi(temp));
		t1602info.cssv_02 = cs;
		csjisju += cs;
		i++;

		//sv_03
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("����:%d,", atoi(temp));
		t1602info.cssv_03 = cs;
		csjisju += cs;
		i++;

		//sv_04
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("����:%d,", atoi(temp));
		t1602info.cssv_04 = cs;
		csjisju += cs;
		i++;

		//sv_05
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("����:%d,", atoi(temp));
		t1602info.cssv_05 = cs;
		csjisju += cs;
		i++;

		//sv_06
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("���:%d,", atoi(temp));
		t1602info.cssv_06 = cs;
		csjisju += cs;
		i++;
		//csjisju += "\n\t";

		//sv_07
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("��Ÿ:%d,", atoi(temp));
		t1602info.cssv_07 = cs;
		//csjisju += cs;
		i++;

		//sv_08
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("����:%d,", atoi(temp));
		t1602info.cssv_08 = cs;
		csjisju += cs;
		i++;

		//sv_11
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("����:%d,", atoi(temp));	
		t1602info.cssv_11 = cs;
		csjisju += cs;
		i++;

		//sv_17
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("�ܱ���:%d,", atoi(temp));	
		t1602info.cssv_17 = cs;
		csjisju += cs;
		i++;

		//sv_18
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("���:%d", atoi(temp));	
		t1602info.cssv_18 = cs;
		csjisju += cs;
		i++;

		csjisju += "\n";

	}

	mysql_free_result(myRes);
	mysql_commit(myData);
	return csjisju;
}


CString CMySQLCMD::getJisuList(CString csmode)
{
	int index = 0, i = 0, j = 0, rc, val = 0;
	double dval = 0;
	CString cs, csjisju, csval, csQuery, csMsg, csCurDate, cstime, cstemp;
	unsigned long *lengths, len = 0;
	char temp[100];

	COleDateTime t;
	COleDateTimeSpan ts;

	if(myData->reconnect != 1)
		return cs;

	ts.SetDateTimeSpan(0, 0, 2, 0);

	t.operator -=(ts);

	t = COleDateTime::GetCurrentTime();
	cstime.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	cstime.Format(_T("%d-%02d-%02d 09:00:00"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	if(myData->reconnect != 1)
		return cs;

	if(atoi(csmode) > 2){
		cstemp.Format("%d", atoi(csmode) - 2);
		csQuery.Format("select * from  1511_tbl where  gubun = '%s' group by indexdate order by cur_date desc", cstemp);
	}
	else{
		csQuery.Format("select * from  1511_tbl where cur_date >= '%s' and gubun = '%s' order by cur_date desc", cstime, csmode);
	}
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	t1511list.itemnumer = 0;
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);

		i = 1;

		//hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		t1511list.item1511[index].cshname = cs;
		i++;

		//cur_date
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", (temp));
		t1511list.item1511[index].cscur_date = cs;
		i++;

		//��������
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", CommaFormat(temp));
		t1511list.item1511[index].cspricejisu = cs;
		i++;


		//diffjisu
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("%d.%02d", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d", (val/100 * -1), (val%100 * -1));
		}
		t1511list.item1511[index].csdiffjisu = cs;
		i++;


		//����
		i++;

		//��������
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("%d.%02d", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d", (val/100 * -1), (val%100 * -1));
		}
		t1511list.item1511[index].csopendiff = cs;
		i++;


		//��������
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("%d.%02d", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d", (val/100 * -1), (val%100 * -1));
		}
		t1511list.item1511[index].cshighdiff = cs;
		csjisju += cs;
		i++;


		//�����ð�
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		t1511list.item1511[index].cshightime = cs;
		i++;


		//��������
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("%d.%02d", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d", (val/100 * -1), (val%100 * -1));
		}
		t1511list.item1511[index].cslowdiff = cs;
		i++;


		//�����ð�
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		t1511list.item1511[index].cslowtime = cs;
		i++;

		//1 hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		t1511list.item1511[index].csfirstjname = cs;
		i++;


		//diffjisu
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("%d.%02d", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d", (val/100 * -1), (val%100 * -1));
		}
		t1511list.item1511[index].csfirdiff = cs;
		i++;

		//2 hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);	
		t1511list.item1511[index].cssecondjname = cs;
		i++;


		//diffjisu
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("%d.%02d", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d", (val/100 * -1), (val%100 * -1));
		}
		t1511list.item1511[index].cssecdiff = cs;
		i++;


		//3 hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);	
		t1511list.item1511[index].csthirdjname = cs;
		i++;


		//diffjisu
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("%d.%02d", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d", (val/100 * -1), (val%100 * -1));
		}
		t1511list.item1511[index].csthrdiff = cs;
		csjisju += cs;
		i++;


		//4 hname
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);	
		t1511list.item1511[index].csfourthjname = cs;
		i++;


		//diffjisu
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atoi(temp);
		if(val >= 0){
			cs.Format("%d.%02d", val/100, val%100);
		}
		else{
			cs.Format("-%d.%02d", (val/100 * -1), (val%100 * -1));
		}
		t1511list.item1511[index].csfordiff = cs;
		i++;


		//���
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));
		t1511list.item1511[index].cshighjo = cs;
		i++;

		//����
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d",  atoi(temp));
		t1511list.item1511[index].csupjo = cs;
		i++;


		//�϶�
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));
		t1511list.item1511[index].cslowjo = cs;
		i++;

		//����
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));	
		t1511list.item1511[index].csdownjo = cs;
		i++;

		index++;
	}
	t1511list.itemnumer = index;
	mysql_free_result(myRes);
	mysql_commit(myData);
	return csjisju;
}

CString CMySQLCMD::getMMListinfo(CString csmode)
{
	int index = 0, i = 0, j = 0, rc, val = 0;
	double dval = 0;
	CString cs, csjisju, csval, csQuery, csMsg, csCurDate, cstime;
	unsigned long *lengths, len = 0;
	char temp[100];

	COleDateTime t;
	COleDateTimeSpan ts;

	if(myData->reconnect != 1)
		return cs;

	ts.SetDateTimeSpan(0, 0, 2, 0);

	t.operator -=(ts);

	t = COleDateTime::GetCurrentTime();
	cstime.Format(_T("%d-%02d-%02d %02d:%02d:%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());
	cstime.Format(_T("%d-%02d-%02d 09:00:00"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	if(myData->reconnect != 1)
		return cs;

	if(atoi(csmode) > 301){
		csmode.Format("%03d", atoi(csmode) - 301);
		csQuery.Format("select * from  1602_tbl where upmode = '%s' and cstime >= 15300000 group by indexdate order by indexdate desc", csmode);
	}
	else{
		csQuery.Format("select * from  1602_tbl where cur_date >= '%s' and upmode = '%s' order by cstime desc", cstime, csmode);
	}

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	t1602list.itemnumer = 0;
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);

		i = 0;
		
		//cscur_date
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);	
		t1602list.item1602[index].cscur_date = cs;
		i++;

		//upmode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);	
		t1602list.item1602[index].csupmode = cs;
		i++;

		//cstime
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		t1602list.item1602[index].cscstime = cs;
		i++;


		//sv_00
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));
		t1602list.item1602[index].cssv_00 = cs;
		i++;

		//sv_01
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));
		t1602list.item1602[index].cssv_01 = cs;
		i++;

		//sv_02
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));
		t1602list.item1602[index].cssv_02 = cs;
		i++;
		//sv_03
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));
		t1602list.item1602[index].cssv_03 = cs;
		i++;

		//sv_04
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));
		t1602list.item1602[index].cssv_04 = cs;
		i++;

		//sv_05
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));
		t1602list.item1602[index].cssv_05 = cs;
		i++;

		//sv_06
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));
		t1602list.item1602[index].cssv_06 = cs;
		i++;

		//sv_07
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));
		t1602list.item1602[index].cssv_07 = cs;
		i++;

		//sv_08
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));
		t1602list.item1602[index].cssv_08 = cs;
		i++;

		//sv_11
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));	
		t1602list.item1602[index].cssv_11 = cs;
		i++;

		//sv_17
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));	
		t1602list.item1602[index].cssv_17 = cs;
		i++;

		//sv_18
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%d", atoi(temp));	
		t1602list.item1602[index].cssv_18 = cs;
		i++;
		index++;

	}
	t1602list.itemnumer = index;
	mysql_free_result(myRes);
	mysql_commit(myData);
	return csjisju;
}


int CMySQLCMD::check_jango_date(CString shcode)
{
	int index = 0, i = 0, j = 0, rc, val = 0;
	double dval = 0;
	CString cs, csjisju, csval, csQuery, csMsg, csCurDate, cstime;
	unsigned long *lengths, len = 0;
	char temp[100];

	COleDateTime t;
	COleDateTimeSpan ts;

	if(myData->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();

	int dayindex = t.GetDayOfWeek();

	if(dayindex >= 4){

		ts.SetDateTimeSpan(13, 0, 0, 0);
	}
	else{
		ts.SetDateTimeSpan(15, 0, 0, 0);
	}
	t.operator -=(ts);

	cstime.Format(_T("%d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay());


	if(myData->reconnect != 1)
		return 0;

	csQuery.Format("select cur_date from  etrade_db.date_jango_tbl where shcode = '%s' and codemode = 0 order by cur_date desc limit 1", shcode);


	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}


	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);

		i = 0;		

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);	
		if(strcmp(cs, cstime) <= 0) 
		  index++;

	}

	mysql_free_result(myRes);
	mysql_commit(myData);
	return index;
}

int nPreHour;
int CMySQLCMD::jangoUpdate(CString cscode, CString cshanme, CString csjanqty, CString cspamt, CString csprice, CString csrate)
{

	int i = 0, rc;
	CString csCurDate, csQuery, csMsg;
	int rev = 0, price = 0, preprice = 0;

	__int64 msval = 0, mdval = 0, val = 0, valcnt = 0;

	COleDateTime t = COleDateTime::GetCurrentTime();

	if(myData->reconnect != 1)
		return 0;


	csCurDate.Format(_T("%d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay());


	csQuery.Format("replace INTO etrade_db.date_jango_tbl(codemode, shcode, hname, cur_date, janqty,  pamt, price, rate) VALUES ('%s', '%s','%s', '%s','%s', '%s', '%s', '%s')", 					
					 gcsLogIndex, cscode, cshanme, csCurDate, csjanqty,  cspamt, csprice, csrate);	

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	mysql_commit(myData);
	return 1;
}


int CMySQLCMD::dailyjangoUpdate(CString tot, CString mstot, CString curprofit, CString frofit)
{

	int i = 0, rc;
	CString csCurDate, csQuery, csMsg, csrate;
	int rev = 0, price = 0, preprice = 0;

	__int64 msval = 0, mdval = 0, val = 0, valcnt = 0;

	COleDateTime t = COleDateTime::GetCurrentTime();

	if(myData->reconnect != 1)
		return 0;

	csCurDate.Format(_T("%02d%02d"), t.GetHour(), t.GetMinute());


	csCurDate.Format(_T("%d%02d%02d_%02d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute(), t.GetSecond());

	if(nPreHour == t.GetHour())
		return 0;

	nPreHour = t.GetHour();

	if (atof(mstot) > 0)
		csrate.Format("%.02f", (atof(tot) * 100) / atof(mstot));
	else
		csrate = "0.00";

	csQuery.Format("REPLACE INTO etrade_db.daily_jango_%s_tbl(cur_date, total_assets,  purchase_amount,  valuation_gain, realized_profit, rate, rev ) VALUES ('%s', '%s','%s', '%s','%s', '%s','%s')", 					
					 gcsLogIndex, csCurDate, CommaFormat(tot), CommaFormat(mstot), CommaFormat(curprofit), CommaFormat(frofit), csrate, "");

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	mysql_commit(myData);
	return 1;
}

CString CMySQLCMD::getJonggb(CString shcode)
{

	int i = 0, rc;
	CString cs, csQuery, csMsg;
	CString csCurDate;
	COleDateTime t;
	COleDateTimeSpan ts;
	unsigned long *lengths, len = 0;
	char temp[100];

	if(myData->reconnect != 1)
		return cs;

	t = COleDateTime::GetCurrentTime();
	csCurDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());
	csQuery.Format("SELECT janginfo FROM  etrade_db.jminfo_tbl  where cur_date = '%s' and  shcode = '%s'", csCurDate, shcode);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
	}
	mysql_free_result(myRes);
	mysql_commit(myData);
	return cs;

}


CString CMySQLCMD::getTMavg(CString shcode)
{

	int i = 0, index = 0, rc;
	CString cs, csQuery, csMsg;
	CString csCurDate;
	COleDateTime t;
	COleDateTimeSpan ts;
	unsigned long *lengths, len = 0;
	char temp[100];

	if(myData->reconnect != 1)
		return cs;

	t = COleDateTime::GetCurrentTime();
	csCurDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());	
	csQuery.Format("SELECT avgdiff FROM  etrade_db.t1532_tbl  where cur_date >= '%s' and  shcode = '%s' order by avgdiff desc limit 1", csCurDate, shcode);
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		//shcode
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		index++;	

	}
	mysql_free_result(myRes);
	mysql_commit(myData);
	return cs;

}

CString CMySQLCMD::checkUPcode(CString cscode)
{
	int i = 0, j = 0, rc, val = 0;
	double dval = 0;
	CString cs,  csQuery, csMsg, cstime;
	unsigned long *lengths, len = 0;
	char temp[100];

	COleDateTime t;
	COleDateTimeSpan ts;

	if(myData->reconnect != 1)
		return cs;

	t = COleDateTime::GetCurrentTime();
	cstime.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	if(myData->reconnect != 1)
		return cs;

	csQuery.Format("select jdiff from  t1516_tbl where cur_date >= '%s' and shcode = '%s' order by jdiff desc limit 1 ", cstime, cscode);
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	while(myRow = mysql_fetch_row(myRes)){
		lengths = mysql_fetch_lengths(myRes);
		 i = 0;
		//jdiff
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);	
	}

	mysql_free_result(myRes);
	mysql_commit(myData);
	return cs;
}

long int CMySQLCMD::getValuefromT1702(CString shcode)
{

	int i = 0, index = 0, rc;
	CString csQuery, csMsg;
	CString csCurDate, csmsdate, cs, cscurT, csedT, csmsmode, csshcode, cshname, csprice, csdiff;
	COleDateTime t;
	COleDateTimeSpan ts;
	unsigned long *lengths, len = 0;
	char temp[32];
	long int val = 0, sumval = 0;
	int rateval[5] = {5, 4, 3, 2, 1};

	if(myData->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();
	cscurT.Format(_T("%d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	csQuery.Format("SELECT forg, org FROM  etrade_db.1702list_tbl  where shcode = '%s' order by cur_date desc limit 5", shcode);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	index = 0;

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		////per
		//memset(temp, 0, sizeof(temp));
		//memcpy(temp, myRow[i], lengths[i]);
		//val = atol(temp);
		//i++;

		//sumval += (val * rateval[index]);

		//forg
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atol(temp);
		i++;

		sumval += (val * rateval[index]);		


		//org
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atol(temp);
		i++;

		sumval += (val * rateval[index]);


		index++;
	}

	mysql_free_result(myRes);
	mysql_commit(myData);

	return sumval;

}

long int CMySQLCMD::getValuetypefromT1702(CString shcode, CString csjuche)
{

	int i = 0, index = 0, rc;
	CString csQuery, csMsg;
	CString csCurDate, csmsdate, cs, cscurT, csedT, csmsmode, csshcode, cshname, csprice, csdiff;
	COleDateTime t;
	COleDateTimeSpan ts;
	unsigned long *lengths, len = 0;
	char temp[32];
	long int val = 0, sumval = 0;
	int rateval[5] = {5, 4, 3, 2, 1};

	if(myData->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();
	cscurT.Format(_T("%d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	csQuery.Format("SELECT %s FROM  etrade_db.1702list_tbl  where shcode = '%s' order by cur_date desc limit 5", csjuche, shcode);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	index = 0;

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		////per
		//memset(temp, 0, sizeof(temp));
		//memcpy(temp, myRow[i], lengths[i]);
		//val = atol(temp);
		//i++;

		//sumval += (val * rateval[index]);

		//forg
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		val = atol(temp);
		i++;

		sumval += (val * rateval[index]);		

		index++;
	}

	mysql_free_result(myRes);
	mysql_commit(myData);

	return sumval;

}

CString CMySQLCMD::chPMstatus(CString shcode, CString &csvalue)
{

	int i = 0, index = 0, rc;
	CString cs,  csQuery, csMsg;
	CString csCurDate;
	COleDateTime t;
	COleDateTimeSpan ts;

	unsigned long *lengths, len = 0;

	char temp[100];

	if(myData->reconnect != 1)
		return cs;

	t = COleDateTime::GetCurrentTime();

	csCurDate.Format(_T("%d%02d%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	csQuery.Format("select avg(svalue), svalue FROM etrade_db.t1637_tbl  where cur_date >= '%s' and shcode = '%s'", csCurDate, shcode);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%.0f", atof(temp));
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csvalue.Format("%ld", atol(temp));			
		
	}
	mysql_free_result(myRes);
	mysql_commit(myData);
	return cs;

}

CString CMySQLCMD::getchlevel(CString shcode, CString &csdiff)
{

	int i = 0, index = 0, rc;
	CString cs,  csQuery, csMsg;
	CString csCurDate;
	COleDateTime t;
	COleDateTimeSpan ts;

	unsigned long *lengths, len = 0;

	char temp[100];

	if(myData->reconnect != 1)
		return cs;

	t = COleDateTime::GetCurrentTime();

	csCurDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	csQuery.Format("SELECT vol, diff FROM  etrade_db.jminfo_tbl  where cur_date >= '%s' and shcode = '%s'", csCurDate, shcode);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}

	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		cs.Format("%s", temp);
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csdiff.Format("%s", temp);	
		
		
	}
	mysql_free_result(myRes);
	mysql_commit(myData);
	return cs;

}

CString CMySQLCMD::getmarkettype(CString cs, int mode)
{
	int i = 0, rc;
	CString csrv, csQuery, csMsg;
	unsigned long *lengths, len = 0;
	char temp[100];

	if(myData->reconnect != 1)
		return csrv;

	if(mode){
		csQuery.Format("select mode from  stocklist_tbl where hname = '%s'", cs);
	}
	else{
		csQuery.Format("select mode from  stocklist_tbl where shcode = '%s'", cs);
	}
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return csrv;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return csrv;
	}

	DBList.number = 0;
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		if(lengths[0]){
			i = 0;
			memset(temp, 0, sizeof(temp));
			memcpy(temp, myRow[i], lengths[i]);
			csrv.Format("%s", temp);
		}
	}
	mysql_free_result(myRes);
	mysql_commit(myData);
	return csrv;
}

CString CMySQLCMD::getMatchID(CString cs, int mode)
{
	int i = 0, rc;
	CString csrv, csQuery, csMsg;
	unsigned long *lengths, len = 0;
	char temp[100];

	if(myData->reconnect != 1)
		return csrv;

	if(mode){
		csQuery.Format("select shcode, hname  from  stocklist_tbl where hname = '%s'", cs);
	}
	else{
		csQuery.Format("select shcode, hname  from  stocklist_tbl where shcode = '%s'", cs);
	}
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return csrv;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return csrv;
	}

	DBList.number = 0;
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		if(lengths[0]){
			i = 0;
			if(mode){
				memset(temp, 0, sizeof(temp));
				memcpy(temp, myRow[i], lengths[i]);
				csrv.Format("%s", temp);
			}
			else{
				i++;
				memset(temp, 0, sizeof(temp));
				memcpy(temp, myRow[i], lengths[i]);
				csrv.Format("%s", temp);
			}

		}
	}
	mysql_free_result(myRes);
	mysql_commit(myData);
	return csrv;
}



int CMySQLCMD::getinfomsg(CString csshcode, CString &csinfo)
{

	int i = 0, rc;
	CString csQuery, csMsg;
	CString csCurDate;
	COleDateTime t;
	char temp[100];
	unsigned long *lengths;

	if(myData->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();
	csCurDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	csQuery.Format("select infomsg from  etrade_db.jminfo_tbl where shcode = '%s' and cur_date = '%s'", csshcode, csCurDate);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	rc = 0;

	while(myRow = mysql_fetch_row(myRes)){
		lengths = mysql_fetch_lengths(myRes);
		i = 0;
		//price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csinfo.Format("%s", temp);
		rc++;

	}
	mysql_free_result(myRes);
	mysql_commit(myData);
	return rc;

}

int CMySQLCMD::getcurprice(CString csshcode, CString &csprice, CString &csdiff)
{

	int i = 0, rc;
	CString csQuery, csMsg;
	CString csCurDate;
	COleDateTime t;
	char temp[16];
	unsigned long *lengths;

	if(myData->reconnect != 1)
		return 0;

	t = COleDateTime::GetCurrentTime();
	csCurDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	csQuery.Format("select price, diff, jnilvolume from  etrade_db.jminfo_tbl where shcode = '%s' and cur_date = '%s'", csshcode, csCurDate);

	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){

		csMsg.Format(_T("%s query : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}

	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return 0;
	}
	rc = 0;

	while(myRow = mysql_fetch_row(myRes)){
		lengths = mysql_fetch_lengths(myRes);
		i = 0;
		//price
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csprice.Format("%d", atoi(temp));
		i++;

		//diff
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		csdiff.Format("%0.2f", atof(temp));
		i++;

		//jnilvol
		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		rc = atoi(temp);
		i++;
	}
	mysql_free_result(myRes);
	mysql_commit(myData);
	return rc;

}

CString CMySQLCMD::searchingjm(CString csprice, CString csrate)
{

	int i = 0, index = 0, rc;
	CString cs,  csQuery, csMsg;
	CString csCurDate;
	COleDateTime t;
	COleDateTimeSpan ts;

	unsigned long *lengths, len = 0;

	char temp[100];

	if(myData->reconnect != 1)
		return cs;
	CString csprice1, csprice2, csrate1, csrate2;

	csprice1.Format("%d", int(atoi(csprice)*0.9));
	csprice2.Format("%d", int(atoi(csprice)*1.1));
	csrate1.Format("%0.2f", (atof(csrate) - 0.9));
	csrate2.Format("%0.2f", (atof(csrate) + 0.9));


	t = COleDateTime::GetCurrentTime();
	csCurDate.Format(_T("%d-%02d-%02d"), t.GetYear(), t.GetMonth(), t.GetDay());

	if(atof(csrate) >= 0){
		csQuery.Format("SELECT shcode, hname, price, diff, volume, jnilvolume  FROM   etrade_db.jminfo_tbl  where cur_date = '%s' and cast(price as signed) >= '%s'and cast(price as signed) <= '%s' and cast(diff as signed) >= '%s' and cast(diff as signed) <= '%s' order by price ", csCurDate, csprice1, csprice2, csrate1, csrate2);
	}
	else{
		csQuery.Format("SELECT shcode, hname, price, diff, volume, jnilvolume  FROM   etrade_db.jminfo_tbl  where cur_date = '%s' and cast(price as signed) >= '%s'and cast(price as signed) <= '%s' and cast(diff as signed) <= '%s' and cast(diff as signed) >= '%s' order by price ", csCurDate, csprice1, csprice2, csrate1, csrate2);

	}
	mysql_set_character_set(myData, "euckr");

	rc = mysql_query(myData, csQuery);
	if(rc){
		csMsg.Format(_T("%s query : %s\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}
	myRes = mysql_store_result(myData);
	if(myRes == NULL){
		csMsg.Format(_T("%s use_result : %s.\n"), __FUNCTION__, mysql_error(myData));
		OutputDebugString(csMsg);
		return cs;
	}
	listitme.itemnumer = 0;
	index = 0;
	while(myRow = mysql_fetch_row(myRes)){

		lengths = mysql_fetch_lengths(myRes);
		i = 0;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		listitme.item1511[index].csshcode.Format("%s", temp);
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		listitme.item1511[index].cshname.Format("%s", temp);
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		listitme.item1511[index].cspricejisu.Format("%d", atoi(temp));
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		listitme.item1511[index].csdiffjisu.Format("%s", temp);
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		listitme.item1511[index].csopendiff.Format("%d", atoi(temp));
		i++;

		memset(temp, 0, sizeof(temp));
		memcpy(temp, myRow[i], lengths[i]);
		listitme.item1511[index].cshighdiff.Format("%d", atoi(temp));
		i++;

		//memset(temp, 0, sizeof(temp));
		//memcpy(temp, myRow[i], lengths[i]);
		//listitme.item1511[index].csvol.Format("%d", atoi(temp));
		//i++;

		//memset(temp, 0, sizeof(temp));
		//memcpy(temp, myRow[i], lengths[i]);
		//listitme.item1511[index].csrevol.Format("%d", atoi(temp));
		//i++;

		index++;

	}
	listitme.itemnumer = index;
	mysql_free_result(myRes);
	mysql_commit(myData);
	return cs;

}




int CMySQLCMD::compreTime(CString sctime , CString tgtime , int gaptime)
{
	CString cs, cstmp1, cstmp2;
	int t1 = 0, t2 = 0;

	cstmp1 = sctime.Left(2);
	cstmp2 = sctime.Mid(2, 2);

	t1 = atoi(cstmp1) * 60 + atoi(cstmp2);

	cstmp1 = tgtime.Left(2);
	cstmp2 = tgtime.Mid(2, 2);

	t2 = atoi(cstmp1) * 60 + atoi(cstmp2);

	if( t1 > (t2 + gaptime))
		return 1;
	return 0;
}

CString CMySQLCMD::CommaFormat(CString val)
{
	CString csval;

	char szBuffer[64];
	CString strFileSize;

	NUMBERFMT fmt = {0, 0, 3, _T("."), _T(","), 1};
	::GetNumberFormat(NULL, NULL, val, &fmt, szBuffer, sizeof(szBuffer));

	csval.Format("%s", szBuffer);

	return csval;
}