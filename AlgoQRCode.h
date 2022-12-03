#pragma once
#include "opencv2/opencv.hpp"
#include "opencv2/wechat_qrcode.hpp"
using namespace cv;
using namespace std;

class AlgoQRCode
{
public:
	AlgoQRCode();
	~AlgoQRCode();

private:
	Ptr<wechat_qrcode::WeChatQRCode> m_detector;

public:
	void initModel(const string modelPath);

	void detectQRCode(cv::Mat image, vector<cv::String>& vStrDecoded);

	bool compression(string inputFileName, string outputFileName, int quality);

	void release();
};

