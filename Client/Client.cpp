#include "Client.h"

bool Client::CreateSocket()
{
	WORD wVersionRequested;
	WSADATA wsaData;

	//WinSock初始化
	wVersionRequested = MAKEWORD(2, 2);//希望使用的WinSock DLL的版本
	if (WSAStartup(wVersionRequested, &wsaData))
	{
		std::cout << "WSAStartup() failed!\n";
		return false;
	}

	//确认WinSock DLL支持版本2.2：
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		std::cout << "Invalid Winsock version!\n";
		return false;
	}

	//创建socket，使用TCP协议：
	sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET)
	{
		WSACleanup();
		std::cout << "socket() failed!\n";
		return false;
	}
	return true;
}

bool Client::Connect2Server(const char* address, u_short port)
{
	struct sockaddr_in saServer;//地址信息

	//构建服务器地址信息：
	saServer.sin_family = AF_INET;//地址家族
	saServer.sin_port = htons(port);//注意转化为网络字节序
	saServer.sin_addr.S_un.S_addr = inet_addr(address);

	//连接服务器：
	bool ret = connect(sClient, (struct sockaddr*)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		closesocket(sClient);//关闭套接字
		WSACleanup();
		return false;
	}
}

////按照预定协议，客户端将发送一个学生的信息：
//strcpy(stu.name, argv[2]);
//stu.age = atoi(argv[3]);
//ret = send(sClient, (char*)&stu, sizeof(stu), 0);
//if (ret == SOCKET_ERROR)
//{
//	printf("send() failed!\n");
//}
//else
//printf("student info has been sent!\n");
//
//closesocket(sClient);//关闭套接字
//WSACleanup();