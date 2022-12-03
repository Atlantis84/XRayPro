//#include "stdafx.h"
#include "AlgoQRCode.h"
//#include "Logger.h"


AlgoQRCode::AlgoQRCode()
{
}


AlgoQRCode::~AlgoQRCode()
{
}

void AlgoQRCode::initModel(const string modelPath) {
	string detect_prototxt = modelPath + "qrdetect.prototxt";
	string detect_caffe_model = modelPath + "qrdetect.caffemodel";
	string sr_prototxt = modelPath + "qrsr.prototxt";
	string sr_caffe_model = modelPath + "qrsr.caffemodel";
	
	m_detector = makePtr<wechat_qrcode::WeChatQRCode>(detect_prototxt, detect_caffe_model, sr_prototxt, sr_caffe_model);
	
}

void AlgoQRCode::detectQRCode(cv::Mat image, vector<cv::String>& vStrDecoded)
{
	vector<Mat> vPoints;
	//vector<cv::String> vStrDecoded;
	
	vStrDecoded = m_detector->detectAndDecode(image, vPoints);
	
}

bool AlgoQRCode::compression(string inputFileName, string outputFileName, int quality) {
	Mat srcImage = imread(inputFileName);

	if (srcImage.data != NULL)
	{
		vector<int>compression_params;
		compression_params.push_back(IMWRITE_JPEG_QUALITY);
		compression_params.push_back(quality);     //图像压缩参数，该参数取值范围为0-100，数值越高，图像质量越高

		bool bRet = imwrite(outputFileName, srcImage, compression_params);

		return bRet;
	}

	return false;
}

void AlgoQRCode::release() {
	m_detector = NULL;
}
