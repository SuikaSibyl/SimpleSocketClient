#include "Interface.h"

network::Interface::Interface()
{
	if (client.CreateSocket())
	{
		str2_console = "console: Socket Create Success";
	}
}

void network::Interface::enterMainLoop()
{
	std::string command;
	do
	{
		system("cls");
		std::cout << str1_funcs << std::endl;
		std::cout << str2_split << str2_console << std::endl;
		std::cout << str4_split << std::endl;
		if (command.length()) { processInput(command); }
	}
	while (std::cin >> command);
}

void network::Interface::processInput(std::string& command)
{
	if (command == "1")
	{
		std::string addr;
		u_short port;
		std::cout << "Please Input server address:" << std::endl;
		std::cin >> addr;
		std::cout << "Please Input server port:" << std::endl;
		std::cin >> port;
		client.Connect2Server(addr.data(), port);
	}
	else if (command == "4")
	{
		client.RequestClientList();
	}
}