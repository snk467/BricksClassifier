#pragma once
#include "opencv2/core/core.hpp"

class Converter
{
	cv::Mat mImage;
public:
	Converter(cv::Mat image);
	cv::Mat_<uchar> toGray();
};

