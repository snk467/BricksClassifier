#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <limits>
#include <iostream>


class Segment
{

	std::vector<cv::Point2i> mPoints;
	bool mGeometricMomentsUpToDate = false;

	double mXCenter;
	double mYCenter;
	double m_00;
	double M_02;
	double M_03;
	double M_11;
	double M_12;
	double M_20;
	double M_21;
	double M_30;

	double M1_value;
	double M2_value;
	double M3_value;
	double M4_value;
	double M5_value;
	double M6_value;
	double M7_value;
	double M8_value;
	double M9_value;
	double M10_value;

	double M1();
	double M2();
	double M3();
	double M4();
	double M5();
	double M6();
	double M7();
	double M8();
	double M9();
	double M10();

	double m(int p, int q);
	double M(int p, int q);

	bool isFive();
	bool isEight();
	bool isZero();
	bool isOne();
	bool isThree();
	bool isFour();
	bool isTen();

	bool isInRange(double value, double low, double high);
public:
	Segment();
	enum class Label { zero, one, three, four, five, eight, ten, unknown };
	Segment::Label whoAmI();
	void addPoint(cv::Point2i p);
	void calculateGeometricMoments(bool verbose = false);
	void drawBox(cv::Mat& image, std::string label = "");
	double area();
	static Segment merge(Segment s1, Segment s2);

};

