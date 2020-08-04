#include <stdio.h>
#include <string>
#include <iostream>
#include <WS2tcpip.h>
#include <WinSock2.h>
#pragma comment(lib, "WS2_32.lib")
#define TCPPORT 3000
#define BUFLEN 1024
using namespace std;

int main()
{
	WSADATA wsaData;
	//初始化wnsocket
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		return FALSE;
	}

	//创建TCP套接字，并使用字节流
	SOCKET tcpServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == tcpServer)
	{
		WSACleanup();
		return FALSE;
	}

	//绑定监听地址
	SOCKADDR_IN addrTCPServer;
	addrTCPServer.sin_family = AF_INET;
	addrTCPServer.sin_port = htons(TCPPORT);  //本地监听端口：3000
	addrTCPServer.sin_addr.S_un.S_addr = INADDR_ANY;
	if (SOCKET_ERROR == bind(tcpServer, (const sockaddr*)&addrTCPServer, sizeof(SOCKADDR_IN)))
	{
		closesocket(tcpServer);
		WSACleanup();
		return FALSE;
	}

	//启动监听
	if (SOCKET_ERROR == listen(tcpServer, SOMAXCONN))//内核参数默认值一般是128
	{
		//如果失败关闭套接字
		closesocket(tcpServer);
		WSACleanup();
	}
	char IPdot[20] = { '\0' };
	//将数值格式转化为点分十进制的ip地址格式,返回值：若成功则为指向结构的指针，若出错则为NULL
	inet_ntop(AF_INET, (void*)&addrTCPServer.sin_addr, IPdot, 16);
	printf("Welcome,the Host %s is running!Now Wating for someone comes in!\n", IPdot);

	//等待连接接入
	sockaddr_in addrClient;
	int addrClientLen = sizeof(addrClient);
	cout << "TCPServer启动成功，等待连接......" << endl;

	//循环等待消息和连接
	while (true)
	{
		SOCKET tcpclient = accept(tcpServer, (sockaddr *)&addrClient, &addrClientLen);
		if (INVALID_SOCKET == tcpclient)
		{
			cout << WSAGetLastError() << endl;
			closesocket(tcpServer);
			closesocket(tcpclient);
			WSACleanup();
			return FALSE;
		}

		//接收客户端传入的数据
		char recvBuf[BUFLEN];
		if (SOCKET_ERROR == recv(tcpclient, recvBuf, sizeof(recvBuf), 0))
		{
			closesocket(tcpServer);
			closesocket(tcpclient);
			WSACleanup();
			return FALSE;
		}
		printf(" %s \n\n", recvBuf);
		Sleep(1000);
		

		//发送数据
		char sendBuf[BUFLEN];
		char IPdotdec[20] = { '\0' };
		inet_ntop(AF_INET, (void*)&addrClient.sin_addr, IPdotdec, 16);
		sprintf_s(sendBuf, "From Server: welcome %s to here", IPdotdec);
		if (SOCKET_ERROR == send(tcpclient, sendBuf, sizeof(sendBuf)+1, 0))
		{
			closesocket(tcpServer);
			closesocket(tcpclient);
			WSACleanup();
			return FALSE;
		}
	}

	return TRUE;
}