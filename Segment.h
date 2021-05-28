#pragma once
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <limits>

class Segment
{
	std::vector<cv::Point2i> mPoints;

	float m(int p, int q);
	float M(int p, int q);
public:
	float mXCenter;
	float mYCenter;

	void addPoint(cv::Point2i p);
	void calculateGeometricMoments();
	void drawBox(cv::Mat& image);
};

