#ifndef __SMARTSOCK_H__
#define __SMARTSOCK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <WinSock2.h>

#define MAX_LEN	256
#define PORT		9000
#define IP		"127.0.0.1"

#define MAX_USER		3
#define NIC_NAME_SIZE	9
#define NIC_NAME_MSG	(9 + 2)
#define ROOM_NAME_SIZE	10

#define BUF_SIZE	255
#define MSG_SIZE	(BUF_SIZE + 1 + NIC_NAME_MSG)

#define MSG_END		"\x01\x02\x03"
#define MSG_START	"\x04"
#define MSG_AGAIN	"\x05"
#define MSG_GET_ROOM	"HELP"

typedef struct _mem {
	int iFd;
	char cName[NIC_NAME_SIZE];
	struct _mem * stPrev;
	struct _mem * stNext;
} Member;

typedef struct _room {
	char cName[ROOM_NAME_SIZE];
	unsigned int uiCnt;
	struct _mem * stMem;
	struct _room * stPrev;
	struct _room * stNext;
} Room;


typedef union _tag{
	char cNum[4];
	UINT iNum;
} unSize;

extern Room * gb_stpRdRoomHead;
extern Member * gb_stpRdMemTail;
extern Room * gb_stpRoomHead;

#include "Server_Mem.h"
#include "Msg_Func.h"

#endif /* __SMARTSOCK_H__ */
