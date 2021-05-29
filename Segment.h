#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <limits>

class Segment
{
	std::vector<cv::Point2i> mPoints;
	bool mGeometricMomentsUpToDate = false;

	float mXCenter;
	float mYCenter;
	float m_00;
	float M_02;
	float M_03;
	float M_11;
	float M_12;
	float M_20;
	float M_21;
	float M_30;

	float m(int p, int q);
	float M(int p, int q);

	float M1();
	float M2();
	float M3();
	float M4();
	float M5();
	float M6();
	float M7();
	float M8();
	float M9();
	float M10();
public:

	void addPoint(cv::Point2i p);
	void calculateGeometricMoments();
	void drawBox(cv::Mat& image);
};

