#include "Client.h"
#include "Packet.h"

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

bool Client::Connect2Server()
{
	std::string addr;
	u_short port;
	std::cout << "Please Input server address:" << std::endl;
	std::cin >> addr;
	std::cout << "Please Input server port:" << std::endl;
	std::cin >> port;
	std::cout << "Connecting..." << std::endl;

	struct sockaddr_in saServer;//地址信息

	//构建服务器地址信息：
	saServer.sin_family = AF_INET;//地址家族
	saServer.sin_port = htons(port);//注意转化为网络字节序
	saServer.sin_addr.S_un.S_addr = inet_addr(addr.data());

	//连接服务器：
	int ret = connect(sClient, (struct sockaddr*)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		closesocket(sClient);//关闭套接字
		WSACleanup();
		return false;
	}
	//设置连接状态为已连接
	connected = true;

	//创建一个接收数据的子线程 -- 循环调用receive()
	auto f0 = std::bind(OutputLoop(), this); //将可调用对象与参数绑定
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

	//组装请求数据包
	Packet::Header header;
	header.packetType = Packet::PacketType::DISCONNECT; //类型设置为断开请求
	header.length = 0;
	//发送请求给服务器
	if (send(sClient, (char*)&header, sizeof(header), 0) < 0) {
		WaitMessage();
		printf("disconnect error\n");
	}

	//断开连接
	if (sClient != INVALID_SOCKET)
	{
		closesocket(sClient);
		sClient = INVALID_SOCKET;
	}
	//设置连接断开
	connected = false;

	//通知并等待子线程关闭
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

	//组装请求数据包
	Packet::Header header;
	header.packetType = Packet::PacketType::REQ4TIME; //类型设置为时间请求
	header.length = 0;

	//发送请求给服务器
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

	//组装请求数据包
	Packet::Header header;
	header.packetType = Packet::PacketType::REQ4NAME; //类型设置为名字请求
	header.length = 0;

	//发送请求给服务器
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

	//组装请求数据包
	Packet::Header header;
	header.packetType = Packet::PacketType::REQ4LIST; //类型设置为列表请求
	header.length = 0;

	//发送请求给服务器
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

	//输入客户端的列表编号和要发送的内容
	std::string tmpId;
	std::string tmpMsg;
	getchar();
	std::cout << "Please enter client's id (3 digits) : ";
	std::cin >> tmpId;
	std::cout << "Please enter your message : ";
	std::cin >> tmpMsg;

	//组装请求数据包
	Packet::Header header;
	header.packetType = Packet::PacketType::SENDINFO; //类型设置为发送信息
	std::string sendline = tmpId + tmpMsg;
	header.length = sendline.length() + 1;

	//发送请求给服务器
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
		closesocket(sClient);//关闭套接字
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