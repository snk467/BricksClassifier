#pragma once
#include "Segment.h"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <algorithm> 

class SegmentMap
{
	cv::Mat3b mMap;
	cv::RNG mRng;
	std::vector<cv::Vec3b> mUsedColors;

public:
	SegmentMap() {}
	SegmentMap(int cols, int rows);
	cv::Vec3b uniqueRandomColor();
	cv::Vec3b& operator()(cv::Point2i p);
	std::vector<Segment> generateSegments();
	cv::Mat3b getMap();
};

