#pragma once

#include <winsock2.h>
#include <Windows.h>
#include<iostream>
#include<string>
#include "ThreadPool.h"

#pragma comment(lib,"ws2_32.lib")  
#define SERVER_PORT	6666 //ÕìÌý¶Ë¿Ú

class Client
{
public:
	bool CreateSocket();
	bool isConnected() { return connected; }
	bool Connect2Server();
	bool DisConnect();
	bool RequestTime();
	bool RequestName();
	bool RequestClientList();
	bool SendInfo();
	bool ExitPro();

	~Client();

	friend class OutputLoop;
private:
	SOCKET sClient;
	ThreadPool threadPool;
	bool connected = false;
};


class OutputLoop
{
public:
	void operator()(Client* client);
};

