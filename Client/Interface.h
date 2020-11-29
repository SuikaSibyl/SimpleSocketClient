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

		std::string str1_funcs = "========================== ���ܱ� ==========================\n\
	1)���ӣ��������ӵ�ָ����ַ�Ͷ˿ڵķ����\n\
	2)�Ͽ����ӣ��Ͽ������˵�����\n\
	3)��ȡʱ��: �������˸�����ǰʱ��\n\
	4)��ȡ���֣��������˸��������������\n\
	5)������б��������˸�����ǰ���ӵ����пͻ�����Ϣ����š�IP��ַ���˿ڵȣ�\n\
	6)����Ϣ���������˰���Ϣת������Ӧ��ŵĿͻ��ˣ��ÿͻ����յ�����ʾ����Ļ��\n\
	7)�˳����Ͽ����Ӳ��˳��ͻ��˳���\n";

		std::string str2_split = "========================== ����̨��� ==========================\n";
		std::string str2_console = "";

		std::string str4_split = "========================== �û����� ==========================";
	};
}

