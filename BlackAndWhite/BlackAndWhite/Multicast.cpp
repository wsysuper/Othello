#include "StdAfx.h"
#include "Multicast.h"
#include "ChildView.h"
const char* MULTICAST_IP = "224.0.0.99"; //�ಥ���ַ
const int MULTICAST_PORT = 2002; //�ಥ��˿�

const int BUFFER_SIZE = 1024;

void initial(void*)
{
	WSAData wsaData;

	if( WSAStartup(MAKEWORD(2,2), &wsaData) != 0 )
	{
		//cout<<"Error in WSAStartup"<<endl;
		return;
	}

	SOCKET server;
	server = socket(AF_INET, SOCK_DGRAM, 0); //����һ��UDP�׽ӿ�
	//cout<<"create socket: "<<server<<endl;

	int ret; //���ܵķ��ش���

	const int on = 1; //�������Ķ��ʵ��������ͬһ̨������
	ret = setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if( ret == SOCKET_ERROR )
	{
		//cout<<"Error in setsockopt(SO_REUSEADDR): "<<WSAGetLastError()<<endl;
		WSACleanup();
		return;
	}

	const int routenum = 10;
	ret = setsockopt(server,IPPROTO_IP,IP_MULTICAST_TTL,\
		(char*)&routenum,sizeof(routenum));
	if( ret == SOCKET_ERROR )
	{
		//cout<<"Error in setsockopt(IP_MULTICAST_TTL): "<<WSAGetLastError()<<endl;
		WSACleanup();
		return;
	}

	const int loopback = 1; // 0��ֹ����  1�������
	ret = setsockopt(server,IPPROTO_IP,IP_MULTICAST_LOOP,\
		(char*)&loopback,sizeof(loopback));
	if( ret == SOCKET_ERROR )
	{
		//cout<<"Error in setsockopt(IP_MULTICAST_LOOP): "<<WSAGetLastError()<<endl;
		WSACleanup();
		return;
	}

	sockaddr_in local;
	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(MULTICAST_PORT);
	local.sin_addr.S_un.S_addr = INADDR_ANY;

	ret = bind(server, (sockaddr*)(&local), sizeof(local));
	if( ret == SOCKET_ERROR )
	{
		//cout<<"Error in bind: "<<WSAGetLastError()<<endl;
		WSACleanup();
		return;
	}

	ip_mreq mreq;
	memset(&mreq, 0, sizeof(mreq));
	mreq.imr_interface.S_un.S_addr = INADDR_ANY;
	mreq.imr_multiaddr.S_un.S_addr = inet_addr(MULTICAST_IP);

	//����һ���ಥ��
	ret = setsockopt(server,IPPROTO_IP,IP_ADD_MEMBERSHIP,\
		(char*)&mreq,sizeof(mreq));
	if( ret == SOCKET_ERROR )
	{
		//cout<<"Error in setsockopt(IP_ADD_MEMBERSHIP): "<<WSAGetLastError()<<endl;
		WSACleanup();
		return;
	}

	//�����������̣߳�һ�����û����벢���ͣ�һ�����ಥ������
	HANDLE hHandle;
//	hHandle[0] = (HANDLE)_beginthread(do_send,0,(void*)server);
	hHandle = (HANDLE)_beginthread(do_read,0,(void*)server);

	//����û�����������������ֹ��
	WaitForSingleObject(hHandle, INFINITE);

	WSACleanup();

	//return 0;
}

int do_send(char* sendline,int size)
{
	SOCKET server;
	server = socket(AF_INET, SOCK_DGRAM, 0); //����һ��UDP�׽ӿ�
	//cout<<"create socket: "<<server<<endl;

	int ret; //���ܵķ��ش���

	const int on = 1; //�������Ķ��ʵ��������ͬһ̨������
	ret = setsockopt(server, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if( ret == SOCKET_ERROR )
	{
		//cout<<"Error in setsockopt(SO_REUSEADDR): "<<WSAGetLastError()<<endl;
		WSACleanup();
		return 0;
	}

	const int routenum = 10;
	ret = setsockopt(server,IPPROTO_IP,IP_MULTICAST_TTL,\
		(char*)&routenum,sizeof(routenum));
	if( ret == SOCKET_ERROR )
	{
		//cout<<"Error in setsockopt(IP_MULTICAST_TTL): "<<WSAGetLastError()<<endl;
		WSACleanup();
		return 0;
	}

	const int loopback = 1; // 0��ֹ����  1�������
	ret = setsockopt(server,IPPROTO_IP,IP_MULTICAST_LOOP,\
		(char*)&loopback,sizeof(loopback));
	if( ret == SOCKET_ERROR )
	{
		//cout<<"Error in setsockopt(IP_MULTICAST_LOOP): "<<WSAGetLastError()<<endl;
		WSACleanup();
		return 0;
	}

	sockaddr_in local;
	memset(&local, 0, sizeof(local));
	local.sin_family = AF_INET;
	local.sin_port = htons(MULTICAST_PORT);
	local.sin_addr.S_un.S_addr = INADDR_ANY;

	ret = bind(server, (sockaddr*)(&local), sizeof(local));
	if( ret == SOCKET_ERROR )
	{
		//cout<<"Error in bind: "<<WSAGetLastError()<<endl;
		WSACleanup();
		return 0;
	}

	ip_mreq mreq;
	memset(&mreq, 0, sizeof(mreq));
	mreq.imr_interface.S_un.S_addr = INADDR_ANY;
	mreq.imr_multiaddr.S_un.S_addr = inet_addr(MULTICAST_IP);

	//����һ���ಥ��
	ret = setsockopt(server,IPPROTO_IP,IP_ADD_MEMBERSHIP,\
		(char*)&mreq,sizeof(mreq));
	if( ret == SOCKET_ERROR )
	{
		//cout<<"Error in setsockopt(IP_ADD_MEMBERSHIP): "<<WSAGetLastError()<<endl;
		WSACleanup();
		return 0;
	}

//	char sendline[BUFFER_SIZE+1];

	sockaddr_in remote;
	memset(&remote, 0, sizeof(remote));
	remote.sin_addr.s_addr = inet_addr ( MULTICAST_IP );
	remote.sin_family = AF_INET ;
	remote.sin_port = htons(MULTICAST_PORT);

//	for(;;) //��ȡ�û�����ֱ���û�����"end"
//	{
		//cin.getline(sendline, BUFFER_SIZE);

//		if(strncmp(sendline,"end",3)==0)
//			break;

		//�����û���������ݵ��ಥ��
		sendto(server, sendline, /*strlen(sendline)*/size, 0, (sockaddr*)(&remote), sizeof(remote)); 
//	}

	//cout<<"do_send end..."<<endl;
        return 0;
}

void do_read(void* arg)
{
	SOCKET server = (SOCKET)arg;

	char buf[BUFFER_SIZE+1];
	int ret;

	sockaddr_in client;
	int clientLen;

	for(;;) //һֱ��ȡ֪�����߳���ֹ
	{
		Sleep(20);
		clientLen = sizeof(client);
		memset(&client, 0, clientLen);

		ret = recvfrom(server, buf, BUFFER_SIZE, 0, (sockaddr*)(&clientLen), &clientLen);
		if ( ret == 0) //do_read���û�ֱ�ӻس�������һ�����ַ���
		{
			continue;
		}
		else if( ret == SOCKET_ERROR )
		{
			if( WSAGetLastError() == WSAEINTR ) //���߳���ֹrecvfrom���صĴ�
				break;

			//cout<<"Error in recvfrom: "<<WSAGetLastError()<<endl;
			break ;
		}
		buf[ret] = '\0';
		SendMessage(CChildView::hwnd,WM_RECV,(WPARAM)buf,0);
		//cout<<"received: "<<buf<<endl;
	}

	//cout<<"do_read end..."<<endl;
}
