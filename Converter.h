#pragma once
#include "opencv2/core/core.hpp"

class Converter
{
public:
	static void bgrToGray(cv::Mat source, cv::Mat1b& destination);
private:
	Converter();
};

