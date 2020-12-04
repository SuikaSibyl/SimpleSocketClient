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
	system("cls");
	std::cout << str1_funcs << std::endl;
	std::cout << str2_split << str2_console << std::endl;
	std::cout << str4_split << std::endl;
	do
	{
		if (command.length()) { processInput(command); }
	} while (std::cin >> command);
}

void network::Interface::processInput(std::string& command)
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
		for (int i = 0; i < 100; i++)
		{
			client.RequestTime();
		}
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
		break;
	default:
		break;
	}
}