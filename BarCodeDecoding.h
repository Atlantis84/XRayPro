#pragma once
#include "opencv2/opencv.hpp"
#include <vector>
#include <string>
using namespace std;
using namespace cv;
#ifndef DLL
#define DLL _declspec(dllexport)
#endif


class BarCodeDecoding
{
public:
	BarCodeDecoding();
	~BarCodeDecoding();

public:
	//����halcon���������
	int decode_h(Mat src,  vector<string>& codes, vector<RotatedRect>& rrects);
	
	int decode_o(Mat src,  string &code);
	
};


