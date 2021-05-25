#pragma once

#include <vector>
#include <string>
#include <opencv2/core/mat.hpp>

class FileSystemHelper
{
public:
	static std::vector<cv::Mat> loadImages(std::string _path);
};

