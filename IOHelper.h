#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Segment.h"
#include "Args.h"
#include <filesystem>

class IOHelper
{
public:
	static std::vector<cv::Mat> loadImages(std::string path);
	static void outputImage(cv::Mat image, std::string title, bool show = false, bool save = false);
	static std::string mapLabel(Segment::Label label);
	static void outputLabel(std::wostream& stream, std::wstring label);
	static void outputInformation(std::wostream& stream, std::wstring information);
	static void outputResults(std::wostream& stream, std::wstring results);
	static void outputImageInfo(std::wostream& stream, int imageId);
	static void outputDoneInformation(std::wostream& stream);
private:
	IOHelper() {}
};