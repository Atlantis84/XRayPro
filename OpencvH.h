#include <iostream>
#include "halconcpp/HalconCpp.h"
#include "opencv2\opencv.hpp"

using namespace cv;
using namespace std;
using namespace HalconCpp;


//Mat转Hobject格式
bool Mat2HObject(cv::Mat cv_mat, HalconCpp::HObject &h_object);


//Hobject转Mat格式
bool HObject2Mat(HalconCpp::HObject h_object, cv::Mat &cv_mat);


