

#include "stdafx.h"
#include "PlaySound.h"
//#include "mmsystem.h"
//#include   <afxdlgs.h> 
//#pragma comment(lib,"winmm.lib") 
//本程序使用的是VC中的MCI编程

MCI_PLAY_PARMS mciPlay; 
UINT DeviceID;
MCI_OPEN_PARMS mciOpen;

void play_mp3(CString dress)//播放MP3音乐
{
	TCHAR buf[128]=_T(""); 
    MCIERROR mciError; 
    mciOpen.lpstrDeviceType = _T("mpegvideo"); 
    mciOpen.lpstrElementName = dress;//播放文件的地址,支持格式mp3
	mciSendCommand(DeviceID,MCI_CLOSE,NULL,NULL); 
    mciError = mciSendCommand(0,MCI_OPEN,MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,(DWORD)&mciOpen); 
    if(mciError) 
	{ 
		mciGetErrorString(mciError,buf,128); 
	} 
	DeviceID = mciOpen.wDeviceID ; 
    mciError = mciSendCommand(DeviceID,MCI_PLAY,MCI_DGV_PLAY_REPEAT ,(DWORD)&mciPlay); 
    //if(mciError) 
	//{ 
	//	MessageBox (NULL, TEXT ("send MCI_PLAY command failed!"), NULL, 0) ;
	//}
}
