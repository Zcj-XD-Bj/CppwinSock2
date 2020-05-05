// NetClient.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define WIN32_LEAN_AND_MEAN	//使用这个宏，否则就必须保证winSock2.h出现在windows.h前面
#define _WINSOCK_DEPRECATED_NO_WARNINGS//或将sdl检查设置为否（属性C++常规）
#include <Windows.h>
#include <winSock2.h>
#include <string>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")	//使用socket必须加载该静态链接库
//或者在工程属性中链接器->输入->附加依赖项

#include <iostream>
using namespace std;

int main()
{
	//初始化套接字
	WORD ver = MAKEWORD(2, 2);	//套接字版本号
	WSAData wsadata;
	WSAStartup(ver, &wsadata);

	//------------------
	//1.建立TCP连接的socket数据流套接字
	SOCKET g_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (g_sock == INVALID_SOCKET) //创建失败
	{
		cout << "套接字创建失败" << endl;
		return 0;
	}
	//2.连接服务器
	sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(80);
	sa.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	
	string receive = "\0";
	while (true)
	{
		if (SOCKET_ERROR == connect(g_sock, (sockaddr*)&sa, sizeof(sockaddr)))
		{
			cout << "连接服务器失败" << endl;
			//return 0;
			continue;
		}

		//3.接受服务器信息
		char rbuf;
		while (recv(g_sock, &rbuf, 1, 0) > 0 && rbuf != '\0')
		{
			receive += rbuf;
		}
		cout << receive << endl;
		break;
	}
	//4.关闭套接字
	closesocket(g_sock);
	//------------------
	WSACleanup();	//关闭套接字
	return 0;
}


