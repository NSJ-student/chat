#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib");

#define MAX_LEN	256
#define PORT		9000
#define IP		"127.0.0.1"

typedef union _tag{
	char cNum[4];
	UINT iNum;
} unSize;

DWORD WINAPI Recv_Chatting(void * data)
{
	SOCKET sockfd = (SOCKET)data;
	unSize unReadN;
	int iNum;
	char cRxBuf[MAX_LEN];

	printf("-- Thread Start\n");
	
	while (1)
	{
		iNum = recv(sockfd, unReadN.cNum, 4, 0);
		if(iNum <= 0)
		{
			printf("-- Thread End by error\n");
			break;
		}
		iNum = recv(sockfd, (char *)cRxBuf, unReadN.iNum, 0);
		if( 0 == strncmp("quit", cRxBuf, 4))
		{
			break;
		}
		printf("server: %s\n", cRxBuf);
	}

	closesocket(sockfd);
	printf("-- Thread End\n");
	return 0;
}

int main(void)
{
	WSADATA	WsaData;
	SOCKADDR_IN	client_addr;
	SOCKET		client_s;
	HANDLE hThread_R;
	unSize ucWriteN;
	char wcBuf[MAX_LEN];

	if (WSAStartup(MAKEWORD(2, 2), &WsaData) != 0)
	{
		printf("WSA Fail\n");
		return 0;
	}

	client_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == client_s)
	{
		printf("socket Fail\n");
		return 0;
	}

	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORT);
	client_addr.sin_addr.S_un.S_addr = inet_addr(IP);

	if (connect(client_s, (struct sockaddr *)&client_addr, sizeof(client_addr)) == SOCKET_ERROR)
	{
		closesocket(client_s);
		printf("connect Fail\n");
		return 0;
	}
	hThread_R = CreateThread(NULL, 0, Recv_Chatting, (void *)client_s, 0, NULL);
	CloseHandle(hThread_R);
	
	while(1)
	{
		ucWriteN.iNum = read(0, wcBuf, MAX_LEN);
		wcBuf[ucWriteN.iNum-1] = 0;
		if( 0 == strncmp("quit", wcBuf, 4))
		{
			break;
		}
		send(client_s, ucWriteN.cNum, 4, 0);
		ucWriteN.iNum = send(client_s, (char *)wcBuf, ucWriteN.iNum, 0);
	}

	printf("Client End\n");

	WSACleanup();
	return 0;
}
