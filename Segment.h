#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <limits>
#include <iostream>


class Segment
{

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

	float m(int p, int q);
	float M(int p, int q);


public:
	std::vector<cv::Point2i> mPoints;
	Segment();
	enum class Label { five, unknown };
	Segment::Label whoAmI();
	void addPoint(cv::Point2i p);
	void calculateGeometricMoments(bool verbose = false);
	void drawBox(cv::Mat& image, std::string label = "");
	float area();

};

