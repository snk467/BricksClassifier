#pragma once
#include "opencv2/core/core.hpp"
#include <iostream>
#include <io.h>
#include <fcntl.h>

class Filter
{
	cv::Mat mImage;
public:
	static cv::Mat rankFilter(cv::Mat _image, int N, int index);
};

