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

	bool isFive();
	bool isEight();
	bool isZero();
	bool isOne();
	bool isThree();
	bool isFour();
	bool isTen();

	bool isInRange(float value, float low, float high);
public:
	Segment();
	enum class Label { zero, one, three, four, five, eight, ten, unknown };
	Segment::Label whoAmI();
	void addPoint(cv::Point2i p);
	void calculateGeometricMoments(bool verbose = false);
	void drawBox(cv::Mat& image, std::string label = "");
	float area();
	static Segment merge(Segment s1, Segment s2);

};

