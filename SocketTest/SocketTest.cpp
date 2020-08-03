#include <stdio.h>
#include <string>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "WS2_32.lib")

using namespace std;

int main()
{
	WSADATA wsaData;
	SOCKET serverSock;
	SOCKADDR_IN serverAddr;
	SOCKET clientSock;
	SOCKADDR_IN clientAddr;


	cout << "Server Start!" << endl;
	//加载套接字库，启动socket的信息和服务
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != err)
	{
		cout << "WSAStartup failed!" << endl;
		return 1;
	}

	//创建服务器的socket，构造监听SOCKET(IPV4)，流式SOCKET
	serverSock = socket(AF_INET, SOCK_STREAM, 0);
	//配置监听地址和端口
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(3000);  //本地监听端口：4399
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	//根据地址和套接字绑定SOCKET端口
	int retVal = bind(serverSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
	if (SOCKET_ERROR == retVal)
	{
		cout << "bind failed!" << endl;
		closesocket(serverSock);
		WSACleanup();
		return -1;
	}

	retVal = listen(serverSock, 5);
	if (SOCKET_ERROR == retVal)
	{
		cout << "listen failed!" << endl;
		closesocket(serverSock);
		WSACleanup();
		return -1;
	}

	char IPdot[20] = { '\0' };
	inet_ntop(AF_INET, (void*)&serverAddr.sin_addr, IPdot, 16);
	printf("Welcome,the Host %s is running!Now Wating for someone comes in!\n", IPdot);

	int addrClientlen = sizeof(clientAddr);
	//等待客户端连接
	clientSock = accept(serverSock, (SOCKADDR*)&clientAddr, &addrClientlen);
	while (1)
	{
		//发送数据
		char sendBuff[50];
		//sprintf_s(sendBuff, "welcome %s to here", inet_ntoa(clientAddr.sin_addr));
		char IPdotdec[20] = { '\0' };
		inet_ntop(AF_INET, (void*)&clientAddr.sin_addr, IPdotdec, 16);
		sprintf_s(sendBuff, "From Server: welcome %s to here", IPdotdec);
		send(clientSock, sendBuff, strlen(sendBuff) + 1, 0);

		//接收数据
		char recvBuff[50];
		recv(clientSock, recvBuff, 50, 0);
		printf(" %s\n", recvBuff);

		Sleep(1000);
	}
	//关闭套接字，关闭加载的套接字库
	closesocket(serverSock);
	WSACleanup();
	return 0;
}