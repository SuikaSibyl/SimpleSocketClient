#pragma once

#include <winsock2.h>
#include <Windows.h>
#include<iostream>
#include<string>

#pragma comment(lib,"ws2_32.lib")  
#define SERVER_PORT	6666 //ÕìÌý¶Ë¿Ú

class Client
{
public:
	bool CreateSocket();
	bool isConnected() { return connected; }
	bool Connect2Server(const char* address, u_short port);
	bool RequestClientList();

private:
	SOCKET sClient;
	bool connected = false;
};

