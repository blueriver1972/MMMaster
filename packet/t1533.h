#ifndef _t1533_H_
#define _t1533_H_

///////////////////////////////////////////////////////////////////////////////////////////////////
// 종목별테마(t1533) ( attr,block,headtype=A )
#pragma pack( push, 1 )

#define NAME_t1533     "t1533"

// 기본입력
typedef struct _t1533InBlock
{
    char    gubun              [   1];    char    _gubun              ;    // [string,    6] 종목코드                       StartPos 0, Length 6
    char    chgdate            [   2];    char    _chgdate              ;    // [string,    6] 종목코드                       StartPos 0, Length 6

} t1533InBlock, *LPt1533InBlock;
#define NAME_t1533InBlock     "t1533InBlock"

// 출력, occurs
typedef struct _t1533OutBlock
{
    char    tmname             [  36];    char    _tmname            ;    // [string,   36] 테마명                         StartPos 0, Length 36
    char    totcnt             [   4];    char    _totcnt            ;    // [float ,  6.2] 평균등락율                     StartPos 37, Length 6
    char    upcnt              [   4];    char    _upcnt             ;    // [string,    4] 테마코드                       StartPos 44, Length 4
    char    dncnt              [  4];    char     _dncnt             ;    // [string,   36] 테마명                         StartPos 0, Length 36
    char    uprate             [   6];    char    _uprate            ;    // [float ,  6.2] 평균등락율                     StartPos 37, Length 6
    char    diff_vol           [   10];    char   _diff_vol          ;    // [string,    4] 테마코드                       StartPos 44, Length 4
 	char    avgdiff            [  6];    char     _avgdiff           ;    // [string,   6.2] 테마명                         StartPos 0, Length 36
    char    chgdiff            [   6];    char    _chgdiff           ;    // [float ,  6.2] 평균등락율                     StartPos 37, Length 6
    char    tmcode             [   4];    char    _tmcode            ;    // [string,    4] 테마코드                       StartPos 44, Length 4
} t1533OutBlock, *LPt1533OutBlock;
#define NAME_t1533OutBlock     "t1533OutBlock"

#pragma pack( pop )
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _t1533_H_
