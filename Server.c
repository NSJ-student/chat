#include <string.h>
#include <stdio.h>
#include <Winsock2.h>

#pragma comment(lib, "Ws2_32.lib");

#define MAX_LEN	256
#define PORT	9000
#define IP	"127.0.0.1"

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
			break;
		}
		iNum = recv(sockfd, (char *)cRxBuf, unReadN.iNum, 0);
		if( 0 == strncmp("quit", cRxBuf, 4))
		{
			break;
		}
		printf("client: %s\n", cRxBuf);
	}

	closesocket(sockfd);
	printf("-- Thread End\n");

	return 0;
}

int main(void)
{
	WSADATA	WsaData;
	struct sockaddr_in 	server_addr;
	struct sockaddr_in 	client_addr;
	HANDLE hThread_R;
	SOCKET		server_s;
	SOCKET		client_s;
	unsigned int addr_len;
	unSize ucWriteN;
	char wcBuf[MAX_LEN];

	if (WSAStartup(MAKEWORD(2, 2), &WsaData) != NO_ERROR)
	{
		printf("WSA Fail\n");
		return 0;
	}

	server_s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == server_s)
	{
		printf("socket Fail\n");
		WSACleanup();
		return 0;
	}

	ZeroMemory(&server_addr, sizeof(server_addr));
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	//server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_addr.S_un.S_addr = inet_addr(IP);

	if (bind(server_s, (SOCKADDR  *)&server_addr, sizeof(SOCKADDR)) == SOCKET_ERROR)
	{
		printf("bind Fail:  %d\n", WSAGetLastError());
		closesocket(server_s);
		WSACleanup();
		return 0;
	}
	if (listen(server_s, 5) == SOCKET_ERROR)
	{
		closesocket(server_s);
		printf("listen Fail\n");
		WSACleanup();
		return 0;
	}

	addr_len = sizeof(struct sockaddr_in);
	client_s = accept(server_s, (struct sockaddr *) &client_addr, &addr_len);
	hThread_R = CreateThread(NULL, 0, Recv_Chatting, (void *)client_s, 0, NULL);
	CloseHandle(hThread_R);
	
	while(1)
	{
		ucWriteN.iNum = read(0, wcBuf, MAX_LEN);
		if( 0 == strncmp("quit", wcBuf, 4))
		{
			break;
		}
		wcBuf[ucWriteN.iNum-1] = 0;
		send(client_s, ucWriteN.cNum, 4, 0);
		ucWriteN.iNum = send(client_s, (char *)wcBuf, ucWriteN.iNum, 0);
	}

	printf("Server End\n");

	closesocket(server_s);
	WSACleanup();
	return 0;
}
