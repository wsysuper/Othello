
//#include <iostream>
#include <winsock2.h> //ע�������include�ļ�˳��
#include <Ws2tcpip.h>
#include <process.h> //_beginthreadҪ��
#pragma comment(lib, "ws2_32.lib")
#define WM_RECV (WM_USER+100)

//using namespace std;


int do_send(char* sendline,int size); //��ȡ�û��䲢���͵��ಥ���̺߳���
void do_read(void* arg); //����ಥ�����ݺ���

void initial(void*);
