/*
 ***************************************************************************
 * MediaTeK Inc.
 * 4F, No. 2 Technology	5th	Rd.
 * Science-based Industrial	Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 2002-2012, MTK.
 *
 * All rights reserved.	MediaTeK's source	code is	an unpublished work	and	the
 * use of a	copyright notice does not imply	otherwise. This	source code
 * contains	confidential trade secret material of MediaTeK Tech. Any attemp
 * or participation	in deciphering,	decoding, reverse engineering or in	any
 * way altering	the	source code	is stricitly prohibited, unless	the	prior
 * written consent of MediaTeK, Inc. is obtained.
 ***************************************************************************

    Module Name:
   stb_test.h

    Abstract:

    Revision History:
    Who         When            What
    --------    ----------      ----------------------------------------------
    Name        Date            Modification logs
*/

#ifndef __STB_TEST
#define __STB_TEST

#define STB_TEST_PIDFILE "/var/run/stb_test.pid"
#define STB_TEST_RESULT "/var/run/stb_test_result"
#define STBTEST_ENTRY "StbTest_Entry"
#define STBTEST_DEBUG_LEVEL "DebugLevel"
#define STBTEST_WORK_STATUS "WorkStatus"
#define STBTEST_TX_COUNT "txCount"
#define STBTEST_RX_COUNT "rxCount"
#define STBTEST_DST_MAC "DstMAC"
#define STBTEST_LOST_RATE "LostRate"
#define STBTEST_RESPONSE_DELAY "ResponseDelay"
#define STBTEST_RESPONSE_SHAKE "ResponseShake"
#define STBTEST_RX_COUNT2 "rxCount2"
#define STBTEST_DST_MAC2 "DstMAC2"
#define STBTEST_LOST_RATE2 "LostRate2"
#define STBTEST_RESPONSE_DELAY2 "ResponseDelay2"
#define STBTEST_RESPONSE_SHAKE2 "ResponseShake2"
#define LAN_IF 		"br0"
//#define LAN_IF 		"eth2"
#define STB_BR_FDB_PATH "/proc/br_fdb_host/stb_list"

#define STBTEST_MAX_LEN		60
#define STBTEST_MAX_TXCOUNT   		24000
#define RESPONSE_TIMEOUT   		4000
#define REPEAT_DATA_LEN		8
#define STBTEST_LOAD_LEN	46
#define ETH_P_STBTEST_S			0x980A
#define ETH_P_STBTEST_R			0x980B
#define STBTEST_OPCODE_REQUEST 		0x08
#define STBTEST_OPCODE_REPLY   		0x00

typedef struct stbTestCfg_s{
	u_char onuMacAddr[6];
	u_char dstMacAddr[6];
	int send_period;
	int freqency;
	u_char state[16];
//	u_char dbgLvl;
}stbTestCfg_t, *stbTestCfg_p;

typedef struct {
    unsigned char  tmac[ETH_ALEN];
    unsigned char  smac[ETH_ALEN];
    unsigned short type;
    unsigned short 	len;
    unsigned char  	reserve[3];
    unsigned char	opcode;
    unsigned short 	seqnum;
	unsigned int 	datetime;
    unsigned char  	content[34];
} __attribute__((packed)) stbtest_packet_t ;

/* debug level define */
#define STBTEST_DEBUG_LEVEL_NONE		0
#define STBTEST_DEBUG_LEVEL_ERROR		1
#define STBTEST_DEBUG_LEVEL_DEBUG		2
#define STBTEST_DEBUG_LEVEL_INFO		3
#define STBTEST_DEBUG_LEVEL_TRACE		4

#if 1  //#ifdef CONFIG_DEBUG
//#define stbTestDbg(L, fmt, args...) {if (stbTestCfg.dbgLvl>=L) tcdbg_printf("%s [%04d]: "fmt, __FILE__, __LINE__, ##args); } 
//#define stbTestDbg(L, fmt, args...) {if (stbTestCfg.dbgLvl>=L) tcdbg_printf(fmt, ##args); } 
//#define stbTestLlidDbg(L, llid, fmt, args...) {if ((stbTestCfg.dbgLvl >= L) && (stbTestCfg.dbgLlidMask & (1<<llid))) tcdbg_printf("%s [%04d]: "fmt, __FILE__, __LINE__, ##args); }
#else
#define stbTestLlidDbg(L, llid, fmt, args...) {}
#define stbTestDbg(L, fmt, args...) {}
#endif

#endif//end of __STB_TEST


