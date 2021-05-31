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
	std::vector<Segment> mSegments;

public:
	SegmentMap() {}
	SegmentMap(int cols, int rows);
	cv::Vec3b uniqueRandomColor();
	cv::Mat3b getMap();
	void setMap(cv::Point2i p, cv::Vec3b color);
	void newSegment();
	std::vector<Segment> getSegments(bool filterSmall = true);
};

