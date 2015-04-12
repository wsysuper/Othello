
//#include <iostream>
#include <winsock2.h> //注意这里的include文件顺序
#include <Ws2tcpip.h>
#include <process.h> //_beginthread要求
#pragma comment(lib, "ws2_32.lib")
#define WM_RECV (WM_USER+100)

//using namespace std;


int do_send(char* sendline,int size); //读取用户输并发送到多播组线程函数
void do_read(void* arg); //读入多播组数据函数

void initial(void*);
