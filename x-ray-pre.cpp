#include"x-ray-pre.h"

void xray_pre(Mat& src)
{
	Mat mask = Mat::zeros(Size(src.cols + 2, src.rows + 2), CV_8UC1);
	circle(mask, Point(1564, 1569), 1440, Scalar(1), -1);
	circle(src, Point(1564, 1569), 53, Scalar(255, 255, 255), -1);

	Rect r;
	floodFill(src, mask, Point(0, 0), Scalar(255, 255, 255), &r, Scalar(50, 50, 50), Scalar(50, 50, 50));

	Mat gray, binary;
	cvtColor(src, gray, COLOR_BGR2GRAY);
	threshold(gray, binary, 200, 255, THRESH_BINARY_INV);


	Mat kernel = Mat::ones(Size(55, 55), CV_8UC1);

	morphologyEx(binary, binary, MORPH_CLOSE, kernel);

	vector<vector<Point>> contours;
	vector<Vec4i> hierachy;
	findContours(binary, contours, hierachy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	float max_are = 0;
	int index = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		float are = contourArea(contours[i]);
		if (are > max_are)
		{
			max_are = are;
			index = i;
		}
	}

	Rect rect = boundingRect(contours[index]);
	rect.x -= 30;
	rect.y -= 30;
	rect.width += 60;
	rect.height += 60;

	src = src(rect);

}


