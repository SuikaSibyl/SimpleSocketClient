#include "Interface.h"

//��ʼ�����棬����socket
network::Interface::Interface()
{
	if (client.CreateSocket())
	{
		str2_console = "console: Socket Create Success";
	}
}

//ѭ����������ָ��
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
	case '1': //���ӣ��������ӵ�ָ����ַ�Ͷ˿ڵķ����
		client.Connect2Server();
		break;
	case '2': //�Ͽ����ӣ��Ͽ������˵�����
		client.DisConnect();
		break;
	case '3': //��ȡʱ��: �������˸�����ǰʱ��
		client.RequestTime();
		break;
	case '4': //��ȡ���֣��������˸��������������
		client.RequestName();
		break;
	case '5': //������б��������˸�����ǰ���ӵ����пͻ�����Ϣ
		client.RequestClientList();
		break;
	case '6':
		client.SendInfo(); //����Ϣ���������˰���Ϣת������Ӧ��ŵĿͻ��ˣ��ÿͻ����յ�����ʾ����Ļ��
		break;
	case '7':
		client.ExitPro(); //�˳����Ͽ����Ӳ��˳��ͻ��˳���
		return true;
	case '8': //��ȡʱ��: �������˸�����ǰʱ��
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