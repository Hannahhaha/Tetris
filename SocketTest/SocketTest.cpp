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
	//�����׽��ֿ⣬����socket����Ϣ�ͷ���
	int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (0 != err)
	{
		cout << "WSAStartup failed!" << endl;
		return 1;
	}

	//������������socket���������SOCKET(IPV4)����ʽSOCKET
	serverSock = socket(AF_INET, SOCK_STREAM, 0);
	//���ü�����ַ�Ͷ˿�
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(3000);  //���ؼ����˿ڣ�4399
	serverAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	//���ݵ�ַ���׽��ְ�SOCKET�˿�
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
	//�ȴ��ͻ�������
	clientSock = accept(serverSock, (SOCKADDR*)&clientAddr, &addrClientlen);
	while (1)
	{
		//��������
		char sendBuff[50];
		//sprintf_s(sendBuff, "welcome %s to here", inet_ntoa(clientAddr.sin_addr));
		char IPdotdec[20] = { '\0' };
		inet_ntop(AF_INET, (void*)&clientAddr.sin_addr, IPdotdec, 16);
		sprintf_s(sendBuff, "From Server: welcome %s to here", IPdotdec);
		send(clientSock, sendBuff, strlen(sendBuff) + 1, 0);

		//��������
		char recvBuff[50];
		recv(clientSock, recvBuff, 50, 0);
		printf(" %s\n", recvBuff);

		Sleep(1000);
	}
	//�ر��׽��֣��رռ��ص��׽��ֿ�
	closesocket(serverSock);
	WSACleanup();
	return 0;
}