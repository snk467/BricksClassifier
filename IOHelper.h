#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Segment.h"

class IOHelper
{
public:
	static std::vector<cv::Mat> loadImages(std::string path);
	static void outputImage(cv::Mat image, std::string title, bool show = false, bool save = true);
	static std::string mapLabel(Segment::Label label);
private:
	IOHelper() {}
};