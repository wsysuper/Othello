#ifndef PLAYSOUND_H
#define PLAYSOUND_H

#include <stdio.h> 
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>//包含windows中与多媒体有关的大多数接口
#include <conio.h>

#define SHOWMSG(_msg) MessageBox(NULL, _msg, TEXT("msg"), MB_OK)
//Download by http://www.codefans.net


#pragma comment(lib,"winmm.lib")//工程必须加的一句话
#include <digitalv.h>

void play_mp3(CString);

#endif