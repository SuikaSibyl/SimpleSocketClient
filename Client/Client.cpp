#include "Client.h"
#include "Packet.h"

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

bool Client::Connect2Server()
{
	std::string addr;
	u_short port;
	std::cout << "Please Input server address:" << std::endl;
	std::cin >> addr;
	std::cout << "Please Input server port:" << std::endl;
	std::cin >> port;
	std::cout << "Connecting..." << std::endl;

	struct sockaddr_in saServer;//��ַ��Ϣ

	//������������ַ��Ϣ��
	saServer.sin_family = AF_INET;//��ַ����
	saServer.sin_port = htons(port);//ע��ת��Ϊ�����ֽ���
	saServer.sin_addr.S_un.S_addr = inet_addr(addr.data());

	//���ӷ�������
	int ret = connect(sClient, (struct sockaddr*)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		closesocket(sClient);//�ر��׽���
		WSACleanup();
		return false;
	}
	//��������״̬Ϊ������
	connected = true;

	//����һ���������ݵ����߳� -- ѭ������receive()
	auto f0 = std::bind(OutputLoop(), this); //���ɵ��ö����������
	threadPool.submit<std::function<void()>>(f0);

	std::cout << "Connected to Server." << std::endl;
	return true;
}

bool Client::DisConnect()
{
	std::cout << "Disonnecting..." << std::endl;
	if (!connected)
	{
		printf("you haven't connected to any server!\n");
		return false;
	}

	//��װ�������ݰ�
	Packet::Header header;
	header.packetType = Packet::PacketType::DISCONNECT; //��������Ϊ�Ͽ�����
	header.length = 0;
	//���������������
	if (send(sClient, (char*)&header, sizeof(header), 0) < 0) {
		WaitMessage();
		printf("disconnect error\n");
	}

	//�Ͽ�����
	if (sClient != INVALID_SOCKET)
	{
		closesocket(sClient);
		sClient = INVALID_SOCKET;
	}
	//�������ӶϿ�
	connected = false;

	//֪ͨ���ȴ����̹߳ر�
	//bool ret = threadPool.empty();
	//std::cout << ret << std::endl;
	return true;
}

bool Client::RequestTime()
{
	std::cout << "Requesting time..." << std::endl;
	if (!connected)
	{
		printf("you haven't connected to any server!\n");
		return false;
	}

	//��װ�������ݰ�
	Packet::Header header;
	header.packetType = Packet::PacketType::REQ4TIME; //��������Ϊʱ������
	header.length = 0;

	//���������������
	if (send(sClient, (char*)&header, sizeof(header), 0) < 0) {
		WaitMessage();
		printf("cannot get time from sever\n");
	}
	return true;
}

bool Client::RequestName() {
	std::cout << "Requesting name..." << std::endl;
	if (!connected)
	{
		printf("you haven't connected to any server!\n");
		return false;
	}

	//��װ�������ݰ�
	Packet::Header header;
	header.packetType = Packet::PacketType::REQ4NAME; //��������Ϊ��������
	header.length = 0;

	//���������������
	if (send(sClient, (char*)&header, sizeof(header), 0) < 0) {
		WaitMessage();
		printf("cannot get name from sever\n");
	}
	return true;
}

bool Client::RequestClientList()
{
	std::cout << "Requesting client list..." << std::endl;
	if (!connected)
	{
		printf("you haven't connected to any server!\n");
		return false;
	}

	//��װ�������ݰ�
	Packet::Header header;
	header.packetType = Packet::PacketType::REQ4LIST; //��������Ϊ�б�����
	header.length = 0;

	//���������������
	if (send(sClient, (char*)&header, sizeof(header), 0) < 0) {
		WaitMessage();
		printf("cannot get clientlist from sever\n");
	}
	return true;
}

bool Client::SendInfo() {
	std::cout << "Sending message..." << std::endl;
	if (!connected)
	{
		printf("you haven't connected to any server!\n");
		return false;
	}

	//����ͻ��˵��б��ź�Ҫ���͵�����
	std::string tmpId;
	std::string tmpMsg;
	getchar();
	std::cout << "Please enter client's id (3 digits) : ";
	std::cin >> tmpId;
	std::cout << "Please enter your message : ";
	std::cin >> tmpMsg;

	//��װ�������ݰ�
	Packet::Header header;
	header.packetType = Packet::PacketType::SENDINFO; //��������Ϊ������Ϣ
	std::string sendline = tmpId + tmpMsg;
	header.length = sendline.length() + 1;

	//���������������
	send(sClient, (char*)&header, sizeof(header), 0);
	send(sClient, sendline.c_str(), header.length, 0);
	return true;
}

bool Client::ExitPro() {
	return true;
}

Client::~Client() {
	if (sClient != NULL)
	{
		closesocket(sClient);//�ر��׽���
		WSACleanup();
	}
}

void OutputLoop::operator()(Client* client)
{
	while (true)
	{
		if (client->connected)
		{
			Packet::RecvState recvState;
			Packet::Header header;
			recvState = Packet::Packet::ReceiveByLength(client->sClient, (char*)&header, sizeof(header));
			if (client->connected == false)
			{
				std::cout << "Connection has been closed";
			}
			if (recvState != Packet::RecvState::SUCCESS)
			{
				//client->connected = false;
				if (recvState == Packet::RecvState::UNEXPECTED) std::cout << "socket unexptected exit";
				if (recvState == Packet::RecvState::CLOSE) std::cout << "socket closed";
				continue;
			}
			std::cout << "Header Type: " << header.packetType << " | Header Length: " << header.length << std::endl;

			char* content = new char[header.length];
			recvState = Packet::Packet::ReceiveByLength(client->sClient, content, header.length);
			if (recvState != Packet::RecvState::SUCCESS)
			{
				//client->connected = false;
				if (recvState == Packet::RecvState::UNEXPECTED) std::cout << "socket unexptected exit";
				if (recvState == Packet::RecvState::CLOSE) std::cout << "socket closed";
				continue;
			}
			std::string str(content, content + header.length);
			std::cout << str << std::endl;
		}
	}
}