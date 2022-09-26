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
	//�������Ĭ��Ϊ0
	int OpenCamera();
	// ch:���¹ر��豸��ť���ر��豸 | en:Click Close button: Close Device
	void CloseCamera();
	int CloseDevice();
	// ch:��ȡ����ģʽ | en:Get Trigger Mode
	int GetTriggerMode();
	// ch:���ô���ģʽ | en:Set Trigger Mode
	int SetTriggerMode(int TriggerMode);
	// ch:����һ�� | en:Click Execute button
	void SoftwareTriggerOnce();
	// ch:���¿�ʼ�ɼ���ť | en:Click Start button
	void StartGrabbing();
	// ch:ȡ���߳� | en:Grabbing thread
	int GrabThreadProcess();
	// ch:���½����ɼ���ť | en:Click Stop button
	void StopGrabbing();
	bool RemoveCustomPixelFormats(enum MvGvspPixelType enPixelFormat);
	// convert data stream in Mat format
	bool Convert2Mat(MV_FRAME_OUT_INFO_EX* pstImageInfo, unsigned char * pData, cv::Mat &out_image);
	// ch:��ȡ����Դ | en:Get Trigger Source
	unsigned int GetTriggerSource();
	// ch:���ô���Դ | en:Set Trigger Source
	int SetTriggerSource(int trigger);
	//��ȡ�����õ�ͼ��
	cv::Mat GetImage();

private:


private:
	// ch:CMyCamera��װ�˳��ýӿ� | en:CMyCamera packed commonly used interface
	CMvCamera*              m_pcMyCamera;   
	MV_CC_DEVICE_INFO_LIST  m_stDevList;
	bool					m_bOpenDevice;
	bool					m_bStartGrabbing;//��ʼץȡ
	//int						m_nTriggerMode;//����ģʽ
	bool					m_bThreadState;//����߳�
	MV_FRAME_OUT_INFO_EX    m_stImageInfo;
	void*                   m_hGrabThread;// ch:ȡ���߳̾�� | en:Grab thread handle
	CRITICAL_SECTION        m_hSaveImageMux;//����ͼ����ٽ���
	unsigned char*          m_pSaveImageBuf;
	unsigned int            m_nSaveImageBufSize;
	cv::Mat					m_cvImage;
};

