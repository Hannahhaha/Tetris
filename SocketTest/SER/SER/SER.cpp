#include <WinSock2.h> 
#include <Windows.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <process.h>
#define BUFLEN 1024
#define TCPPORT 3000
#pragma comment(lib, "ws2_32.lib") 
using namespace std;

//定义结构体用来设置
typedef struct My_File
{
	SOCKET clientSocket; 
	sockaddr_in clientAddr; 
	int id; 
}F;

DWORD WINAPI transmmit(const LPVOID arg)
{

	F* temp = (F*)arg;
	cout << "连接成功,等待客户端发送消息..." << endl;

	//循环等待消息和连接
	while (true)
	{
		if (INVALID_SOCKET == temp->clientSocket)
		{
			cout << WSAGetLastError() << endl;
			closesocket(temp->clientSocket);
			WSACleanup();
			return FALSE;
		}

		//接收客户端传入的数据
		char Buffer[BUFLEN] = { 0 }; //缓冲区
		if (SOCKET_ERROR == recv(temp->clientSocket, Buffer, BUFLEN, 0))
		{
			closesocket(temp->clientSocket);
			return FALSE;
		}
		cout << "线程" << temp->id << "从客户端的" << ntohs(temp->clientAddr.sin_port) << "号端口收到:" << Buffer << endl;
		
		//发送数据
		const char* s = "Message Recieved Successfully";
		if (SOCKET_ERROR == send(temp->clientSocket, s, strlen(s) * sizeof(char) + 1, NULL))
		{
			closesocket(temp->clientSocket);
			return FALSE;
		}
		cout << "线程" << temp->id << "通过客户端的" << ntohs(temp->clientAddr.sin_port) << "号端口发送:" << s << endl;
	}
	return 0;
}



int main()
{
	WSADATA wsaData;
	//初始化wnsocket
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	//创建TCP套接字，并使用字节流
	SOCKET servSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//绑定监听地址(服务器的socket地址，包含sin_addr表示IP地址，sin_port保持端口号和sin_zero填充字节)
	sockaddr_in servAddr; 
	memset(&servAddr, 0, sizeof(SOCKADDR)); //初始化socket地址
	servAddr.sin_family = PF_INET; 
	servAddr.sin_port = htons(TCPPORT); 
	servAddr.sin_addr.s_addr = INADDR_ANY; 
	::bind(servSocket, (SOCKADDR*)&servAddr, sizeof(SOCKADDR)); 

	HANDLE hThread[20];
	if (SOCKET_ERROR == listen(servSocket, SOMAXCONN))//内核参数默认值一般是128
	{
		//如果失败关闭套接字
		closesocket(servSocket);
		WSACleanup();
	}
	for (int i = 0; i < 20; i++)
	{ 
		//创建新的传输结构体
		F* temp = new F;
		sockaddr_in clntAddr;
		int nSize = sizeof(SOCKADDR);
		SOCKET clientSock = accept(servSocket, (SOCKADDR*)&clntAddr, &nSize);
		//temp数据成员赋值
		temp->clientSocket = clientSock;
		temp->id = i + 1;
		temp->clientAddr = clntAddr;
		//创建子线程
		hThread[i] = CreateThread(NULL, 0, &transmmit, temp, 0, NULL);

	}
	//等待子线程完成
	WaitForMultipleObjects(20, hThread, TRUE, INFINITE);
	cout << WSAGetLastError() << endl; 

	//关闭socket，释放winsock
	closesocket(servSocket);
	WSACleanup();

	cout << "服务器连接已关闭" << endl;
	system("pause");

	return 0;
}