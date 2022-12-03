#pragma once
#include "OpencvH.h"
#include<opencv2\wechat_qrcode.hpp>
using namespace std;
using namespace cv;
using namespace HalconCpp;



#ifndef DLL
#define DLL _declspec(dllexport)
#endif


class DLL BarCodeDecoding
{
public:
	BarCodeDecoding();
	~BarCodeDecoding();

public:
	//基于halcon库条码解码
	int decode_h(Mat src,  vector<string>& codes, vector<RotatedRect>& rrects);
	
	int decode_o(Mat src,  string &code);

	void QRdetect(const string config_dir, cv::Mat image, vector<cv::String>& vStrDecoded);
	
private:
	Ptr<wechat_qrcode::WeChatQRCode> m_detector;
};


