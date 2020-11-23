#include "Client.h"

bool Client::CreateSocket()
{
	WORD wVersionRequested;
	WSADATA wsaData;

	//WinSock��ʼ��
	wVersionRequested = MAKEWORD(2, 2);//ϣ��ʹ�õ�WinSock DLL�İ汾
	if (WSAStartup(wVersionRequested, &wsaData))
	{
		std::cout << "WSAStartup() failed!\n";
		return false;
	}

	//ȷ��WinSock DLL֧�ְ汾2.2��
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		std::cout << "Invalid Winsock version!\n";
		return false;
	}

	//����socket��ʹ��TCPЭ�飺
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
	struct sockaddr_in saServer;//��ַ��Ϣ

	//������������ַ��Ϣ��
	saServer.sin_family = AF_INET;//��ַ����
	saServer.sin_port = htons(port);//ע��ת��Ϊ�����ֽ���
	saServer.sin_addr.S_un.S_addr = inet_addr(address);

	//���ӷ�������
	bool ret = connect(sClient, (struct sockaddr*)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		closesocket(sClient);//�ر��׽���
		WSACleanup();
		return false;
	}
}

////����Ԥ��Э�飬�ͻ��˽�����һ��ѧ������Ϣ��
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
//closesocket(sClient);//�ر��׽���
//WSACleanup();