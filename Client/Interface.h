#pragma once
#include <iostream>
#include <string>
#include "Client.h"

namespace network
{
	class Interface
	{
	public:
		Interface();
		void enterMainLoop();

	private:
		Client client;
		void processInput(std::string& command);

		std::string str1_funcs = "========================== 功能表单 ==========================\n\
	1)连接：请求连接到指定地址和端口的服务端\n\
	2)断开连接：断开与服务端的连接\n\
	3)获取时间: 请求服务端给出当前时间\n\
	4)获取名字：请求服务端给出其机器的名称\n\
	5)活动连接列表：请求服务端给出当前连接的所有客户端信息（编号、IP地址、端口等）\n\
	6)发消息：请求服务端把消息转发给对应编号的客户端，该客户端收到后显示在屏幕上\n\
	7)退出：断开连接并退出客户端程序\n";

		std::string str2_split = "========================== 控制台输出 ==========================\n";
		std::string str2_console = "";

		std::string str4_split = "========================== 用户输入 ==========================";
	};
}

