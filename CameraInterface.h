#pragma once
#ifndef DLL
#define DLL _declspec(dllexport)
#endif
#include "MvCamera.h"
#include "opencv2/opencv.hpp"
#include <string>
#include <process.h>
#include <windows.h>

using namespace std;
using namespace cv;

unsigned int __stdcall GrabThread(void* pUser);

class CameraInterface
{
public:
	CameraInterface();
	~CameraInterface();

public:
	void EnumDevice();
	//打开相机，默认为0
	int OpenCamera();
	// ch:按下关闭设备按钮：关闭设备 | en:Click Close button: Close Device
	void CloseCamera();
	int CloseDevice();
	// ch:获取触发模式 | en:Get Trigger Mode
	int GetTriggerMode();
	// ch:设置触发模式 | en:Set Trigger Mode
	int SetTriggerMode(int TriggerMode);
	// ch:软触发一次 | en:Click Execute button
	void SoftwareTriggerOnce();
	// ch:按下开始采集按钮 | en:Click Start button
	void StartGrabbing();
	// ch:取流线程 | en:Grabbing thread
	int GrabThreadProcess();
	// ch:按下结束采集按钮 | en:Click Stop button
	void StopGrabbing();
	bool RemoveCustomPixelFormats(enum MvGvspPixelType enPixelFormat);
	// convert data stream in Mat format
	bool Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData, cv::Mat &out_image);
	// ch:获取触发源 | en:Get Trigger Source
	unsigned int GetTriggerSource();
	// ch:设置触发源 | en:Set Trigger Source
	int SetTriggerSource(int trigger);
	//获取相机获得的图像
	cv::Mat GetImage();

private:


private:
	// ch:CMyCamera封装了常用接口 | en:CMyCamera packed commonly used interface
	CMvCamera*              m_pcMyCamera;   
	MV_CC_DEVICE_INFO_LIST  m_stDevList;
	bool					m_bOpenDevice;
	bool					m_bStartGrabbing;//开始抓取
	//int						m_nTriggerMode;//出发模式
	bool					m_bThreadState;//相机线程
	MV_FRAME_OUT_INFO_EX    m_stImageInfo;
	void*                   m_hGrabThread;// ch:取流线程句柄 | en:Grab thread handle
	CRITICAL_SECTION        m_hSaveImageMux;//保存图像的临界区
	unsigned char*          m_pSaveImageBuf;
	unsigned int            m_nSaveImageBufSize;
	cv::Mat					m_cvImage;
};

