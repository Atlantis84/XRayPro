#pragma once
#include "opencv2/opencv.hpp"
#include "HalconCpp.h"
//#include "HDevThread.h"
//#include "Utils/OpencvH.h"
//#include "GlobalDefs.h"
using namespace cv;
using namespace std;
using namespace HalconCpp;

#ifndef DLL
#define DLL _declspec(dllexport)
#endif

struct  UnitInfo
{
	Mat im_fg;
	vector<vector<Point>> contours;
	vector<RotatedRect> units_rect;
	int units_num;

};

class DLL TemplateBasedMethod
{
public:
	TemplateBasedMethod();
	~TemplateBasedMethod();

private:
	cv::Mat m_image;
	cv::Mat m_template_image;
	double m_threshold;
	int m_max_match;
	UnitInfo m_units_info;
	double m_angle_step;//»¡¶È

	HTuple m_hv_model_id;


public:
	void setImage(cv::Mat im);
	void setThreshold(double thres = 0.7);
	void setMaxMatch(int num = 5000);
	void setTemplateImage(cv::Mat im);
	UnitInfo getCheckResult();
	void setAngleStep(double angle = 0.1);

	void CreateNCCTemplate();
	void TemplateMatchWithNCC();

	void CreateShapeTemplate();
	void TemplateMatchWithShape();
};




