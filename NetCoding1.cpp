// NetCoding1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define WIN32_LEAN_AND_MEAN	//使用这个宏，否则就必须保证winSock2.h出现在windows.h前面
#define _WINSOCK_DEPRECATED_NO_WARNINGS				
#include <Windows.h>
#include <winSock2.h>

#include <iostream>

#pragma comment(lib, "ws2_32.lib")	//使用socket必须加载该静态链接库
//或者在工程属性中链接器->输入->附加依赖项

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

	//2.绑定用于接受客户端连接的网络端口
	sockaddr_in sa;	//sockaddr_in和sockaddr差不多，不过sockaddr不利于用户操作
	sa.sin_family = AF_INET;	//协议
	sa.sin_port = htons(80);	//host to net 端口号
	sa.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");	//绑定ip地址
//	sa.sin_addr.S_un.S_addr = INADDR_ANY;	//默认ip地址
	if (SOCKET_ERROR == bind(g_sock, (sockaddr*)&sa, sizeof(sockaddr)))
	{
		cout << "绑定端口失败" << endl;
		return 0;
	}

	//3.监听网络端口
	if (SOCKET_ERROR == listen(g_sock, 5)) //第二个参数：The maximum length of the queue of pending connections
	{
		cout << "监听失败" << endl;
		return 0;
	}
	else 
	{
		cout << "监听网络端口成功" << endl;
	}

	//4.等待接受客户端连接
	sockaddr_in client_addr = {};
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET client;
	char msgbuff[] = "Hello, I'm Server.";
	
	while (true)
	{
		client = accept(g_sock, (sockaddr*)&client_addr, &nAddrLen);
		if (INVALID_SOCKET == client)
		{
			cout << "接受到无效客户端Socket!" << endl;
			int t = 2000;
			while (t--);
			continue;
		}

		cout << "新客户端加入：IP = " << inet_ntoa(client_addr.sin_addr) << endl;

		//5.向客户端发送一条数据
		int num = send(client, msgbuff, strlen(msgbuff) + 1, 0);	//+1指的是字符串结束符
		//num表示成功发送的字节数
		if (SOCKET_ERROR == num)
		{
			cout << "向客户端发送信息失败" << endl;
		}
	}
	//6.关闭套接字
	closesocket(g_sock);
	//------------------
	WSACleanup();	//关闭套接字
	return 0;
}

