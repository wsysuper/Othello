#ifndef PLAYSOUND_H
#define PLAYSOUND_H

#include <stdio.h> 
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>//����windows�����ý���йصĴ�����ӿ�
#include <conio.h>

#define SHOWMSG(_msg) MessageBox(NULL, _msg, TEXT("msg"), MB_OK)
//Download by http://www.codefans.net


#pragma comment(lib,"winmm.lib")//���̱���ӵ�һ�仰
#include <digitalv.h>

void play_mp3(CString);

#endif