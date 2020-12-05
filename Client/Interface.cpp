#include "Interface.h"

//初始化界面，创建socket
network::Interface::Interface()
{
	if (client.CreateSocket())
	{
		str2_console = "console: Socket Create Success";
	}
}

//循环处理输入指令
void network::Interface::enterMainLoop()
{
	std::string command;
	do
	{
		system("cls");
		std::cout << str1_funcs << std::endl;
		std::cout << str2_split << str2_console << std::endl;
		std::cout << str4_split << std::endl;
		if (command.length()) 
		{ 
			if (processInput(command))
			{
				return;
			}
		}
	} while (std::cin >> command);
}

bool network::Interface::processInput(std::string& command)
{
	switch (command[0])
	{
	case '1': //连接：请求连接到指定地址和端口的服务端
		client.Connect2Server();
		break;
	case '2': //断开连接：断开与服务端的连接
		client.DisConnect();
		break;
	case '3': //获取时间: 请求服务端给出当前时间
		client.RequestTime();
		break;
	case '4': //获取名字：请求服务端给出其机器的名称
		client.RequestName();
		break;
	case '5': //活动连接列表：请求服务端给出当前连接的所有客户端信息
		client.RequestClientList();
		break;
	case '6':
		client.SendInfo(); //发消息：请求服务端把消息转发给对应编号的客户端，该客户端收到后显示在屏幕上
		break;
	case '7':
		client.ExitPro(); //退出：断开连接并退出客户端程序
		return true;
	case '8': //获取时间: 请求服务端给出当前时间
		client.count = 0;
		for (int i = 0; i < 100; i++)
		{
			client.RequestTime();
		}
	default:
		break;
	}
	return false;
}