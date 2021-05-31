#pragma once
#include "opencv2/core/core.hpp"

class Converter
{
public:
	static void BGRToGray(cv::Mat source, cv::Mat1b& destination);
	static void BGRToHLS(cv::Mat3b source, cv::Mat3b& destination);
	static void BGRToHSV(cv::Mat3b source, cv::Mat3b& destination);
private:
	Converter();
};